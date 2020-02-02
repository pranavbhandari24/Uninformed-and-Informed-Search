#include <iostream>

void uninformed_search(std::string input_file, std::string origin_city, std::string destination_city) {

}

void informed_search(std::string input_file, std::string origin_city, std::string destination_city, std::string heuristic_file) {
    
}

void main(int argc, char* argv[]) {
    if(argc == 3) {
        //uninformed search
        std::string input_file(argv[0]);
        std::string origin_city(argv[1]);
        std::string destination_city(argv[2]);

        uninformed_search(input_file, origin_city, destination_city);
    }
    else if(argc == 4) {
        //informed search
        std::string input_file(argv[0]);
        std::string origin_city(argv[1]);
        std::string destination_city(argv[3]);
        std::string heuristic_file(argv[4]);

        informed_search(input_file, origin_city, destination_city, heuristic_file);
    }
    else {
        std::cout<<"Wrong number of command line arguments.\n";
    }
}