#include <iostream>
#include <fstream>
#include <map>
#include <string.h>
#include <vector>

std::vector<std::string> tokenize(std::string text) {
    char* token = strtok(const_cast<char*>(text.c_str()), " ");
    std::vector<std::string> result;
    while(token) {
        std::string word(token);
        result.push_back(word);
        token = strtok(NULL, " ");
    }
    return result;
}

std::map<std::string, std::map<std::string, int>> read_file(std::string input_file) {
    std::map<std::string, std::map<std::string, int>> data;
    std::fstream file(input_file);
    std::string text;
    while(getline(file, text)) {
        if(text == "END OF INPUT") 
            break;
        else{
            std::vector<std::string> line = tokenize(text);
            int cost = std::stoi(line[2]);
            auto start = data.find(line[0]);
            auto end   = data.find(line[1]);
            if(start == data.end()) {
                std::map<std::string, int> temp;
                temp[line[1]] = cost;
                data.insert( std::pair<std::string, std::map<std::string, int>>(line[0], temp) );
            }
            else {
                (data.find(line[0])->second).insert(std::pair<std::string, int>(line[1], cost));
            }

            if(end == data.end()) {
                std::map<std::string, int> temp;
                temp[line[0]] = cost;
                data.insert( std::pair<std::string, std::map<std::string, int>>(line[1], temp) );
            }
            else {
                (data.find(line[1])->second).insert(std::pair<std::string, int>(line[0], cost));
            }
        }
    }
    file.close();
    return data;
}

void uninformed_search(std::string input_file, std::string origin_city, std::string destination_city) {
    std::map<std::string, std::map<std::string, int>> data = read_file(input_file);
}

void informed_search(std::string input_file, std::string origin_city, std::string destination_city, std::string heuristic_file) {
    std::map<std::string, std::map<std::string, int>> data =  read_file(input_file);
}

int main(int argc, char* argv[]) {
    if(argc == 4) {
        //uninformed search
        std::string input_file(argv[1]);
        std::string origin_city(argv[2]);
        std::string destination_city(argv[3]);

        uninformed_search(input_file, origin_city, destination_city);
    }
    else if(argc == 5) {
        //informed search
        std::string input_file(argv[1]);
        std::string origin_city(argv[2]);
        std::string destination_city(argv[3]);
        std::string heuristic_file(argv[4]);

        informed_search(input_file, origin_city, destination_city, heuristic_file);
    }
    else {
        std::cout<<"Wrong number of command line arguments.\n";
    }
    return 0;
}