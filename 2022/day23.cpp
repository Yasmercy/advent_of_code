#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

/* maps positions to a monkey (dir)
 * positions are encoded in row * width + col form
 * with a 10 space margin
 *
 * monkeys can move if their first choice does not collide
 * first choice determined by int (and their neighbor count)
 */

struct Game
{
    std::set<int> monkeys;
    int width;
    int height;
    int min_r = std::numeric_limits<int>::max();
    int max_r = std::numeric_limits<int>::min();
    int min_c = std::numeric_limits<int>::max();
    int max_c = std::numeric_limits<int>::min();

    std::vector<int> get_neighbors(int position)
    {
        return {
            position - 1,         // NOLINT // W
            position - 1 - width, // NOLINT // NW
            position - 1 + width, // NOLINT // SW
            position - width,     // NOLINT // N
            position + width,     // NOLINT // S
            position + 1,         // NOLINT // E
            position + 1 - width, // NOLINT // NE
            position + 1 + width, // NOLINT // SE
        };
    }

    bool empty(int pos)
    {
        return monkeys.find(pos) == monkeys.end();
    }

    int propose(int position, int dir)
    {
        /* rules:
         * 0) no adjacent elves -> no move
         * 1) no adjacent elves N, NE, NW -> north propose
         * 2) no adjacent elves S, SE, SW -> south propose
         * 3) no adjacent elves W, NW, SW -> west propose
         * 4) no adjacent elves E, NE, SE -> east propose
         * ^ rules 1-4 are shuffled
         */
        const auto &neighbors = get_neighbors(position);

        // check if no adjacents
        bool adjacent = true;
        for (int n : neighbors)
            if (!empty(n))
                adjacent = false;
        if (adjacent)
            return position;

        std::vector<std::vector<int>> checks = {{3, 6, 1}, {4, 2, 7}, {0, 1, 2}, {5, 6, 7}};
        for (int i = 0; i < 4; ++i)
        {
            // check all of the given neighbors are empty
            bool e = true;
            for (int n : checks[(dir + i) % 4])
                if (!empty(neighbors[n]))
                    e = false;
            // return that proposition if its valid
            if (e)
                return neighbors[checks[(dir + i) % 4][0]];
        }
        // don't move if can't move
        return position;
    }

    void update_bounds(int pos)
    {
        int r = pos / width;
        int c = pos % width;
        max_r = std::max(max_r, r);
        min_r = std::min(min_r, r);
        max_c = std::max(max_c, c);
        min_c = std::min(min_c, c);
    }

    bool move(int dir)
    {
        std::map<int, int> p;
        std::set<int> m;
        for (const auto pos : monkeys)
            ++p[propose(pos, dir)];
        for (const auto pos : monkeys)
        {
            int prop = propose(pos, dir);
            int next = (p[prop] == 1) ? prop : pos;
            update_bounds(next);
            m.insert(next);
        }

        bool out = monkeys != m;
        monkeys = m;
        return out;
    };

    void print()
    {
        for (int r = min_r; r <= max_r; ++r)
        {
            for (int c = min_c; c <= max_c; ++c)
                std::cout << ((empty(r * width + c)) ? '.' : '#');
            std::cout << '\n';
        }
    }
};

Game read_file(std::string filename)
{
    int width;
    int pad = 1000;

    Game out;
    std::string line;
    std::ifstream ifs(filename);
    int row = pad;
    while (std::getline(ifs, line))
    {
        int col = pad;
        std::istringstream ss(line);
        width = ss.str().size();
        char c;
        for (int i = 0; i < width; ++i, ++col)
        {
            ss >> c;
            if (c == '#')
                out.monkeys.insert(row * (width + 2 * pad) + col);
        }
        ++row;
    }

    out.width = width + (2 * pad);
    out.height = row + pad;
    return out;
}

void part_one()
{
    Game g = read_file("day23.in");
    for (int i = 0; i < 10; ++i)
        g.move(i % 4);

    int sol = (g.max_r - g.min_r + 1) * (g.max_c - g.min_c + 1) - g.monkeys.size();
    std::cout << sol << '\n';
}

void part_two()
{
    Game g = read_file("day23.in");
    int sol = 0;
    while (g.move(sol++ % 4))
    {
    }

    std::cout << sol << '\n';
}

int main()
{
    part_one();
    part_two();
}
