#include <fstream>
#include <iostream>
#include <sstream>
#include <tuple>
#include <utility>
#include <vector>

struct Graph
{
    // modified adjacency list
    // stores the next node you go to after each direction
    // for example, has wrapping logic and wall logic

    // EAST, SOUTH, WEST, NORTH
    std::vector<std::vector<int>> data;
    int start;
    int width;
    int height;
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

std::vector<std::vector<int>> get_map(const std::string &filename, int width, int height)
{
    // 0 for void
    // 1 for empty
    // 2 for wall
    std::vector<std::vector<int>> out(height, std::vector<int>(width, 0));

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
                out[row][col] = 1;
                break;
            case '#':
                out[row][col] = 2;
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

bool in_bounds(int row, int col, int width, int height)
{
    return 0 <= row && row < height && 0 <= col && col < width;
}

int get_connection(int row, int col, int dr, int dc, int wrap_row, int wrap_col,
                   const std::vector<std::vector<int>> &map)
{
    int new_row = row + dr;
    int new_col = col + dc;
    int width = map[0].size();
    int height = map.size();

    if (map[row][col] == 0)
        return row * width + col;

    if (in_bounds(new_row, new_col, width, height) && map[new_row][new_col] == 1)
        return (new_row * width + new_col);
    if (in_bounds(new_row, new_col, width, height) && map[new_row][new_col] == 2)
        return row * width + col;

    // handle wrapping logic
    while (map[wrap_row][wrap_col] == 0)
    {
        wrap_row += dr;
        wrap_col += dc;
    }

    if (map[wrap_row][wrap_col] == 1)
        return wrap_row * width + wrap_col;
    return row * width + col;
}

std::vector<int> get_edges(int row, int col, const std::vector<std::vector<int>> &map)
{
    std::vector<int> out;
    int height = map.size();
    int width = map[0].size();

    std::vector<std::vector<int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    std::vector<std::vector<int>> wrap = {{row, 0}, {0, col}, {row, width - 1}, {height - 1, col}};
    for (int i = 0; i < 4; ++i)
        out.push_back(get_connection(row, col, directions[i][0], directions[i][1], wrap[i][0], wrap[i][1], map));

    return out;
}

int get_start(const std::vector<std::vector<int>> &map)
{
    int out = -1;
    while (map[0][++out] != 1)
    {
    }
    return out;
}

Graph get_graph(const std::string &filename)
{
    Graph g;
    std::ifstream ifs(filename);

    const auto &[w, h] = get_dimensions(filename);
    const auto &map = get_map(filename, w, h);
    g.start = get_start(map);
    g.width = w;
    g.height = h;

    for (int i = 0; i < w * h; ++i)
        g.data.push_back(get_edges(i / w, i % w, map));

    return g;
}

std::tuple<int, int, int> simulate(const Graph &graph, const std::vector<int> &moves)
{
    int pos = graph.start;
    int dir = 0;
    int width = graph.width;

    for (int i = 0; i < moves.size() / 2; ++i)
    {
        for (int k = 0; k < moves[2 * i]; ++k)
            pos = graph.data[pos][dir];
        dir = (dir + 4 + moves[2 * i + 1]) % 4;
    }

    for (int k = 0; k < moves.back(); ++k)
        pos = graph.data[pos][dir];

    return {pos / width, pos % width, dir};
}

void part_one()
{
    const auto &graph = get_graph("day22.in");
    const auto &moves = get_moves("day22.in");
    const auto &[row, col, facing] = simulate(graph, moves);

    std::cout << (row + 1) * 1000 + 4 * (col + 1) + facing << '\n';
}

void part_two()
{

}

int main()
{
    part_one();
}
