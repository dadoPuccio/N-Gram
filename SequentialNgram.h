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
void sequentialNgramChar(std::string& file_name, std::string& out_folder_sequential, int n);

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

void sequentialNgramChar(std::string& file_name, std::string& out_folder_sequential, int n){
    /* Sequential implementation of an algorithm to extract character ngrams */
    std::ifstream infile(file_name);
    std::string line;

    std::ofstream outputFile;
    outputFile.open(out_folder_sequential + std::to_string(n) + "gram_outputSerialVersion.txt");

    std::string fileStrings, noPunctuationLine;
    fileStrings = "";
    int counter = 0;

    while(std::getline(infile, line)){

        std::remove_copy_if(
                line.begin(),
                line.end(),
                std::back_inserter(noPunctuationLine),
                std::ptr_fun<int,int>(&std::ispunct));

        std::remove_copy_if(
                line.begin(),
                line.end(),
                std::back_inserter(noPunctuationLine),
                std::ptr_fun<int,int>(&std::isdigit));

        // std::cout << noPunctuationLine << std::endl;
        fileStrings += noPunctuationLine;
        counter += 1;

        if(counter > 500){
            std::cout << "Finished reading" << std::endl;
            break;
        }
    }

    unsigned long len = fileStrings.length();
    std::string ngram;
    bool toBePrinted;
    int spaceCounter;

    for(int i = 0; i < len - n + 1; i++){
        ngram = fileStrings.substr(i, n);
        toBePrinted = true;
        spaceCounter = 0;

        for(char& c : ngram){
            if(!isalpha(c) && c!=*"à" && c!=*"è" && c!=*"é" && c!=*"ì" && c!=*"ò" && c!=*"ù") {
                if(c == *" ") {
                    spaceCounter += 1;
                    if(spaceCounter > 1){
                        toBePrinted = false;
                        break;
                    }
                } else {
                    toBePrinted = false;
                    break;
                }
            }
            if(isupper(c))
                c = (char) tolower(c);
        }

        if(toBePrinted)
            outputFile << ngram << std::endl;  // write to file the ngram
    }

    outputFile.close();


    /*
     * OTHER IMPLEMENTATION (incorrect: considers char n-grams of each word separately)
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

            }
        }
    }
     outputFile.close();
     */
}

#endif //N_GRAM_SEQUENTIALNGRAM_H
