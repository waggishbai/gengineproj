#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include <fstream>
#include <sstream>

std::string getFileText(std::string filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

#endif
