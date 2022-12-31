#include "global.hpp"

#include <filesystem>
#include <fstream>

#include "project.hpp"

void Global::init()
{
    switch (OPERATING_SYSTEM) {
    case LINUX:
        if (!filesystem::exists("~/.taco")) {
            filesystem::create_directory("~/.taco");

            data["compiler"] = "/usr/bin/gcc";
            data["compiler-shared-args"] = {"-fPIC"};
            data["linker-shared-args"] = {"-fPIC", "-rdynamic", "-shared"};

            save();
        }
        break;
    case WINDOWS:
    case MAC:
        cout << "Only linux is supported for now\n";
        exit(1);
    }
}

void Global::save()
{
    ofstream stream;
    switch (OPERATING_SYSTEM) {
    case LINUX:
        stream = ofstream("~/.taco/global.json");
        stream << data;
        stream.close();
        break;
    case WINDOWS:
    case MAC:
        cout << "Only linux is supported for now\n";
        exit(1);
    }
}

string Global::get(string key)
{
    return data[key];
}

void Global::set(string key, string value)
{
    data[key] = value;
}
