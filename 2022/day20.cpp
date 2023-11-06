#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

std::pair<std::vector<int>, int> read_file(std::string filename)
{
    std::vector<int> out;
    std::ifstream ifs(filename);
    int x;
    int zero;

    while (ifs.good())
    {
        ifs >> x;
        if (x == 0)
            zero = out.size();

        if (ifs.eof())
            break;
        out.push_back(x);
    }

    return {out, zero};
}

void update_indices(std::vector<int> &indices, int pos, int diff, int n)
{
    // indices = mapping from mixing order -> mixed
    // pos = place in indices we're at
    // diff = how much to move indices[pos] by
    // n = size of the array

    if (diff == 0)
        return;

    // establish the bounds (indices) that are affected:
    int other = (n + ((indices[pos] + diff) % n)) % n;
    int lo = (indices[pos] < other) ? indices[pos] : other;
    int hi = (indices[pos] >= other) ? indices[pos] : other;

    // other useful variables
    int change = (diff < 0) ? 1 : -1;
    int tmp = indices[pos];
    bool wrapped = (diff > 0 && other == lo) || (diff < 0 && other == hi);

    // get the affected indices (in the range ^ wrapped)
    for (int i = 0; i < n; ++i)
        if (((lo < indices[i] && indices[i] < hi) ^ wrapped) || indices[i] == lo || indices[i] == hi)
            indices[i] = (n + ((indices[i] + change) % n)) % n;

    // pos is special
    indices[pos] = (n + ((tmp + diff) % n)) % n;
}

void print_indices(const std::vector<int> &indices, const std::vector<int> &data, int n)
{
    std::vector<int> real(n);
    for (int i = 0; i < n; ++i)
        real[indices[i]] = data[i];

    std::cout << '[';
    for (int i = 0; i < n; ++i)
        std::cout << real[i] << ", ";
    std::cout << "]\n";
}

int sum_indices(const std::vector<int> &indices, const std::vector<int> &data, const std::vector<int> &goal)
{
    int out = 0;

    for (int i = 0; i < indices.size(); ++i)
        if (std::find(goal.begin(), goal.end(), indices[i]) != goal.end())
            out += data[i];

    return out;
}

void part_one()
{
    auto [data, zero] = read_file("day20.in");
    int n = data.size();

    std::vector<int> indices;
    for (int i = 0; i < n; ++i)
        indices.push_back(i);
    for (int i = 0; i < n; ++i)
        update_indices(indices, i, data[i], n);

    std::vector<int> goal_indices = {(zero + 1000) % n, (zero + 2000) % n, (zero + 3000) % n};
    std::cout << sum_indices(indices, data, goal_indices) << '\n';
}

int main()
{
    part_one();
}
