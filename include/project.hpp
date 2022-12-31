#pragma once

#include <string>
#include "json.hpp"

using namespace nlohmann;
using namespace std;

enum OS {
    WINDOWS,
    LINUX,
    MAC
};

#ifdef __linux__
const OS OPERATING_SYSTEM = LINUX;
#endif
#ifdef __APPLE__
const OS OPERATING_SYSTEM = MAC;
#endif
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
const OS OPERATING_SYSTEM = WINDOWS;
#endif

const vector<string> CEXTENSIONS = {".c", ".cpp", ".cc", ".cxx"};

class Project {
    string path;
    json data;

    void linux_build(bool release, bool static_lib, bool compile_commands);
    vector<string> linux_get_dependency_flags(bool static_lib);

    vector<string> get_c_files();
    vector<string> get_include_dirs_as_compile_args();
public:
    Project(string path);

    void build(bool release, bool static_lib, bool compile_commands);
    void save();

    static Project create(string path, string name, string version);
};
