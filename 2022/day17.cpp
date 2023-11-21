#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

constexpr int64_t width = 7;
constexpr int64_t N = 5;

struct Pieces
{
    Pieces(int64_t row, int64_t col, int64_t width, int64_t height,
           std::vector<std::pair<int64_t, int64_t>> rel_locations)
        : row(row), col(col), width(width), height(height), rel_locations(rel_locations)
    {
    }
    int64_t row;
    int64_t col;
    int64_t width;
    int64_t height;
    std::vector<std::pair<int64_t, int64_t>> rel_locations; // relative from topleft
};

struct Data
{
    uint64_t heights;
    int64_t index;
    int64_t position;

    bool operator==(const Data &other) const
    {
        return heights == other.heights && position == other.position;
    }
};

template <> struct std::hash<Data>
{
    std::size_t operator()(const Data &k) const
    {
        return hash<uint64_t>()(k.heights) ^ (hash<int64_t>()(k.position) << 1);
    }
};

struct Board
{
    // a row x col representation
    std::vector<std::vector<bool>> board;
    int64_t height = 0;
    int64_t cleared_height = 0;

    void setDefault(Pieces &piece)
    {
        // puts the row and col of the pieces in the right place
        piece.col = 2;
        piece.row = height + 3;

        while ((board.size() + cleared_height) <= piece.row + piece.height)
            board.push_back(std::vector<bool>(width, 0));
    }

    void setPiece(const Pieces &piece)
    {
        for (const auto &[r, c] : piece.rel_locations)
            board[r + piece.row - cleared_height][c + piece.col] = 1;

        // check for creating full rows
        for (int64_t r = piece.row + piece.height; r >= piece.row; --r)
        {
            int c = 0;
            for (; c < width; ++c)
                if (!board[r - cleared_height][c])
                    break;
            if (c == width)
            {
                auto diff = r - cleared_height;
                cleared_height = r;
                board.erase(board.begin(), board.begin() + diff);
                return;
            }
        }
    }

    bool canMove(Pieces &piece, int64_t dr, int64_t dc)
    {
        for (const auto &[r, c] : piece.rel_locations)
        {
            int64_t r_new = r + dr + piece.row;
            int64_t c_new = c + dc + piece.col;
            if (0 > r_new || 0 > c_new || c_new >= width || board[r_new - cleared_height][c_new])
                return false;
        }
        return true;
    }

    void movePiece(Pieces &piece, int64_t dr, int64_t dc)
    {
        piece.row += dr;
        piece.col += dc;
    }

    // moves the blocks by a direction (0 is left, 1 is right)
    // returns whether it has now set
    bool move(Pieces &piece, int64_t dc)
    {
        if (canMove(piece, 0, dc))
            movePiece(piece, 0, dc);

        if (!canMove(piece, -1, 0))
            return true;

        movePiece(piece, -1, 0);
        return false;
    }

    uint64_t hash_heights()
    {
        // uses 8 bits to get the height of each column
        uint64_t out = 0;
        for (int col = 0; col < width; ++col)
        {
            out <<= 8;
            uint8_t max = 0;
            for (int row = board.size() - 1; row >= 0 && max == 0; --row)
                if (board[row][col])
                    max = row;
            out += max;
        }
        return out;
    }

    void run(int64_t n, Pieces pieces[N], std::vector<int64_t> directions)
    {
        int64_t dir_index = 0;
        int64_t piece_index = 0;

        std::unordered_map<Data, int64_t> heights;

        while (piece_index < n)
        {
            auto &piece = pieces[piece_index % N];

            if (piece.row == -1)
                setDefault(piece);

            if (move(piece, directions[dir_index++ % directions.size()]))
            {
                setPiece(piece);
                height = std::max(height, piece.height + piece.row);
                piece.row = -1;
                piece.col = 0;
                ++piece_index;

                auto m = N * static_cast<int64_t>(directions.size());
                Data data{hash_heights(), piece_index, dir_index % m};

                auto search = heights.find(data);
                if (search == heights.end())
                    heights.insert({data, height});
                else
                {
                    auto old_height = search->second;
                    auto old_index = search->first.index;
                    auto period = piece_index - old_index;
                    auto dh = height - old_height;
                    auto remaining = n - piece_index;
                    auto cycles = remaining / period;

                    height += dh * cycles;
                    cleared_height += dh * cycles;
                    piece_index += period * cycles;
                }
            }
        }
    }

    void printRow(int64_t r)
    {
        const auto &row = board[r];

        std::cout << "|";
        char c[2] = {'.', '@'};
        for (const auto &cell : row)
        {
            std::cout << c[cell];
        }
        std::cout << "|\n";
    }

    void printArray()
    {
        for (int64_t i = height - 1 - cleared_height; i >= 0; --i)
            printRow(i);
        std::cout << "+------+\n\n";
    }
};

std::vector<int64_t> read_file(std::string filename)
{
    std::vector<int64_t> out;
    std::ifstream ifs(filename);
    char c;
    while (ifs.good())
    {
        ifs >> c;
        if (ifs.eof())
            break;
        out.push_back((c == '>') * 2 - 1);
    }
    return out;
};

void part_one()
{
    // create the five pieces
    Pieces pieces[] = {
        {-1, 0, 4, 1, {{0, 0}, {0, 1}, {0, 2}, {0, 3}}},
        {-1, 0, 3, 3, {{0, 1}, {1, 0}, {1, 1}, {1, 2}, {2, 1}}},
        {-1, 0, 3, 3, {{2, 2}, {1, 2}, {0, 0}, {0, 1}, {0, 2}}},
        {-1, 0, 1, 4, {{0, 0}, {1, 0}, {2, 0}, {3, 0}}},
        {-1, 0, 2, 2, {{0, 0}, {0, 1}, {1, 0}, {1, 1}}},
    };

    Board board;
    std::vector<int64_t> directions = read_file("day17.in");
    board.run(2022, pieces, directions);

    std::cout << board.height << "\n";
}

void part_two()
{
    // create the five pieces
    Pieces pieces[] = {
        {-1, 0, 4, 1, {{0, 0}, {0, 1}, {0, 2}, {0, 3}}},
        {-1, 0, 3, 3, {{0, 1}, {1, 0}, {1, 1}, {1, 2}, {2, 1}}},
        {-1, 0, 3, 3, {{2, 2}, {1, 2}, {0, 0}, {0, 1}, {0, 2}}},
        {-1, 0, 1, 4, {{0, 0}, {1, 0}, {2, 0}, {3, 0}}},
        {-1, 0, 2, 2, {{0, 0}, {0, 1}, {1, 0}, {1, 1}}},
    };

    Board board;
    std::vector<int64_t> directions = read_file("day17.in");
    board.run(1'000'000'000'000, pieces, directions);

    std::cout << board.height << "\n";
}

int main()
{
    // part_one();
    part_two();
}
