#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include "path_finder.hpp"

class Program
{
public:
    Program ()
    {
        std::cout << "Program initialized" << std::endl;
    }
    static void programMethod (void)
    {
        std::cout << "Hello World from Program class" << std::endl;
    }
};

int main (int argc, char* argv[])
{
    std::cout << "Hello World" << std::endl;
    std::unique_ptr<Program> newProgram = std::make_unique<Program>();
    newProgram->programMethod();
    std::unique_ptr<LibraryClass> newLibrary = std::make_unique<LibraryClass>();
    newLibrary->useLibrary();

    return 0;
}