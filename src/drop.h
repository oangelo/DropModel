#include <fstream>
#include <vector>
#include <cmath>

#include "gnuplot_i/gnuplot_i.h"

class Drop{
    public:
        Drop(size_t grid_size);
        
        size_t size() const;
        std::vector<int> &  operator[](size_t index) ;

    private:
        std::vector<std::vector<int>> grid;
        unsigned drop_ray;
        bool inside_drop(size_t i, size_t j);
};

class Gnuplot{
    public:
        Gnuplot();
        ~Gnuplot();
        void operator()(Drop & drop, unsigned time) const;
    private:
        gnuplot_ctrl * h;
};

