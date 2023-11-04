#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

struct Point
{
    int x;
    int y;
    int z;

    bool operator==(const Point &other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    std::vector<std::vector<Point>> raytrace(int n) const
    {
        std::vector<std::vector<Point>> out(6);
        for (int i = 1; i <= n; ++i)
        {
            out[0].push_back({x + i, y, z}); // NOLINT
            out[1].push_back({x - i, y, z}); // NOLINT
            out[2].push_back({x, y + i, z}); // NOLINT
            out[3].push_back({x, y - i, z}); // NOLINT
            out[4].push_back({x, y, z + i}); // NOLINT
            out[5].push_back({x, y, z - i}); // NOLINT
        }

        return out;
    }
};

template <> struct std::hash<Point>
{
    std::size_t operator()(const Point &p) const noexcept
    {
        std::size_t h1 = std::hash<int>{}(p.x);
        std::size_t h2 = std::hash<int>{}(p.y);
        std::size_t h3 = std::hash<int>{}(p.z);
        return h1 ^ (h2 << 2) ^ (h3 << 1);
    }
};

std::unordered_set<Point> read_file(std::string filename)
{
    std::ifstream ifs(filename);
    std::unordered_set<Point> out;

    int x, y, z;
    char gb;
    while (ifs.good())
    {
        ifs >> x;
        ifs >> gb;
        ifs >> y;
        ifs >> gb;
        ifs >> z;

        out.insert({x, y, z});
    }

    return out;
}

int find_area(const std::unordered_set<Point> &points)
{
    int solution = points.size() * 6;
    for (const auto &p : points)
        for (const auto &n : p.raytrace(1))
            if (const auto search = points.find(n[0]); search != points.end())
                --solution;

    return solution;
}

std::unordered_set<Point> find_interior(const std::unordered_set<Point> &points)
{
    constexpr int max = 20;

    std::unordered_set<Point> out;

    // find the universe set of points
    std::vector<Point> universe;
    for (int x = 0; x < max; ++x)
        for (int y = 0; y < max; ++y)
            for (int z = 0; z < max; ++z)
                universe.push_back({x, y, z});

    for (const auto &p : universe)
    {
        if (const auto search = points.find(p); search != points.end())
            continue;

        const auto rays = p.raytrace(max);
        std::vector<bool> seen(6);
        for (int i = 0; i < 6; ++i)
            for (int j = 0; j < max && !seen[i]; ++j)
                if (const auto search = points.find(rays[i][j]); search != points.end())
                    seen[i] = true;

        bool all = true;
        for (int i = 0; i < 6 && all; ++i)
            if (!seen[i])
                all = false;

        if (all)
            out.insert(p);
    }

    return out;
}

void part_one()
{
    const auto &points = read_file("day18.in");
    std::cout << find_area(points) << "\n";
}

void part_two()
{
    const auto &points = read_file("day18.in");
    const auto &interior = find_interior(points);
    std::cout << find_area(points) - find_area(interior) << "\n";
}

int main()
{
    part_one();
    part_two();
}
