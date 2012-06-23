#include "drop.h"

Drop::Drop(size_t grid_lenght, size_t grid_hight, unsigned radius, double g, double noise): grid(), radius(radius), g(g), noise(noise)
{
    std::vector<int> row(grid_hight, -1); 
    for (size_t i = 0; i < grid_lenght; ++i)
    {
        grid.push_back(row);
    }
    //creating the drop
    for (size_t i = 0; i < grid.size(); ++i)
        for (size_t j = 0; j < grid[i].size(); ++j)
            if(inside_drop(i,j))
                grid[i][j] = 1;
    //creating the substract
    //    for (size_t i = 0; i < grid.size(); ++i)
    //        grid[i][0] = 1;
}

bool Drop::inside_drop(size_t i, size_t j){
    double distance = sqrt(pow(i - (grid.size()/2.0), 2) + pow(j, 2));
    if(distance < radius)
        return true;
    else
        return false;
}

std::vector<int>::iterator  Drop::get_wet(){
    //random number generator
    std::vector<std::vector<int>::iterator> changes; 
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1,grid.size()-2);
    while(true){
        size_t rand = dist(gen);
        std::vector<int> &random_vec = grid[rand];
        std::vector<int> value{-1};
        std::uniform_int_distribution<> dist_hight(0,grid[0].size()-1);
        unsigned rd_hight  = dist_hight(gen);
        if(grid[rand][rd_hight] == 1)
            if(rd_hight > 0){
                if(grid[rand-1][rd_hight] == -1 || grid[rand+1][rd_hight] == -1 || grid[rand][rd_hight+1] == -1 || grid[rand][rd_hight-1] == -1){
                    return(random_vec.begin() + rd_hight);
                }
            }else{
                if(grid[rand-1][rd_hight] == -1 || grid[rand+1][rd_hight] == -1 || grid[rand][rd_hight+1] == -1){
                    return(random_vec.begin() + rd_hight);
                }
            }

    }
}

std::vector<int>::iterator Drop::get_dry(){
    //random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1,grid.size()-2);
    while(true){
        size_t rand = dist(gen);
        std::vector<int> &random_vec = grid[rand];
        std::vector<int> value{-1};
        std::uniform_int_distribution<> dist_hight(0,grid[0].size()-1);
        unsigned rd_hight  = dist_hight(gen);
        if(grid[rand][rd_hight] == -1)
            if(rd_hight > 0){
                if(grid[rand-1][rd_hight] == 1 || grid[rand+1][rd_hight] == 1 || grid[rand][rd_hight+1] == 1 || grid[rand][rd_hight-1] == 1){
                    return(random_vec.begin() + rd_hight);
                }
            }else{
                if(grid[rand-1][rd_hight] == 1 || grid[rand+1][rd_hight] == 1 || grid[rand][rd_hight+1] == 1){
                    return(random_vec.begin() + rd_hight);
                }
            }

    }
}

void Drop::operator()(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0,1);

    double e = Energy();
    auto wet = this->get_wet();
    *wet = -*wet;
    auto dry = this->get_dry();
    *dry = -*dry;
    if(dist(gen) > noise)
        if(Energy() > e){
            *wet = -*wet;
            *dry = -*dry;
        }
}

double Drop::Energy() const{
    double energy = 0;
    int n = 1;
    //spin interaction term
    for (size_t i = n; i < grid.size()-n; ++i){
        int energy_neighbours = 0;
        for (size_t j = n; j < grid[i].size()-n; ++j){
            int sij = grid[i][j];
            for (int x = -n; x < (n+1); ++x)
                for (int y = -n; y < (n+1); ++y)
                    if((x != 0) && (y != 0))
                        energy_neighbours -= 2*sij * grid[i + x ][j + y ];
            energy += energy_neighbours;

        }
    }
    //gravitational field
    double g_energy = 0;
    for (size_t j = 0; j < grid[0].size(); ++j){
        double sum = 0;
        for (size_t i = 0; i < grid.size(); ++i){
            sum += grid[i][j];
        }
        g_energy += g * j * sum;  
    }
    energy += g_energy;
    return energy;  
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
    plot.open("buffer");
    for (size_t i = 0; i < drop.size(); ++i)
        for (size_t j = 0; j < drop[i].size(); ++j)
            if(drop[i][j] == -1)
                plot << i << " \t " << j << std::endl; 
    plot.close();

    char myfile[] = "buffer";   
    gnuplot_cmd(h, "plot  \"%s\" pt 7 ps 0.5 notitle", myfile);
    sleep(time);
}
