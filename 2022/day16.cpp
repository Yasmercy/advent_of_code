#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <vector>

constexpr size_t N = 10;
constexpr int T = 30;

/*
 * S: N x N x 2 x t subproblems
 * s[i][j][t][b] = maximum value given that at time t,
 *      you are at node i and node j is b (true or false)
 * R:
 *  max of:
 *      activating that node (flow_rate * time)
 *          (at i, when i was not active before)
 *      go to i from any previous node j -> i
 *          (that j is not active)
 *
 * T: t: 0->t, fill out all NxN array in any order
 * B: s[i][j][0] = 0 for all i, j
 * O: max {s['AA'][j][0][b] for all j, b}
 * T: N x N x 2 x t
 */

struct TunnelData
{
    // adj list stores all nodes with incoming edges (rather than outgoing for normal)
    std::vector<std::vector<size_t>> adjList = std::vector<std::vector<size_t>>(N);
    std::vector<int> values = std::vector<int>(N);
    size_t start = 0;
};

int solve(const TunnelData &data)
{
    int dp[N][N][T + 1][2];

    // base case: at time t, maximum score is 0
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            for (int b = 0; b < 2; ++b)
                dp[i][j][T][b] = 0;

    // for each time step
    for (int t = T - 1; t >= 0; --t)
    {
        // for each node i
        for (int i = 0; i < N; ++i)
        {
            // for each node j
            // todo: maybe swap this to p (prev) (default 0 for everything)
            for (int j = 0; j < N; ++j)
            {
                // max with b = 0
                // max of any previous node p that incomes to node i
                // max { dp[p][j][t+1][1], dp[i][i][t+1][0] + (t * v[i]) if i != j}
                dp[i][j][t][0] = dp[i][j][t + 1][0];
                for (int p : data.adjList[i])
                    dp[i][j][t][0] = std::max(dp[i][j][t][0], dp[p][j][t + 1][0]);
                if (i != j)
                    dp[i][j][t][0] = std::max(dp[i][j][t][0], dp[i][j][t + 1][0] + data.values[i] * t);

                // max with b = 1
                // max of activating it from previous time
                // max of any previous node p that incomes to node i
                // max { dp[p][j][t+1][1], dp[i][i][t+1][0] + (t * v[i]) }
                dp[i][j][t][1] = dp[i][j][t + 1][1];
                for (int p : data.adjList[i])
                    dp[i][j][t][1] = std::max(dp[i][j][t][1], dp[p][j][t + 1][1]);
                dp[i][j][t][1] = std::max(dp[i][j][t][1], dp[i][i][t + 1][0] + t * data.values[i]);
            }
        }
    }

    int sol = 0;
    // solve original problem: i = start, t = 0
    for (int j = 0; j < N; ++j)
        for (int b = 0; b < 2; ++b)
            sol = std::max(sol, dp[data.start][j][0][b]);

    return sol;
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
