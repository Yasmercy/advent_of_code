#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <vector>

// NOTE: need to change this for differnet inputs
const int N = 10;

struct Graph
{
    std::vector<std::vector<int>> adjList;
    std::vector<std::vector<int>> distances;
    std::vector<int> values;
    std::vector<int> node_map;
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
            {
                nodes[name] = nodes.size();
            }
            g.adjList[nodes[name]].push_back(start);
        }
    }

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

int solve_helper(const Graph &graph, int cur1, int time1, int cur2, int time2, std::vector<bool> &seen, int max_time)
{
    // two actors are independent iff the time difference is less than the min of travel time
    // in this case, we can move them both simultaneously
    // otherwise we act one at a time

    int best = 0;
    int size = graph.node_map.size();

    std::vector<int> dependent_actions;
    std::vector<int> independent_actions;

    // partition action space into those that are less than time difference
    int actor = (time1 < time2) ? cur1 : cur2;
    int other = (actor == cur1) ? cur2 : cur1;
    int actor_time = std::min(time1, time2);
    int other_time = std::max(time1, time2);
    int time_diff = other_time - actor_time;
    for (int i = 0; i < size; ++i)
    {
        if (seen[i])
            continue;

        if (graph.distances[actor][i] < time_diff)
            dependent_actions.push_back(i);
        else
            independent_actions.push_back(i);
    }

    // do actions for behind actor
    for (int next : dependent_actions)
    {
        int dist = graph.distances[actor][next];
        if (dist >= (max_time - actor_time) || seen[next])
            continue;

        seen[next] = true;
        int score = solve_helper(graph, next, actor_time + dist + 1, other, other_time, seen, max_time);
        score += graph.values[next] * (max_time - actor_time - dist);
        best = std::max(score, best);
        seen[next] = false;
    }

    // do actions for both actors
    for (int next1 : independent_actions)
    {
        int dist1 = graph.distances[actor][next1];
        if (dist1 >= (max_time - actor_time) || seen[next1])
            continue;

        for (int next2 = 0; next2 < size; ++next2)
        {
            int dist2 = graph.distances[other][next2];
            if (dist2 >= (max_time - other_time) || seen[next2] || next1 == next2)
                continue;

            seen[next1] = true;
            seen[next2] = true;

            int score =
                solve_helper(graph, next1, actor_time + dist1 + 1, next2, other_time + dist2 + 1, seen, max_time);
            score += graph.values[next1] * (max_time - actor_time - dist1);
            score += graph.values[next2] * (max_time - other_time - dist2);
            best = std::max(score, best);

            seen[next1] = false;
            seen[next2] = false;
        }
    }

    return best;
}

void part_one()
{
    auto data = read_file("day16.in");
    allPairsShortestPaths(data);
    auto graph = reduceGraph(data);

    int size = graph.node_map.size();
    int T = 30;
    int best = 0;

    for (int start = 0; start < size; ++start)
    {
        std::vector<bool> seen(size);
        auto orig = graph.node_map[start];
        auto dist = data.distances[orig][start];
        int score = solve_helper(graph, start, 1 + dist, 0, T + 1, seen, T);

        best = std::max(score, best);
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

    for (int start1 = 0; start1 < size; ++start1)
    {
        for (int start2 = 0; start2 < size; ++start2)
        {
            std::vector<bool> seen(size);
            auto orig1 = graph.node_map[start1];
            auto dist1 = data.distances[orig1][start1];
            auto orig2 = graph.node_map[start2];
            auto dist2 = data.distances[orig2][start2];
            int score = solve_helper(graph, start1, 1 + dist1, start2, 1 + dist2, seen, T);

            best = std::max(score, best);
        }
    }

    std::cout << "part two sol: " << best << "\n";
}

int main()
{
    part_one();
    part_two();
}
