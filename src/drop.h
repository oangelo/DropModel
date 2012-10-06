#include <fstream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <iostream>
#include <map>
#include <string>

#include "gnuplot_i/gnuplot_i.h"


typedef std::vector<int> column;
typedef std::vector<column> matrix;

template <typename element>
class MeanShape;

class Drop{

    public:

        Drop(size_t grid_length, size_t grid_height, unsigned geometrical_parameter_one, unsigned geometrical_parameter_two);
        size_t size() const;
        const std::vector<int> &  operator[](size_t index) ;
        std::pair<double, double> CenterOfMass() const;
        //double BulkEnergy(double k) const;
        //iterate the simulation
        void operator()(double g = 9.8, double J1 = 1, double J2 = 1, double temperature = 0.0);

    private:
        
        matrix grid;
        unsigned drop_ray;
        bool inside_drop(size_t i, size_t j);

        std::vector<int>::iterator  get_wet();
        std::vector<int>::iterator get_dry();
        unsigned geometrical_parameter_one, geometrical_parameter_two;
        bool ignore_fist_row;
        const unsigned wet = 1, dry = -1;
};

double GravitationalEnergy(const matrix & grid, double g);
double InteractionEnergy(const matrix & grid, double J1, double J2);

bool IsOnEdge(size_t i, size_t j, const matrix & grid);
void GetBorders(matrix & grid, int color_one, int color_two, 
                std::vector<column::iterator> & one, 
                std::vector<column::iterator> & two, unsigned substract_height = 0);

class Print{
    public:
        //void operator()(Drop & drop, std::string file_name) const;
        void operator()(std::vector<Drop> &drops, std::string file_name) const;
        void operator()(const MeanShape<Drop> & drop, const std::string & file_name) const;

        template<class element>
        void operator()(element & item, std::string file_name) const{
            std::ofstream file;  
            file.open(file_name);
            for (size_t i = 0; i < item.size(); ++i)
                for (size_t j = 0; j < (item[i]).size(); ++j)
                    if(item[i][j] < 0)
                        file << i << " \t " << j << std::endl; 
            file.close();
            
        }

};

class Gnuplot{
    public:
        Gnuplot();
        ~Gnuplot();
        void operator()(std::vector<Drop> &drops, unsigned time) const;
        void mean(std::vector<Drop> &drops, unsigned time) const;
        void operator()(Drop & drop, unsigned time) const;
    private:
        gnuplot_ctrl * h;
};

template <typename element>
class MeanShape{

    public:

        MeanShape(element & drop):grid(), counter(1){
            for(size_t i = 0; i < drop.size(); ++i){
                std::vector<double> aux; 
                for(auto item: (drop[i]))
                    aux.push_back(item);
                grid.push_back(aux);
            }
        };
        

        void  operator()(element & drop){
            for(size_t i = 0; i < drop.size(); ++i){
                for(size_t j = 0; j < drop[i].size(); ++j)
                   grid[i][j] = (drop[i][j] + grid[i][j] * counter) / (counter + 1); 
            }
            ++counter;
        };

        const std::vector<double> &  operator[](size_t index){return grid[index];};
        size_t size() const {return grid.size();};

    private:

        std::vector<std::vector<double>> grid;
        unsigned counter;
};
