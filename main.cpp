#include <iostream>
#include <filesystem>
#include <omp.h>

#include "SequentialNgram.h"
#include "ParallelNgram.h"
#include "Utils.h"

int main() {

    int n = 5; // n-gram
    int chunk_size = 3000; // chunk_size of jobs in the parallel implementation

    std::string file_name = "../../Testi/Proust.txt";

    file_name = mergeBooks("../../Testi/", "MergedBooks.txt");

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

    // Sequential Solution
    std::cout << "Sequential solution..." << std::endl;
    double start = omp_get_wtime();
    sequentialNgramWords(file_name, out_folder_sequential, n);
    double end = omp_get_wtime( );
    std::cout << "Time: " << end-start << std::endl << std::endl;

    // Parallel Solution
    std::cout << "Parallel solution..." << std::endl;
    start = omp_get_wtime();
    parallelNgramWords(file_name, out_folder_parallel, n, chunk_size, 8);
    end = omp_get_wtime( );
    std::cout << "Time: " << end-start << std::endl << std::endl;

}



