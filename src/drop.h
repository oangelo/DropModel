#include <fstream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <iostream>
#include <map>

#include "gnuplot_i/gnuplot_i.h"

class Drop{
    public:
        Drop(size_t grid_length, size_t grid_height, unsigned geometrical_parameter_one, unsigned geometrical_parameter_two, 
             double g = 9.8, double k = 1, double J1 = 1, double J2 = 1, double noise = 0.0, bool ignore_fist_row = false);
        size_t size() const;
        std::vector<int> &  operator[](size_t index) ;
        double Energy() const;
        double BulkEnergy() const;
        std::pair<double, double> CenterOfMass() const;
        //iterate the simulation
        void operator()();

    private:
        std::vector<std::vector<int>> grid;
        unsigned drop_ray;
        bool inside_drop(size_t i, size_t j);

        std::vector<int>::iterator  get_wet();
        std::vector<int>::iterator get_dry();
        unsigned geometrical_parameter_one, geometrical_parameter_two;
        double  J1, J2, g, k, noise;
        bool ignore_fist_row;
        const unsigned wet = 1, dry = -1;
};

class Gnuplot{
    public:
        Gnuplot();
        ~Gnuplot();
        void operator()(Drop & drop, unsigned time) const;
    private:
        gnuplot_ctrl * h;
};

