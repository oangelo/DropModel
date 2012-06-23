#include "gtest/gtest.h"
#include "../src/drop.h"

TEST(Drop, Init){
    Drop drop(10,10,10);
    EXPECT_EQ(drop.size(),10);
    for(size_t i; i<10; i++)
        EXPECT_EQ(drop[i].size(),10);
}
TEST(Drop, energy){
    Drop drop(100,30,20);
    std::cout << drop.Energy() << std::endl;
}
TEST(Drop, plot){
    Gnuplot plot;
    Drop drop(100,30,20);
    for (size_t i = 0; i < 100; ++i)
    {
        plot(drop,1);
        for (size_t i = 0; i < 200; ++i)
            drop();
    std::cout << drop.Energy() << std::endl;
    }
    
} 


int main(int argc, char *argv[])
{  
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
