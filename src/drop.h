#include <fstream>

#include "gnuplot_i/gnuplot_i.h"



class Gnuplot{
    public:
        Gnuplot();
        ~Gnuplot();
//        void operator()(Swarm & swarm, unsigned time) const;
    private:
        gnuplot_ctrl * h;
};

