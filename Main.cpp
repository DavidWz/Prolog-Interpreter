#include <iostream>
#include <fstream>
#include "Common.h"
#include "Parser.h"
#include "Interpreter.h"

int main(int argc, const char* argv[]) {
    std::shared_ptr<Program> program;

    // load the program
    if (argc == 2) {
        std::ifstream inputFileStream(argv[1]);
        if (inputFileStream.good()) {
            Parser parser(inputFileStream);
            program = parser.parseProgram();
            inputFileStream.close();
            if (parser.isSuccessful()) {
                std::cout << *program << std::endl;
            }
            else {
                return 0;
            }
        }
        else {
            std::cout << "Could not read file." << std::endl;
            return 0;
        }
    }
    else {
        std::cout << "Please specify a program to run on." << std::endl;
        return 0;
    }

    // init interpreter
    Interpreter interpreter(program);

    // main loop
    bool isRunning = true;
    while (isRunning) {
        std::cout << "?- ";
        std::string inputLine;
        std::getline(std::cin, inputLine);

        if (inputLine == ":quit") {
            isRunning = false;
        }
        else {
            Parser queryParser(inputLine);
            ExpsT query = queryParser.parseQuery();
            if (queryParser.isSuccessful()) {
                std::experimental::optional<Substitution> answer = interpreter.evaluate(query);
                if (answer) {
                    std::cout << "True. " << *answer << std::endl;
                }
                else {
                    std::cout << "False." << std::endl;
                }
            }
        }
    }
}