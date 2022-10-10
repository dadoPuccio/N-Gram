//
// Created by davide on 07/10/22.
//

#ifndef N_GRAM_UTILS_H
#define N_GRAM_UTILS_H

#include <iostream>

bool processInputChar(char& c);
std::string mergeBooks(std::string dir, std::string file_name);

bool processInputChar(char& c){
    /* Processes the input, removing everything apart alphabet letters and spaces */
    if(std::isalpha(c) || std::isspace(c)){
        if (std::isupper(c))
            c = (char) std::tolower(c); // also it lowers the input
        return false;
    } else {
        return true;
    }
}

std::string mergeBooks(std::string dir, std::string file_name){
    std::ifstream book;
    std::ofstream mergedBook;
    mergedBook.open(dir + file_name, std::ios_base::binary);

    // std::cout << mergedBook.is_open() << std::endl;

    for (const auto& entry : std::filesystem::directory_iterator(dir)){
        // std::cout << entry.path() << std::endl;
        if(std::filesystem::file_size(entry.path()) > 0 && entry.path() != dir+file_name) { // as empty files might be created with large chunk sizes
            book.open(entry.path(), std::ios_base::binary);
            mergedBook << book.rdbuf(); // read from partial and write to total
            book.close();
        }
    }
    mergedBook.close();

    return dir + file_name;
}

#endif //N_GRAM_UTILS_H
