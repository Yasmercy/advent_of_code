#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

struct Job
{
    // ATOM
    uint64_t num;

    // NOT ATOM
    char op; // +, -, /, *
    std::string m1;
    std::string m2;
};

std::unordered_map<std::string, Job> read_file(std::string filename)
{
    std::unordered_map<std::string, Job> out;
    std::ifstream ifs(filename);

    std::string line;
    while (std::getline(ifs, line))
    {
        std::istringstream ss(line);

        // get name
        std::string name;
        std::getline(ss, name, ':');

        std::vector<std::string> in;
        std::string s;
        while (std::getline(ss, s, ' '))
            in.push_back(s);

        // not atom
        if (in.size() == 2)
            out.insert({name, {static_cast<uint64_t>(std::stoi(in[1]))}});
        else
            out.insert({name, {0, in[2][0], in[1], in[3]}});
    }

    return out;
}

uint64_t reverse_op(char op, bool first, uint64_t other, uint64_t val)
{
    switch (op)
    {
    case '+':
        return val - other;
    case '*':
        return val / other;
    case '-':
        if (first)
            return val + other;
        else
            return other - val;
    case '/':
        if (first)
            return val * other;
        else
            return other / val;
    }

    throw std::runtime_error("breaking");
}

uint64_t do_op(char op, uint64_t x, uint64_t y)
{
    switch (op)
    {
    case '+':
        return x + y;
    case '-':
        return x - y;
    case '*':
        return x * y;
    case '/':
        return x / y;
    }

    throw std::runtime_error("breaking");
}

uint64_t get_call(const std::unordered_map<std::string, Job> &jobs, const std::string &name)
{
    const Job &j = jobs.at(name);
    if (j.op == '\0')
        return j.num;

    uint64_t x = get_call(jobs, j.m1);
    uint64_t y = get_call(jobs, j.m2);

    return do_op(j.op, x, y);
}

std::unordered_map<std::string, uint64_t> get_values(const std::unordered_map<std::string, Job> &jobs,
                                                     const std::unordered_map<std::string, Job> &modify)
{
    // maps monkeys to their output
    // if the output relies on "humn", then the output is 0 (sentinel)
    std::unordered_map<std::string, uint64_t> out;

    for (const auto &[key, _] : jobs)
    {
        int x = get_call(jobs, key);
        int y = get_call(modify, key);
        out.insert({key, x * (x == y)});
    }

    return out;
}

uint64_t make_equal(const std::unordered_map<std::string, Job> &jobs,
                    const std::unordered_map<std::string, uint64_t> &values, const std::string &m, uint64_t val)
{
    // finds the monkey that is not dependent on "humn"
    // calls make_equal on the sub
    // base case: m1 or m2 is "humn" ==> just returns that value

    if (m == "humn")
        return val;

    const std::string &m1 = jobs.at(m).m1;
    const std::string &m2 = jobs.at(m).m2;

    // new_val is just the reversed operand
    // for example, to make x / 3 = 2, x = 2 * 3 = 6
    //              to make 6 / x = 2, x = 6 / 2 = 3

    if (values.at(m1) == 0)
        return make_equal(jobs, values, m1, reverse_op(jobs.at(m).op, 1, values.at(m2), val));
    if (values.at(m2) == 0)
        return make_equal(jobs, values, m2, reverse_op(jobs.at(m).op, 0, values.at(m1), val));

    throw std::runtime_error("breaking");
}

void part_one()
{
    const auto &data = read_file("day21.in");
    std::cout << get_call(data, "root") << '\n';
}

void part_two()
{
    auto data = read_file("day21.in");
    data["root"].op = '-';
    auto modify = data;
    modify["humn"].num = std::numeric_limits<uint64_t>::max();

    auto values = get_values(data, modify);
    std::cout << make_equal(data, values, "root", 0) << '\n';
}

int main()
{
    part_one();
    part_two();
}
