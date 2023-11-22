#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <utility>
#include <vector>

const int SIZE = 4;

struct Point
{
    int row = 0;
    int col = 0;

    auto operator<=>(const Point &) const = default;

    Point operator+(const Point &other) const
    {
        return {row + other.row, col + other.col};
    }

    Point operator-(const Point &other) const
    {
        return {row - other.row, col - other.col};
    }

    bool in_bounds(int width, int height) const
    {
        return 0 <= row && row < height && 0 <= col && col < width;
    }

    int manhattan(const Point &other) const
    {
        return std::abs(row - other.row) + std::abs(col - other.col);
    }
};

struct Graph
{
    // modified adjacency list
    // stores the next node you go to after each direction
    // for example, has wrapping logic and wall logic

    // EAST, SOUTH, WEST, NORTH
    std::map<Point, std::vector<Point>> data;
    int width;
    int height;
};

struct Cube
{
    std::vector<Point> nodes;
    std::map<Point, std::set<Point>> edges;

    Cube reference_cube() const
    {
        std::vector<Point> nodes = {
            {0, 0}, // NOLINT
            {1, 0}, // NOLINT
            {0, 1}, // NOLINT
            {1, 1}, // NOLINT
            {2, 0}, // NOLINT
            {3, 0}, // NOLINT
            {2, 1}, // NOLINT
            {3, 1}, // NOLINT
        };
        std::map<Point, std::set<Point>> edges = {
            {nodes[0], {nodes[1], nodes[2], nodes[4]}},
            {nodes[1], {nodes[3], nodes[5]}},
            {nodes[2], {nodes[3], nodes[6]}},
            {nodes[3], {nodes[7]}},
            {nodes[4], {nodes[5], nodes[6]}},
            {nodes[5], {nodes[7]}},
            {nodes[6], {nodes[7]}},
        };

        return {nodes, edges};
    }

    Cube shuffle(const std::vector<int> &remap) const
    {
        std::vector<Point> new_nodes = {
            {0, 0}, // NOLINT
            {1, 0}, // NOLINT
            {0, 1}, // NOLINT
            {1, 1}, // NOLINT
            {2, 0}, // NOLINT
            {3, 0}, // NOLINT
            {2, 1}, // NOLINT
            {3, 1}, // NOLINT
        };

        std::map<Point, Point> unmap;
        for (int i = 0; i < new_nodes.size(); ++i)
            unmap.insert({nodes[i], new_nodes[remap[i]]});

        std::map<Point, std::set<Point>> new_edges;
        for (const auto &key : nodes)
            for (const auto &p : edges.at(key))
                new_edges[unmap[key]].insert(unmap[p]);

        return {new_nodes, new_edges};
    };

    bool has_edge(const Point &a, const Point &b) const
    {
        if (edges.contains(b) && edges.at(b).contains(a))
            return true;
        if (edges.contains(a) && edges.at(a).contains(b))
            return true;
        return false;
    }

    bool equal(const Cube &other) const
    {
        for (const auto &[key, value] : other.edges)
            for (const auto &to : value)
                if (!has_edge(key, to))
                    return false;
        return true;
    }

    bool is_cube(const Cube &reference) const
    {
        for (const auto &[_, value] : edges)
            if (value.size() != 3)
                return false;

        // checking isomorphism
        std::vector<int> remap = {0, 1, 2, 3, 4, 5, 6, 7};
        do
        {
            const auto &canon = shuffle(remap);
            if (canon.equal(reference))
                return true;
        } while (std::next_permutation(remap.begin(), remap.end()));
        return false;
    }

    void init_junctions(std::vector<Point> &unknown, std::vector<std::vector<Point>> &junctions) const
    {
        for (int i = 0; i < nodes.size(); ++i)
        {
            int in_junction = -1;
            for (int j = 0; in_junction == -1 && j < junctions.size(); ++j)
                for (const auto &p : junctions[j])
                    if (nodes[i].manhattan(p) <= 2)
                        in_junction = j;

            if (in_junction == -1)
                junctions.push_back({nodes[i]});
            else
                junctions[in_junction].push_back(nodes[i]);
        }

        while (true)
        {
            int min = 0;
            int min_size = 3;
            for (int i = 0; i < junctions.size(); ++i)
                if (junctions[i].size() < min_size)
                {
                    min_size = junctions[i].size();
                    min = i;
                }
            if (min_size > 1)
                break;
            std::move(junctions[min].begin(), junctions[min].end(), std::back_inserter(unknown));
            junctions.erase(junctions.begin() + min);
        }
        junctions.resize(8);
    }

