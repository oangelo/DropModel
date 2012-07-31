#include <fstream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <iostream>
#include <map>
#include <string>

#include "gnuplot_i/gnuplot_i.h"

class Drop{
    public:
        Drop(size_t grid_length, size_t grid_height, unsigned geometrical_parameter_one, unsigned geometrical_parameter_two);
        size_t size() const;
        std::vector<int> &  operator[](size_t index) ;
        std::pair<double, double> CenterOfMass() const;
        double GravitationalEnergy(double g) const;
        //double BulkEnergy(double k) const;
        double InteractionEnergy(double J1, double J2) const;
        //iterate the simulation
        void operator()(double g = 9.8, double J1 = 1, double J2 = 1, double temperature = 0.0);

    private:
        std::vector<std::vector<int>> grid;
        unsigned drop_ray;
        bool inside_drop(size_t i, size_t j);

        std::vector<int>::iterator  get_wet();
        std::vector<int>::iterator get_dry();
        unsigned geometrical_parameter_one, geometrical_parameter_two;
        bool ignore_fist_row;
        const unsigned wet = 1, dry = -1;
};

class Print{
    public:
        void operator()(Drop & drop, std::string file_name) const;
        void operator()(std::vector<Drop> &drops, std::string file_name) const;
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

