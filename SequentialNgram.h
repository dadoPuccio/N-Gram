//
// Created by davide on 06/10/22.
//

#ifndef N_GRAM_SEQUENTIALNGRAM_H
#define N_GRAM_SEQUENTIALNGRAM_H

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

#include "Utils.h"

void sequentialNgramWords(std::string& file_name, std::string& out_folder_sequential, int n);

void sequentialNgramWords(std::string& file_name, std::string& out_folder_sequential, int n){
    /* Sequential implementation of an algorithm to extract words N-grams */

    // INPUT PHASE
    std::ifstream infile(file_name);
    std::string line;
    std::string processedLine;

    std::vector<std::string> words; // vector in which we memorize all the words we read from the file
    std::string tmp; // to store the last word in the line

    while(std::getline(infile, line)){

        /*Remove from the line what is not a letter nor a space */
        std::remove_copy_if(
            line.begin(),
            line.end(),
            std::back_inserter(processedLine),
            std::ptr_fun<char&,bool>(&processInputChar));

        size_t pos = processedLine.find(' '); // we look for spaces in order to distinguish words

        while(pos != std::string::npos){
            if(pos > 0) // in such a way we ignore double spaces
                words.push_back(processedLine.substr(0, pos));
            processedLine.erase(0, pos + 1); // erase the space as well
            pos = processedLine.find(' ');
        }

        // check if there is another word at the end of the line
        if(!processedLine.empty()) {
            tmp = processedLine.substr(0, processedLine.length() - 1);
            if (!tmp.empty())
                words.push_back(tmp);
        }

        processedLine.clear();
    }

    // std::cout << words.size() << std::endl;

    // OUTPUT PHASE
    std::ofstream outputFile;
    outputFile.open(out_folder_sequential + std::to_string(n) + "gram_outputSerialVersion.txt");

    std::string ngram;
    ngram = "";

    /* First output */
    for(int j=0; j < n; j++)
        ngram += words[j] + " ";
    outputFile << ngram << std::endl;

    size_t pos;
    pos = ngram.find(' '); // detect where the first space is in order to locate the first word

    /* Following outputs */
    for(int i=n; i < words.size(); i++){
        ngram.erase(0, pos + 1); // we remove the first word in the previous output
        ngram += words[i] + " "; // and concatenate the last word for the following output

        outputFile << ngram << std::endl;
        pos =  ngram.find(' ');
    }

    outputFile.close();
}

#endif //N_GRAM_SEQUENTIALNGRAM_H