    void push_junctions(const std::vector<Point> &points, std::vector<std::vector<Point>> &junctions) const
    {
        int index = 0;
        for (int i = 0; i < junctions.size() && index < points.size(); ++i)
            if (junctions[i].size() < 3)
                junctions[i--].push_back(points[index++]);
        for (int i = 0; i < junctions.size(); ++i)
            std::sort(junctions[i].begin(), junctions[i].end());
    }

    std::map<Point, std::set<Point>> union_junction_edges(const std::vector<std::vector<Point>> &junctions,
                                                          const std::map<Point, std::set<Point>> &edges) const
    {
        std::map<Point, Point> to_canon;
        for (const auto &junction : junctions)
            for (const auto &node : junction)
                to_canon.insert({node, junction[0]});

        std::map<Point, std::set<Point>> out;
        for (const auto &[key, value] : edges)
            for (const auto &node : value)
            {
                out[to_canon[key]].insert(to_canon[node]);
                out[to_canon[node]].insert(to_canon[key]);
            }
        return out;
    }

    std::vector<std::vector<Point>> connect_cube() const
    {
        const auto &reference = reference_cube();
        std::vector<Point> unknown;
        std::vector<std::vector<Point>> junctions; // 8x3
        init_junctions(unknown, junctions);

        do
        {
            std::vector<std::vector<Point>> junctions_cp = junctions;
            push_junctions(unknown, junctions_cp);

            std::vector<Point> new_nodes;
            for (const auto &junction : junctions_cp)
                new_nodes.push_back(junction[0]);
            auto new_edges = union_junction_edges(junctions_cp, edges);

            if (Cube{new_nodes, new_edges}.is_cube(reference))
                return junctions_cp;
        } while (std::next_permutation(unknown.begin(), unknown.end()));

        throw std::runtime_error("cannot become cube");
    }
};

std::pair<int, int> get_dimensions(const std::string &filename)
{
    size_t width = 0;
    size_t height = 0;

    std::ifstream ifs(filename);
    std::string line;

    while (std::getline(ifs, line))
    {
        ++height;
        width = std::max(width, line.size());
    }

    return {width, height};
}

std::map<Point, bool> get_mask(const std::string &filename, int width, int height)
{
    std::map<Point, bool> out;
    // 0 for no wall
    // 1 for wall

    std::ifstream ifs(filename);
    std::string line;
    int row = 0;
    while (std::getline(ifs, line))
    {
        if (line.empty())
            break;

        for (int col = 0; col < line.size(); ++col)
            switch (line[col])
            {
            case '.':
                out.insert({{row, col}, 0});
                break;
            case '#':
                out.insert({{row, col}, 1});
                break;
            }
        ++row;
    }

    return out;
}

std::vector<int> get_moves(const std::string &filename)
{
    std::vector<int> out;
    std::ifstream ifs(filename);
    std::string line;
    while (std::getline(ifs, line))
        if (line.empty())
            break;

    int n = 0;
    char dir;

    while (ifs.good())
    {
        ifs >> n;
        ifs >> dir;

        out.push_back(n);

        if (ifs.eof())
            break;

        switch (dir)
        {
        case 'R':
            out.push_back(1);
            break;
        default:
            out.push_back(-1);
            break;
        }
    }
    return out;
}

std::vector<Point> get_corners(const std::map<Point, bool> mask)
{

    // corners are on multiples of SIZE (for both row and col)
    std::vector<Point> out;
    for (int r = 0; r < 6; ++r)
        for (int c = 0; c < 6; ++c)
            if (mask.contains({r * SIZE, c * SIZE}))
                out.push_back({r * SIZE, c * SIZE});
    return out;
}

Point get_start(const std::map<Point, bool> mask)
{
    return get_corners(mask)[0];
}

