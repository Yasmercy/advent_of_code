#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Sensor
{
    int x;
    int y;
    int bx;
    int by;

    // return the bounds that the sensor acts on the row
    int lower(int row) const
    {
        int distance = std::abs(bx - x) + std::abs(by - y) - std::abs(row - y);
        if (distance < 0)
        {
            return std::numeric_limits<int>::min();
        }
        return x - distance;
    }

    int upper(int row) const
    {
        int distance = std::abs(bx - x) + std::abs(by - y) - std::abs(row - y);
        if (distance < 0)
        {
            return std::numeric_limits<int>::max();
        }
        return x + distance;
    }
};

struct RangeType
{
    int bound;
    int type; // 1 or -1
};

bool comparator(const RangeType &r1, const RangeType &r2)
{
    return (r1.bound == r2.bound) ? r1.type > r2.type : r1.bound < r2.bound;
}

std::vector<Sensor> read_file(const std::string &filename)
{
    std::vector<Sensor> out;
    std::ifstream ifs(filename);
    std::string line;
    std::string junk;
    int x, y, bx, by;
    while (std::getline(ifs, line))
    {
        std::istringstream ss(line);
        std::getline(ss, junk, '=');
        ss >> x;
        std::getline(ss, junk, '=');
        ss >> y;
        std::getline(ss, junk, '=');
        ss >> bx;
        std::getline(ss, junk, '=');
        ss >> by;
        out.push_back({x, y, bx, by});
    }

    return out;
};

int sum_interval(std::vector<RangeType> intervals)
{
    std::sort(intervals.begin(), intervals.end(), comparator);
    int out = 0;
    int count = 0;
    int prev = 0;

    for (const auto &s : intervals)
    {
        if (count == 0)
        {
            prev = s.bound;
        }

        count += s.type;
        if (count == 0)
        {
            out += s.bound - prev;
        }
    }

    return out;
}

int find_gap_interval(std::vector<RangeType> intervals, int lower, int higher)
{
    std::sort(intervals.begin(), intervals.end(), comparator);
    int count = 0;

    // check for edges (< lower and > higher)
    if (intervals[0].bound > lower)
    {
        return lower;
    }
    else if (intervals.back().bound < higher)
    {
        return higher;
    }

    for (size_t index = 0; index < intervals.size(); ++index)
    {
        const auto &s = intervals[index];
        count += s.type;
        if (count == 0 && index + 1 < intervals.size() && intervals[index + 1].bound > s.bound + 1)
        {
            return s.bound + 1;
        }
    }

    return -1;
}

void part_one()
{
    const int row = 2'000'000;
    const auto data = read_file("day15.in");
    std::vector<RangeType> intervals;
    for (const auto &sensor : data)
    {
        int lower = sensor.lower(row);
        int upper = sensor.upper(row);
        if (lower != std::numeric_limits<int>::min())
        {
            intervals.push_back({lower, -1});
            intervals.push_back({upper, 1});
        }
    }

    int solution = sum_interval(intervals);
    std::cout << "part one solution is : " << solution << "\n";
}

int constrain(int x, int lower, int higher)
{
    return std::min(std::max(x, lower), higher);
}

uint64_t find_frquency(uint64_t x, uint64_t y)
{
    return x * 4'000'000 + y;
}

void part_two()
{
    int size = 4'000'000;
    auto data = read_file("day15.in");
    std::vector<RangeType> intervals;

    for (int row = 0; row <= size; ++row)
    {
        for (const auto &sensor : data)
        {
            int lower = sensor.lower(row);
            int upper = sensor.upper(row);
            if (lower != std::numeric_limits<int>::min())
            {
                intervals.push_back({constrain(lower, 0, size), -1});
                intervals.push_back({constrain(upper, 0, size), 1});
            }
        }

        int x = find_gap_interval(intervals, 0, size);
        if (x >= 0)
        {
            uint64_t solution = find_frquency(x, row);
            std::cout << "part two solution is : " << solution << "\n";
        }
        intervals.clear();
    }
}

int main()
{
    part_one();
    part_two();
}
