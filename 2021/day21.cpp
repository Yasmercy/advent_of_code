#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Player
{
    uint64_t pos;
    uint64_t score;
};

std::vector<Player> read_file(std::string filename)
{
    std::vector<Player> out;
    std::ifstream ifs(filename);

    std::string line;
    while (std::getline(ifs, line))
        out.push_back({static_cast<uint64_t>(line[28] - '0' - 1)});

    return out;
}

uint64_t solve_deterministic(std::vector<Player> &players, uint64_t end, uint64_t mod)
{
    uint64_t turn = 0;
    uint64_t p = 0;

    while (players[(p + 1) % 2].score < end)
    {
        players[p].pos += ++turn;
        players[p].pos += ++turn;
        players[p].pos += ++turn;
        players[p].pos %= 10;

        players[p].score += players[p].pos + 1;

        p = (p + 1) % 2;
    }

    return turn * players[p].score;
}

uint64_t solve_quantum(uint64_t p1_start, uint64_t p2_start, uint64_t end, uint64_t mod, int t)
{
    // score1 (0-end inclusive), pos1, pos2, turn
    // stores the amount of ways to win from that
    uint64_t dp[end + 1][mod][mod][2];

    // base case
    for (uint64_t i = 0; i < mod; ++i)
        for (uint64_t j = 0; j < mod; ++j)
            for (uint64_t b = 0; b < 2; ++b)
                dp[end][i][j][b] = b;

    const std::vector<int> n = {1, 3, 6, 7, 6, 3, 1};

    for (int score = end - 1; score >= 0; --score)
        for (uint64_t i = 0; i < mod; ++i)
            for (uint64_t j = 0; j < mod; ++j)
            {
                dp[score][i][j][0] = 0;
                dp[score][i][j][1] = 0;

                // ordering is non-decreasing score
                // if score is increasing, then guaranteed to have been initailized
                // new_pos = 0, then i,j >= 1 ==> dp[score][0][..] has to have been initialized
                for (int roll = 3; roll <= 9; ++roll)
                {
                    uint64_t new_pos_1 = (i + roll) % mod;
                    dp[score][i][j][0] += n[roll - 3] * dp[std::min(end, score + new_pos_1)][new_pos_1][j][1];

                    uint64_t new_pos_2 = (j + roll) % mod;
                    dp[score][i][j][1] += n[roll - 3] * dp[std::min(end, score + new_pos_2)][i][new_pos_2][0];
                }
            }

    return dp[0][p1_start][p2_start][t];
}

void part_one()
{
    auto players = read_file("day21.in");
    uint64_t solution = solve_deterministic(players, 1000, 10);
    std::cout << solution << '\n';
}

void part_two()
{
    auto players = read_file("day21.in");
    auto score1 = solve_quantum(players[0].pos, players[1].pos, 21, 10, 0);
    auto score2 = solve_quantum(players[1].pos, players[0].pos, 21, 10, 1);
    std::cout << score1 << ' ' << score2 << '\n';
}

int main()
{
    part_one();
    part_two();
}
