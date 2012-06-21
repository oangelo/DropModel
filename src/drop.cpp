#include "drop.h"

Drop::Drop(size_t grid_size):grid(), drop_ray(grid_size / 4) {
    for (size_t i = 0; i < grid_size; ++i)
    {
        std::vector<int> row(grid_size,0); 
        grid.push_back(row);
    }
    for (size_t i = 0; i < grid_size; ++i)
        for (size_t j = 0; j < grid_size; ++j)
            if(!inside_drop(i,j))
                grid[i][j] = 1;

    
}

bool Drop::inside_drop(size_t i, size_t j){
    double distance = sqrt(pow(i - (grid.size()/2.0), 2) + pow(j, 2));
    if(distance < drop_ray)
        return true;
    else
        return false;
}
size_t Drop::size() const { return grid.size(); }
std::vector<int> &  Drop::operator[](size_t index) { return grid[index]; }

Gnuplot::Gnuplot():h(){
    h = gnuplot_init() ; 
}

Gnuplot::~Gnuplot(){
    h = gnuplot_init() ; 
    gnuplot_close(h);
}

void Gnuplot::operator()(Drop &drop, unsigned time) const{
    std::ofstream plot;  
    plot.open("./DATA/buffer");
    for (size_t i = 0; i < drop.size(); ++i)
        for (size_t j = 0; j < drop.size(); ++j)
            if(drop[i][j])
                plot << i << " \t " << j << std::endl; 
    plot.close();

    char myfile[] = "DATA/buffer";   
    gnuplot_cmd(h, "plot  \"%s\" pt 7 ps 0.5 notitle", myfile);
    sleep(time);
}
