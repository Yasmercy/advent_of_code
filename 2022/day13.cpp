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
        while (++index < length) {
            if (data[index] == ',') {
                if (cur == nullptr) {
                    cur = new List(str);
                    next = cur;
                } else {
                    cur->next = new List(str);
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
                cur = new List(str);
                next = cur;
            } else {
                cur->next = new List(str);
                cur = cur->next;
            }
        }
    }

    bool operator<(const List& other) const {
        // both are int:
        if (is_int() && other.is_int()) {
            if (data == other.data) {
                return next < other.next;
            }
            return data < other.data;
        }
        
        // one is a list:
        // upgrade one to list and try again
        if (is_int() && other.is_list()) {
            List cmp = List {new List(data)};
            bool out = cmp < other;
            return out;
        } else if (is_list() && other.is_int()) {
            List cmp = List {new List(other.data)};
            bool out = *this < cmp;
            return out;
        }

        // both are lists
        // zip values together
        List* it_this = list;
        List* it_other = other.list;

        // iterate
        while (true) {
            // check for empties
            if (it_this == nullptr && it_other == nullptr) {
                return false;
            } else if (it_this == nullptr) {
                return false;
            } else if (it_other == nullptr) {
                return true;
            }

            // both are non-empty =========

            // check if they are equal
            if (*it_this == *it_other) {
                it_this = it_this->next;
                it_other = it_other->next;
                continue;
            }

            // return whether this element is lt or gt
            return *it_this < *it_other;
        }
    }
 
    bool operator==(const List& other) const {
        // both are int:
        if (is_int() && other.is_int()) {
            return data == other.data;
        }
        
        // one is a list:
        // upgrade one to list and try again
        if (is_int() && other.is_list()) {
            List cmp = List {new List{data}};
            bool out = cmp < other;
            delete cmp.list;
            return out;
        } else if (is_list() && other.is_int()) {
            List cmp = List {new List{other.data}};
            bool out = *this < cmp;
            delete cmp.list;
            return out;
        }
        
        // both are lists
        // zip values together
        List* it_this = list;
        List* it_other = other.list;

        // iterate
        while (true) {
            // check for empties
            if (it_this == nullptr && it_other == nullptr) {
                return true;
            } else if (it_this == nullptr || it_other == nullptr) {
                return false;
            }

            // both are non-empty =========
            // check both are ints
            if (it_this->is_int() && it_other->is_int()) {
                return it_this->data == it_other->data;
            }

            // check both are lists
            if (it_this->is_list() && it_this->is_list()) {
                if (*it_this != *it_other) {
                    return false;
                }
                it_this = it_this->next;
                it_other = it_other->next;
                continue;
            }

            // otherwise cast to same type and try again
            if (it_this->is_int()) {
                List cmp = List {new List{it_this->data}};
                bool out = cmp < *it_other;
                delete cmp.list;
                return out;
            }
            List cmp = List {new List{it_other->data}};
            bool out = *it_this < cmp;
            delete cmp.list;
            return out;
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

    List(List& other) = delete;
    List& operator=(List& other) = delete;
    List& operator=(List&& other) = delete;
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
        if (data[2 * i].next < data[2 * i + 1].next) {
            index_sum += i + 1;
        }
    }

    std::cout << index_sum << '\n';
}

int main() {
    part_one();
}
