#include "project.hpp"
#include "util.hpp"

#include <indicators.hpp>

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <fstream>

#include <boost/algorithm/string.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>

using namespace indicators;

Project::Project(string path)
{
    this->path = path;
    if (filesystem::exists(path + "/taco.json")) {
        ifstream stream(path + "/taco.json");
        data = json::parse(stream);
        stream.close();
    }
}

void Project::build(bool release, bool static_lib, bool compile_commands)
{
    switch (OPERATING_SYSTEM) {
        case LINUX:
            linux_build(release, static_lib, compile_commands);
            break;
        case WINDOWS:
        case MAC:
            cout << "Building only works on linux for now\n";
            exit(1);
            break;
    }
}

void Project::save()
{
    ofstream stream(path + "/taco.json");
    stream << setw(4) << data;
    stream.close();
}

Project Project::create(string path, string name, string version)
{
    if (filesystem::exists(path + "/taco.json")) {
        cout << "A project is already present\n";
        exit(1);
    }
    Project project(path);
    project.data["name"] = name;
    project.data["version"] = version;
    project.data["type"] = "executable";
    project.data["source"] = {{"src", "src"}, {"include", "include"}};
    project.data["include_dirs"] = json::array();
    project.data["dependencies"] = json::array();
    project.save();

    filesystem::create_directory("src");
    filesystem::create_directory("include");

    return project;
}

void Project::linux_build(bool release, bool static_lib, bool compile_commands)
{
    auto flags = linux_get_dependency_flags(static_lib);
    if (data.contains("include_dirs")) {
        for (const auto& i : data["include_dirs"]) {
            flags.push_back("-I" + i.get<string>());
        }
    }

    // Also checks if data["source"] exists
    auto files = get_c_files();

    if (data["source"].contains("include")) {
        flags.push_back("-I" + data["source"]["include"].get<string>());
    }

    filesystem::create_directory("build");

    vector<string> to_build;
    for (const auto& i : files) {
        auto out_file = get_build_path(i);
        if (filesystem::exists(out_file)) {
            auto otime = filesystem::last_write_time(out_file);
            auto ctime = filesystem::last_write_time(i);

            if (ctime > otime) {
                to_build.push_back(i);
            }
        }
    }

    cout << "Building...";

    map<string, vector<string>> cc_flags;

    // Do all files because of compile_commands
    for (const auto& i : files) {
        cc_flags[i] = vector<string>(flags); // Should be a copy
        if (!static_lib) {
            cc_flags[i].push_back("-fPIC");
        }
        cc_flags[i].insert(cc_flags[i].end(), {"-c", i, "-o", get_build_path(i)});
    }

    auto stdout_buf = cout.rdbuf();
    boost::iostreams::filtering_ostreambuf filtering_buf{};
    filtering_buf.push(IndicatorsFilter());
    filtering_buf.push(*stdout_buf);
    cout.rdbuf(&filtering_buf);
    ostream os(stdout_buf);

    ProgressBar bar{
        option::BarWidth{50},
        option::Start{"["},
        option::Fill{"■"},
        option::Lead{"■"},
        option::Remainder{" "},
        option::End{" ]"},
        option::ForegroundColor{Color::green},
        option::ShowElapsedTime{true},
        option::ShowRemainingTime{true},
        option::FontStyles{std::vector<FontStyle>{FontStyle::bold}},
        option::Stream{os},
        option::MaxProgress{to_build.size()}
    };
    show_console_cursor(false);

    for (const auto& i : to_build) {
        bar.set_option(option::PostfixText(filesystem::path(i).filename().string()));

        auto output = execute("/usr/bin/gcc " + boost::algorithm::join(cc_flags[i], " "));

        bar.tick();
    }

    show_console_cursor(true);
    cout.rdbuf(stdout_buf);
}

vector<string> Project::linux_get_dependency_flags(bool static_lib)
{
    vector<string> ret;
    
    if (data.contains("dependencies")) {
        for (const auto& i : data["dependencies"]) {
            string dependency = i.get<string>();
            string cmd = "/usr/bin/pkg-config --cflags ";
            if (static_lib) {
                cmd += "--static ";
            }
            auto res = execute(cmd + dependency);

            if (res.find("not found") != string::npos) {
                cout << "Error resolving package " + dependency + ".\nPlease install package in a location where pkg-config can find it.\n";
                exit(1);
            }

            boost::algorithm::trim(res);
            auto stuff = split(res, " ");
            ret.insert(ret.end(), stuff.begin(), stuff.end());
        }
    }

    return ret;
}

vector<string> Project::get_c_files()
{
    vector<string> ret;

    if (!data.contains("source") || !data["source"].contains("src")) {
        cout << "No input files. Check taco.json\n";
        exit(1);
    }

    for (const auto& entry : filesystem::recursive_directory_iterator(data["source"]["src"])) {
        if (!entry.is_directory()) {
            if (find(CEXTENSIONS.begin(), CEXTENSIONS.end(), entry.path().extension()) != CEXTENSIONS.end()) {
                ret.push_back(entry.path());
            }
        }
    }

    return ret;
}
