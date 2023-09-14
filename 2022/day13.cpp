#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// data structure for representing n-dimensional lists of non-negative integers
// if its an integer then list will be empty
// if its a list then then list will be the List of integers
// default = empty_list
struct List {
    // exactly one of the following two fields are initialized
    int data = -1;
    List* list = nullptr;
    // pointer to next element of list
    List* next = nullptr;

    List() = default;
    List(int data, List* list, List* next) : data(data), list(list), next(next) {}
    explicit List(int data) : data(data) {}
    explicit List(List* list) : list(list) {}
    explicit List(const std::string& data) {
        // base case: is just an int
        try {
            int d = std::stoi(data);
            this->data = d;
            return;
        } catch (std::invalid_argument const& ex) { }

        // remove the outer-most [...]
        int length = data.size() - 1;
        int index = 0;

        // split the data by ,
        // created a linked list of each element
        List* cur = nullptr;
        std::string str;
        int counter = 1;
        while (++index < length) {
            if (data[index] == '[') {
                ++counter;
            } else if (data[index] == ']') {
                --counter;
            }
            if (data[index] == ',' && counter == 1) {
                if (cur == nullptr) {
                    List* tmp = new List(str);
                    if (str.front() == '[' && str.back() == ']') {
                        tmp = new List(tmp);
                    }
                    cur = tmp;
                    next = cur;
                } else {
                    List* tmp = new List(str);
                    if (str.front() == '[' && str.back() == ']') {
                        tmp = new List(tmp);
                    }
                    cur->next = tmp;
                    cur = cur->next;
                }
                str = "";
            } else {
                str.push_back(data[index]);
            }
        }

        // add the last element
        if (!str.empty()) {
            if (cur == nullptr) {
                List* tmp = new List(str);
                if (str.front() == '[' && str.back() == ']') {
                    tmp = new List(tmp);
                }
                cur = tmp;
                next = cur;
            } else {
                List* tmp = new List(str);
                if (str.front() == '[' && str.back() == ']') {
                    tmp = new List(tmp);
                }
                cur->next = tmp;
                cur = cur->next;
            }
        }
        if (next != nullptr) {
            *this = *next;
        }
    }

    bool operator<(const List& other) const {
        const List* it_this = this;
        const List* it_other = &other;

        while (true) {
            if (it_this == nullptr) {
                return it_other != nullptr;
            } else if (it_other == nullptr) {
                return false;
            }

            // do a comparison on the current element
            List cur_this {it_this->data, it_this->list, nullptr};
            List cur_other {it_other->data, it_other->list, nullptr};
            bool eq = cur_this == cur_other;
            // manually delete so no double free
            cur_this.list = nullptr;
            cur_other.list = nullptr;
            if (eq) {
                it_this = it_this->next;
                it_other = it_other->next;
                continue;
            }

            // found the element that is not equal
            if (it_this->is_int() && it_other->is_int() && (it_this->data != it_other->data)) {
                return it_this->data < it_other->data;
            }

            // traverse up the list, comparing elements
            if (it_this->data == -1 && it_this->list == nullptr) {
                return true;
            } else if (it_other->data == -1 && it_other->list == nullptr) {
                return false;
            }

            if (it_this->is_list()) {
                it_this = it_this->list;
            }
            if (it_other->is_list()){
                it_other = it_other->list;
            }
            return *it_this < *it_other;
        }
    }
 
    bool operator==(const List& other) const {
        // check each corresponding element is correct
        const List* it_this = this;
        const List* it_other = &other;

        while (true) {
            // check for null conditions
            if (it_this == nullptr && it_other == nullptr) {
                return true;
            } else if (it_this == nullptr || it_other == nullptr) {
                return false;
            }

            // check each element is equal
            if ((it_this->data != it_other->data) || (*it_this->list != *it_other->list)) {
                return false;
            }

            it_this = it_this->next;
            it_other = it_other->next;
        }
    }

    bool operator>(const List& other) const {
        return !(*this < other || *this == other);
    }

    bool is_int() const {
        return data != -1;
    }

    bool is_list() const {
        return list != nullptr && data == -1;
    }

    bool is_empty() const {
        return next == nullptr && list == nullptr;
    }

    List(List& other) = default;
    List& operator=(List& other) = default;
    List& operator=(List&& other) = default;
    List(List&& other) {
        std::swap(list, other.list);
        std::swap(next, other.next);
        std::swap(data, other.data);
    }

    ~List() {
        if (list != nullptr) {
            delete list;
        }
        if (next != nullptr) {
            delete next;
        }
    }
};

std::vector<List> process_file(std::string filename) {
    std::vector<List> out;
    std::ifstream ifs(filename);
    for (std::string line; std::getline(ifs, line);) {
        if (line.empty()) {
            continue;
        }
        out.emplace_back(line);
    }
    return out;
}

void part_one() {
    std::vector<List> data = process_file("day13.in");
    int index_sum = 0;

    // loop through pairs and add to index if a < b
    for (std::size_t i = 0 ; i < data.size() / 2; ++i) {
        if (data[2 * i] < data[2 * i + 1]) {
            index_sum += i + 1;
        }
    }

    std::cout << index_sum << '\n';
}

int main() {
    part_one();
}
