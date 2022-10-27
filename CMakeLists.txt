cmake_minimum_required(VERSION 3.3)
project(n_gram)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

add_executable(n_gram main.cpp JobsQueue.h SequentialNgram.h ParallelNgram.h Utils.h SharedHistogram.h HistogramCollector.h PartialHistogramsQueue.h)
