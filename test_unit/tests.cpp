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

TEST(Drop, plot){
    Gnuplot plot;
    Drop drop(40, 40 ,10 ,0);
              
    for (size_t i = 0; i < 1000; ++i)
    {
        plot(drop,1);
        for (size_t i = 0; i < 200; ++i)
            drop(1.0, 4.0,
              0.0, 0.0, 
              0.00);
    //std::cout << drop.Energy() << " " << drop.BulkEnergy() << " " << drop.CenterOfMass().second << std::endl;
    }
    
} 




int main(int argc, char *argv[])
{  
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
