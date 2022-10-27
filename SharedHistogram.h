//
// Created by davide on 25/10/22.
//

#ifndef N_GRAM_SHAREDHISTOGRAM_H
#define N_GRAM_SHAREDHISTOGRAM_H

#include <omp.h>
#include <iostream>
#include <fstream>
#include <numeric>
#include <map>

class SharedHistogram {

public:

    SharedHistogram(){
        omp_init_lock(&insertionLock);
    }

    ~SharedHistogram(){
        omp_destroy_lock(&insertionLock);
    };

    void mergeHistogram(std::map<std::string, int>& partial_histogram){
        std::map<std::string, int>::iterator it;

        // FIRST IMPLEMENTATION
        omp_set_lock(&insertionLock);
        if(histogram.empty())
            histogram = partial_histogram; // allows to save some time
        else{
            for(auto& kv : partial_histogram) {
                it = histogram.find(kv.first); // we look for the key in the shared histogram
                if(it != histogram.end()) // the key is already in the histogram
                    it->second += kv.second;
                else
                    histogram.insert(std::make_pair(kv.first, kv.second)); // create the key-value pair
            }
        }
        omp_unset_lock(&insertionLock);


        // SECOND IMPLEMENTATION
        /*
        omp_set_lock(&insertionLock);
        if (histogram.empty())
            histogram = partial_histogram;
        else {
            for (auto &kv: partial_histogram)
                histogram[kv.first] += kv.second;
        }
        omp_unset_lock(&insertionLock);
         */
    }

    void addNgram(std::string& ngram, int value){
        std::map<std::string, int>::iterator it;
        it = histogram.find(ngram);
        omp_set_lock(&insertionLock);
        if(it != histogram.end())
            it->second += value;
        else
            histogram.insert(std::make_pair(ngram, value));
        omp_unset_lock(&insertionLock);
    }

    void writeHistogramToFile(std::string path){
        /* Allows to write to file the current shared histogram
         * It is called once all Consumers have done sharing their own private histogram */
        std::ofstream outputFile;
        outputFile.open(path);
        for(auto& kv : histogram) // sorted by string, as we use std::map
            outputFile << kv.first << "\t" << kv.second << std::endl;

        outputFile.close();
    }

private:
    std::map<std::string, int> histogram;
    omp_lock_t insertionLock;
};


#endif //N_GRAM_SHAREDHISTOGRAM_H

