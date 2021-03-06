#include <string>
#include <sstream>
#include "drop.h"

int main(int argc, char** argv) {
    int grid_height = 40, grid_length = 40;
    int geo_parameter1 = 10, geo_parameter2 = 0;
    double J1 = 1, J2 = 1, g = 9.8, k = 1, temp = 1;
    size_t samples = 20;
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
            temp = atof(argv[i+4]);
        }
        if(std::string(argv[i]) == "--sample" || std::string(argv[i]) == "-sp" )
            samples = atof(argv[i+1]);

    }
    std::cerr << "#> Parameters: "  << std::endl;
    std::cerr << "#> J1 = " << J1 << std::endl;
    std::cerr << "#> J2 = " << J2 << std::endl;
    std::cerr << "#> g = " << g << std::endl;
    std::cerr << "#> temp = " << temp << std::endl;
    for (size_t i = 1; i < argc; ++i)
    {
        if(std::string(argv[i]) == "--plot"){
            Drop drop(grid_length, grid_height, geo_parameter1, geo_parameter2);
            Gnuplot plot;
            for (size_t i = 0; i < 10000; ++i)
            {
                plot(drop, 1);
                for (size_t i = 0; i < 200; ++i)
                    drop(g, J1, J2, temp);
            }
        }

        if(std::string(argv[i]) == "--print"){
            Drop drop(grid_length, grid_height, geo_parameter1, geo_parameter2);
            Print print_data;
            std::string file_name(argv[i+1]);
            for (size_t i = 0; i < 10000; ++i)
            {
                print_data(drop, file_name);
                for (size_t i = 0; i < 200; ++i)
                    drop(g, J1, J2, temp);
            }
        }

        if(std::string(argv[i]) == "--print-ensemble"){
            std::vector<Drop> drops(atoi(argv[i + 1]), Drop(grid_length, grid_height, geo_parameter1, geo_parameter2));
            Print print_data;
            std::string file_name(argv[i + 2]);
            for (size_t i = 0; i < 10000; ++i)
            {
                print_data(drops, file_name);
                for (size_t i = 0; i < 200; ++i)
                    for (size_t j = 0; j < drops.size(); ++j)
                        drops[j](g, J1, J2, temp);
            }
        }

        if(std::string(argv[i]) == "--movie"){
            std::vector<Drop> drops(atoi(argv[i + 1]), Drop(grid_length, grid_height, geo_parameter1, geo_parameter2));
            Print print_data;
            std::string file_name(argv[i + 2]), file_name_aux;
            unsigned counter = 0;
            for (size_t i = 0; i < 10000; ++i)
            {
                std::ostringstream oss;
                oss << counter;
                file_name_aux = file_name + "_" + oss.str();
                double y_cm_mean = 0;
                for(auto item: drops)
                    y_cm_mean += item.CenterOfMass().second;
                std::cout << y_cm_mean / drops.size() << std::endl;
                for (size_t i = 0; i < 200; ++i)
                    for (size_t j = 0; j < drops.size(); ++j)
                        drops[j](g, J1, J2, temp);
                print_data(drops, file_name_aux);
                counter++;
            }
        }

        if(std::string(argv[i]) == "--final-ensamble") {
            std::vector<Drop> drops(atoi(argv[i + 1]), Drop(grid_length, grid_height, geo_parameter1, geo_parameter2));
            Print print_data;
            std::string file_name(argv[i + 2]), file_name_aux;
            unsigned counter = atoi(argv[i + 3]);
            
            for (size_t i = 0; i < counter; ++i)
            {
                for (size_t j = 0; j < drops.size(); ++j)
                    drops[j](g, J1, J2, temp);
            }

            double y_cm_mean = 0;
            for(auto item: drops)
                y_cm_mean += item.CenterOfMass().second;
            std::cout << g << " " << y_cm_mean / drops.size() << std::endl;

            std::ostringstream oss;
            oss << g;
            file_name_aux = file_name + "_" + oss.str();
            print_data(drops, file_name_aux);
        }

        if(std::string(argv[i]) == "--mean-time") {
            Drop drop(grid_length, grid_height, geo_parameter1, geo_parameter2);
            Print print_data;

            std::string file_name(argv[i + 2]), file_name_aux;
            unsigned counter = atoi(argv[i + 3]);
            unsigned counter_final = 500;
            
            double y_cm_mean = 0;
            for (size_t i = 0; i < counter; ++i)
            {
                drop(g, J1, J2, temp);
            }
            MeanShape<Drop> drop_shape(drop);
            for (size_t i = 0; i < counter_final; ++i)
            {
                for (size_t i = 0; i < 2 * counter_final; ++i)
                {
                    drop(g, J1, J2, temp);
                }
                drop(g, J1, J2, temp);
                drop_shape(drop);
                y_cm_mean += drop.CenterOfMass().second;
            }

            std::cout << g << " " << y_cm_mean / counter_final << std::endl;

            std::ostringstream oss;
            oss << g;
            file_name_aux = file_name + "_" + oss.str();
            print_data(drop_shape, file_name_aux);
            //*/
        }



        if(std::string(argv[i]) == "--help" || std::string(argv[i]) == "-h"){
            std::cout << "Usage:" << std::endl;
            std::cout << "./drop --plot or --print [file name]" << std::endl;
            std::cout << "or" << std::endl;
            std::cout << "./drop --print-ensemble [number of samples]  [file name]" << std::endl;
            std::cout << "----------------------------------------------------------------------" << std::endl;
            std::cout << "--grid   \t receives two args [height] and [length]" << std::endl;
            std::cout << "--circle \t receives two args [radius] and [height of the center]" << std::endl;
            std::cout << "--model  \t receives four args [J1] ,[J2], [g], [T]. Where the J1, J2 are the spin interaction, g is the gravitational constant, and T is the temperature." << std::endl;
        }       
    }
    if(argc == 1){
        std::cout << "Usage:" << std::endl;
        std::cout << "./drop --plot or --print [file name]" << std::endl;
        std::cout << "or" << std::endl;
        std::cout << "./drop --print-ensemble [number of samples]  [file name]" << std::endl;
        std::cout << "----------------------------------------------------------------------" << std::endl;
        std::cout << "--grid   \t receives two args [height] and [length]" << std::endl;
        std::cout << "--circle \t receives two args [radius] and [height of the center]" << std::endl;
        std::cout << "--model  \t receives four args [J1] ,[J2], [g], [T]. Where the J1, J2 are the spin interaction, g is the gravitational constante, and T is the temperature.." << std::endl;
    }
    return 0;
}

