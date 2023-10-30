#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <vector>

constexpr size_t N = 62;
constexpr size_t T = 30;

struct TunnelData
{
    // adj list stores all nodes with incoming edges (rather than outgoing for normal)
    std::vector<std::vector<size_t>> adjList = std::vector<std::vector<size_t>>(N);
    std::vector<int> values = std::vector<int>(N);
    std::vector<std::vector<size_t>> distances;
    std::vector<int> reduced_mapping;
    size_t start = 0;
};

void allPairsShortestPaths(TunnelData &data)
{
    std::vector<std::vector<size_t>> dist(N, std::vector<size_t>(N, std::numeric_limits<size_t>::max()));
    for (size_t u = 0; u < N; ++u)
    {
        for (const auto v : data.adjList[u])
        {
            dist[u][v] = 1;
        }
        dist[u][u] = 0;
    }
    for (size_t k = 0; k < N; ++k)
    {
        for (size_t i = 0; i < N; ++i)
        {
            for (size_t j = 0; j < N; ++j)
            {
                if (dist[i][k] != std::numeric_limits<size_t>::max() &&
                    dist[k][j] != std::numeric_limits<size_t>::max())
                {
                    dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }

    data.distances = dist;
}

TunnelData reduceGraph(const TunnelData &data)
{
    // removes all nodes with value = 0
    std::vector<int> keep;
    for (int i = 0; i < N; ++i)
    {
        if (data.values[i] != 0)
            keep.push_back(i);
    }

    TunnelData out;

    std::vector<std::vector<size_t>> distances(keep.size(), std::vector<size_t>(keep.size(), 0));
    std::vector<int> values(keep.size(), 0);

    for (int i = 0; i < keep.size(); ++i)
    {
        values[i] = data.values[keep[i]];
        for (int j = 0; j < keep.size(); ++j)
        {
            distances[i][j] = data.distances[keep[i]][keep[j]];
        }
    }

    out.distances = distances;
    out.values = values;
    out.reduced_mapping = keep;

    return out;
}

size_t solve_helper(const TunnelData &data, int cur, std::vector<bool> &seen, int time, size_t size)
{
    size_t best = 0;
    if (time > T)
        return 0;

    for (int i = 0; i < size; ++i)
    {
        if (seen[i])
            continue;

        seen[i] = true;
        size_t dist = data.distances[cur][i];

        if (time + dist > T)
        {
            seen[i] = false;
            continue;
        }

        best = std::max(best, data.values[i] * (T - time - dist) + solve_helper(data, i, seen, time + dist + 1, size));
        seen[i] = false;
    }

    return best;
}

int solve(const TunnelData &data, const TunnelData &reduced)
{
    size_t size = reduced.values.size();
    std::vector<bool> seen = std::vector<bool>(size, false);

    // because start might not be in the matrix
    // do the first step outside of the solve_helper
    size_t best = 0;
    for (int i = 0; i < size; ++i)
    {
        auto start = reduced.reduced_mapping[i];
        best = std::max(best, solve_helper(reduced, i, seen, 1 + data.distances[data.start][start], size));
    }

    return best;
}

TunnelData read_file(std::string filename)
{
    TunnelData out;
    std::ifstream ifs(filename);
    std::map<std::string, size_t> nodes;
    std::string line;
    std::string name;
    std::string flow;
    std::string garbage;
    while (std::getline(ifs, line))
    {
        std::istringstream ss(line);

        // get start
        ss.ignore(6);
        ss >> name;
        if (auto search = nodes.find(name); search == nodes.end())
        {
            nodes[name] = nodes.size();
        }
        int start = nodes[name];

        // get flow rate
        ss.ignore(std::numeric_limits<std::streamsize>::max(), '=');
        std::getline(ss, flow, ';');
        out.values[start] = std::stoi(flow);

        // get all connections
        for (int i = 0; i < 4; ++i)
            ss >> garbage;
        ss.ignore(1);
        while (std::getline(ss, name, ','))
        {
            ss.ignore(1);
            if (auto search = nodes.find(name); search == nodes.end())
            {
                nodes[name] = nodes.size();
            }
            out.adjList[nodes[name]].push_back(start);
        }
    }

    out.start = nodes["AA"];
    return out;
}

void part_one()
{
    auto data = read_file("day16.in");
    allPairsShortestPaths(data);
    auto reduced = reduceGraph(data);
    auto sol = solve(data, reduced);
    std::cout << "part one solution: " << sol << std::endl;
}

int main()
{
    part_one();
}
