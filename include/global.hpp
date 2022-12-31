#pragma once

#include "util.hpp"
#include "json.hpp"

using namespace nlohmann;

class Global {
    static json data;
public:
    static void init();
    static void save();
    static string get(string key);
    static void set(string key, string value);
};
