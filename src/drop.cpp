#include "drop.h"

Drop::Drop(size_t grid_length, size_t grid_height, unsigned geometrical_parameter_one, unsigned geometrical_parameter_two)
: grid(grid_height, std::vector<int>(grid_length, -1)), geometrical_parameter_one(geometrical_parameter_one), geometrical_parameter_two(geometrical_parameter_two), ignore_fist_row(true)
{
    //creating the drop
    for (size_t i = 0; i < grid.size(); ++i)
        for (size_t j = 0; j < grid[i].size(); ++j)
            if(inside_drop(i,j))
                grid[i][j] = 1;
//    creating the substract
        for (size_t i = 0; i < grid.size(); ++i)
            grid[i][0] = -1;
}

bool Drop::inside_drop(size_t i, size_t j){
    double distance = sqrt(pow(i - (grid.size()/2.0), 2.0) + pow(j - (grid[0].size()/2.0) + geometrical_parameter_two, 2.0));
    if(distance < geometrical_parameter_one)
        return true;
    else
        return false;
}


void Drop::operator()(double g, double J1, double J2, double temperature){
    double beta = 1 / temperature;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0,1);

    std::vector<column::iterator>  dry_element; 
    std::vector<column::iterator>  wet_element;

    GetBorders(grid, wet, dry, wet_element, dry_element);
    std::uniform_int_distribution<> dry_dist(0,dry_element.size()-1);
    std::uniform_int_distribution<> wet_dist(0,wet_element.size()-1);

    double e_init = GravitationalEnergy(grid, g) + InteractionEnergy(grid, J1, J2);
    column::iterator &  wet = wet_element[wet_dist(gen)];
    *wet = -*wet;
    column::iterator & dry = dry_element[dry_dist(gen)];
    *dry = -*dry;
    double e_end = GravitationalEnergy(grid, g) +  InteractionEnergy(grid, J1, J2);
    //std::cout << e_end - e_init << std::endl;  
    if(!(e_end - e_init < 0)){
        if(dist(gen) > exp(-beta*(e_end-e_init))){
            *wet = -*wet;
            *dry = -*dry;
        }
    }
}

double InteractionEnergy(const matrix & grid, double J1, double J2){
    double energy = 0;
    //spin interaction term
    for (size_t i = 2; i < grid.size()-2; ++i){
        for (size_t j = 2; j < grid[i].size()-2; ++j){

            int sij = grid[i][j];

            for (int x = -1; x < 2; ++x) {
                for (int y = -1; y < 2; ++y) {
                    if((x != 0) && (y != 0)){
                        energy -= J1 *sij * grid[i + x][j + y];
                        //std::cout << "e: " << J1 *sij * grid[i + x][j + y] << std::endl;
                    }
                }
            }
            
            for (int x = -2; x < 3; x += 2) {
                for (int y = -2; y < 3; y += 2) {
                    if((x != 0) && (y != 0)) {
                        energy -= J2 *sij * grid[i + x][j + y];
                    }
                }
            }
            
            //std::cout << energy << std::endl;
        }
    }
    return energy;  
}
 
double GravitationalEnergy(const matrix & grid, double g) {
    //gravitational field
    double g_energy = 0;
    for (size_t j = 0; j < grid[0].size(); ++j){
        double sum = 0;
        for (size_t i = 0; i < grid.size(); ++i){
            sum += grid[i][j];
        }
        g_energy += g * (j+1) * sum;  
    }
    return g_energy;  
}

std::pair<double, double> Drop::CenterOfMass() const{
    std::pair<double, double> cm;
    double x_cm = 0, y_cm = 0;
    unsigned counter = 0;
    for (size_t i = 0; i < grid.size(); ++i)
        for (size_t j = 0; j < grid[i].size(); ++j)
            if(grid[i][j] == 1){
                x_cm += i;
                y_cm += j;
                counter++;
            }
    cm.first = x_cm / counter;
    cm.second = y_cm / counter;
    return cm;
}

size_t Drop::size() const { return grid.size(); }
std::vector<int> &  Drop::operator[](size_t index) { return grid[index]; }

