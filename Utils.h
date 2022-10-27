//
// Created by davide on 07/10/22.
//

#ifndef N_GRAM_UTILS_H
#define N_GRAM_UTILS_H

#include <iostream>

bool processInputChar(char& c);
std::string mergeBooks(std::string dir, std::string file_name);
std::string createFileWith_M_Words(std::string target_dir, std::string out_path, int M);
int countWords(std::string& file_path);

bool processInputChar(char& c){
    /* Processes the input, removing everything apart alphabet letters and spaces */
    if(std::isalpha(c) || std::isspace(c)){
        if (std::isupper(c))
            c = (char) std::tolower(c); // also it lowers uppercase characters
        return false;
    } else {
        return true;
    }
}

std::string mergeBooks(std::string& dir, std::string& file_name){
    /* Merges the books (.txt files) in a directory in order to create a single, larger book */
    std::ifstream book;
    std::ofstream mergedBook;
    mergedBook.open(dir + file_name, std::ios_base::binary); // output file

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

std::string createFileWith_M_Words(std::string target_dir, std::string out_path, int M){
    /* Creates a file with approx. M words given a bunch of .txt files in the directory dir */

    std::ifstream book;
    std::string line;
    std::string tmp;
    size_t pos;

    std::ofstream MwordsBook;
    MwordsBook.open(out_path);

    int counter = 0;

    while(counter < M) { // we iterate over all the books as long as we don't gather M words

        for (const auto &entry: std::filesystem::directory_iterator(target_dir)) {
            if (std::filesystem::file_size(entry.path()) > 0 &&
                entry.path() != out_path) {
                book.open(entry.path());

                while (std::getline(book, line)) {

                    if (counter <= M)
                        MwordsBook << line << std::endl;
                    else
                        break;

                    pos = line.find(' ');
                    while (pos != std::string::npos) {
                        if (pos > 0) // in such a way we ignore double spaces
                            counter += 1;
                        line.erase(0, pos + 1); // erase the space as well
                        pos = line.find(' ');
                    }

                    // check if there is another word at the end of the line
                    if (!line.empty()) {
                        tmp = line.substr(0, line.length() - 1);
                        if (!tmp.empty())
                            counter += 1;
                    }
                }

                book.close();

                if (counter > M)
                    break;
            }
        }
    }
    MwordsBook.close();
    std::cout << "This file has approx " << counter << " words" <<  std::endl;

    return out_path;
}

int countWords(std::string& file_path){
    /* Approx count of the words in a .txt file */

    std::ifstream book;
    book.open(file_path);

    std::string line;
    std::string tmp;
    size_t pos;

    int counter = 0;

    while (std::getline(book, line)) {

        pos = line.find(' ');
        while (pos != std::string::npos) {
            if (pos > 0) // in such a way we ignore double spaces
                counter += 1;
            line.erase(0, pos + 1); // erase the space as well
            pos = line.find(' ');
        }

        // check if there is another word at the end of the line
        if (!line.empty()) {
            tmp = line.substr(0, line.length() - 1);
            if (!tmp.empty())
                counter += 1;
        }
    }
    book.close();
    std::cout << "This file has approx " << counter << " words" <<  std::endl;

    return counter;
}



#endif //N_GRAM_UTILS_H
