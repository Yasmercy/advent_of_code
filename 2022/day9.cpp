#include <cmath>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>

struct Move
{
    char dir;
    int n;
};

struct Location
{
    int row = 0;
    int col = 0;

    void move(const Move &m)
    {
        if (m.dir == 'R')
            col += m.n;
        else if (m.dir == 'L')
            col -= m.n;
        else if (m.dir == 'U')
            row += m.n;
        else
            row -= m.n;
    }

    void follow(const Location &other)
    {
        if (adjacent(other))
            return;

        if (row != other.row && col != other.col)
        {
            // move diagonal to be 1 away
            int dr = (other.row > row) ? 1 : -1;
            int dc = (other.col > col) ? 1 : -1;
            row += dr;
            col += dc;
        }
        else
        {
            // move straight to be 1 away
            if (row == other.row)
                if (col < other.col)
                    col = other.col - 1;
                else
                    col = other.col + 1;
            else if (row < other.row)
                row = other.row - 1;
            else
                row = other.row + 1;
        }
    }

    bool adjacent(const Location &other)
    {
        return std::abs(other.row - row) < 2 && std::abs(other.col - col) < 2;
    }

    auto operator<=>(const Location &other) const = default;
};

struct Rope
{
    std::vector<Location> knots;

    void move(const Move &m)
    {
        knots[0].move(m);
        // each element follows the one before
        for (int i = 1; i < knots.size(); ++i)
            knots[i].follow(knots[i - 1]);
    }
};

std::vector<Move> read_file(std::string filename)
{
    std::ifstream ifs(filename);
    std::vector<Move> out;
    char dir;
    int n;

    while (ifs.good())
    {
        ifs >> dir;
        ifs >> n;
        if (ifs.eof())
            break;
        out.push_back(Move{dir, n});
    }

    return out;
}

void part_one(const std::vector<Move> &moves)
{
    std::set<Location> locations;
    Rope rope;
    for (int i = 0; i < 2; ++i)
    {
        rope.knots.push_back(Location{0, 0});
    }

    for (auto move : moves)
    {
        for (int i = 0; i < move.n; ++i)
        {
            rope.move(Move{move.dir, 1});
            locations.insert(rope.knots.back());
        }
    }

    std::cout << locations.size() << "\n";
}

void part_two(const std::vector<Move> &moves)
{
    std::set<Location> locations;
    Rope rope;
    for (int i = 0; i < 10; ++i)
    {
        rope.knots.push_back(Location{0, 0});
    }

    for (auto move : moves)
    {
        for (int i = 0; i < move.n; ++i)
        {

            rope.move(Move{move.dir, 1});
            locations.insert(rope.knots.back());
        }
    }

    std::cout << locations.size() << "\n";
}

int main()
{
    std::vector<Move> moves = read_file("day9.in");
    part_one(moves);
    part_two(moves);
}
