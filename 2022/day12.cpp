#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

struct Item
{
    int key;
    int value;
};

void heapify(std::vector<Item> &heap, int start)
{
    int left = 2 * start;
    int right = left + 1;
    int min = start;

    if (left < heap.size() && heap[left].key < heap[min].key)
        min = left;
    if (right < heap.size() && heap[right].key < heap[min].key)
        min = right;

    if (min != start)
    {
        std::swap(heap[start], heap[min]);
        heapify(heap, min);
    }
}

Item pop_min(std::vector<Item> &heap)
{
    Item out = heap[0];
    std::swap(heap[0], heap.back());
    heap.pop_back();
    heapify(heap, 0);
    return out;
}

void push(std::vector<Item> &heap, Item element)
{
    heap.push_back(element);
    int index = heap.size() - 1;
    int parent = (index - 1) / 2;

    while (index > 0 && heap[parent].key > element.key)
    {
        std::swap(heap[index], heap[parent]);
        index = parent;
        parent = (index - 1) / 2;
    }
}

int shortest_path(const std::vector<std::vector<int>> &graph, int start, int end)
{
    int size = graph.size();
    std::vector<bool> seen(size);
    std::vector<int> distances(size, size + 1);
    distances[start] = 0;

    std::vector<Item> heap;
    push(heap, {distances[start], start});

    while (!heap.empty())
    {
        Item cur = pop_min(heap);
        seen[cur.value] = true;

        for (int n : graph[cur.value])
        {
            if (seen[n])
                continue;

            int alt = distances[cur.value] + 1;
            if (alt < distances[n])
            {
                distances[n] = alt;
                push(heap, {distances[n], n});
            }
        }
    }

    return distances[end];
}

struct Data
{
    std::vector<std::vector<int>> graph;
    std::vector<int> height_a;
    int start;
    int end;
};

Data process_file(std::string filename)
{
    std::ifstream ifs(filename);
    std::vector<char> heights;
    std::vector<int> height_a;
    int start;
    int end;

    // collect all chars into a 1d list of node
    char c;
    while (ifs.good())
    {
        ifs >> c;
        if (ifs.eof())
        {
            break;
        }
        if (c == 'S')
        {
            start = heights.size();
            heights.push_back('a');
        }
        else if (c == 'E')
        {
            end = heights.size();
            heights.push_back('z');
        }
        else
        {
            heights.push_back(c);
        }

        if (heights.back() == 'a')
            height_a.push_back(heights.size() - 1);
    }

    // add all edges
    std::vector<std::vector<int>> graph(heights.size());

    int width = 61;
    int height = static_cast<int>(heights.size()) / width;
    for (std::size_t i = 0; i < heights.size(); ++i)
    {
        // neighbors
        int row = static_cast<int>(i) / width;
        int col = static_cast<int>(i) % width;
        std::pair<int, int> neighbors[4] = {{row + 1, col}, {row - 1, col}, {row, col + 1}, {row, col - 1}};

        // add the edges if in_bounds and dh < 2
        for (const auto &[r, c] : neighbors)
            if (0 <= r && r < height && 0 <= c && c < width && (heights[r * width + c] - heights[i] < 2))
                graph[i].push_back(r * width + c);
    }
    return Data{graph, height_a, start, end};
}

void part_one()
{
    auto data = process_file("day12.in");
    int path_length = shortest_path(data.graph, data.start, data.end);
    std::cout << path_length << "\n";
};

void part_two()
{
    auto data = process_file("day12.in");
    int size = data.graph.size();
    int best = size * size + 1;

    for (int start : data.height_a)
    {
        int path = shortest_path(data.graph, start, data.end);
        best = std::min(path, best);
    }

    std::cout << best << "\n";
};

int main()
{
    part_one();
    part_two();
}
