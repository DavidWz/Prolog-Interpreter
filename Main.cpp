#include <iostream>
#include <fstream>
#include "Common.h"
#include "Parser.h"

int main(int argc, const char* argv[]) {
    if (argc == 2) {
        std::ifstream inputFileStream(argv[1]);
        if (!inputFileStream.good()) {
            std::cout << "Could not read file." << std::endl;
        }
        else {
            Parser parser(inputFileStream);
            std::shared_ptr<Program> program = parser.parse();
            inputFileStream.close();
            if (parser.isSuccessful()) {
                std::cout << *program << std::endl;
            }
            else {
                std::cout << "Parsing not successful." << std::endl;
            }
        }
    }
    else {
        std::cout << "Please specify a program to run on." << std::endl;
    }
}