#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <array>
#include <bitset>
#include <fstream>
#include <functional>
#include <unordered_map>

using namespace ::testing;

using Regs = std::array<int, 4>;

struct Ins {
    int op;
    int a;
    int b;
    int c;
};

using op = std::function<Regs(Regs, Ins)>;

template <typename Op, typename ValA, typename ValB>
auto make_op(Op opr, ValA lhs, ValB rhs)
{
    return [=](Regs r, Ins i) {

        r[i.c] = opr(lhs(r, i.a), rhs(r, i.b));

        return r;
    };
}

auto r = [](const Regs& r, int v) { return r[v]; };
auto i = [](const Regs& r, int v) { return v; };

auto add = [](int a, int b) { return a + b; };
auto mul = [](int a, int b) { return a * b; };
auto ban = [](int a, int b) { return a & b; };
auto bor = [](int a, int b) { return a | b; };
auto set = [](int a, int b) { return a; };
auto grt = [](int a, int b) { return (a>b)? 1 : 0; };
auto eqt = [](int a, int b) { return (a==b)? 1 : 0; };

auto addr = make_op(add, r, r);
auto addi = make_op(add, r, i);

auto mulr = make_op(mul, r, r);
auto muli = make_op(mul, r, i);

auto banr = make_op(ban, r, r);
auto bani = make_op(ban, r, i);

auto borr = make_op(bor, r, r);
auto bori = make_op(bor, r, i);

auto setr = make_op(set, r, r);
auto seti = make_op(set, i, i);

auto gtir = make_op(grt, i, r);
auto gtri = make_op(grt, r, i);
auto gtrr = make_op(grt, r, r);

auto eqir = make_op(eqt, i, r);
auto eqri = make_op(eqt, r, i);
auto eqrr = make_op(eqt, r, r);

std::array<op, 16> ops = {addr,
                          addi,
                          mulr,
                          muli,
                          banr,
                          bani,
                          borr,
                          bori,
                          setr,
                          seti,
                          gtir,
                          gtri,
                          gtrr,
                          eqir,
                          eqri,
                          eqrr};

std::bitset<16> matching_ops(Regs before, Regs after, Ins instruction)
{
    std::bitset<16> matching_ops{0};
    for(auto it = ops.begin(); it != ops.end(); it++) 
    {
      Regs result = (*it)(before, instruction);
      if (result == after)
          matching_ops[std::distance(begin(ops), it)] = true;
    }
    return matching_ops;
}

TEST(Day16, sample)
{  
    auto matching = matching_ops({3,2,1,1}, {3,2,2,1}, {9,2,1,2});
    ASSERT_EQ(3, matching.count());
    ASSERT_THAT(matching.to_ulong(), Eq(1 << 9 | 1 << 2 | 1 << 1));
}

struct Case {
    Regs b, a;
    Ins  i;
};

bool operator==(const Ins& l, const Ins& r)
{
    return l.op == r.op && l.a == r.a && l.b == r.b && l.c == r.c;
}

bool operator==(const Case& l, const Case& r)
{
    return l.a == r.a && l.b == r.b && l.i == r.i;
}

Ins parse_ins(const std::string& ins) {
    Ins i;
    std::sscanf(ins.c_str(), "%d %d %d %d", &i.op, &i.a, &i.b, &i.c);
    return i;
}

Regs parse_regs(const std::string& reg) {
    Regs r;
    std::sscanf(reg.c_str() + reg.find_first_of("["),
                "[%d, %d, %d, %d]",
                &r[0],
                &r[1],
                &r[2],
                &r[3]);
    return r;
}

Case parse_case(const std::string& b,
                const std::string& i,
                const std::string& a)
{
  Case c;
  c.b = parse_regs(b);
  c.i = parse_ins(i);
  c.a = parse_regs(a);
  return c;
}

