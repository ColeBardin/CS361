#include <iostream>

int main(int argc, char **argv){
    if(argc != 3){
        std::cerr << "ERROR: Incorrect number of arguments." << std::endl;
        std::cout << "USAGE: " << argv[0] << " [target_string] [directory]" << std::endl;
        return 1;
    }


    return 0;
}
