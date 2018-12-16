
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <unordered_map>
#include <utility>
#include <iostream>
#include <fstream>


TEST(Day6, input)
{
    std::ifstream input{"input/day6.txt"};
    std::string line;
    std::vector<std::pair<int, int>> points;
    while(std::getline(input, line))
    {
        int x,y;
        std::sscanf(line.c_str(), "%d,%d", &x, &y);
        points.emplace_back(x,y);
    }
    ASSERT_EQ(points.size(), 50);
    std::sort(points.begin(), points.end());

    int x_max, y_max, x_min, y_min;
    x_max = std::max_element(points.begin(), points.end(), [](auto l, auto r){ return l.first < r.first; })->first;
    y_max = std::max_element(points.begin(), points.end(), [](auto l, auto r){ return l.second < r.second; })->second;
    x_min = std::max_element(points.begin(), points.end(), [](auto l, auto r){ return l.first > r.first; })->first;
    y_min = std::max_element(points.begin(), points.end(), [](auto l, auto r){ return l.second > r.second; })->second;

    //ASSERT_EQ(0, x_max);
    //ASSERT_EQ(0, x_min);

    //ASSERT_EQ(0, y_max);
    //ASSERT_EQ(0, y_min);
}
