#include <gtest/gtest.h>
#include <fstream>
#include <unordered_set>

TEST(Day1, a)
{
    std::ifstream input{"input/day1.txt"};
    int totalOffset{0};
    int offset;

    while(input >> offset) {
      totalOffset += offset;
    }
    
    EXPECT_EQ(totalOffset, 540);
}

TEST(Day1, b)
{
    std::ifstream input{"input/day1.txt"};
    int totalOffset{0};
    int offset;
    std::vector<int> numbers;
    std::unordered_set<int> seen;

    while(input >> offset) {
      numbers.push_back(offset);
    }
    auto it = numbers.begin();
    while(seen.find(totalOffset) == seen.end()) {
        seen.insert(totalOffset);  
        totalOffset += *it;
        ++it;
        if(it == numbers.end()) it = numbers.begin();
    }
    int repeated{totalOffset};

    EXPECT_EQ(repeated, 73056);
}
