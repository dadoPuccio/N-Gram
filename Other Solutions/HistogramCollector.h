//
// Created by davide on 25/10/22.
//

#ifndef N_GRAM_HISTOGRAMCOLLECTOR_H
#define N_GRAM_HISTOGRAMCOLLECTOR_H

#include <iostream>
#include <omp.h>
#include <vector>
#include <map>
#include <fstream>

class HistogramCollector {
public:

    HistogramCollector(){
        omp_init_lock(&insertionLock);
    }

    ~HistogramCollector(){
        omp_destroy_lock(&insertionLock);
    }

    void addPartialHistogram(std::map<std::string, int>& partial_histogram){
        omp_set_lock(&insertionLock);
        histogramCollection.push_back(partial_histogram);
        omp_unset_lock(&insertionLock);
    }

    void writeHistogramToFile(std::string path){
        std::ofstream outputFile;
        outputFile.open(path);

        int count;
        std::map<std::string, int>::iterator it;

        for(int i=0; i < histogramCollection.size(); i++){
            count = 0;
            for(auto& kv : histogramCollection[i]) {

                count += kv.second;

                for(int j=i+1; j < histogramCollection.size(); j++) {
                    it = histogramCollection[j].find(kv.first);
                    if (it != histogramCollection[j].end()) {
                        count += it->second;
                        histogramCollection[j].erase(it->first);
                    }
                }

                outputFile << kv.first << "\t" << kv.second << std::endl;
            }
        }


        outputFile.close();
    }

private:
    std::vector<std::map<std::string, int>> histogramCollection;
    omp_lock_t insertionLock;
};
#endif //N_GRAM_HISTOGRAMCOLLECTOR_H
