#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

std::vector<std::vector<int>> get_heights(std::string filename) {
    std::ifstream ifs(filename);
    std::vector<std::vector<int>> out;
    std::string row;

    std::getline(ifs, row);
    for (char c : row) {
        out.push_back({c - '0'});
    }

    while (std::getline(ifs, row)) {
        for (int i = 0; i < row.size(); ++i) {
            out[i].push_back(row[i] - '0');
        }
    }

    return out;
}

void read_columns(std::vector<std::vector<bool>>& mask, const std::vector<std::vector<int>>& treeHeights) {
    int num_cols = mask[0].size();
    for (int c = 0; c < num_cols; ++c) {
        int min = -1;
        for (int r = 0; r < mask.size(); ++r) {
            mask[r][c] = mask[r][c] || treeHeights[r][c] > min;
            min = std::max(min, treeHeights[r][c]);
        }
    }
    for (int c = 0; c < num_cols; ++c) {
        int min = -1;
        for (int r = mask.size() - 1; r >= 0; --r) {
            mask[r][c] = mask[r][c] || treeHeights[r][c] > min;
            min = std::max(min, treeHeights[r][c]);
        }
    }
}

void read_rows(std::vector<std::vector<bool>>& mask, const std::vector<std::vector<int>>& treeHeights) {
    int num_rows = mask.size();
    for (int r = 0; r < num_rows; ++r) {
        int min = -1;
        for (int c = 0; c < mask[0].size(); ++c) {
            mask[r][c] = mask[r][c] || treeHeights[r][c] > min;
            min = std::max(min, treeHeights[r][c]);
        }
    }
    for (int r = 0; r < num_rows; ++r) {
        int min = -1;
        for (int c = mask[0].size() - 1; c >= 0; --c) {
            mask[r][c] = mask[r][c] || treeHeights[r][c] > min;
            min = std::max(min, treeHeights[r][c]);
        }
    }
}

int count_true(const std::vector<std::vector<bool>>& mask) {
    int count = 0;
    for (int r = 0; r < mask.size(); ++r) {
        for (int c = 0; c < mask[0].size(); ++c) {
            count += mask[r][c];
        }
    }
    return count;
}

void part_one() {
    std::vector<std::vector<int>> heights = get_heights("day8.in");
    std::vector<std::vector<bool>> mask(heights.size(), std::vector<bool>(heights[0].size(), false));
    read_columns(mask, heights);
    read_rows(mask, heights);
    std::cout << count_true(mask) << "\n";
}

int scenic_score(const std::vector<std::vector<int>>& heights, int row, int col) {
    int num_cols = heights[0].size();
    int num_rows = heights.size();
    if (row == 0 || col == 0 || (num_rows - row) == 1 || (num_cols - col) == 1) {
        return 0;
    }

    int score = 1;
    int h = heights[row][col];
    // right
    int c = col + 1;
    for (; c < num_cols - 1 && heights[row][c] < h; ++c) { }
    score = score * (c - col);
    // down
    int r = row + 1;
    for (; r < num_rows - 1 && heights[r][col] < h; ++r) { }
    score = score * (r - row);
    // left
    c = col - 1;
    for (; c > 0 && heights[row][c] < h; --c) { }
    score = score * (col - c);
    // up
    r = row - 1;
    for (; r > 0 && heights[r][col] < h; --r) { }
    score = score * (row - r);

    return score;
}

std::vector<std::vector<int>> calc_scenic_scores(const std::vector<std::vector<int>>& heights) {
    std::vector<std::vector<int>> out(heights.size(), std::vector<int>(heights[0].size()));
    for (int r = 0; r < heights.size(); ++ r) {
        for (int c = 0; c < heights[0].size(); ++c) {
            out[r][c] = scenic_score(heights, r, c);
        }
    }
    return out;
}


int find_max(const std::vector<std::vector<int>>& scenic_score) {
    int max = -1;
    for (int r = 0; r < scenic_score.size(); ++r) {
        for (int c = 0; c < scenic_score[0].size(); ++c) {
            max = std::max(max, scenic_score[r][c]);
        }
    }
    return max;
}

void part_two() {
    std::vector<std::vector<int>> heights = get_heights("day8.in");
    std::vector<std::vector<int>> scenic_score = calc_scenic_scores(heights);
    std::cout << find_max(scenic_score) << "\n";
}

int main() {
    part_one();
    part_two();
}
