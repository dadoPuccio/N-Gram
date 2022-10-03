#include <iostream>
#include <fstream>
#include <vector>


void sequentialNgram(std::string& file_name, int n);

int main() {
    std::string file_name = "../../Testi/Albertazzi.txt";
    sequentialNgram(file_name, 7);
}

void sequentialNgram(std::string& file_name, int n){
    std::ifstream infile(file_name);
    std::string line;
    std::getline(infile, line);

    std::vector<std::string> words;

    while(std::getline(infile, line)){

        size_t pos = line.find(' ');

        while(pos != std::string::npos){
            words.push_back(line.substr(0, pos));
            line.erase(0, pos + 1);
            pos = line.find(' ');
        }
    }

    unsigned int len = 0;
    std::string ngram;
    bool printed;

    for(std::string& w : words){
        len = w.length();
        if(len >= n){
            for(int i=0; i < len - n + 1; i++){

                ngram = w.substr(i, n);
                printed = true;
                for(char& c: ngram){
                    if(!isalpha(c)){
                        printed = false;
                    } else {
                        c = tolower(c);
                    }
                }

                if(printed)
                    std::cout << ngram << std::endl;
            }
        }
    }
}
