#include <iostream>

#include "argparse.hpp"
#include "config.h"
#include "project.hpp"

using namespace std;

int main(int argc, char** argv) {
    argparse::ArgumentParser program("taco", VERSION);
    
    argparse::ArgumentParser build_command("build");
    build_command.add_argument("-d", "--debug")
        .help("Build project with debug info and no optimizations. This is the default option");
    build_command.add_argument("-r", "--release")
        .help("Build project with optimizations")
        .default_value(false)
        .implicit_value(true);
    build_command.add_argument("--dynamic")
        .help("Build using shared libraries");
    build_command.add_argument("--static")
        .help("Build using static libraries")
        .default_value(false)
        .implicit_value(true);
    build_command.add_argument("-c", "--compile_commands")
        .help("Generates a compile_commands.json in the build folder")
        .default_value(false)
        .implicit_value(true);;
    
    program.add_subparser(build_command);

    argparse::ArgumentParser init_command("init");
    init_command.add_argument("name")
        .help("Name of the project")
        .required();
    init_command.add_argument("version")
        .help("Version number")
        .required()
        .default_value(string{"0.1.0"});

    program.add_subparser(init_command);

    try {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    Project project(".");

    if (program.is_subcommand_used(init_command)) {
        project = Project::create(".", init_command.get("name"), init_command.get("version"));
    } else if (program.is_subcommand_used(build_command)) {
        project.build(build_command["-r"] == true, build_command["--static"] == true, build_command["-c"] == true);
    }

    return 0;
}
