#include <iostream>
#include <fstream>
#include <string>

bool all_unique(const std::string& str, int start, int length) {
    for (int i = 0; i < length; ++i) {
        char c = str[i + start];
        for (int j = i + 1; j < length; ++j) {
            if (c == str[j + start]) {
                return false;
            }
        }
    }
    return true;
}

std::string read_file(std::string filename) {
    std::ifstream ifs(filename);
    std::string out;
    ifs >> out;
    return out;
}

void part1() {
    std::string data = read_file("day6.in");
    for (int s = 0; s < data.size() - 3; ++s) {
        if (all_unique(data, s, 4)) {
            std::cout << s + 4 << "\n";
            break;
        }
    }
}

void part2() {
    std::string data = read_file("day6.in");
    for (int s = 0; s < data.size() - 3; ++s) {
        if (all_unique(data, s, 14)) {
            std::cout << s + 14 << "\n";
            break;
        }
    }
}

int main() {
    part1();
    part2();
}
