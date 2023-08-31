#include <iostream>
#include <fstream>
#include <vector>
#include <string>

std::vector<std::vector<int>> processFileOne(std::string filename) {
    std::vector<std::vector<int>> out;
    std::ifstream ifs(filename);

    int n;
    std::vector<int> cur;
    while (ifs.good()) {
        for (int i = 0; i < 4; ++i) {
            ifs >> n;
            ifs.ignore(1);
            cur.push_back(n);
        }

        if (ifs.eof()) {
            break;
        }

        out.push_back(std::move(cur));
    }

    return out;
}

bool contained(int l1, int h1, int l2, int h2) {
    // returns whether (h1,l1) <= (l2,h2)
    return (l2 <= l1) && (h2 >= h1);
}

std::vector<std::vector<int>> remove_contained(const std::vector<std::vector<int>>& data) {
    std::vector<std::vector<int>> out;

    for (auto d : data) {
        if (!contained(d[0], d[1], d[2], d[3]) && !contained(d[2], d[3], d[0], d[1])) {
            out.push_back(d);
        }
    }

    return out;
}

int overlap(const std::vector<int>& d) {
    return ((d[0] <= d[2] && d[2] <= d[1]) ||
            (d[0] <= d[3] && d[3] <= d[1]) ||
            (d[2] <= d[0] && d[0] <= d[3]) ||
            (d[2] <= d[1] && d[1] <= d[3]));
}
int count_overlapped_ranges(const std::vector<std::vector<int>>& data) {
    int count = 0;
    
    for (auto d : data) {
        count += overlap(d);
    }

    return count;
}

int main() {
    std::vector<std::vector<int>> data1 = processFileOne("day4.in");
    std::vector<std::vector<int>> filtered = remove_contained(data1);
    std::cout << data1.size() - filtered.size() << "\n";

    std::cout << count_overlapped_ranges(data1) << "\n";

}
