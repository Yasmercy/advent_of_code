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
        if (node == width * height)
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
            return {i, i + 1};
        if (i == width * height)
            return {i, i - 1};

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

int h(const Position &pos, const BlizzardMap &blizzards)
{
    // return manhattan distance to end
    int row = pos.pos / blizzards.width;
    int col = pos.pos % blizzards.width;
    int dist = blizzards.height - row + blizzards.width - col;
    return dist;
}

int f(const Position &pos, const BlizzardMap &blizzards)
{
    return pos.time + h(pos, blizzards);
};

void heap_push(std::vector<Position> &heap, const BlizzardMap &blizzards, const Position &pos)
{
    heap.push_back(pos);

    // heapify up
    int i = heap.size() - 1;
    int parent = (i - 1) / 2;
    while (i > 0 && f(heap[i], blizzards) < f(heap[parent], blizzards))
    {
        std::swap(heap[i], heap[parent]);
        i = parent;
        parent = (i - 1) / 2;
    }
}

Position heap_pop(std::vector<Position> &heap, const BlizzardMap &blizzards)
{
    Position out = heap[0];
    heap[0] = heap.back();
    heap.pop_back();

    // heapify down
    int start = 0;
    while (true)
    {
        int left = 2 * start + 1;
        int right = 2 * start + 2;
        int min = start;

        if (left < heap.size() && f(heap[min], blizzards) > f(heap[left], blizzards))
            min = left;
        if (right < heap.size() && f(heap[min], blizzards) > f(heap[right], blizzards))
            min = right;

        if (min == start)
            break;

        std::swap(heap[start], heap[min]);
        start = min;
    };

    return out;
}

int solve(BlizzardMap blizzards, int start, int start_time, int end)
{
    std::vector<Position> heap{{start, start_time}};
    std::set<Position> seen;

    int lcm = std::lcm(blizzards.width, blizzards.height);

    while (!heap.empty())
    {
        Position next = heap_pop(heap, blizzards);
        if (seen.contains(next))
            continue;
        seen.insert(next);

        if (next.pos == end)
            return next.time;

        for (int j : blizzards.neighbors(next.pos))
            if (!blizzards.blizzard(j, (next.time + 1) % lcm) && !seen.contains({j, next.time + 1}))
                heap_push(heap, blizzards, {j, next.time + 1});
    };

    throw std::runtime_error("failed");
}

void part_one()
{
    BlizzardMap blizzards = read_file("day24.in");
    int solution = solve(blizzards, -1, 0, blizzards.width * blizzards.height - 1) + 1;
    std::cout << solution << '\n';
};

void part_two()
{
    BlizzardMap blizzards = read_file("day24.in");
    int begin = -1;
    int end = blizzards.width * blizzards.height;
    int time = 0;

    time = solve(blizzards, begin, time, end - 1) + 1;
    time = solve(blizzards, end, time, begin + 1) + 1;
    time = solve(blizzards, begin, time, end - 1) + 1;

    std::cout << time << '\n';
}

int main()
{
    part_one();
    part_two();
}
