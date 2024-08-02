#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <filesystem>

namespace fs = std::filesystem;

void shredFile(const char* filename) {
    std::ofstream file(filename, std::ios::out | std::ios::binary);

    if (!file) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
    }

    const int bufferSize = 1024;
    char buffer[bufferSize];

    std::srand(std::time(0));

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < bufferSize; ++j) {
            buffer[j] = std::rand() % 256;
        }

        file.write(buffer, bufferSize);
    }

    file.close();
    std::remove(filename);

    std::cout << "File shredded successfully: " << filename << std::endl;
}

void shredDirectory(const char* directoryPath) {
    for (const auto& entry : fs::recursive_directory_iterator(directoryPath)) {
        if (fs::is_regular_file(entry.path())) {
            shredFile(entry.path().c_str());
        }
    }

    fs::remove_all(directoryPath);
    std::cout << "Directory and its contents shredded successfully: " << directoryPath << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <directory_path>" << std::endl;
        return 1;
    }

    const char* path = argv[1];

    if (fs::is_directory(path)) {
        shredDirectory(path);
    } else {
        std::cerr << "Invalid directory path: " << path << std::endl;
        return 1;
    }

    return 0;
}

