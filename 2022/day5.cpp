#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

std::vector<std::vector<char>> getBlocks(std::ifstream& file) {
    std::vector<std::vector<char>> out;
    std::string line;

    while (std::getline(file, line, '\n')) {
        if (line.empty()) {
            break;
        } else if (line[1] == '1') {
            continue;
        }

        int length = line.size() + 1;
        int num_cols = length / 4;
        if (out.size() < num_cols) {
            out.resize(num_cols);
        }

        for (int i = 0; i < num_cols; ++i) {
            std::string block = line.substr(i * 4, 4);
            if (!(block[1] == ' ')) {
                out[i].push_back(block[1]);
            }
        }
    }

    for (auto& stack : out) {
        std::reverse(stack.begin(), stack.end());
    }

    return out;
}

std::vector<std::vector<int>> getMoves(std::ifstream& file) {
    std::vector<std::vector<int>> out;

    int num;
    int from;
    int to;

    while (file.good()) {
        // processing moves
        file.ignore(5);
        file >> num;
        file.ignore(6);
        file >> from;
        file.ignore(4);
        file >> to;

        if (file.eof()) {
            break;
        }

        out.push_back({num, from, to});
    }

    return out;
}

void updateStacks(std::vector<std::vector<char>>& blocks, const std::vector<std::vector<int>>& moves) {
    for (auto move : moves) {
        int num = move[0];
        int from = move[1] - 1;
        int to = move[2] - 1;

        for (int i = 0; i < num; ++i) {
            char c = blocks[from].back();
            blocks[from].pop_back();
            blocks[to].push_back(c);
        }
    }
}

void updateStacks2(std::vector<std::vector<char>>& blocks, const std::vector<std::vector<int>>& moves) {
    for (auto move : moves) {
        int num = move[0];
        int from = move[1] - 1;
        int to = move[2] - 1;

        std::vector<char> buffer;
        
        for (int i = 0; i < num; ++i) {
            char c = blocks[from].back();
            blocks[from].pop_back();
            buffer.push_back(c);
        }

        for (int i = 0; i < num; ++i) {
            blocks[to].push_back(buffer.back());
            buffer.pop_back();
        }
    }
}

void printTop(const std::vector<std::vector<char>>& blocks) {
    for (auto stack : blocks) {
        std::cout << stack.back();
    }
    std::cout << std::endl;
}

std::ifstream get_file(std::string filename) {
    std::ifstream out(filename);
    return out;
}

void part1() {
    std::string filename = "day5.in";

    std::ifstream f = get_file(filename);
    auto blocks = getBlocks(f);
    auto moves = getMoves(f);

    updateStacks(blocks, moves);
    printTop(blocks);
}

void part2() {
    std::string filename = "day5.in";

    std::ifstream f = get_file(filename);
    auto blocks = getBlocks(f);
    auto moves = getMoves(f);

    updateStacks2(blocks, moves);
    printTop(blocks);
}

int main() {
    part1();
    part2();
}
