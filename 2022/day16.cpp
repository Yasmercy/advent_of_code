#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <vector>

constexpr size_t N = 10;

struct TunnelData
{
    std::vector<std::vector<size_t>> adjList = std::vector<std::vector<size_t>>(N);
    std::vector<std::vector<size_t>> distances;
    std::vector<size_t> values = std::vector<size_t>(N);
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

size_t findMax(TunnelData &data, size_t time)
{
    size_t score = 0;
    size_t cur = data.start;
    while (time > 1)
    {
        // find the best next_node to go to
        size_t best = 0;
        size_t next_to = 0;
        for (size_t i = 0; i < data.adjList.size(); ++i)
        {
            if (data.distances[cur][i] == 0 || (time <= data.distances[cur][i]))
                continue;
            size_t value = data.values[i] * (time - 1 - data.distances[cur][i]);
            if (value > best)
            {
                best = value;
                next_to = i;
            }
        }

        score += best;
        time -= data.distances[cur][next_to] + 1;
        cur = next_to;
        data.values[cur] = 0;
        if (best == 0)
        {
            break;
        }
    }
    return score;
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
            if (nodes[name] == 11)
            {
                std::cout << nodes[name] << std::endl;
            }
            out.adjList[start].push_back(nodes[name]);
        }
    }

    out.start = nodes["AA"];
    return out;
}

void part_one()
{
    auto data = read_file("day16.in");
    allPairsShortestPaths(data);
    size_t sol = findMax(data, 30);
    std::cout << sol << "\n";
}

int main()
{
    part_one();
}
