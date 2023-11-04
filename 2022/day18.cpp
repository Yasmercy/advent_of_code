#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

constexpr int MAX = 25;

struct Point
{
    int x;
    int y;
    int z;

    bool operator==(const Point &other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    std::vector<Point> neighbors() const
    {
        return {
            {x + 1, y, z}, // NOLINT
            {x - 1, y, z}, // NOLINT
            {x, y + 1, z}, // NOLINT
            {x, y - 1, z}, // NOLINT
            {x, y, z + 1}, // NOLINT
            {x, y, z - 1}, // NOLINT
        };
    }

    int to_int() const
    {
        return x * MAX * MAX + y * MAX + z;
    }
};

template <> struct std::hash<Point>
{
    std::size_t operator()(const Point &p) const noexcept
    {
        return std::hash<int>{}(p.to_int());
    }
};

struct DisjointSet
{
    DisjointSet(int n) : ds(std::vector<int>(n, -1))
    {
    }

    void join(int x, int y)
    {
        int i = find(x);
        int j = find(y);

        if (i == j)
            return;

        // union by size
        if (size(i) < size(j))
        {
            ds[i] += ds[j];
            ds[j] = i;
        }
        else
        {
            ds[j] += ds[i];
            ds[i] = j;
        }
    }

    int find(int x)
    {
        // does path compression
        if (ds[x] >= 0)
        {
            int out = find(ds[x]);
            ds[x] = out;
            return out;
        }

        return x;
    }

    int size(int x)
    {
        return -ds[find(x)];
    }

    std::vector<int> ds;
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
        for (const auto &n : p.neighbors())
            if (const auto search = points.find(n); search != points.end())
                --solution;

    return solution;
}

std::unordered_set<Point> find_interior(const std::unordered_set<Point> &points)
{
    // find the universe set of points
    std::vector<Point> universe;
    for (int x = 0; x < MAX; ++x)
        for (int y = 0; y < MAX; ++y)
            for (int z = 0; z < MAX; ++z)
                universe.push_back({x, y, z});

    // create a disjoint set, equivalence relation = connected
    DisjointSet ds(MAX * MAX * MAX);

    // connect border with itself
    const auto border = points.begin()->to_int();
    for (const auto &p : points)
        ds.join(border, p.to_int());

    // connect all other points (can't pass boundary)
    for (const auto &p : universe)
        for (const auto &n : p.neighbors())
            if (n.to_int() >= 0 && n.to_int() < ds.ds.size() && ds.find(n.to_int()) != ds.find(border) &&
                ds.find(p.to_int()) != ds.find(border))
                ds.join(n.to_int(), p.to_int());

    // do a linear search to find all points that aren't on edge and boundary
    std::unordered_set<Point> out;
    const auto edge = universe.back().to_int();
    for (const auto &p : universe)
        if (ds.find(p.to_int()) != ds.find(border) && ds.find(p.to_int()) != ds.find(edge))
            out.insert(p);

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