Point get_connection(const Point &cur, const Point &dir, const Point &wrap, const std::map<Point, bool> &mask,
                     int width, int height)
{
    Point next = cur + dir;
    if (mask.contains(next))
        return (mask.at(next) == 0) ? next : cur;
    return (mask.at(wrap) == 0) ? wrap : cur;
}

std::vector<Point> get_edges(const Point &cur, const std::vector<Point> &wrap, const std::map<Point, bool> &mask, int w,
                             int h)
{
    std::vector<Point> out;
    std::vector<Point> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    for (int i = 0; i < 4; ++i)
        out.push_back(get_connection(cur, directions[i], wrap.at(i), mask, w, h));
    return out;
}

std::vector<Point> get_wrap1(const Point &cur, const std::map<Point, bool> &mask, int w, int h)
{
    std::vector<Point> wrap = {{cur.row, 0}, {0, cur.col}, {cur.row, w - 1}, {h - 1, cur.col}};
    std::vector<Point> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    std::vector<Point> out;

    for (int i = 0; i < 4; ++i)
    {
        Point find = wrap[i];
        while (!mask.contains(find))
            find = find + directions[i];
        out.push_back(find);
    }
    return out;
}

Graph get_graph1(const std::map<Point, bool> mask, int w, int h)
{
    Graph g;
    g.width = w;
    g.height = h;

    for (int r = 0; r < h; ++r)
        for (int c = 0; c < w; ++c)
            if (mask.contains({r, c}) && mask.at({r, c}) == 0)
            {
                const Point &cur = {r, c};
                const auto &wrap = get_wrap1(cur, mask, w, h);
                g.data.insert({cur, get_edges(cur, wrap, mask, w, h)});
            }

    return g;
}

std::pair<Point, int> simulate1(const Graph &graph, const std::vector<int> &moves, const Point &start)
{
    int dir = 0;
    Point pos = start;

    for (int i = 0; i < moves.size() / 2; ++i)
    {
        for (int k = 0; k < moves[2 * i]; ++k)
            pos = graph.data.at(pos)[dir];
        dir = (dir + 4 + moves[2 * i + 1]) % 4;
    }

    for (int k = 0; k < moves.back(); ++k)
        pos = graph.data.at(pos)[dir];

    return {pos, dir};
}

Cube build_disconnected_cube(const std::map<Point, bool> mask)
{
    std::vector<Point> corners = get_corners(mask);
    std::map<Point, std::set<Point>> edges;

    for (int i = 0; i < 6; ++i)
    {
        Point a = corners[i];
        int row = a.row;
        int col = a.col;

        Point b = {row + 3, col};
        Point c = {row + 3, col + 3};
        Point d = {row, col + 3};

        corners.push_back(b);
        corners.push_back(c);
        corners.push_back(d);

        edges[a].insert(b);
        edges[a].insert(d);
        edges[b].insert(c);
        edges[d].insert(c);
        edges[b].insert(a);
        edges[d].insert(a);
        edges[c].insert(b);
        edges[c].insert(d);
    }

    return {corners, edges};
}

std::pair<Point, Point> matching_points(const Point &c1, const Point &c2, const Cube &cube,
                                        const std::vector<std::vector<Point>> &junctions)
{
    // 1) find the two junctions that they both are at
    // 2) find the two other points in each junction that also have a connection
    int j1 = 0;
    int j2 = 0;
    for (int i = 0; i < junctions.size(); ++i)
    {
        auto j = junctions[i];
        if (std::find(j.begin(), j.end(), c1) != j.end())
            j1 = i;
        if (std::find(j.begin(), j.end(), c2) != j.end())
            j2 = i;
    }

    for (int i = 0; i < 3; ++i)
    {
        i += (junctions[j1][i] == c1);
        for (int j = 0; j < 3; ++j)
        {
            j += (junctions[j1][i] == c1);
            if (cube.has_edge(junctions[j1][i], junctions[j2][j]))
                return {junctions[j1][i], junctions[j2][j]};
        }
    }

    throw std::runtime_error("cannot find matching junction");
}

