//
// Created by davide on 06/10/22.
//

#ifndef N_GRAM_SEQUENTIALNGRAM_H
#define N_GRAM_SEQUENTIALNGRAM_H

#include <iostream>
#include <vector>
#include <fstream>

void sequentialNgram(std::string& file_name, std::string& out_folder_sequential, int n);

void sequentialNgram(std::string& file_name, std::string& out_folder_sequential, int n){
    /* Sequential implementation of an algorithm to extract ngrams */
    std::ifstream infile(file_name);
    std::string line;

    std::vector<std::string> words; // vector in which we memorize all the words we read from the file

    while(std::getline(infile, line)){

        size_t pos = line.find(' '); // we look for spaces in order to distinguish words

        while(pos != std::string::npos){
            words.push_back(line.substr(0, pos));
            line.erase(0, pos + 1);
            pos = line.find(' ');
        }
    }

    unsigned int len;
    std::string ngram;
    bool printed;

    std::ofstream outputFile;
    outputFile.open(out_folder_sequential + std::to_string(n) + "gram_outputSerialVersion.txt");

    for(std::string& w : words){
        len = w.length();
        if(len >= n){
            for(int i=0; i < len - n + 1; i++){
                // for each word we look for the consecutive substrings of length n (the ngrams)

                ngram = w.substr(i, n);
                printed = true;
                for(char& c: ngram){
                    if(!isalpha(c)) // if the ngram contains a symbol or a number it is discarded
                        printed = false;
                    else
                        c = (char) tolower(c); // we produce an output of only lower-case letters
                }

                if(printed)
                    outputFile << ngram << std::endl;  // write to file the ngram
            }
        }
    }

    outputFile.close();
}

#endif //N_GRAM_SEQUENTIALNGRAM_H
