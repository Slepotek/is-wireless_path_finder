#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "path_finder.hpp"

LibraryClass::LibraryClass(void)
{
    std::cout << "Hello from the library class" << std::endl;
    libraryValue = 0;
}

void LibraryClass::useLibrary(void)
{
    libraryValue++;
    std::cout << "Using the library function. libraryValue=" << libraryValue << std::endl;        
}