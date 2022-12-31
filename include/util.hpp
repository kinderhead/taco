#pragma once

#include <array>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <filesystem>

#include <boost/iostreams/filter/line.hpp>

using namespace std;

// https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po
inline string execute(string cmd) {
    array<char, 128> buf;
    string res;
    unique_ptr<FILE, decltype(&pclose)> pipe(popen((cmd + " 2>&1").c_str(), "r"), pclose);

    if (!pipe) {
        cout << "Could not run process " + cmd + "\n";
        exit(1);
    }

    while (fgets(buf.data(), buf.size(), pipe.get()) != nullptr) {
        res += buf.data();
    }

    return res;
}

// https://cplusplus.com/faq/sequences/strings/split/#string-find_first_of
inline vector<string> split(string str, string delim) {
    vector<string> ret;
    string::size_type current = 0;
    string::size_type next = -1;

    do {
        current = next + 1;
        next = str.find_first_of(delim, current);
        ret.push_back(str.substr(current, next - current));
    } while (next != string::npos);

    return ret;
}

inline filesystem::path get_build_path(string path) {
    return filesystem::path("build/" + filesystem::path(path).stem().string() + ".o");
}

inline filesystem::path get_build_path(filesystem::path path) {
    return get_build_path(path.string());
}

// https://github.com/p-ranav/indicators/issues/107
struct IndicatorsFilter : boost::iostreams::line_filter {
    string do_filter(const string& line) {
        return "\n\033[A\033[1L" + line;
    }
};
