#include <string>
#include "drop.h"

int main(int argc, char** argv) {
    int grid_height = 40, grid_length = 40;
    int geo_parameter1 = 10, geo_parameter2 = 0;
    double J1 = 1, J2 = 1, g = 9.8, k = 1;
    for (size_t i = 1; i < argc; ++i)
    {
        if(std::string(argv[i]) == "--grid"){
                grid_length = atoi(argv[i+1]);
                grid_height = atoi(argv[i+2]);
            }
        if(std::string(argv[i]) == "--circle"){
                geo_parameter1 = atoi(argv[i+1]);
                geo_parameter2 = atoi(argv[i+2]);
            }
        if(std::string(argv[i]) == "--model"){
                J1 = atof(argv[i+1]);
                J2 = atof(argv[i+2]);
                g = atof(argv[i+3]);
                k = atof(argv[i+4]);
            }
    }
    std::cout << "Parameters: "  << std::endl;
    std::cout << "J1 = " << J1 << std::endl;
    std::cout << "J2 = " << J2 << std::endl;
    std::cout << "g = " << g << std::endl;
    std::cout << "k = " << k << std::endl;
    for (size_t i = 1; i < argc; ++i)
    {
        if(std::string(argv[i]) == "--plot"){
            Drop drop(grid_length, grid_height, geo_parameter1, geo_parameter2);
            Gnuplot plot;
            for (size_t i = 0; i < 10000; ++i)
            {
                for (size_t i = 0; i < 200; ++i)
                    drop(g, k, J1, J2);
                plot(drop, 1);
            }
        }
     if(std::string(argv[i]) == "--help" || std::string(argv[i]) == "--help"){
        std::cout << "Usage:" << std::endl;
        std::cout << "./drop --plot" << std::endl;
        std::cout << "--grid   \t receives two args [height] and [length]" << std::endl;
        std::cout << "--circle \t receives two args [radius] and [height of the center]" << std::endl;
        std::cout << "--model  \t receives four args [J1] ,[J2], [g], [k]. Where the J1, J2 are the spin interaction, g is the gravitational constant, and k is the intensity of the central force." << std::endl;
    }       
    }
    if(argc == 1){
        std::cout << "Usage:" << std::endl;
        std::cout << "./drop  --plot" << std::endl;
        std::cout << "--grid   \t receives two args [height] and [length]" << std::endl;
        std::cout << "--circle \t receives two args [radius] and [height of the center]" << std::endl;
        std::cout << "--model  \t receives four args [J1] ,[J2], [g], [k]. Where the J1, J2 are the spin interaction, g is the gravitational constant, and k is the intensity of the central force." << std::endl;
    }
    return 0;
}

