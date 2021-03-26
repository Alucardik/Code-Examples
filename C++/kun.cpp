#include <iostream>
#include <string>
#include <queue>
#include <vector>

int max_pr_sz = 0;

bool kun_dfs(int cur_v, std::vector<bool>& used, std::vector<int>& matches, const std::vector<std::vector<int>>& gr) {
    if (used[cur_v]) {
        return false;
    }

    used[cur_v] = true;

    for (auto v : gr[cur_v]) {
        if (matches[v] == -1 || kun_dfs(matches[v], used, matches, gr)) {
            if (matches[v] == -1)
                ++max_pr_sz;
            matches[v] = cur_v;
            return true;
        }
    }
    return false;
}

int main() {
    int v_num, r_num;
    std::cin >> v_num >> r_num;
    std::vector<std::vector<int>> bi_graph(v_num);
    for (size_t i = 0; i < r_num; ++i) {
        int l_v, r_v;
        std::cin >> l_v >> r_v;
        bi_graph[l_v - 1].push_back(r_v - 1);
    }
    std::vector<int> matchings(v_num, -1);
    for (size_t i = 0; i < v_num; ++i) {
        std::vector<bool> used(v_num, false);
        kun_dfs(i, used, matchings, bi_graph);
    }

    std::cout << v_num - max_pr_sz;
    return 0;
}