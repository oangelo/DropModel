#include "gtest/gtest.h"
#include "../src/drop.h"

TEST(Drop, GravitationaEnergy){
    matrix grid; 
    column v4({-1, -1, -1, -1, -1, -1, -1});
    column v3({-1, -1,  1,  1, -1, -1, -1});
    column v2({-1, -1,  1,  1, -1, -1, -1});
    column v1({-1, -1, -1, -1, -1, -1, -1});
    
    grid.push_back(v1);
    grid.push_back(v2);
    grid.push_back(v3);
    grid.push_back(v4);

    matrix grid_check; 
    column c4({-1, -1, -1, -1, -1, -1, -1});
    column c3({-1, -1, -1,  1,  1, -1, -1});
    column c2({-1, -1, -1,  1,  1, -1, -1});
    column c1({-1, -1, -1, -1, -1, -1, -1});
    
    grid_check.push_back(c1);
    grid_check.push_back(c2);
    grid_check.push_back(c3);
    grid_check.push_back(c4);

    EXPECT_EQ(GravitationalEnergy(grid, 1), -84);
    EXPECT_EQ(GravitationalEnergy(grid_check, 1), -76);
    
}

TEST(Drop, InteractionEnergy){
    matrix grid; 
    column v6({-1, -1, -1, -1, -1, -1, -1, -1});
    column v5({-1, -1, -1, -1, -1, -1, -1, -1});
    column v4({-1, -1, -1,  1,  1, -1, -1, -1});
    column v3({-1, -1, -1,  1,  1, -1, -1, -1});
    column v2({-1, -1, -1, -1, -1, -1, -1, -1});
    column v1({-1, -1, -1, -1, -1, -1, -1, -1});
    
    grid.push_back(v1);
    grid.push_back(v2);
    grid.push_back(v3);
    grid.push_back(v4);
    grid.push_back(v5);
    grid.push_back(v6);

    matrix grid_check; 
    column c6({-1, -1, -1, -1, -1, -1, -1, -1});
    column c5({-1, -1, -1, -1, -1, -1, -1, -1});
    column c4({-1, -1, -1, -1, -1, -1, -1, -1});
    column c3({-1, -1,  1,  1,  1,  1, -1, -1});
    column c2({-1, -1, -1, -1, -1, -1, -1, -1});
    column c1({-1, -1, -1, -1, -1, -1, -1, -1});
    
    grid_check.push_back(c1);
    grid_check.push_back(c2);
    grid_check.push_back(c3);
    grid_check.push_back(c4);
    grid_check.push_back(c5);
    grid_check.push_back(c6);

    std::cout << InteractionEnergy(grid, 2, 1) << std::endl;
    std::cout << InteractionEnergy(grid_check, 2, 1) << std::endl;
}

TEST(Drop, GetBoarders){
    matrix grid; 
    column v1({0, 0, 0, 0, 0, 0, 0});
    column v2({0, 0, 0, 0, 0, 0, 0});
    column v3({0, 0, 1, 1, 1, 0, 0});
    column v4({0, 0, 1, 1, 1, 0, 0});
    column v5({0, 0, 1, 1, 1, 0, 0});
    column v6({0, 0, 0, 1, 0, 0, 0});
    column v7({0, 0, 0, 1, 0, 0, 0});
    column v8({0, 0, 0, 0, 0, 0, 0});
    
    grid.push_back(v1);
    grid.push_back(v2);
    grid.push_back(v3);
    grid.push_back(v4);
    grid.push_back(v5);
    grid.push_back(v6);
    grid.push_back(v7);
    grid.push_back(v8);

    matrix grid_check; 
    column c1({0, 0, 0, 0, 0, 0, 0});
    column c2({0, 2, 2, 2, 2, 2, 0});
    column c3({0, 2, 3, 3, 3, 2, 0});
    column c4({0, 2, 3, 1, 3, 2, 0});
    column c5({0, 2, 3, 3, 3, 2, 0});
    column c6({0, 2, 2, 3, 2, 2, 0});
    column c7({0, 0, 2, 3, 2, 0, 0});
    column c8({0, 0, 2, 2, 2, 0, 0});
    
    grid_check.push_back(c1);
    grid_check.push_back(c2);
    grid_check.push_back(c3);
    grid_check.push_back(c4);
    grid_check.push_back(c5);
    grid_check.push_back(c6);
    grid_check.push_back(c7);
    grid_check.push_back(c8);


    int white = 0, black = 1;
    std::vector<column::iterator> w, b;
    GetBorders(grid, white, black, w, b);
    for(auto item: w)
        *item = 2;
    for(auto item: b)
        *item = 3;
    for(size_t i = 0; i < grid.size(); ++i){ 
        for(size_t j = 0; j < grid[i].size(); ++j) 
            EXPECT_EQ(grid[i][j], grid_check[i][j]);
    }
}

TEST(Drop, Init){
    Drop drop(10,10,10,15);
    EXPECT_EQ(drop.size(),10);
    for(size_t i; i<10; i++)
        EXPECT_EQ(drop[i].size(),10);
}

TEST(Drop, CenterOfMass){
    Drop drop(100, 100, 35 ,0 );
    EXPECT_EQ(drop.CenterOfMass().first,50);
    EXPECT_EQ(drop.CenterOfMass().second,50);
}

TEST(Drop, energy){
    Drop drop(100,30,20,15);
    //std::cout << drop.Energy() << std::endl;
}

TEST(Drop, plot_mean){
    Gnuplot plot;
    std::vector<Drop> drops(20, Drop(40, 40, 9, 10));
    for (size_t i = 0; i < 1000; ++i)
    {
        plot.mean(drops,1);
        for (size_t i = 0; i < 400; ++i) {
            for (size_t j = 0; j < drops.size(); ++j) {
                drops[j](0.32, 
                     4.0, 1.0, 
                     4.600001);
                double y_cm_mean = 0;
                for(auto item: drops)
                    y_cm_mean += item.CenterOfMass().second;
//                std::cout << y_cm_mean / drops.size() << std::endl;
            }
        }
    }
}

TEST(Drop, plot){
    Gnuplot plot;
    Drop drop(40, 40 ,10 ,0);
              
    for (size_t i = 0; i < 1000; ++i)
    {
        plot(drop,1);
        for (size_t i = 0; i < 200; ++i)
            drop(0.0, 
              1.0, 1.0, 
              0.50);
    }
    
} 

int main(int argc, char *argv[])
{  
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
