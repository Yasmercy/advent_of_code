#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

constexpr long long width = 7;
constexpr long long N = 5;

struct Pieces
{
    Pieces(long long row, long long col, long long width, long long height,
           std::vector<std::pair<long long, long long>> rel_locations)
        : row(row), col(col), width(width), height(height), rel_locations(rel_locations)
    {
    }
    long long row;
    long long col;
    long long width;
    long long height;
    std::vector<std::pair<long long, long long>> rel_locations; // relative from topleft
};

struct Board
{
    // a row x col representation
    std::vector<std::vector<long long>> board;
    long long height = 0;
    long long cleared_height = 0;

    void setDefault(Pieces &piece)
    {
        // puts the row and col of the pieces in the right place
        piece.col = 2;
        piece.row = height + 3;

        while ((board.size() + cleared_height) <= piece.row + piece.height)
        {
            board.push_back(std::vector<long long>(width, 0));
        }
    }

    void setPiece(const Pieces &piece)
    {
        for (const auto &[r, c] : piece.rel_locations)
            board[r + piece.row - cleared_height][c + piece.col] = 1;

        // check for creating full rows
        for (long long r = piece.row + piece.height; r >= piece.row; --r)
        {
            int c = 0;
            for (; c < width; ++c)
            {
                if (!board[r - cleared_height][c])
                    break;
            }
            if (c == width)
            {
                auto diff = r - cleared_height;
                cleared_height = r;
                board.erase(board.begin(), board.begin() + diff);
                return;
            }
        }
    }

    bool canMove(Pieces &piece, long long dr, long long dc)
    {
        for (const auto &[r, c] : piece.rel_locations)
        {
            long long r_new = r + dr + piece.row;
            long long c_new = c + dc + piece.col;
            if (0 > r_new || 0 > c_new || c_new >= width || board[r_new - cleared_height][c_new])
                return false;
        }
        return true;
    }

    void movePiece(Pieces &piece, long long dr, long long dc)
    {
        piece.row += dr;
        piece.col += dc;
    }

    // moves the blocks by a direction (0 is left, 1 is right)
    // returns whether it has now set
    bool move(Pieces &piece, long long dc)
    {
        if (canMove(piece, 0, dc))
            movePiece(piece, 0, dc);

        if (!canMove(piece, -1, 0))
            return true;

        movePiece(piece, -1, 0);
        return false;
    }

    void run(long long n, Pieces pieces[N], std::vector<long long> directions)
    {
        long long dir_index = 0;
        long long piece_index = 0;

        while (piece_index < n)
        {
            if (piece_index % (10'000'000) == 0)
                std::cout << piece_index << "\n";
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
            }
        }
    }

    void printRow(long long r)
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
        for (long long i = height - 1 - cleared_height; i >= 0; --i)
            printRow(i);
        std::cout << "+------+\n\n";
    }
};

std::vector<long long> read_file(std::string filename)
{
    std::vector<long long> out;
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
    std::vector<long long> directions = read_file("day17.in");
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
    std::vector<long long> directions = read_file("day17.in");
    board.run(1'000'000'000'000, pieces, directions);

    std::cout << board.height << "\n";
}

int main()
{
    part_one();
    part_two();
}
