#include <iostream>
#include <string>
#include <fstream>

bool won(char opp, char me) {
    if (opp == 'A') {
        return me == 'B';
    } else if (opp == 'B') {
        return me == 'C';
    } else {
        return me == 'A';
    }
}

bool draw(char opp, char me) {
    return opp == me;
}

bool lost(char opp, char me) {
    return !(won(opp, me) || draw(opp, me));
}

int score_action(char me) {
    int out = me - 'A';
    return out + 1;
}

int score(char opp, char me) {
    // opp moves: A = rock, B = paper, C = scissors
    // my moves:  X = rock, Y = paper, Z = scissors
    // scoring: 6 for winning, 3 for draw, 0 for loss
    // 1 for rock, 2 for paper, 3 for scissors
    if (won(opp, me)) {
        return score_action(me) + 6;
    } else if (draw(opp, me)) {
        return score_action(me) + 3;
    } else {
        return score_action(me);
    }
}

int get_score(std::string filename) {
    std::ifstream ifs(filename);
    int acc = 0;

    char opp;
    char me;

    while (ifs.good()) {
        ifs >> opp;
        ifs >> me;
        if (ifs.eof()) {
            break;
        }
        acc += score(opp, me - 'X' + 'A');

    }
    return acc;
}

char get_my_action(char opp, char outcome) {
    if (outcome == 'X') {
        // lose
        if (opp == 'A') {
            return 'C';
        } else if (opp == 'B') {
            return 'A';
        } else {
            return 'B';
        }
    } else if (outcome == 'Y') {
        return opp;
    } else {
        if (opp == 'A') {
            return 'B';
        } else if (opp == 'B') {
            return 'C';
        } else {
            return 'A';
        }
    }
}

int get_score_2(std::string filename) {
    std::ifstream ifs(filename);
    int acc = 0;

    char opp;
    char me;

    while (ifs.good()) {
        ifs >> opp;
        ifs >> me;
        if (ifs.eof()) {
            break;
        }
        acc += score(opp, get_my_action(opp, me));
    }
    return acc;
}

int main() {
    int s = get_score("day2.in");
    int s2 = get_score_2("day2.in");
    std::cout << s << "\n";
    std::cout << s2 << "\n";
}
