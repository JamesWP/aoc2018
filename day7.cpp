//
// Created by PeachJames on 16/12/2018.
//

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

#include <fstream>
#include <iostream>
#include <list>
#include <unordered_map>

struct Task {
    char              name;
    std::list<Task *> deps{};   // who is dependent on me
    int               reqs{0};  // how may people am i dependent on

    Task(char c)
    : name(c)
    {
    }
};

using task_it = std::list<Task>::iterator;

bool name_sort(Task *a, Task *b)
{
    return a->name < b->name;
}

Task *add(char                             name,
          std::unordered_map<char, Task *> lookup,
          std::list<Task *>                ready,
          std::list<Task>                  list)
{
    if (lookup.find(name) != end(lookup))
        return lookup.find(name)->second;
    list.emplace_back(name);
    Task *new_job_p = std::addressof(list.back());

    // new jobs are ready
    auto ready_pos =
        std::lower_bound(begin(ready), end(ready), new_job_p, name_sort);

    ready.insert(ready_pos, new_job_p);

    lookup.insert(std::make_pair(name, new_job_p));

    return new_job_p;
}

void parse(std::string        filename,
           std::list<Task *>& ready,
           std::list<Task>    list)
{
    std::unordered_map<char, Task *> task_map;
    ASSERT_THAT(ready.empty(), Eq(true));
    std::ifstream sample_in{filename};
    std::string  line;
    while (std::getline(sample_in, line)) {
        char a, b;
        std::sscanf(line.c_str(),
                    "Step %1c must be finished before step %1c can begin.",
                    &a,
                    &b);
        Task *a_p = add(a, task_map, ready, list);
        Task *b_p = add(b, task_map, ready, list);

        if (b_p->reqs == 0) {
            // previously b was ready
            ready.erase(std::find(begin(ready), end(ready), b_p));
        }

        b_p->reqs++;  // b has a new dependency
        a_p->deps.emplace_back(b_p);
    }
}

TEST(Day7, sample)
{
    std::list<Task *> ready;
    std::list<Task>   storage_list;
    parse("input/day7.txt.sample", ready, storage_list);

    ASSERT_FALSE(ready.empty());
    ASSERT_EQ(1, ready.size());

    auto job = begin(ready);

    for (auto child_dep : (*job)->deps) {
        child_dep->reqs--;
        if (child_dep->reqs == 0) {
            auto new_ready_pos = std::lower_bound(
                begin(ready), end(ready), child_dep, name_sort);
            ready.insert(new_ready_pos, child_dep);
        }
    }
    ready.erase(job);
}
