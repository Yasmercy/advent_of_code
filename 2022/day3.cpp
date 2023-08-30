#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <fstream>
#include <algorithm>
#include <iterator>

std::vector<std::string> processFile1(std::string filename) {
    std::vector<std::string> out;
    std::ifstream ifs(filename);
    std::string rucksack;
    while (ifs.good()) {
        ifs >> rucksack;
        if (ifs.eof()) {
            break;
        }
        out.push_back(rucksack);
    }
    return out;
}

std::vector<std::vector<std::string>> processFile2(std::string filename) {
    std::vector<std::vector<std::string>> out;
    std::ifstream ifs(filename);
    std::string str;
    int count = 0;

    while (ifs.good()) {
        if (count == 0) {
            out.push_back(std::vector<std::string>{});
        }

        ifs >> str;
        if (ifs.eof()) {
            break;
        }

        out.back().push_back(str);
        count = ++ count % 3;
    }

    out.pop_back();
    return out;
}

int toPriority(char c) {
    if ('a' <= c && c <= 'z') {
        return c - 'a' + 1;
    }
    return c - 'A' + 27;
}

char findDuplicate(std::string a, std::string b) {
    std::set<char> chars(a.begin(), a.end());
    for (char c : b) {
        if (chars.contains(c)) {
            return c;
        }
    }
    return 'a';
}

int addPriority(std::string rucksack) {
    // splits the string and returns the score of the duplicate
    char dup = findDuplicate(rucksack.substr(0, rucksack.length() / 2), rucksack.substr(rucksack.length() / 2));
    return toPriority(dup);
}

int sumPriority(std::vector<std::string> data) {
    int sum = 0;
    for (std::string rucksack : data) {
        sum += addPriority(rucksack);
    }
    return sum;
}

// ================= part 2
char findCommon(std::string a, std::string b, std::string c) {
    std::set<char> c1(a.begin(), a.end());
    std::set<char> c2(b.begin(), b.end());
    std::set<char> c3(c.begin(), c.end());

    // find intersection of these three
    std::vector<char> common12;
    std::vector<char> common123;

    std::set_intersection(c1.begin(), c1.end(), c2.begin(), c2.end(), std::back_inserter(common12));
    std::set_intersection(common12.begin(), common12.end(), c3.begin(), c3.end(), std::back_inserter(common123));

    return common123[0];
}

int sumPriorityBadge(std::vector<std::vector<std::string>> data) {
    int sum = 0;
    for (std::vector<std::string> strings : data) {
        sum += toPriority(findCommon(strings[0], strings[1], strings[2]));
    }
    return sum;
}

int main() {
    std::vector<std::string> data = processFile1("day3.in");
    int priority = sumPriority(data);
    std::cout << priority << "\n";
    
    std::vector<std::vector<std::string>> data2 = processFile2("day3.in");
    int priorityBadge = sumPriorityBadge(data2);
    std::cout << priorityBadge << "\n";
}
