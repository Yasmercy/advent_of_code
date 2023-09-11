#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <string>

void heapify(std::vector<int>& heap, int start) {
    int left = 2 * start;
    int right = left + 1;
    int min = start;

    if (left < heap.size() && heap[left] < heap[min]) {
        min = left;
    }
    if (right < heap.size() && heap[right] < heap[min]) {
        min = right;
    }

    if (min != start) {
        std::swap(heap[start], heap[min]);
        heapify(heap, min);
    }
}

int pop_min(std::vector<int>& heap) {
    int out = heap[0];
    std::swap(heap[0], heap.back());
    heap.pop_back();
    heapify(heap, 0);
    return out;
}

void push(std::vector<int>& heap, int element) {
    heap.push_back(element);
    int index = heap.size() - 1;
    int parent = (index - 1) / 2;

    while (index > 0 && heap[parent] > element) {
        heap[index] = heap[parent];
        index = parent;
        parent = (index - 1) / 2;
    }
    heap[index] = element;
}

int shortest_path(const std::vector<std::vector<int>>& graph, int start, int end) {
    // finds the shortest path from a start node to an end node

    // set of visited nodes
    std::unordered_set<int> visited;
    // build a heap for the priority queue
    std::vector<int> heap = {};

    // build a vector of the distances
    std::vector<int> distances(graph.size(), graph.size() + 1);
    distances[start] = 0;

    // start from the start node
    while (true) {
        // terminal condition, reached end
        if (start == end) {
            break;
        }

        // add self to visited
        visited.insert(start);

        // update neighbors score
        for (int neighbor : graph[start]) {
            // skip over already visited nodes
            if (auto search = visited.find(neighbor); search == visited.end()) {
                // update weights (all edges are weight=1)
                if (distances[start] + 1 < distances[neighbor]) {
                    distances[neighbor] = distances[start] + 1;
                    push(heap, neighbor);
                }
            }
        }

        // update start
        if (heap.empty()) {
            std::cout << "cannot find path\n";
            break;
        }
        start = pop_min(heap);
    };
    return distances[start];
}

struct Data {
    std::vector<std::vector<int>> graph;
    int start;
    int end;
};

Data process_file(std::string filename) {
    std::ifstream ifs(filename);
    std::vector<char> heights;
    int start;
    int end;
    
    // collect all chars into a 1d list of node
    char c;
    while (ifs.good()) {
        ifs >> c;
        if (ifs.eof()) {
            break;
        }
        if (c == 'S') {
            start = heights.size();
            heights.push_back('a');
        } else if (c == 'E') {
            end = heights.size();
            heights.push_back('z');
        } else {
            heights.push_back(c);
        }
    }

    // add all edges
    std::vector<std::vector<int>> graph(heights.size());
    
    int width = 61;
    int height = static_cast<int>(heights.size()) / width;
    for (std::size_t i = 0; i < heights.size(); ++i) {
        // neighbors
        int row = static_cast<int>(i) / width;
        int col = static_cast<int>(i) % width;
        std::pair<int, int> neighbors[4] = {
            {row + 1, col},
            {row - 1, col},
            {row, col + 1},
            {row, col - 1}
        };

        // add the edges if in_bounds and dh < 2
        for (const auto& [r, c] : neighbors) {
            if (0 <= r && r < height && 0 <= c && c < width && (heights[r * width + c] - heights[i] < 2)) {
                graph[i].push_back(r * width + c);
            }
        }
    }
    return Data {graph, start, end};
}

void part_one() {
    auto data = process_file("day12.in");
    int path_length = shortest_path(data.graph, data.start, data.end);
    std::cout << path_length << "\n";
};

int main() {
    part_one();
}
