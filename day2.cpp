#include <unordered_map>
#include <vector>
#include <fstream>
#include <algorithm>

#include <gtest/gtest.h>

bool exact(int n, const std::string&s);

TEST(Day2, exact) 
{
    EXPECT_FALSE(exact(2, "abcdef")); 
    EXPECT_FALSE(exact(3, "abcdef")); 
    EXPECT_TRUE(exact(2, "bababc")); 
    EXPECT_TRUE(exact(3, "bababc")); 
/*
abbcde contains two b, but no letter appears exactly three times.
abcccd contains three c, but no letter appears exactly two times.
aabcdd contains two a and two d, but it only counts once.
abcdee contains two e.
ababab contains three a and three b, but it only counts once.
*/
}

TEST(Day2, a)
{
    std::ifstream input{"input/day2.txt"};
    std::string line;
  
    int exact_2_count{0};
    int exact_3_count{0};

    while(std::getline(input, line)){
      if(exact(2, line)) exact_2_count++; 
      if(exact(3, line)) exact_3_count++; 
    }

#if 0
    EXPECT_EQ(0, exact_2_count);
    EXPECT_EQ(0, exact_3_count);
#endif
    EXPECT_EQ(5368, exact_2_count * exact_3_count);
}


auto char_counts(const std::string& s)
{
  std::unordered_map<char, int> counts;

  for(char c:s) counts[c]++;
  
  return counts;
}

bool exact(int n, const std::string& s)
{ 
  auto counts = char_counts(s);
  for(auto it = counts.begin(); it != counts.end(); it++) {
    if (it->second == n) return true;
  }
  return false;
}

bool just_one_difference(const std::string& a, const std::string&b)
{
    if(a.size() != b.size()) return false;

    bool flag{false};
    
    for(auto ait = a.begin(), bit = b.begin(); ait != a.end(); ++ait, ++bit){
      if(*ait == *bit) continue;
      if(flag) return false;
      flag = true;
    }

    return flag; 
}

TEST(Day2, just_one_difference)
{
    EXPECT_FALSE(just_one_difference("abcde", "axcye"));
    EXPECT_TRUE(just_one_difference("fghij", "fguij"));
}

std::string common_parts(const std::string& a, const std::string& b)
{
    std::string c;
    c.reserve(a.size());
    for(auto ait = a.begin(), bit = b.begin(); ait != a.end(); ++ait, ++bit){
        if (*ait == *bit)
            c += *ait;
    }
    return c;
}

TEST(Day2, b)
{
    std::ifstream input{"input/day2.txt"};
    std::string line;
    std::vector<std::string> ids; 

    while(std::getline(input, line)){
      ids.emplace_back(std::move(line));
    }


    for(auto it = ids.begin(); it != ids.end(); ++it)
    {
       for(auto jt = it+1; jt != ids.end(); ++jt)
       {
          if(it == jt) continue;
          if(just_one_difference(*it, *jt)) {
              EXPECT_EQ("cvgywxqubnuaefmsljdrpfzyi", common_parts(*it, *jt)); 
          }
       }
    }  
}
