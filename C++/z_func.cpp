#include <iostream>
#include <string>
#include <queue>
#include <vector>

void z_func(std::vector<int>& f_vec, const std::string& obj) {
    int lb = 0, rb = 0;
    for (int i = 1; i < f_vec.size(); ++i) {
        if (i <= rb) {
            f_vec[i] = std::min(f_vec[i - lb], rb - i + 1);
        }
        int j = f_vec[i], k = f_vec[i] + i;
        while (k < obj.size() && obj[j] == obj[k]) {
            ++f_vec[i], ++j, ++k;
        }
        if (i + f_vec[i] - 1 > rb) {
            lb = i;
            rb = i + f_vec[i] - 1;
        }
    }
}

int main() {
    int lim_len;
    std::string inp_str;
    std::cin >> lim_len >> inp_str;

    if (inp_str.size() <= lim_len) {
        std::cout << "1\n" << inp_str;
        return 0;
    }

    std::vector<int> f_vec1(inp_str.size());
    z_func(f_vec1, inp_str);

    for (size_t i = 1; i <= lim_len; ++i) {
        if (f_vec1[i] % i == 0 && f_vec1[i] + i == inp_str.size()) {
            std::cout << "1\n";
            std::cout << inp_str.substr(0, i);
            return 0;
        }
    }

    bool a = false, b = false, c = false;
    for (auto ch : inp_str) {
        switch (ch) {
            case 'a':
                a = true;
                break;
            case 'b':
                b = true;
                break;
            case 'c':
                c = true;
                break;
        }
    }


    if (a + b + c == 2) {
        std::cout << "2\n";
        if (a) {
            std::cout << "a\n";
        }
        if (b) {
            std::cout << "b\n";
        }
        if (c) {
            std::cout << "c\n";
        }
        return 0;
    }

    if (inp_str.size() <= 2 * lim_len) {
        std::cout << "2\n";
        std::cout << inp_str.substr(0, lim_len) << "\n" << inp_str.substr(lim_len);
        return 0;
    }

    for (size_t pref_len = 1; pref_len <= lim_len; ++pref_len) {
        // we're skipping the same words here
        size_t start = pref_len + f_vec1[pref_len] - (f_vec1[pref_len] % pref_len);

        std::string cur_substr = inp_str.substr(start);
        std::vector<int> f_vec2(cur_substr.size());
        z_func(f_vec2, cur_substr);
        std::vector<bool> visited(inp_str.size(), false);

        for (size_t sec_pref = 1; sec_pref <= lim_len; ++sec_pref) {
            size_t ind = start + sec_pref;
            while (ind != inp_str.size()) {
                if (visited[ind]) {
                    break;
                }
                visited[ind] = true;
                if (f_vec2[ind - start] >= sec_pref) {
                    ind += sec_pref;
                } else if (f_vec1[ind] >= pref_len) {
                    ind += pref_len;
                } else {
                    break;
                }
            }
            if (ind == inp_str.size()) {
                std::cout << "2\n" << inp_str.substr(0, pref_len) << '\n' << inp_str.substr(start, sec_pref);
                return 0;
            }
        }
    }

    std::cout << "3\na \n b \nc \n";
    return 0;
}