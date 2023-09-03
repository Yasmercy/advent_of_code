#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

struct TreeNode {
    std::string name;
    TreeNode* parent = nullptr;
    TreeNode* sibling = nullptr;
    TreeNode* child = nullptr;
    int size = 0;

    ~TreeNode() {
        if (sibling != nullptr) {
            delete sibling;
        } 
        if (child != nullptr) {
            delete child;
        }
    }
};

void do_ls(TreeNode* root, std::vector<std::string>&& listing) {
    std::reverse(listing.begin(), listing.end());
    while (!listing.empty()) {
        // reads in two strings at a time
        std::string s1 = listing.back();
        listing.pop_back();
        std::string s2 = listing.back();
        listing.pop_back();

        // create new node
        TreeNode* add = new TreeNode {s2, root};
        if (s1[0] != 'd') {
            add->size = std::stoi(s1);
        }

        // prepend node to child list
        add->sibling = root->child;
        root->child = add;
    }
}

TreeNode* do_cd(TreeNode* root, const std::string& to) {
    if (to == "..") {
        return root->parent;
    }

    TreeNode* cur = root->child;
    while (cur->name != to) {
        cur = cur->sibling;
    }
    return cur;
}

TreeNode* create_tree(std::string filename) {
    TreeNode* root = new TreeNode{"\\"};
    TreeNode* cur = root;

    std::ifstream ifs(filename);
    std::string s;
    std::getline(ifs, s); // dummy call to remove '$ cd /'
    ifs >> s; // dummy call to remove first '$'


    while (ifs.good()) {
        // invariant: ifs is on a command (after $ is removed)
        // OR is eof
        if (ifs.eof()) {
            break;
        }

        // always on command
        ifs >> s;
        if (s == "cd") {
            ifs >> s;
            cur = do_cd(cur, s);
            ifs >> s; // maintain invariant
        } else if (s == "ls") {
            ifs >> s;
            std::vector<std::string> v;
            while (!ifs.eof() && s != "$") {
                v.push_back(s);
                ifs >> s;
            }
            do_ls(cur, std::move(v));
        } else {
            std::cout << "crying\n";
            break;
        }
    }

    return root;
}

void set_sizes(TreeNode* root) {
    // precondition: root != nullptr
    TreeNode* cur = root->child;
    while (cur != nullptr) {
        set_sizes(cur);
        root->size += cur->size;
        cur = cur->sibling;
    }
}

void filter_max_size(TreeNode* root, int max_size, std::vector<TreeNode*>& nodes) {
    if (root == nullptr) {
        return;
    }

    if (root->child == nullptr) {
        return;
    }

    if (root->size <= max_size) {
        nodes.push_back(root);
    }

    TreeNode* child = root->child;
    while (child != nullptr) {
        filter_max_size(child, max_size, nodes);
        child = child->sibling;
    }
}

int sum_sizes(const std::vector<TreeNode*>& nodes) {
    int s = 0;
    for (auto* node : nodes) {
        s += node->size;
    }
    return s;
}

void part_one() {
    TreeNode* tree = create_tree("day7.in");
    set_sizes(tree);
    std::vector<TreeNode*> nodes;
    filter_max_size(tree, 100'000, nodes);
    std::cout << sum_sizes(nodes) << "\n";
    delete tree;
}

int min_above_threshold(TreeNode* root, int threshold, int acc) {
    if (root->child == nullptr || root->size < threshold) {
        return acc;
    }

    TreeNode* child = root->child;
    while (child != nullptr) {
        acc = std::min(acc, min_above_threshold(child, threshold, acc));
        child = child->sibling;
    }

    return std::min(acc, root->size);
}

void part_two() {
    TreeNode* tree = create_tree("day7.in");
    set_sizes(tree);
    int threshold = tree->size - 40'000'000 - 1;
    std::cout << min_above_threshold(tree, threshold, tree->size) << "\n";
    delete tree;
}

int main() {
    part_one();
    part_two();
}
