#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <vector>

// # of robots
constexpr int N = 4;

int ceil_divide(int x, int y)
{
    if (x == 0)
        return 0;

    if (y == 0)
        return std::numeric_limits<int>::max();

    return x / y + ((x % y) != 0);
}

struct Robot
{
    void collect(std::vector<int> &bag, int t, int k) const
    {
        bag[resource_type] += t * k;
    }

    void build(std::vector<int> &bag, std::vector<int> &robots) const
    {
        for (int i = 0; i < N; ++i)
            bag[i] -= cost[i];
        ++robots[resource_type];
    }

    void unbuild(std::vector<int> &bag, std::vector<int> &robots) const
    {
        for (int i = 0; i < N; ++i)
            bag[i] += cost[i];
        --robots[resource_type];
    }

    int time_to_build(const std::vector<int> &bag, const std::vector<int> &robots) const
    {
        int out = std::numeric_limits<int>::min();
        for (int i = 0; i < N; ++i)
            out = std::max(out, ceil_divide(std::max(0, cost[i] - bag[i]), robots[i]));
        return out;
    }

    int resource_type;
    std::vector<int> cost;
};

struct Blueprint
{
    std::vector<Robot> robots;
};

std::vector<Blueprint> read_file(std::string filename)
{
    std::vector<Blueprint> out;
    std::ifstream ifs(filename);

    int ore = 0, clay = 0, obsidian = 0, geode = 0;
    std::string line;
    int index = 0;
    while (++index, std::getline(ifs, line))
    {
        std::stringstream ss(line);

        ss.ignore(33 + (index >= 10));
        ss >> ore;
        Robot ore_robot{0, {ore, clay, obsidian, geode}};
        ore = 0;

        ss.ignore(27);
        ss >> ore;
        Robot clay_robot{1, {ore, clay, obsidian, geode}};
        ore = 0;

        ss.ignore(31);
        ss >> ore;
        ss.ignore(8);
        ss >> clay;
        Robot obsidian_robot{2, {ore, clay, obsidian, geode}};
        ore = clay = 0;

        ss.ignore(29);
        ss >> ore;
        ss.ignore(8);
        ss >> obsidian;
        Robot geode_robot{3, {ore, clay, obsidian, geode}};
        ore = obsidian = 0;

        out.push_back(Blueprint{{ore_robot, clay_robot, obsidian_robot, geode_robot}});
    }

    return out;
}

bool has_potential(const std::vector<int> &bag, const std::vector<int> &robots, int time, int best)
{
    // whether the best possible score can beat the current global_best
    // this is whether at every time step building a robot, we can surpass the best
    // with t left, we can make (t-1) + (t-2) + .. + 1 = t * (t-1) / 2
    // (this doesnt always work because of integer divison)
    return (time * time / 2 + robots[N - 1] * time) > (best - bag[N - 1]);
}

int max_score(const Blueprint &bp, std::vector<int> &bag, std::vector<int> &robots, int time, int *global_best)
{
    // make a choice to build one more of any robot (and take the max)
    // returns the # of geodes it created (starting at time)

    // finds the limiting resource to create that robot (by time)
    // increment bag by time * # collecting robots
    // build the robot (decrement inventory)
    // recursive call

    int best = bag[N - 1];
    for (int i = 0; i < N; ++i)
    {
        if (!has_potential(bag, robots, time, *global_best))
            continue;

        int t = bp.robots[i].time_to_build(bag, robots);
        if (t >= time)
            continue;

        // spend 1 time actually building the robot
        ++t;

        // do action
        for (int i = 0; i < N; ++i)
            bp.robots[i].collect(bag, t, robots[i]);
        bp.robots[i].build(bag, robots);

        best = std::max(best, max_score(bp, bag, robots, time - t, global_best));
        *global_best = std::max(best, *global_best);

        // undo action
        bp.robots[i].unbuild(bag, robots);
        for (int i = 0; i < N; ++i)
            bp.robots[i].collect(bag, -t, robots[i]);
    }

    return best;
}

void part_one()
{
    const auto blueprints = read_file("day19.in");
    std::vector<int> robots = {1, 0, 0, 0};
    std::vector<int> bag(N);

    int solution = 0;
    for (int i = 0; i < blueprints.size(); ++i)
    {
        int global_best = 0;
        auto score = max_score(blueprints[i], bag, robots, 24, &global_best);
        solution += (i + 1) * score;
    }
    std::cout << solution << "\n";
}

void part_two()
{
    const auto blueprints_all = read_file("day19.in");
    const std::vector<Blueprint> blueprints(blueprints_all.begin(), blueprints_all.begin() + 3);
    std::vector<int> robots = {1, 0, 0, 0};
    std::vector<int> bag(N);

    int solution = 1;
    for (int i = 0; i < blueprints.size(); ++i)
    {
        int global_best = 0;
        auto score = max_score(blueprints[i], bag, robots, 32, &global_best);
        solution *= score;
    }
    std::cout << solution << "\n";
}

int main()
{
    // part_one();
    part_two();
}
