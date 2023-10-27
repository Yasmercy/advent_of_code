#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <vector>

constexpr size_t N = 10;
constexpr int T = 4;

struct TunnelData
{
    // adj list stores all nodes with incoming edges (rather than outgoing for normal)
    std::vector<std::vector<size_t>> adjList = std::vector<std::vector<size_t>>(N);
    std::vector<int> values = std::vector<int>(N);
    size_t start = 0;
};

int solve(const TunnelData &data)
{
    int dp[T + 1][N][N + 1];

    // base case: at time t, maximum score is 0
    for (int i = 0; i < N; ++i)
        for (int j = 0; j <= N; ++j)
            for (int b = 0; b < 2; ++b)
                dp[T][i][j] = 0;
    // base case 2: using no nodes, maximum score is 0
    for (int i = 0; i < N; ++i)
        for (int t = 0; t <= T; ++t)
            dp[t][i][0] = 0;

    // while at time = t
    for (int t = T - 1; t >= 0; --t)
    {
        // while sitting at node = i
        for (int i = 0; i < N; ++i)
        {
            // by including node j - 1
            for (int j = 1; j <= N; ++j)
            {
                // worst case, just copy what we were at a time before,
                // or when we did not include j
                dp[t][i][j] = std::max(dp[t + 1][i][j], dp[t][i][j - 1]);

                // if we are sitting on j, we can turn valve j on
                if (i == j - 1)
                    dp[t][i][j] = std::max(dp[t][i][j], dp[t + 1][i][j - 1] + (T - t) * data.values[i]);
                // else we max all states that we could've been before i
                else
                    for (int p : data.adjList[i])
                        dp[t][i][j] = std::max(dp[t][i][j], dp[t + 1][p][j]);
            }
        }
    }

    // solve original problem: t = 0, i = start, j = N
    return dp[0][data.start][N];
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
    auto sol = solve(data);
    std::cout << "part one solution: " << sol << std::endl;
}

int main()
{
    part_one();
}
