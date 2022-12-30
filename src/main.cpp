#include <iostream>

#include "argparse.hpp"
#include "config.h"

using namespace std;

int main(int argc, char** argv) {
    argparse::ArgumentParser program("taco", VERSION);

    program.add_argument("dir")
        .help("The path to the project")
        .default_value(string{"."});
    
    argparse::ArgumentParser build_command("build");
    build_command.add_argument("-d", "--debug")
        .help("Build project with debug info and no optimizations")
        .default_value(false)
        .implicit_value(true);

    try {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    cout << program.get("dir");

    return 0;
}
