#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

std::pair<std::vector<long>, long> read_file(std::string filename)
{
    std::vector<long> out;
    std::ifstream ifs(filename);
    long x;
    long zero;

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

long modulus_add(long x, long y, long m)
{
    return (((x + y) % m) + m) % m;
}

void update_indices(std::vector<long> &indices, long pos, long diff, long n)
{
    // indices = mapping from mixing order -> mixed
    // pos = place in indices we're at
    // diff = how much to move indices[pos] by
    // n = size of the array

    diff = modulus_add(diff, 0, n - 1);
    if (diff == 0)
        return;

    // establish the bounds (indices) that are affected:
    long other = modulus_add(indices[pos], diff, n);
    long lo = (indices[pos] < other) ? indices[pos] : other;
    long hi = (indices[pos] >= other) ? indices[pos] : other;

    // other useful variables
    long change = (diff < 0) ? 1 : -1;
    long tmp = indices[pos];
    bool wrapped = other == lo;

    // get the affected indices (in the range ^ wrapped)
    for (long i = 0; i < n; ++i)
        if (((lo < indices[i] && indices[i] < hi) ^ wrapped) || indices[i] == lo || indices[i] == hi)
            indices[i] = modulus_add(indices[i], change, n);

    // pos is special
    indices[pos] = modulus_add(tmp, diff, n);
}

void prlong_indices(const std::vector<long> &indices, const std::vector<long> &data, long n)
{
    std::vector<long> real(n);
    for (long i = 0; i < n; ++i)
        real[indices[i]] = data[i];

    std::cout << '[';
    for (long i = 0; i < n; ++i)
        std::cout << real[i] << ", ";
    std::cout << "]\n";
}

long sum_indices(const std::vector<long> &indices, const std::vector<long> &data, const std::vector<long> &goal)
{
    long out = 0;

    for (long i = 0; i < indices.size(); ++i)
        if (std::find(goal.begin(), goal.end(), indices[i]) != goal.end())
            out += data[i];

    return out;
}

void part_one()
{
    auto [data, zero] = read_file("day20.in");
    long n = data.size();

    std::vector<long> indices;
    for (long i = 0; i < n; ++i)
        indices.push_back(i);

    for (long i = 0; i < n; ++i)
        update_indices(indices, i, data[i], n);

    zero = indices[zero];
    std::vector<long> goal_indices = {(zero + 1000) % n, (zero + 2000) % n, (zero + 3000) % n};
    std::cout << sum_indices(indices, data, goal_indices) << '\n';
}

void part_two()
{
    auto [data, zero] = read_file("day20.in");
    long n = data.size();

    for (long i = 0; i < n; ++i)
        data[i] *= 811589153;

    std::vector<long> indices;
    for (long i = 0; i < n; ++i)
        indices.push_back(i);

    for (int j = 0; j < 10; ++j)
        for (long i = 0; i < n; ++i)
            update_indices(indices, i, data[i], n);

    zero = indices[zero];
    std::vector<long> goal_indices = {(zero + 1000) % n, (zero + 2000) % n, (zero + 3000) % n};
    std::cout << sum_indices(indices, data, goal_indices) << '\n';
}

int main()
{
    part_one();
    part_two();
}
