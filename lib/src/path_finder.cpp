#include <iostream>
#include "path_finder.hpp"

LibraryClass::LibraryClass()
{
    std::cout << "Hello from the library class\n";
    libraryValue = 0;
}

void LibraryClass::useLibrary()
{
    libraryValue++;
    std::cout << "Using the library function. libraryValue=" << libraryValue << "\n";        
}