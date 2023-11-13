#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Snafu
{
    Snafu() = default;

    Snafu(const std::string &ss, int size) : digits(size, '0')
    {
        for (int i = 0; i < ss.size(); ++i)
            digits[i + size - ss.size()] = ss[i];
    }

    Snafu add(const Snafu &other) const
    {
        // base case: other == 0 => return this
        // create a new array (0), put all the carries in
        // return recursive call with the added and carries

        if (other.is_zero())
            return *this;

        Snafu res("", digits.size());
        Snafu carry("", digits.size());
        for (int i = 1; i < digits.size(); ++i)
        {
            const auto &s = add_single_digits(digits[i], other.digits[i]);
            res.digits[i] = s[1];
            carry.digits[i - 1] = s[0];
        }

        return res.add(carry);
    }

    bool is_zero() const
    {
        for (const auto d : digits)
            if (d != '0')
                return false;
        return true;
    }

    int digit_to_decimal(char x) const
    {
        switch (x)
        {
        case '=':
            return -2;
        case '-':
            return -1;
        default:
            return x - '0';
        }
    };

    std::string add_single_digits(char x, char y) const
    {
        std::string table[5][5] = {{"-1", "-2", "0=", "0-", "00"},
                                   {"-2", "0=", "0-", "00", "01"},
                                   {"0=", "0-", "00", "01", "02"},
                                   {"0-", "00", "01", "02", "1="},
                                   {"00", "01", "02", "1=", "1-"}};
        return table[digit_to_decimal(x) + 2][digit_to_decimal(y) + 2];
    }

    // reversed digits
    std::vector<char> digits;
};

std::vector<Snafu> read_file(std::string filename)
{
    std::vector<Snafu> out;
    size_t max_size = 0;
    int num = 0;

    std::ifstream ifs(filename);
    std::string line;
    while (++num, std::getline(ifs, line))
        max_size = std::max(max_size, line.size());
    max_size = max_size + num / 2; // conservative padding

    std::ifstream ifs2(filename);
    while (std::getline(ifs2, line))
        out.push_back(Snafu(line, max_size));

    return out;
}

void part_one()
{
    const auto &nums = read_file("day25.in");
    Snafu sum("", nums[0].digits.size());
    for (const auto &n : nums)
        sum = sum.add(n);

    std::string out(sum.digits.begin(), sum.digits.end());
    std::cout << out << '\n';
}

int main()
{
    part_one();
}
