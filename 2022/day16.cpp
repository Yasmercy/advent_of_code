#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

// NOTE: need to change this for differnet inputs
const int N = 62;

struct Graph
{
    std::vector<std::vector<int>> adjList;
    std::vector<std::vector<int>> distances;
    std::vector<int> values;
    std::vector<int> node_map;
    int start;
};

Graph read_file(std::string filename)
{

    Graph g;
    g.adjList.resize(N);
    g.values.resize(N);

    std::ifstream ifs(filename);
    std::map<std::string, int> nodes;
    std::string line, name, flow, garbage;

    while (std::getline(ifs, line))
    {
        std::istringstream ss(line);

        // get start
        ss.ignore(6);
        ss >> name;
        if (auto search = nodes.find(name); search == nodes.end())
            nodes[name] = nodes.size();
        int start = nodes[name];

        // get flow rate
        ss.ignore(std::numeric_limits<std::streamsize>::max(), '=');
        std::getline(ss, flow, ';');
        g.values[start] = std::stoi(flow);

        // get all connections
        for (int i = 0; i < 4; ++i)
            ss >> garbage;
        ss.ignore(1);
        while (std::getline(ss, name, ','))
        {
            ss.ignore(1);
            if (auto search = nodes.find(name); search == nodes.end())
                nodes[name] = nodes.size();
            g.adjList[nodes[name]].push_back(start);
        }
    }

    g.start = nodes["AA"];
    return g;
};

void allPairsShortestPaths(Graph &data)
{
    std::vector<std::vector<int>> dist(N, std::vector<int>(N, std::numeric_limits<int>::max()));
    for (int u = 0; u < N; ++u)
    {
        for (const auto v : data.adjList[u])
            dist[u][v] = 1;
        dist[u][u] = 0;
    }

    for (int k = 0; k < N; ++k)
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                if (dist[i][k] != std::numeric_limits<int>::max() && dist[k][j] != std::numeric_limits<int>::max())
                    dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);

    data.distances = dist;
}

Graph reduceGraph(const Graph &data)
{
    // removes all nodes with value = 0
    std::vector<int> keep;
    for (int i = 0; i < N; ++i)
        if (data.values[i] != 0)
            keep.push_back(i);

    Graph out;

    std::vector<std::vector<int>> distances(keep.size(), std::vector<int>(keep.size(), 0));
    std::vector<int> values(keep.size(), 0);

    for (int i = 0; i < keep.size(); ++i)
    {
        values[i] = data.values[keep[i]];
        for (int j = 0; j < keep.size(); ++j)
            distances[i][j] = data.distances[keep[i]][keep[j]];
    }

    out.distances = distances;
    out.values = values;
    out.node_map = keep;

    return out;
}

int ideal_score(const Graph &graph, int t1, int t2, std::vector<bool> &seen, int max_time)
{
    std::vector<int> values;
    for (int i = 0; i < seen.size(); ++i)
        if (!seen[i])
            values.push_back(graph.values[i]);

    // assumes all are 1 distance away
    int score = 0;
    std::sort(values.begin(), values.end());
    for (int i = values.size() - 1; i >= 0; --i)
        if (t1 < t2)
            score += (max_time - --t1 - 2) * values[i];
        else
            score += (max_time - --t2 - 2) * values[i];
    return score;
}

int hash_position(int cur1, int cur2, int time1, int time2)
{
    if (cur2 > cur1)
    {
        std::swap(cur1, cur2);
        std::swap(time1, time2);
    }

    return cur1 * std::pow(50, 3) + cur2 * std::pow(50, 2) + time1 * 50 + time2;
}

void solve_helper(const Graph &graph, int cur1, int time1, int cur2, int time2, std::vector<bool> &seen, int max_time,
                  int cur_score, int *best, std::map<int, int> &visited)
{
    if (cur_score + ideal_score(graph, time1, time2, seen, max_time) < *best)
        return;

    if (auto search = visited.find(hash_position(cur1, cur2, time1, time2));
        search != visited.end() && search->second >= cur_score)
        return;

    visited.insert({hash_position(cur1, cur2, time1, time2), cur_score});

    int size = graph.node_map.size();
    // have cur2 do nothing
    for (int n1 = 0; n1 < size; ++n1)
    {
        int d1 = graph.distances[cur1][n1];
        if (time1 + d1 <= max_time && !seen[n1])
        {
            int score = cur_score + graph.values[n1] * (max_time - d1 - time1);
            *best = std::max(score, *best);

            seen[n1] = true;
            solve_helper(graph, n1, time1 + d1 + 1, cur2, time2, seen, max_time, score, best, visited);
            seen[n1] = false;
        }
    }

    // have cur1 do nothing
    for (int n2 = 0; n2 < size; ++n2)
    {
        int d2 = graph.distances[cur2][n2];
        if (time2 + d2 <= max_time && !seen[n2])
        {
            int score = cur_score + graph.values[n2] * (max_time - d2 - time2);
            *best = std::max(score, *best);

            seen[n2] = true;
            solve_helper(graph, cur1, time1, n2, time2 + d2 + 1, seen, max_time, score, best, visited);
            seen[n2] = false;
        }
    }
}

void part_one()
{
    auto data = read_file("day16.in");
    allPairsShortestPaths(data);
    auto graph = reduceGraph(data);

    int size = graph.node_map.size();
    int T = 30;
    int best = 0;
    std::map<int, int> visited;

    for (int start = 0; start < size; ++start)
    {
        std::vector<bool> seen(size);
        auto dist = data.distances[data.start][graph.node_map[start]];
        solve_helper(graph, start, 1 + dist, 0, T + 1, seen, T, 0, &best, visited);
    }

    std::cout << "part one sol: " << best << "\n";
}

void part_two()
{
    auto data = read_file("day16.in");
    allPairsShortestPaths(data);
    auto graph = reduceGraph(data);

    int size = graph.node_map.size();
    int T = 26;
    int best = 0;
    std::map<int, int> visited;

    // lower bound with greedy method
    for (int start1 = 0; start1 < size; ++start1)
    {
        for (int start2 = start1; start2 < size; ++start2)
        {
            std::vector<bool> seen(size);
            auto dist1 = data.distances[data.start][graph.node_map[start1]];
            auto dist2 = data.distances[data.start][graph.node_map[start2]];
            solve_helper(graph, start1, 1 + dist1, start2, 1 + dist2, seen, T, 0, &best, visited);
        }
    }

    std::cout << "part two sol: " << best << "\n";
}

int main()
{
    part_one();
    part_two();
}
