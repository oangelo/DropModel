#include <fstream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <iostream>

#include "gnuplot_i/gnuplot_i.h"

class Drop{
    public:
        Drop(size_t grid_lenght, size_t grid_hight);
        
        size_t size() const;
        std::vector<int> &  operator[](size_t index) ;
        double Energy() const;
        std::vector<std::vector<int>::iterator> RandomChange();

    private:
        std::vector<std::vector<int>> grid;
        unsigned drop_ray;
        bool inside_drop(size_t i, size_t j);

        std::vector<int>::iterator  get_wet();
        std::vector<int>::iterator get_dry();
};

class Gnuplot{
    public:
        Gnuplot();
        ~Gnuplot();
        void operator()(Drop & drop, unsigned time) const;
    private:
        gnuplot_ctrl * h;
};

