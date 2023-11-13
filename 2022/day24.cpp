/* DP
 *
 * S: whether you can reach the end from node i starting at blizzard state t, within n steps
 * R: any dp[n-1][j][t-1] where j is a neighbor of i
 * T: increasing n
 * B: dp[0][i][t] = false, dp[n][size - 1][t] = true
 * O: dp[n][start][0] where n is the first state that it is true
 * T: we have T * V * N problems, and for each N has O(E) work => O(T * N * V * E)
 */

#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <set>
#include <string>
#include <vector>
#include <numeric>

struct BlizzardMap
{
    bool blizzard(int node, int time)
    {
        int row = node / width;
        int col = node % width;

        int N = (height + ((row + time) % height)) % height;
        int S = (height + ((row - time) % height)) % height;
        int E = (width + ((col - time) % width)) % width;
        int W = (width + ((col + time) % width)) % width;

        return blizzards[0].contains(N * width + col)     // NOLINT
               || blizzards[1].contains(S * width + col)  // NOLINT
               || blizzards[2].contains(row * width + E)  // NOLINT
               || blizzards[3].contains(row * width + W); // NOLINT
    }

    std::vector<int> neighbors(int i)
    {
        std::vector<int> out;
        int row = i / width;
        int col = i % width;

        if (row > 0)
            out.push_back(i - width);
        if (row + 1 < height)
            out.push_back(i + width);
        if (col > 0)
            out.push_back(i - 1);
        if (col + 1 < width)
            out.push_back(i + 1);
        out.push_back(i);

        return out;
    }

    int width;
    int height;
    // NORTH, SOUTH, EAST, WEST blizzards
    std::vector<std::set<int>> blizzards = std::vector<std::set<int>>(4);
};

BlizzardMap read_file(std::string filename)
{
    BlizzardMap out;
    std::vector<std::string> file;
    std::ifstream ifs(filename);
    std::string line;
    while (std::getline(ifs, line))
        file.push_back(line);

    out.height = file.size() - 2;
    out.width = file[0].size() - 2;

    int index = -1;
    for (const auto &f : file)
        for (const char c : f)
            switch (c)
            {
            case '^':
                out.blizzards[0].insert(index++);
                break;
            case 'v':
                out.blizzards[1].insert(index++);
                break;
            case '>':
                out.blizzards[2].insert(index++);
                break;
            case '<':
                out.blizzards[3].insert(index++);
                break;
            case '.':
                ++index;
                break;
            }

    return out;
}

std::vector<std::vector<bool>> make_blank(BlizzardMap blizzards, int size, int lcm)
{
    std::vector<std::vector<bool>> tmp(size - 1, std::vector<bool>(lcm, false));
    std::vector<bool> last;
    for (int t = 0; t < lcm; ++t)
        last.push_back(!blizzards.blizzard(size - 1, t));
    tmp.push_back(last);
    return tmp;
}

int solve(BlizzardMap blizzards)
{
    const int size = blizzards.width * blizzards.height;
    const int lcm = std::lcm(blizzards.width, blizzards.height);
    std::vector<std::vector<std::vector<bool>>> dp{make_blank(blizzards, size, lcm)};

    // int done = std::numeric_limits<int>::max();
    bool done = false;
    while (dp.size() && !done)
    {
        std::cout << dp.size() << '\n';
        for (auto b : dp.back()[0])
            done = done || b;
            // if (b)
            //     // done = std::min(done, static_cast<int>(dp.size() + size));

        auto add = dp.back();
        for (int i = 0; i < size; ++i)
            for (int t = 0; t < lcm; ++t)
                for (int j : blizzards.neighbors(i))
                {
                    int next_time = (t + lcm + 1) % lcm;
                    add[i][t] = add[i][t] || (!blizzards.blizzard(j, next_time) && dp.back()[j][next_time]);
                    // add[i][t] = add[i][t] || dp.back()[j][next_time];
                }
        dp.push_back(add);
    }

    int best = std::numeric_limits<int>::max();
    for (int i = 0; i < dp.size(); ++i)
        for (int b = 0; b < lcm; ++b)
            if (dp[i][0][b])
                best = std::min(best, i + ((b + lcm - 1) % lcm));
    return best + 2; // + 1 for each start and end
};

void part_one()
{
    BlizzardMap blizzards = read_file("day24.in");
    int solution = solve(blizzards);
    std::cout << solution << '\n';
};

int main()
{
    part_one();
}
