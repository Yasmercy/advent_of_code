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
#include <numeric>
#include <queue>
#include <set>
#include <string>
#include <vector>

struct BlizzardMap
{
    bool blizzard(int node, int time)
    {
        if (node == -1)
            return false;

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
        if (i == -1)
            return {-1, 0};

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

struct Position
{
    int pos;
    int time;

    auto operator<=>(const Position &) const = default;
};

// replace this with A*
int solve(BlizzardMap blizzards)
{
    std::queue<Position> queue;
    std::set<Position> seen;
    queue.push({-1, 0});
    int end = blizzards.width * blizzards.height - 1;
    int lcm = std::lcm(blizzards.width, blizzards.height);

    while (!queue.empty())
    {
        Position next = queue.front();
        seen.insert(next);
        queue.pop();

        if (next.pos == end)
            return next.time + 1;

        for (int j : blizzards.neighbors(next.pos))
            if (!blizzards.blizzard(j, (next.time + 1) % lcm) && !seen.contains({j, next.time + 1}))
                queue.push({j, next.time + 1});
    };
    throw std::runtime_error("failed");
}

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