TEST(Day16, parse_case)
{
    Case c =
        parse_case("Before: [0, 3, 1, 1]", "6 0 0 3", "After:  [0, 3, 1, 0]");

    Case exp{{0, 3, 1, 1}, {0, 3, 1, 0}, {6, 0, 0, 3}};

    EXPECT_EQ(exp, c);
    EXPECT_EQ(exp.b, c.b);
    EXPECT_EQ(exp.a, c.a);
    EXPECT_EQ(exp.i, c.i);
}

auto num_matching = [](auto&& b, auto&& a, auto&& i) {
    return matching_ops(b, a, i).count();
};

auto matching_is_three = [](const Case& c) {
    return num_matching(c.b, c.a, c.i) >= 3;
};

TEST(Day16, a)
{
   
    std::vector<Case> cases;

    std::ifstream inp_cases{"input/day16samples.txt"};
    std::string b, i, a, blnk;

    while (std::getline(inp_cases, b),
           std::getline(inp_cases, i),
           std::getline(inp_cases, a),
           std::getline(inp_cases, blnk)) {
        cases.push_back(parse_case(b, i, a));
    }

    auto first_non_three =
        std::partition(begin(cases), end(cases), matching_is_three);

    size_t has_three = std::distance(begin(cases), first_non_three);
  
    EXPECT_EQ(493, has_three);
}

int get_set_bit(std::bitset<16> bs)
{
    return 15 - bs.to_string().find_first_of("1");
}

TEST(Day16, get_set_bit)
{
  std::bitset<16> bs1{0};
  bs1[10] = true;
  ASSERT_THAT(bs1.count(), Eq(1));
  ASSERT_THAT(get_set_bit(bs1), Eq(10));
}

std::unordered_map<int,int> get_expected_mapping()
{
    std::unordered_map<int, int> expected_mapping;
    expected_mapping[5]=15;
    expected_mapping[6]=11;
    expected_mapping[12]=13;
    expected_mapping[15]=12;
    expected_mapping[9]=14;
    expected_mapping[3]=8;
    expected_mapping[7]=10;
    expected_mapping[0]=4;
    expected_mapping[14]=9;
    expected_mapping[8]=6;
    expected_mapping[10]=5;
    expected_mapping[1]=3;
    expected_mapping[2]=7;
    expected_mapping[4]=1;
    expected_mapping[11]=0;
    expected_mapping[13]=2;
    return expected_mapping;
}

std::unordered_map<int, int> expected_mapping = get_expected_mapping();

TEST(Day16, mapping)
{
    std::vector<Case> cases;
    std::array<std::bitset<16>, 16> pos;

    for (auto&& a : pos)
        a = std::bitset<16>{}.set();

    std::ifstream inp_cases{"input/day16samples.txt"};
    std::string b, i, a, blnk;

    std::unordered_map<int, int> mapping;
    std::bitset<16>              not_solved;
    not_solved.set();

    while (not_solved.any()) {
        while (std::getline(inp_cases, b),
               std::getline(inp_cases, i),
               std::getline(inp_cases, a),
               std::getline(inp_cases, blnk)) {
            const Case c        = parse_case(b, i, a);
            auto       possible = matching_ops(c.b, c.a, c.i);

#if 0
        std::cout << c.i.op << ' ' << pos[c.i.op] << '.' << possible
                  << std::endl;
#endif
            auto& posop = pos[c.i.op];
            
            posop &= possible;
            posop &= not_solved;
            if (posop.count() == 1) {
                auto sb = get_set_bit(posop);
                not_solved[sb] = false;
                mapping[c.i.op] = sb;
            }
        }
        inp_cases.clear();
        inp_cases.seekg(0, std::ios::beg);
    }

    ASSERT_THAT(mapping.size(), Eq(16));
    ASSERT_THAT(mapping, Eq(expected_mapping));
}


TEST(Day16, b)
{
    Regs r{0};
    std::ifstream prog{"input/day16prog.txt"};
    std::string i;

    while(std::getline(prog, i))
    {
        Ins instruction = parse_ins(i);
        instruction.op = expected_mapping[instruction.op];

        op operation = ops[instruction.op];
        
        r = operation(r, instruction);
    }

    EXPECT_THAT(r[0], Eq(0));
}