bool IsOnEdge(size_t i, size_t j, const matrix & grid){
    if(i <  grid.size() - 1) {
        if(j < grid[i].size() - 1)
            if(grid[i][j] != grid[i + 1][j + 1])
                return true;
        if(j > 0)
            if(grid[i][j] != grid[i + 1][j - 1])
                return true;
        if(grid[i][j] != grid[i + 1][j])
            return true;
    }
    if(i > 0) {
        if(j > 0)
            if(grid[i][j] != grid[i - 1][j - 1])
                 return true;
        if(grid[i][j] != grid[i - 1][j])
            return true;
        if(j < grid[i].size() - 1)
            if(grid[i][j] != grid[i - 1][j + 1])
                return true;
    }
  
    if(j < grid[i].size() - 1)
        if(grid[i][j] != grid[i][j + 1])
            return true;
    if(j > 0)
        if(grid[i][j] != grid[i][j - 1])
            return true;

}

//*
void GetBorders(matrix & grid, int color_one, int color_two,  
                std::vector<column::iterator> & one, 
                std::vector<column::iterator> & two) {
    for(size_t i = 0; i < grid.size(); ++i) { 
        for(size_t j = 0; j < grid[i].size(); ++j) {
            if(IsOnEdge(i, j, grid)) {
                column &vec = grid[i];
                if(j > 1 && i > 1 && i < grid.size() - 2) //should not atualize the substract
                if(grid[i][j] == color_one) {
                    one.push_back(vec.begin() + j);
                }else{
                    if(grid[i][j] == color_two)
                        two.push_back(vec.begin() + j);
                }
    
            }
        }
    }
}
//*/
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

void Gnuplot::operator()(std::vector<Drop> &drops, unsigned time) const{
    std::vector<std::vector<double>> mean_grid;
    for(size_t i = 0; i < drops[0].size(); ++i)
         mean_grid.push_back(std::vector<double>(drops[0][0].size(),0.0));
    for(auto droplet : drops)
        for(size_t i = 0;i < droplet.size(); ++i)
            for(size_t j = 0;j < droplet[i].size(); ++j)
                mean_grid[i][j] += static_cast<double>(droplet[i][j]) / drops.size();
    std::ofstream plot;  
    plot.open("buffer");
    for (size_t i = 0; i < mean_grid.size(); ++i)
        for (size_t j = 0; j < mean_grid[i].size(); ++j)
                plot << i << " \t " << j << " \t " << mean_grid[i][j] << std::endl; 
    plot.close();

    char myfile[] = "buffer";   

    gnuplot_cmd(h, "plot  \"%s\" u 1:2:3 with points pt 7 linetype palette", myfile);
    sleep(time);
}
void Gnuplot::mean(std::vector<Drop> &drops, unsigned time) const{
    std::vector<std::vector<double>> mean_grid;
    for(size_t i = 0; i < drops[0].size(); ++i)
         mean_grid.push_back(std::vector<double>(drops[0][0].size(),0.0));
    for(auto droplet : drops)
        for(size_t i = 0;i < droplet.size(); ++i)
            for(size_t j = 0;j < droplet[i].size(); ++j)
                mean_grid[i][j] += static_cast<double>(droplet[i][j]) / drops.size();
    std::ofstream plot;  
    plot.open("buffer");
    for (size_t i = 0; i < mean_grid.size(); ++i)
        for (size_t j = 0; j < mean_grid[i].size(); ++j)
            if(mean_grid[i][j] < 0)
                plot << i << " \t " << j << std::endl; 
    plot.close();

    char myfile[] = "buffer";   
    gnuplot_cmd(h, "plot  \"%s\" pt 7 ps 0.5 notitle", myfile);
    sleep(time);
}

void Print::operator()(Drop &drop, std::string file_name) const{
    std::ofstream plot;  
    plot.open(file_name);
    for (size_t i = 0; i < drop.size(); ++i)
        for (size_t j = 0; j < drop[i].size(); ++j)
            if(drop[i][j] == -1)
                plot << i << " \t " << j << std::endl; 
    plot.close();
}

void Print::operator()(std::vector<Drop> &drops, std::string file_name) const{
    std::vector<std::vector<double>> mean_grid;
    for(size_t i = 0; i < drops[0].size(); ++i)
         mean_grid.push_back(std::vector<double>(drops[0][0].size(),0.0));
    for(auto droplet : drops)
        for(size_t i = 0;i < droplet.size(); ++i)
            for(size_t j = 0;j < droplet[i].size(); ++j)
                mean_grid[i][j] += static_cast<double>(droplet[i][j]) / drops.size();
    std::ofstream plot;  
    plot.open(file_name);
    for (size_t i = 0; i < mean_grid.size(); ++i)
        for (size_t j = 0; j < mean_grid[i].size(); ++j)
            if( mean_grid[i][j] < 0 )
                plot << i << " \t " << j << std::endl; 
    plot.close();
}
