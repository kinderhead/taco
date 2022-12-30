#include "project.hpp"

#include <filesystem>
#include <iostream>
#include <fstream>

Project::Project(string path)
{
    this->path = path;
    if (filesystem::exists(path + "/taco.json")) {
        ifstream stream(path + "/taco.json");
        data = json::parse(stream);
        stream.close();
    }
}

void Project::build(bool release, bool compile_commands)
{
    
}

void Project::save()
{
    ofstream stream(path + "/taco.json");
    stream << setw(4) << data;
    stream.close();
}

Project Project::create(string path, string name, string version)
{
    Project project(path);
    project.data["name"] = name;
    project.data["version"] = version;
    project.save();
    return project;
}
