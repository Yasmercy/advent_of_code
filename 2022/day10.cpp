#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

struct Instruction {
    int operation; // 0 for no-op, 1 for add
    int n = 0; // optional: adds n if its not a no-op
    
    void do_action(std::vector<int>& states) const {
        states.push_back(states.back());
        if (operation == 1) {
            states.push_back(states.back() + n);
        }
    }
};

std::vector<Instruction> read_file(std::string filename) {
    std::ifstream ifs(filename);
    std::vector<Instruction> out;
    std::string op;
    int n;

    while (ifs.good()) {
        ifs >> op;
        if (ifs.eof()) {
            break;
        }

        if (op == "noop") {
            out.push_back(Instruction{0});
        } else {
            ifs >> n;
            out.push_back(Instruction{1, n});
        }
    }
    return out;
}

std::vector<int> simulate(const std::vector<Instruction>& instructions) {
    std::vector<int> states = {1};
    // each index is each cpu cycle
    // the value at the index is the value in register X

    for (auto instruction : instructions) {
        instruction.do_action(states);
    }

    return states;
}

int sum_signal_strengths(const std::vector<int>& states, const std::vector<int>& cycles) {
    int out = 0;
    for (int cycle : cycles) {
        out += cycle * states[cycle - 1];
    }
    return out;
}

void part_one() {
    std::vector<Instruction> instructions = read_file("day10.in");
    std::vector<int> states = simulate(instructions);
    int sss = sum_signal_strengths(states, {20, 60, 100, 140, 180, 220});
    std::cout << "Part One solution: " << sss << "\n";
}

void render(const std::vector<int>& states, int width) {
    auto size = states.size();
    for (int i = 0; i < size - 1; ++i) {
        if (std::abs(states[i] - (i % width)) < 2) {
            std::cout << '#';
        } else {
            std::cout << '.';
        }

        if ((i + 1) % width == 0) {
            std::cout << "\n";
        }
    }
}

void part_two() {
    std::vector<Instruction> instructions = read_file("day10.in");
    std::vector<int> states = simulate(instructions);
    render(states, 40);
}

int main() {
    part_one();
    part_two();
}
