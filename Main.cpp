#include <iostream>
#include <fstream>
#include "Common.h"
#include "Parser.h"
#include "Interpreter.h"
#include "ast/VariablePool.h"

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
#ifdef DEBUG
                std::cout << *program << std::endl;
#endif
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
    VariablePool::rememberState();

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
            VariablePool::resetToRememberedState();
        }
    }
}