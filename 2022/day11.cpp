#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <sstream>
#include <vector>

constexpr uint64_t MAX = std::numeric_limits<uint64_t>::max();

struct Monkey
{
    Monkey(std::vector<std::string> inputs)
    {
        get_starting(inputs[0]);
        get_operation(inputs[1]);
        modulus = get_last_number(inputs[2]);
        true_to = get_last_number(inputs[3]);
        false_to = get_last_number(inputs[4]);
    }

    void get_starting(std::string starting)
    {
        std::istringstream ss(starting);
        std::string line;
        std::getline(ss, line, ':');

        while (std::getline(ss, line, ','))
            items.push_back(std::stoi(line));
    }

    void get_operation(std::string operation)
    {
        std::istringstream ss(operation);
        std::string line;
        std::getline(ss, line, '=');

        std::string x;
        ss >> x;
        args.push_back((x == "old") ? MAX : std::stoi(x));
        ss >> op;
        ss >> x;
        args.push_back((x == "old") ? MAX : std::stoi(x));
    }

    uint64_t get_last_number(std::string line)
    {
        std::vector<std::string> data;
        std::istringstream ss(line);

        while (std::getline(ss, line, ' '))
            data.push_back(line);
        return std::stoi(data.back());
    }

    uint64_t inspect_item()
    {
        ++inspected;
        uint64_t item = items.back();

        std::vector<uint64_t> x;
        x.push_back((args[0] == MAX) ? item : args[0]);
        x.push_back((args[1] == MAX) ? item : args[1]);

        switch (op)
        {
        case '*':
            return x[0] * x[1];
        case '/':
            return x[0] / x[1];
        case '-':
            return x[0] - x[1];
        case '+':
            return x[0] + x[1];
        }

        throw std::runtime_error("failed");
    }

    std::vector<uint64_t> items;
    std::vector<uint64_t> args;
    char op;
    uint64_t modulus;
    uint64_t true_to;
    uint64_t false_to;

    uint64_t inspected = 0;
};

struct Troop
{
    std::vector<Monkey> monkeys;

    void process_monkey(Monkey &monkey, bool relief, uint64_t modulus)
    {
        while (!monkey.items.empty())
        {
            auto item = monkey.inspect_item();
            monkey.items.pop_back();

            if (relief)
                item = item / 3;
            item = item % modulus;
            int next = (item % monkey.modulus == 0) ? monkey.true_to : monkey.false_to;
            monkeys[next].items.push_back(item);
        }
    }

    void do_round(bool relief, uint64_t modulus)
    {
        for (auto &monkey : monkeys)
            process_monkey(monkey, relief, modulus);
    }

    uint64_t multiply_inspected()
    {
        uint64_t max = 0;
        for (uint64_t i = 0; i < monkeys.size(); ++i)
            for (uint64_t j = 0; j < monkeys.size(); ++j)
                max = std::max(max, (j != i) * monkeys[i].inspected * monkeys[j].inspected);
        return max;
    }

    uint64_t lcm_modulus()
    {
        uint64_t lcm = 1;
        for (const auto &monkey : monkeys)
            lcm = std::lcm(lcm, monkey.modulus);
        return lcm;
    }
};

Troop read_file(std::string filename)
{
    Troop troop;

    std::ifstream ifs(filename);
    std::string dummy;

    while (ifs.good())
    {
        std::vector<std::string> data;

        std::getline(ifs, dummy); // dummy call
        for (int i = 0; i < 5; ++i)
        {
            std::getline(ifs, dummy);
            data.push_back(dummy);
        }
        std::getline(ifs, dummy); // dummy call

        troop.monkeys.push_back(Monkey(data));

        if (ifs.eof())
            break;
    }

    return troop;
};

void part_one()
{
    Troop troop = read_file("day11.in");
    uint64_t modulus = troop.lcm_modulus();
    for (int i = 0; i < 20; ++i)
        troop.do_round(true, modulus);
    std::cout << troop.multiply_inspected() << '\n';
}

void part_two()
{
    Troop troop = read_file("day11.in");
    uint64_t modulus = troop.lcm_modulus();
    for (int i = 0; i < 10'000; ++i)
        troop.do_round(false, modulus);
    std::cout << troop.multiply_inspected() << '\n';
}

int main()
{
    part_one();
    part_two();
}