std::vector<Point> get_wrap2(const Point &cur, const std::map<Point, bool> &mask, const Cube &cube,
                             const std::vector<std::vector<Point>> &junctions)
{
    std::vector<Point> out(4);
    std::vector<Point> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    // find the two corners that cur is the closest to (3 if on the corner)

    Point closest = cube.nodes[0];
    for (const auto &corner : cube.nodes)
        if (closest.manhattan(cur) > corner.manhattan(cur))
            closest = corner;

    std::vector<Point> second{closest};

    for (const auto &corner : cube.nodes)
    {
        if (corner == closest)
            continue;
        if ((corner.row / SIZE == cur.row / SIZE && corner.col / SIZE == cur.col / SIZE) &&
            (closest.row == corner.row || closest.col == corner.col) &&
            ((second[0] == closest) || (second[0].manhattan(cur) > corner.manhattan(cur))))
            second[0] = corner;
    }
    if (closest == cur)
    {
        const auto &e = cube.edges.at(cur);
        second = std::vector<Point>(e.begin(), e.end());
    }

    // fill out the wraps return vector
    if (cur.row != closest.row && cur.col != closest.col)
        return out;

    for (int i = 0; i < 4; ++i)
    {
        // the other point that defines the edge we are on
        // use the one that is perpendicular to direction if on corner
        Point other;
        if (second.size() == 1)
            other = second[0];
        else if (directions[i].row == 0 && closest.col == second[0].col)
            other = second[0];
        else if (directions[i].col == 0 && closest.row == second[0].row)
            other = second[0];
        else if (directions[i].row == 0 && closest.col == second[1].col)
            other = second[1];
        else if (directions[i].col == 0 && closest.row == second[1].row)
            other = second[1];

        const auto &[c1, c2] = matching_points(closest, other, cube, junctions);

        Point dir = c2 - c1;
        int dist = closest.manhattan(cur);
        if (dir.row != 0)
            dir.row = dir.row / std::abs(dir.row) * dist;
        if (dir.col != 0)
            dir.col = dir.col / std::abs(dir.col) * dist;
        out[i] = c1 + dir;
    }

    return out;
}

Graph get_graph2(const std::map<Point, bool> mask, int w, int h)
{
    Graph g;
    g.width = w;
    g.height = h;

    const auto &cube = build_disconnected_cube(mask);
    const auto &junctions = cube.connect_cube();

    for (int r = 0; r < h; ++r)
        for (int c = 0; c < w; ++c)
            if (mask.contains({r, c}) && mask.at({r, c}) == 0)
            {
                const Point &cur = {r, c};
                const auto &wrap = get_wrap2(cur, mask, cube, junctions);
                g.data.insert({cur, get_edges(cur, wrap, mask, w, h)});
            }

    return g;
}

std::pair<Point, int> simulate2(const Graph &graph, const std::vector<int> &moves, const Point &start)
{
    int dir = 0;
    Point pos = start;

    for (int i = 0; i < moves.size() / 2; ++i)
    {
        for (int k = 0; k < moves[2 * i]; ++k)
            pos = graph.data.at(pos)[dir];
        dir = (dir + 4 + moves[2 * i + 1]) % 4;
    }

    for (int k = 0; k < moves.back(); ++k)
        pos = graph.data.at(pos)[dir];

    return {pos, dir};
}

void part_one()
{
    const auto &[w, h] = get_dimensions("day22.in");
    const auto &mask = get_mask("day22.in", w, h);
    const auto &graph = get_graph1(mask, w, h);
    const auto &moves = get_moves("day22.in");
    const auto &start = get_start(mask);

    const auto &[point, facing] = simulate1(graph, moves, start);
    std::cout << (point.row + 1) * 1000 + 4 * (point.col + 1) + facing << '\n';
}

void part_two()
{
    const auto &[w, h] = get_dimensions("day22.in");
    const auto &mask = get_mask("day22.in", w, h);
    const auto &graph = get_graph2(mask, w, h);
    const auto &moves = get_moves("day22.in");
    const auto &start = get_start(mask);

    const auto &[point, facing] = simulate2(graph, moves, start);
    std::cout << (point.row + 1) * 1000 + 4 * (point.col + 1) + facing << '\n';
}

int main()
{
    part_one();
    part_two();
}
