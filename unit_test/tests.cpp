#include "gtest/gtest.h"
#include "../src/drop.h"

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
//*
TEST(Drop, plot_mean){
    Gnuplot plot;
    std::vector<Drop> drops(50,Drop(40, 40 ,8 ,8));
              
    for (size_t i = 0; i < 1000; ++i)
    {
        plot.mean(drops,1);
        
        for (size_t i = 0; i < 200; ++i)
            for (size_t j = 0; j < drops.size(); ++j)
                drops[j](0.1, 
                     1.0, 1.0, 
                     0.1);
              
    }
    
}
//*/
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
