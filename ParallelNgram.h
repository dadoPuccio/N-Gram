//
// Created by davide on 06/10/22.
//

#ifndef N_GRAM_PARALLELNGRAM_H
#define N_GRAM_PARALLELNGRAM_H

#include <iostream>
#include <vector>
#include <fstream>
#include <omp.h>
#include <filesystem>

#include "JobsQueue.h"

void parallelNgram(std::string& file_name, std::string& out_folder_parallel, int n, int chunk_size);

void parallelNgram(std::string& file_name, std::string& out_folder_parallel, int n, int chunk_size){
    /* Parallel implementation of an algorithm to extract ngrams
     * This implementation is based on Producer-Consumers pattern */
    std::ifstream infile(file_name);
    JobsQueue jobsQueue; // Queue in which jobs are stored

    int tid;

    /* Each Consumer thread will produce an output file with partial data in a /tmp/ folder,
     * at the end it will be merged into one single file. */
    std::filesystem::create_directory(out_folder_parallel + "tmp/");

    #pragma omp parallel default(none) shared(infile, out_folder_parallel, jobsQueue, n, chunk_size) private(tid)
    {
        #pragma omp single nowait
        {
            /* PRODUCER THREAD: reads the input file and generates chunks of words (vector of strings)
             * which are enqueued and passed to CONSUMERS threads */
            std::vector<std::string> wordsLoaded;
            std::string line;
            int counter;

            while (std::getline(infile, line)) {

                size_t pos;
                pos = line.find(' '); // we look for spaces in order to distinguish words

                while (pos != std::string::npos) {
                    wordsLoaded.push_back(line.substr(0, pos));
                    line.erase(0, pos + 1);
                    pos = line.find(' ');

                    counter += 1;

                    if(counter > chunk_size){ // every chunk of words is enqueued in the jobsQueue
                        jobsQueue.enqueue(wordsLoaded);
                        wordsLoaded.clear();
                        counter = 0;
                    }
                }
            }

            if(counter > 0){ // last chunk of data (it will be smaller in size)
                jobsQueue.enqueue(wordsLoaded);
                wordsLoaded.clear();
                counter = 0;
            }

            jobsQueue.producerEnd(); // notify that the PRODUCES work is done
        }

        /* CONSUMER THREAD */
        unsigned int len;
        std::string ngram;
        bool printed;

        std::vector<std::string> wordsChunk;

        tid = omp_get_thread_num();

        std::ofstream partOutData;
        partOutData.open(out_folder_parallel + "tmp/" + std::to_string(n) + "gram_outputParallelVersion"+std::to_string(tid)+".txt");

        while(!jobsQueue.done()){ // until the PRODUCER hasn't finished
            if(jobsQueue.dequeue(wordsChunk)){ // gather a job from the jobsQueue

                for (std::string &w: wordsChunk) {
                    len = w.length();
                    if (len >= n) {
                        for (int i = 0; i < len - n + 1; i++) {
                            // for each word we look for the consecutive substrings of length n (the ngrams)

                            ngram = w.substr(i, n);
                            printed = true;
                            for (char &c: ngram) {
                                if (!isalpha(c)) // if the ngram contains a symbol or a number it is discarded
                                    printed = false;
                                else
                                    c = (char) tolower(c);  // we produce an output of only lower-case letters
                            }

                            if (printed)
                                partOutData << ngram << std::endl; // write to file the ngram
                        }
                    }
                }
            }
        }
        partOutData.close();
    }

    /* SEQUENTIAL logic to merge all the partial output files */
    std::ifstream partOutData;

    std::ofstream totalOutData;
    totalOutData.open(out_folder_parallel + std::to_string(n) + "gram_outputParallelVersion.txt", std::ios_base::binary);

    for (const auto& entry : std::filesystem::directory_iterator(out_folder_parallel + "tmp/")){
        partOutData.open(entry.path(), std::ios_base::binary);
        totalOutData << partOutData.rdbuf(); // read from partial and write to total
        partOutData.close();
    }

    totalOutData.close();

    std::filesystem::remove_all(out_folder_parallel + "tmp/"); // clean up partial files
}

#endif //N_GRAM_PARALLELNGRAM_H
