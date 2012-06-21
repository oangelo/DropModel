#include "gtest/gtest.h"
#include "../src/drop.h"

TEST(Drop, Init){
    Drop drop(10);
    EXPECT_EQ(drop.size(),10);
    for(size_t i; i<10; i++)
        EXPECT_EQ(drop[i].size(),10);
}

TEST(Drop, plot){
    Gnuplot plot;
    Drop drop(100);
    plot(drop,4);
} 

int main(int argc, char *argv[])
{  
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
