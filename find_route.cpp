#include <iostream>
#include <fstream>
#include <map>
#include <string.h>
#include <vector>
#include <deque>

struct node {
    std::vector<std::string> route;
    std::string city_name;
    int depth;
    int cumulative_cost;
};

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
        else {
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
    std::deque<struct node> fringe;

    struct node temp;
    temp.city_name = origin_city;
    //temp.parent_city = "\0";
    std::vector<std::string> r;
    temp.route = r;
    temp.depth = 0;
    temp.cumulative_cost = 0;
    fringe.push_back(temp);

    // Variables needed
    int nodes_expanded = 0, nodes_generated = 0, max_nodes = 1;
    while(!fringe.empty()) {
        // Updating max_nodes if current size is greater than max_nodes
        if(fringe.size() > max_nodes)
            max_nodes = fringe.size();
        
        struct node current_node = fringe[0];
        fringe.pop_front();
        //std::cout<<"Current City = "<<current_node.city_name<<std::endl;
        if(current_node.city_name == destination_city) {
            std::cout<<"\nNodes Expanded  = "<<nodes_expanded;
            std::cout<<"\nNodes Generated = "<<nodes_generated;
            std::cout<<"\nMax   Nodes     = "<<max_nodes;
            std::cout<<"\nDistance        = "<<current_node.cumulative_cost;
            std::cout<<"\nRoute: \n";
            int i;
            for(i = 0; i<current_node.route.size()-1;i++) {
                std::cout<<current_node.route[i]<<" to "<<current_node.route[i+1]<<std::endl;
            }
            std::cout<<current_node.route[i]<<" to "<<destination_city<<std::endl;
            return;
        }

        auto it = data.find(current_node.city_name);
        if(it!=data.end()) {
            nodes_expanded++;
            for(auto t: it->second) {
                nodes_generated++;
                temp.city_name = t.first;               //temp is already initialized before as a struct node, I am just using the variable again.
                r = current_node.route;                 //r is already initialized before as a vector of strings, I am just using the variable again.
                r.push_back(current_node.city_name);
                temp.route = r;
                temp.cumulative_cost = current_node.cumulative_cost + t.second;
                temp.depth = current_node.depth + 1;
                fringe.push_back(temp);
            }
        }
        
    }
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