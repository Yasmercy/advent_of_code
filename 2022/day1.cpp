// calorie counting

#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <fstream>
#include <iostream>

std::vector<std::vector<int>> process_file() {
    std::string filename = "day1.in";
    std::ifstream ifs(filename);
    std::vector<std::vector<int>> out;
    std::vector<int> cur;

    std::string var;
    int cals;
    while (std::getline(ifs, var)) {
        if (var.empty()) {
            out.push_back(std::move(cur));
        } else { 
            cals = std::stoi(var);
            cur.push_back(cals);
        }
    }
    return out;
}

int sum_vec(std::vector<int>& data) {
    return std::accumulate(data.begin(), data.end(), 0);
}

int find_best(std::vector<std::vector<int>>& data) {
    int best = -1;
    for (auto v : data) {
        best = std::max(best, sum_vec(v));
    }
    return best;
}

void heapify(int heap[], std::size_t size, int root) {
    // mutates the array into a min heap

    int left = root * 2;
    int right = root * 2 + 1;
    int smallest = root;

    // find min of left and right (that are within size)
    if (left < size && heap[left] < heap[smallest]) {
        smallest = left;
    } 
    if (right < size && heap[right] < heap[smallest]) {
        smallest = right;
    }

    // swap and recurse
    if (smallest != root) {
        std::swap(heap[smallest], heap[root]);
        heapify(heap, size, smallest);
    }
}

template <std::size_t k>
int find_top_k(std::vector<std::vector<int>>& data) {
    // allocate a min heap of size k
    // if next is > min, then replace min with next and heapify
    // return sum of elements in heap
    // n = len(data), T(n, k) = O(n logk)

    int heap[k] {}; 
    int size = 0;

    for (auto datapoint : data) {
        int s = sum_vec(datapoint);
        if (size < k) {
            // does not have heap property
            heap[size] = s;
            ++size;
            if (size == k) {
                heapify(heap, k, 0);
            }
        } else if (s > heap[0]) {
            // always has heap property
            heap[0] = s;
            heapify(heap, k, 0);
        }
    }

    // return sum
    int out = std::accumulate(heap, heap + k, 0);
    return out;
}

int main() {
    std::vector<std::vector<int>> data = process_file();

    int best = find_best(data);
    std::cout << "Best is: " << best << "\n";
    
    constexpr size_t k = 3;
    int topk = find_top_k<k>(data);
    std::cout << "Sum of top " << k << " elements is: " << topk << "\n";
}
