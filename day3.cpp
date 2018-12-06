#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fstream>
#include <vector>

using namespace ::testing;

template<size_t X, size_t Y>
struct sheet
{
    constexpr static int total = X*Y;
    int _i[total];
    
    sheet(){
      for(int i=0;i<total;i++) _i[i] = 0;
    }

    int& get(int i) { return _i[i]; }
    int& get(int x, int y) { return _i[x + X*(y)]; }

    void claim(int x, int y, int w, int h) 
    {
      for(int i = x; i< x+w; i++)
        for(int j = y; j < y+h; j++)
        {
          get(i, j)++;
        }
    }
};

TEST(Day3, sheet)
{
    sheet<8, 8> s;
    s.claim(1,3,4,4);
    s.claim(3,1,4,4);
    s.claim(5,5,2,2);

    int count{0};

    for(int i=0; i<8; i++)
      for(int j=0; j<8; j++)
        if(s.get(i,j)>1)
          count++;

    EXPECT_EQ(4, count);
}

TEST(Day3, a)
{
    sheet<1000, 1000> s;
    int count{0};

    std::ifstream finput{"input/day3.txt"};
    ASSERT_TRUE((bool)finput);
    char ignore; int id,x,y,w,h;
    std::string line;
    while (std::getline(finput, line)) {
      std::istringstream input{line};
      input >> ignore >> id;
      input >> ignore >> x >> ignore >> y >> ignore >>
          w >> ignore >> h;
      s.claim(x, y, w, h);
    }

    for(int i=0; i<decltype(s)::total; i++)
        if(s.get(i)>1)
          count++;

    EXPECT_EQ(118840, count);
}

struct claim { int x; int y; int w; int h; int id; };

std::ostream& operator<<(std::ostream& out, claim c){
  out << c.x << ',' << c.y << ' ' << c.w << 'x' << c.h << '.';
  return out;  
}

bool valueInRange(int value, int min, int max)
{
    return (value >= min) && (value <  max);
}

bool intersects(const claim& c1, const claim& c2)
{
    bool xOverlap = valueInRange(c1.x, c2.x, c2.x + c2.w) ||
                    valueInRange(c2.x, c1.x, c1.x + c1.w);

    bool yOverlap = valueInRange(c1.y, c2.y, c2.y + c2.h) ||
                    valueInRange(c2.y, c1.y, c1.y + c1.h);

    return xOverlap && yOverlap;
}

TEST(Day3, intersects) {
    claim a{10,10,10,10};
    claim b{14,14,1,1};
    claim c{20,20,10,10};

    ASSERT_TRUE(intersects(a,b));
    ASSERT_FALSE(intersects(a,c));
}

TEST(Day3, intersects2) {
    claim a{0,0,1,1};
    claim b{0,1,1,1};

    ASSERT_TRUE(intersects(a,a));
    ASSERT_FALSE(intersects(a,b));
}

bool intersects(const claim& c, const std::vector<claim>& cs)
{
    for (const auto& co : cs)
        if (intersects(co, c))
            return true;
    return false;
}

TEST(Day3, b)
{
    sheet<1000, 1000> s;
    std::vector<claim> claims;


    std::ifstream finput{"input/day3.txt"};
    ASSERT_TRUE((bool)finput);
    char ignore; int id,x,y,w,h;
    std::string line;
    while (std::getline(finput, line)) {
      std::istringstream input{line};
      input >> ignore >> id;
      input >> ignore >> x >> ignore >> y >> ignore >>
          w >> ignore >> h;

      claim c;
      c.id = id; c.x = x; c.y = y; c.w = w; c.h = h;

      claims.push_back(c);
    }

    EXPECT_THAT(claims.size(), Eq(1373));

    auto not_collides = [&claims](const claim& c){ 
        for(const auto& co : claims) {
          if (co.id == c.id) continue;
          if (intersects(co, c)) {
              return false;
          };
        }
        return true;
    };

    auto unique = std::find_if(claims.begin(), claims.end(), not_collides);
    
    ASSERT_NE(claims.end(), unique);

    EXPECT_EQ(claims.end(), std::find_if(unique+1, claims.end(), not_collides));

    ASSERT_EQ(919, unique->id);
}
