#include <iostream>
#include <memory>
#include "path_finder.hpp"

class Program
{
public:
    Program ()
    {
        std::cout << "Program initialized\n";
    }
    static void programMethod ()
    {
        std::cout << "Hello World from Program class\n";
    }
};

int main (int argc, char* argv[])
{
    std::cout << "Hello World\n";
    std::unique_ptr<Program> newProgram = std::make_unique<Program>();
    Program::programMethod();
    std::unique_ptr<LibraryClass> newLibrary = std::make_unique<LibraryClass>();
    newLibrary->useLibrary();

    return 0;
}