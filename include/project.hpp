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

class Project {
    string path;

    json data;
public:
    Project(string path);

    void build(bool release, bool compile_commands);
    void save();

    static Project create(string path, string name, string version);
};
