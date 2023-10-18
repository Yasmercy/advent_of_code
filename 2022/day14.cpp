#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Sand
{
    int r;
    int c;
};

struct Board
{
    // 0 = null, 1 = sand, 2 = wall
    // south is increasing row
    // east is increasing col
    std::vector<std::vector<int>> board;
    int min_row = -1;

    // methods
    Board(std::vector<std::vector<int>> data) : board(1000, std::vector<int>(1000, 0))
    {
        for (const auto &path : data)
        {
            add_path(path);
        }
    }

    void add_path(const std::vector<int> &nodes)
    {
        int r1 = nodes[0];
        int c1 = nodes[1];
        for (size_t i = 2; i < nodes.size(); i += 2)
        {
            int r2 = nodes[i];
            int c2 = nodes[i + 1];
            add_path(r1, c1, r2, c2);
            r1 = r2;
            c1 = c2;
        }
    }

    void add_path(int r1, int c1, int r2, int c2)
    {
        min_row = std::max(min_row, r1);
        min_row = std::max(min_row, r2);

        int dr = (r2 < r1) ? -1 : r2 != r1;
        int dc = (c2 < c1) ? -1 : c2 != c1;
        r2 += dr; // make inclusive range
        c2 += dc; // make inclusive range

        while (r1 != r2 || c1 != c2)
        {
            board[r1][c1] = 2;
            r1 += dr;
            c1 += dc;
        }
    }

    void set(const Sand &sand)
    {
        board[sand.r][sand.c] = 1;
    }

    // returns the new sand
    void iterate_sand(Sand &sand)
    {
        // south
        if (board[sand.r + 1][sand.c] == 0)
        {
            ++sand.r;
        }
        // south west
        else if (board[sand.r + 1][sand.c - 1] == 0)
        {
            ++sand.r;
            --sand.c;
        }
        // south east
        else if (board[sand.r + 1][sand.c + 1] == 0)
        {
            ++sand.r;
            ++sand.c;
        }
    }

    // checks if the sand is at rest
    bool rest(const Sand &sand)
    {
        Sand cp = sand;
        iterate_sand(cp);
        return cp.r == sand.r && cp.c == sand.c;
    }

    // checks if the sand has fallen into the abyss
    bool abyss(const Sand &sand)
    {
        return sand.r > min_row;
    }
};

std::vector<std::vector<int>> read_file(std::string filename)
{
    std::vector<std::vector<int>> out;
    std::ifstream ifs(filename);
    std::string line;

    while (std::getline(ifs, line))
    {
        std::vector<int> add;
        std::istringstream ss(line);
        std::string r, c;
        while (std::getline(ss, c, ','))
        {
            std::getline(ss, r, ' ');
            ss.ignore(3);
            add.push_back(std::stoi(r));
            add.push_back(std::stoi(c));
        }
        out.push_back(add);
    }

    return out;
}

void part_one()
{
    std::vector<std::vector<int>> data = read_file("day14.in");
    Board b(data);
    int num = 0;

    const Sand start = Sand{0, 500};
    Sand cur = start;

    while (!b.abyss(cur) && !b.rest(start))
    {
        b.iterate_sand(cur);
        if (b.rest(cur))
        {
            ++num;
            b.set(cur);
            cur = start;
        }
    }

    std::cout << "part one solution is: " << num << "\n";
}

void part_two()
{
    std::vector<std::vector<int>> data = read_file("day14.in");
    Board b(data);
    std::vector<int> floor = {b.min_row + 2, 0, b.min_row + 2, 999};
    b.add_path(floor);
    int num = 1;

    const Sand start = Sand{0, 500};
    Sand cur = start;

    while (!b.abyss(cur) && !b.rest(start))
    {
        b.iterate_sand(cur);
        if (b.rest(cur))
        {
            ++num;
            b.set(cur);
            cur = start;
        }
    }

    std::cout << "part two solution is: " << num << "\n";
}

int main()
{
    part_one();
    part_two();
}
