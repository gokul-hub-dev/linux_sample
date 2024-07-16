#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    struct stat fileStat;
    const char* filePath = "example.txt";

    // Check if the file exists and retrieve its statistics
    if (stat(filePath, &fileStat) == 0) {
        std::cout << "File: " << filePath << std::endl;
        std::cout << "Size: " << fileStat.st_size << " bytes" << std::endl;
    } else {
        std::cerr << "File does not exist: " << filePath << std::endl;
    }

    return 0;
}

/*
this will check file exist or not
*/
