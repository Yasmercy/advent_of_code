#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <utility>
#include <functional>
#include <cmath>
#include <cinttypes>


struct Monkey {
    Monkey(std::vector<std::string> inputs) {
        get_starting(inputs[0]);
        get_operation(inputs[1]);
        get_test(inputs[2], inputs[3], inputs[4]);
    }

    void get_starting(std::string starting) {
        std::stringstream ss(starting.substr(18));

        uint64_t x;
        while (!ss.eof()) {
            ss >> x;
            items.push_back(x);
            ss.ignore(2);
        }
    }

    void get_operation(std::string operation) {
        std::stringstream ss(operation.substr(23));

        // get the operation type
        // *, +
        char c;
        uint64_t x = 0;
        ss >> c;
        if (c == '*') {
            // get the operand: number or "old"
            ss >> x;
            if (x == 0) {
                // old
                this->operation = [x](uint64_t a) {return a * a;};
            } else {
                // number
                this->operation = [x](uint64_t a) {return a * x;};
            }
        } else { // c == '+'
            // get the operand: number or "old"
            ss >> x;
            if (x == 0) {
                // old
                this->operation = [x](uint64_t a) {return a + a;};
            } else {
                // number
                this->operation = [x](uint64_t a) {return a + x;};
            }
        }
    }

    void get_test(std::string test_str, std::string t, std::string f) {
        std::stringstream ss;
        
        // get the test number (divisble by)
        ss << test_str;
        ss.ignore(21);
        uint64_t x;
        ss >> x;
        
        // get the test number (true)
        ss = std::stringstream();
        ss << t.substr(29);
        uint64_t y;
        ss >> y;
        
        // get the test number (false)
        ss = std::stringstream();
        ss << f.substr(30);
        uint64_t z;
        ss >> z;

        // create function
        test = [x, y, z](uint64_t a) { 
            return (a % x == 0) ? y : z; 
        };
    }

    std::vector<uint64_t> items;
    std::function<uint64_t(uint64_t)> operation;
    std::function<uint64_t(uint64_t)> test;
    uint64_t inspected = 0;

    void process(std::vector<Monkey>& states, bool flag) {
        for (uint64_t item : items) {
            item = operation(item);
            if (flag) {
                item = item / 3;
            }
            uint64_t next = test(item);
            states[next].items.push_back(item);
        }
        inspected += items.size();
        items.clear();
    }
};

struct Troop {
    std::vector<Monkey> states;
    void step(uint64_t n, bool flag) {
        if (n == 0) {
            return;
        }
        for (Monkey& s : states) {
            s.process(states, flag);
        }

        step(--n, flag);
    }

    uint64_t monkey_business() {
        // find the top two inspected counts
        uint64_t max = 0;
        for (uint64_t i = 0; i < states.size(); ++i) {
            for (uint64_t j = 0; j < states.size(); ++j) {
                if (j == i) {
                    continue;
                }
                max = std::max(max, states[i].inspected * states[j].inspected);
            }
        }
        return max;
    }
};

Troop process_file(std::string filename) {
    Troop troop;

    std::ifstream ifs(filename);
    std::string dummy;

    while (ifs.good()) {
        std::string starting;
        std::string operation;
        std::string test;
        std::string t;
        std::string f;

        std::getline(ifs, dummy); // dummy call
        std::getline(ifs, starting);
        std::getline(ifs, operation);
        std::getline(ifs, test);
        std::getline(ifs, t);
        std::getline(ifs, f);
        std::getline(ifs, dummy); // dummy call

        troop.states.push_back(Monkey({
            starting,
            operation,
            test,
            t,
            f
        }));
        
        if (ifs.eof()) {
            break;
        }

    }

    return troop;
};

void part_one() {
    Troop t = process_file("day11.in");
    t.step(20, true);
    std::cout << t.monkey_business() << "\n";
}

void part_two() {
    Troop t = process_file("day11.in");
    t.step(20, false);

    for (auto m : t.states) {
        std::cout << m.inspected << "\n";
    }

    std::cout << t.monkey_business() << "\n";
}

int main() {
    part_one();
    part_two();
}
