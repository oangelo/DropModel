#include "drop.h"

Drop::Drop(size_t grid_lenght, size_t grid_hight):grid(), drop_ray(grid_lenght / 8) {
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
    for (size_t i = 0; i < grid.size(); ++i)
        grid[i][0] = 1;
}

bool Drop::inside_drop(size_t i, size_t j){
    double distance = sqrt(pow(i - (grid.size()/2.0), 2) + pow(j, 2));
    if(distance < drop_ray)
        return true;
    else
        return false;
}

std::vector<int>::iterator  Drop::get_wet(){
    bool invalid = true;
    //random number generator
    std::vector<std::vector<int>::iterator> changes; 
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1,grid.size()-2);
    while(invalid){
        size_t rand = dist(gen);
        std::vector<int> &random_vec = grid[rand];
        std::vector<int> value{-1};

        auto dry = std::find_first_of(random_vec.begin(), random_vec.end(),value.begin(),value.end());
        size_t distance =  std::distance(random_vec.begin(),dry-1);
        if(distance > 0){
                return(dry-1);
        }
    }
}

std::vector<int>::iterator Drop::get_dry(){
    bool invalid = true;
    //random number generator
    std::vector<std::vector<int>::iterator> changes; 
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1,grid.size()-2);
    while(invalid){
        size_t rand = dist(gen);
        std::vector<int> &random_vec = grid[rand];
        std::vector<int> value{-1};

        auto dry = std::find_first_of(random_vec.begin(), random_vec.end(),value.begin(),value.end());
        size_t distance =  std::distance(random_vec.begin(),dry);
        //if(distance == 1){
            if(grid[rand-1][distance] == 1 || grid[rand+1][distance] == 1){
                return(dry);
            }
        /*}else{
            return(dry);
        }
        */
               
    }
}

std::vector<std::vector<int>::iterator> Drop::RandomChange(){
    double e = Energy();
    auto wet = this->get_wet();
    *wet = -*wet;
    auto dry = this->get_dry();
    *dry = -*dry;
    if(Energy() > e){
        *wet = -*wet;
        *dry = -*dry;
    }
 
    //std::cout << *dry <<   std::endl;
    /*
    for(auto i: grid[rd_vector])
        std::cout << i << " ";
    std::cout <<  std::endl;
    //if(lower != grid[rd_vector].end()) 
    //std::cout << std::distance(grid[rd_vector].begin(),lower) << std::endl;
    *lower = -*lower;
    for(auto i: grid[rd_vector])
        std::cout << i << " ";
    std::cout <<  std::endl;
   */ 
    std::vector<std::vector<int>::iterator> changes;
    return changes;
}

double Drop::Energy() const{
    double energy = 0;
    double g = 9.8;
    //spin interaction term
    for (size_t i = 1; i < grid.size()-1; ++i){
        int energy_neighbours = 0;
        for (size_t j = 1; j < grid[i].size()-1; ++j){
            int sij = grid[i][j];
            for (int x = -1; x < 2; x += 1)
                for (int y = -1; y < 2; y += 1)
                    if((x != 0) && (y != 0))
                        energy_neighbours -= sij * grid[i + x ][j + y ];
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
    plot.open("./DATA/buffer");
    for (size_t i = 0; i < drop.size(); ++i)
        for (size_t j = 0; j < drop[i].size(); ++j)
            if(drop[i][j] == -1)
                plot << i << " \t " << j << std::endl; 
    plot.close();

    char myfile[] = "DATA/buffer";   
    gnuplot_cmd(h, "plot  \"%s\" pt 7 ps 0.5 notitle", myfile);
    sleep(time);
}
