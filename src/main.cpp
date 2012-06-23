#include <string>
#include "drop.h"

int main(int argc, char** argv) {
    for (size_t i = 1; i < argc; ++i)
    {
        if(std::string(argv[i]) == "--plot"){
            Drop drop(atoi(argv[i+1]),atoi(argv[i+2]),atoi(argv[i+3]),atof(argv[i+4]),atoi(argv[i+5]));
            Gnuplot plot;
            for (size_t i = 0; i < 10000; ++i)
            {
                for (size_t i = 0; i < 200; ++i)
                    drop();
                plot(drop, 1);
            }
        }
        if(std::string(argv[i]) == "--help" || std::string(argv[i]) == "-h"){
            std::cout << "Usage:" << std::endl;
            std::cout << "./drop --plot [grid hight (int)] [grid lenght (int)] [radius (int)] [g (double)] [noise (double [0-1])]" << std::endl;
        }
    }
    if(argc == 1){
        std::cout << "Usage:" << std::endl;
        std::cout << "./drop --plot [grid hight (int)] [grid lenght (int)] [radius (int)] [g (double)] [noise (double [0-1])]" << std::endl;
    }
    return 0;
}

