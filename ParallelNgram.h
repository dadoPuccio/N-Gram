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
#include "Utils.h"

void parallelNgramWords(std::string& file_name, std::string& out_folder_parallel, int n, int chunk_size, int num_threads);

void parallelNgramWords(std::string& file_name, std::string& out_folder_parallel, int n, int chunk_size, int num_threads){
    /* Parallel implementation of an algorithm to extract words N-grams
     * This implementation is based on Producer-Consumers pattern */
    std::ifstream infile(file_name);
    JobsQueue jobsQueue; // Queue in which jobs are stored

    int tid;

    /* Each Consumer thread will produce an output file with partial data in a /tmp/ folder,
     * at the end it will be merged into one single file. */
    std::filesystem::create_directory(out_folder_parallel + "tmp/");

    #pragma omp parallel num_threads(num_threads) default(none) shared(infile, out_folder_parallel, jobsQueue, n, chunk_size, std::cout) private(tid)
    {
        #pragma omp single nowait
        {
            /* PRODUCER THREAD: reads the input file and generates chunks of words (vector of strings)
             * which are enqueued and passed to CONSUMERS threads */

            std::vector<std::string> wordsLoaded;
            std::string border[n-1];

            std::string line;
            std::string processedLine;
            int counter;
            size_t pos;

            counter = 0;

            while (std::getline(infile, line)) {

                /* Remove from the line what is not a letter nor a space */
                std::remove_copy_if(
                        line.begin(),
                        line.end(),
                        std::back_inserter(processedLine),
                        std::ptr_fun<char&,bool>(&processInputChar));

                pos = processedLine.find(' '); // we look for spaces in order to distinguish words
                // std::cout << processedLine << std::endl;

                while (pos != std::string::npos) {
                    if(pos > 0) { // in such a way we ignore double spaces
                        wordsLoaded.push_back(processedLine.substr(0, pos));
                        counter += 1;
                    }

                    processedLine.erase(0, pos + 1);
                    pos = processedLine.find(' ');

                    if (counter >= chunk_size) { // every chunk of words is enqueued in the jobsQueue
                        jobsQueue.enqueue(wordsLoaded);

                        for(int i=0; i < n-1; i++) // we need to save the last n-1 words as they are needed for the next chunk
                            border[i] = wordsLoaded[chunk_size - n + 1 + i];

                        wordsLoaded.clear(); // clear the local current chunk

                        for(int i=0; i < n-1; i++) // add the last n-1 words
                            wordsLoaded.push_back(border[i]);
                        counter = n-1;
                    }
                }

                processedLine.clear();
            }

            if(counter > 0){ // last chunk of data (it will be smaller in size)
                jobsQueue.enqueue(wordsLoaded);
            }

            jobsQueue.producerEnd(); // notify that the PRODUCES work is done
            // std::cout << "PRODUCER has finished reading" << std::endl;
        }

        /* CONSUMER THREAD */
        std::vector<std::string> wordsChunk;

        tid = omp_get_thread_num();

        std::ofstream partOutData; // init part of output stream
        partOutData.open(out_folder_parallel + "tmp/" + std::to_string(n) + "gram_outputParallelVersion"+std::to_string(tid)+".txt");

        std::string ngram;
        size_t pos;

        while(!jobsQueue.done()){ // until the PRODUCER hasn't finished
            if(jobsQueue.dequeue(wordsChunk)){ // gather a job from the jobsQueue

                 //std::cout << "CONSUMER is working..." << std::endl;

                ngram = "";

                /* First output */
                for(int j=0; j < n; j++)
                    ngram += wordsChunk[j] + " ";
                partOutData << ngram << std::endl;

                pos = ngram.find(' '); // detect where the first space is in order to locate the first word

                /* Following outputs */
                for(int i=n; i < wordsChunk.size(); i++){
                    ngram.erase(0, pos + 1); // we remove the first word in the previous output
                    ngram += wordsChunk[i] + " "; // and concatenate the last word for the following output

                    partOutData << ngram << std::endl;
                    pos =  ngram.find(' ');
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
        if(std::filesystem::file_size(entry.path()) > 0) { // as empty files might be created with large chunk sizes
            partOutData.open(entry.path(), std::ios_base::binary);
            totalOutData << partOutData.rdbuf(); // read from partial and write to total
            partOutData.close();
        }
    }

    totalOutData.close();

    std::filesystem::remove_all(out_folder_parallel + "tmp/"); // clean up partial files
}

#endif //N_GRAM_PARALLELNGRAM_H
