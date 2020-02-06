/*
    Pranav Bhandari
*/

#include <iostream>
#include <fstream>
#include <map>
#include <string.h>
#include <vector>
#include <deque>
#include <algorithm>

//Used class, as I might need extra functionality
class Node {
    public:
    std::vector<std::string> route;
    std::vector<int> costs;
    std::string city_name;
    int depth;
    int cumulative_cost;
    int f_n;                            // This value is only used in informed (A*) Search
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

std::map<std::string, int> heuristic_read_file(std::string heuristic_file) {
    std::fstream file(heuristic_file);
    std::string text;
    std::map<std::string, int> result;
    while(getline(file,text)) {
        if(text == "END OF INPUT") 
            break;
        std::vector<std::string> words = tokenize(text);
        result[words[0]] = std::stoi(words[1]);
    }
    return result;
}

void uninformed_search(std::string input_file, std::string origin_city, std::string destination_city) {
    // Uniform Cost Search is implemented using BFS
    // The algorithm starts at the origin city and searches breadth first, 
    // Already expanded nodes are deleted, to avoid loops
    // Fringe is sorted after each expansion to guarantee optimal solution.
    std::map<std::string, std::map<std::string, int>> data = read_file(input_file);
    std::deque<Node> fringe;

    //Creating the start node
    Node temp;
    temp.city_name = origin_city;
    std::vector<std::string> r;
    std::vector<int> c;
    temp.route = r;
    temp.costs = c;
    temp.depth = 0;
    temp.cumulative_cost = 0;
    //Adding the start node to the fringe
    fringe.push_back(temp);

    // Variables needed
    int nodes_expanded = 0, nodes_generated = 0, max_nodes = 1;
    while(!fringe.empty()) {
        // Updating max_nodes if current size is greater than max_nodes
        nodes_expanded++;
        if(fringe.size() > max_nodes)
            max_nodes = fringe.size();
        
        Node current_node = fringe[0];
        fringe.pop_front();
        
        // If the destination is found, print required details
        if(current_node.city_name == destination_city) {
            std::cout<<"\nnodes expanded: "<<nodes_expanded;
            std::cout<<"\nnodes generated: "<<nodes_generated;
            std::cout<<"\nmax nodes in memory: "<<max_nodes;
            std::cout<<"\ndistance: "<<current_node.cumulative_cost<<" km";
            std::cout<<"\nroute: \n";
            int i;
            for(i = 0; i<current_node.route.size()-1;i++) {
                std::cout<<current_node.route[i]<<" to "<<current_node.route[i+1]<<", "<<current_node.costs[i]<<" km"<<std::endl;
            }
            std::cout<<current_node.route[i]<<" to "<<destination_city<<", "<<current_node.costs[i]<<" km"<<std::endl;
            return;
        }

        // Find the node in the data read from the file, and expand the node
        auto it = data.find(current_node.city_name);
        
        if(it!=data.end()) {
            //Loop over all the adjacent cities and add them to fringe
            for(auto t: it->second) {
                nodes_generated++;
                temp.city_name = t.first;               //temp is already initialized before as a struct node, I am just using the variable again.
                r = current_node.route;                 //r is already initialized before as a vector of strings, I am just using the variable again.
                r.push_back(current_node.city_name);
                temp.route = r;
                c = current_node.costs;
                c.push_back(t.second);
                temp.costs = c;
                temp.cumulative_cost = current_node.cumulative_cost + t.second;
                temp.depth = current_node.depth + 1;
                fringe.push_back(temp);
            }
            data.erase(it);     //Deleting the expanded node, to make sure next time it produces no successors
                                //That makes this algorithm a graph search
            std::sort(fringe.begin(),fringe.end(),[](const Node& a, const Node& b) {  //sorting the fringe after each expansion
                return a.cumulative_cost < b.cumulative_cost;
            });
        }
    }
    //Node not found
    std::cout<<"\nnodes expanded: "<<nodes_expanded;
    std::cout<<"\nnodes generated: "<<nodes_generated;
    std::cout<<"\nmax nodes in memory: "<<max_nodes;
    std::cout<<"\ndistance: infinity";
    std::cout<<"\nroute: \nnone\n";
}

void informed_search(std::string input_file, std::string origin_city, std::string destination_city, std::string heuristic_file) {
    // Informed Search is implemented using A* Search,
    // The same class Node is used, which is used in Uninformed Search, 
    // but with an additional variable called f_n, which is the f(n) value for the particular node
    // The code is basically the same except that, the fringe is now sorted according to the f(n) value
    std::map<std::string, std::map<std::string, int>> data =  read_file(input_file);
    std::map<std::string, int> heuristic_data = heuristic_read_file(heuristic_file);

    std::deque<Node> fringe;

    //Creating the start node
    Node temp;
    temp.city_name = origin_city;
    std::vector<std::string> r;
    std::vector<int> c;
    temp.route = r;
    temp.costs = c;
    temp.depth = 0;
    temp.cumulative_cost = 0;
    temp.f_n = 0;
    //Adding the start node to the fringe
    fringe.push_back(temp);

    // Variables needed
    int nodes_expanded = 0, nodes_generated = 0, max_nodes = 1;
    while(!fringe.empty()) {
        // Updating max_nodes if current size is greater than max_nodes
        nodes_expanded++;
        if(fringe.size() > max_nodes)
            max_nodes = fringe.size();
        
        Node current_node = fringe[0];
        fringe.pop_front();

        // If the destination is found, print required details
        if(current_node.city_name == destination_city) {
            std::cout<<"\nnodes expanded: "<<nodes_expanded;
            std::cout<<"\nnodes generated: "<<nodes_generated;
            std::cout<<"\nmax nodes in memory: "<<max_nodes;
            std::cout<<"\ndistance:"<<current_node.cumulative_cost<<" km";
            std::cout<<"\nroute: \n";
            int i;
            for(i = 0; i<current_node.route.size()-1;i++) {
                std::cout<<current_node.route[i]<<" to "<<current_node.route[i+1]<<", "<<current_node.costs[i]<<" km"<<std::endl;
            }
            std::cout<<current_node.route[i]<<" to "<<destination_city<<", "<<current_node.costs[i]<<" km"<<std::endl;
            return;
        }

        // Find the node in the data read from the file, and expand the node
        auto it = data.find(current_node.city_name);
        
        if(it!=data.end()) {
            //Loop over all the adjacent cities and add them to fringe
            for(auto t: it->second) {
                nodes_generated++;
                temp.city_name = t.first;               //temp is already initialized before as a struct node, I am just using the variable again.
                r = current_node.route;                 //r is already initialized before as a vector of strings, I am just using the variable again.
                r.push_back(current_node.city_name);
                temp.route = r;
                c = current_node.costs;
                c.push_back(t.second);
                temp.costs = c;
                temp.cumulative_cost = current_node.cumulative_cost + t.second;
                temp.depth = current_node.depth + 1;
                temp.f_n = temp.cumulative_cost + heuristic_data.find(t.first)->second;
                fringe.push_back(temp);
            }
            data.erase(it);     //Deleting the expanded node, to prevent loops
            std::sort(fringe.begin(),fringe.end(),[](const Node& a, const Node& b) {  //sorting the fringe after each expansion
                return a.f_n < b.f_n;
            });
        }
    }  
    //Node not found
    std::cout<<"\nnodes expanded: "<<nodes_expanded;
    std::cout<<"\nnodes generated: "<<nodes_generated;
    std::cout<<"\nmax nodes in memory: "<<max_nodes;
    std::cout<<"\ndistance: infinity";
    std::cout<<"\nroute: \nnone\n";
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