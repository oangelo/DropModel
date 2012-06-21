#include "drop.h"

Gnuplot::Gnuplot():h(){
    h = gnuplot_init() ; 
}

Gnuplot::~Gnuplot(){
    h = gnuplot_init() ; 
    gnuplot_close(h);
}
/*
void Gnuplot::operator()(Swarm &swarm, unsigned time) const{
    std::ofstream plot;  
    plot.open("./DATA/buffer");
    for (size_t i = 0; i < swarm.size(); ++i)
        plot << swarm[i]->get_position()[0] << " \t " << swarm[i]->get_position()[1] << std::endl; 
    plot.close();

    char myfile[] = "DATA/buffer";   
    gnuplot_cmd(h, "plot  \"%s\" pt 7 ps 2 notitle", myfile);
    sleep(time);
}
*/
