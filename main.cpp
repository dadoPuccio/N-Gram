#include <iostream>
#include <filesystem>
#include <omp.h>

#include "SequentialNgram.h"
#include "ParallelNgram.h"
#include "Utils.h"

int main() {

    std::string file_name;

    // file_name = mergeBooks("../../Testi/", "MergedBooks.txt");

    // file_name = createFileWith_M_Words("../../Testi/", "../../GeneratedBooks/100K.txt", 100000);
    // file_name = createFileWith_M_Words("../../Testi/", "../../GeneratedBooks/250K.txt", 250000);
    // file_name = createFileWith_M_Words("../../Testi/", "../../GeneratedBooks/500K.txt", 500000);
    // file_name = createFileWith_M_Words("../../Testi/", "../../GeneratedBooks/1M.txt", 1000000);
    // file_name = createFileWith_M_Words("../../Testi/", "../../GeneratedBooks/2.5M.txt", 2500000);
    // file_name = createFileWith_M_Words("../../Testi/", "../../GeneratedBooks/5M.txt", 5000000);
    // file_name = createFileWith_M_Words("../../Testi/", "../../GeneratedBooks/10M.txt", 10000000);
    // file_name = createFileWith_M_Words("../../Testi/", "../../GeneratedBooks/25M.txt", 25000000);
    // file_name = createFileWith_M_Words("../../Testi/", "../../GeneratedBooks/50M.txt", 50000000);
    // file_name = createFileWith_M_Words("../../Testi/", "../../GeneratedBooks/100M.txt", 100000000);
    // file_name = createFileWith_M_Words("../../Testi/", "../../GeneratedBooks/200M.txt", 200000000);
    // file_name = createFileWith_M_Words("../../Testi/", "../../GeneratedBooks/300M.txt", 300000000);
    // file_name = createFileWith_M_Words("../../Testi/", "../../GeneratedBooks/400M.txt", 400000000); // Too Large for 16GB RAM
    // file_name = createFileWith_M_Words("../../Testi/", "../../GeneratedBooks/500M.txt", 500000000); // Too Large for 16GB RAM


    std::string out_folder_sequential = "../OutSequential/";
    std::string out_folder_parallel = "../OutParallel/";

    if(std::filesystem::exists(out_folder_sequential)){
        // std::filesystem::remove_all("../OutSequential/");
    } else {
        std::filesystem::create_directory(out_folder_sequential); // create the folder if it doesn't exist
    }

    if(std::filesystem::exists(out_folder_parallel)){
        // std::filesystem::remove_all("../OutSequential/");
    } else {
        std::filesystem::create_directory(out_folder_parallel); // create the folder if it doesn't exist
    }

    int n_runs = 10;
    int n = 2; // n-gram
    int chunk_size = 10000; // chunk_size of jobs in the parallel implementation
    int n_threads = 8;

    // file_name = "../../GeneratedBooks/10M.txt";
    // file_name = "../../TooLarge/400M.txt";
    file_name = "../../Testi/Proust.txt";

    std::cout << "#########################" << std::endl;
    std::cout << file_name << std::endl;
    std::cout << "#########################" << std::endl;

    countWords(file_name);

    double start, end, s, p;
    double avgSequentialTime, avgParallelTime, avgSpeedup;
    avgSequentialTime = 0;
    avgParallelTime = 0;
    avgSpeedup = 0;

    for (int i = 0; i < n_runs; i++) {

        // Sequential Solution
        start = omp_get_wtime();
        sequentialNgramWords(file_name, out_folder_sequential, n);
        end = omp_get_wtime();
        s = end - start;
        avgSequentialTime += s;
        std::cout << "Sequential Time: " << s << std::endl;

        // Parallel Solution
        start = omp_get_wtime();
        parallelNgramWords(file_name, out_folder_parallel, n, chunk_size, n_threads);
        end = omp_get_wtime();
        p = end - start;
        avgParallelTime += p;
        std::cout << "Parallel Time: " << p << std::endl;

        avgSpeedup += s / p;
        std::cout << "Speedup: " << s / p << std::endl << std::endl;
    }

    avgSequentialTime = avgSequentialTime / n_runs;
    avgParallelTime = avgParallelTime / n_runs;
    avgSpeedup = avgSpeedup / n_runs;

    std::cout << std::endl << "Average Sequential Time: " << avgSequentialTime << std::endl;
    std::cout << "Average Parallel Time: " << avgParallelTime << std::endl;
    std::cout << "Average Speedup: " << avgSpeedup << std::endl;
    std::cout << "Average Speedup: " << avgSequentialTime / avgParallelTime << std::endl;

    std::cout << std::endl << std::endl;

}



