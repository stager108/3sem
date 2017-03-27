
#include <iostream>
#include <thread>
#include <atomic>
#include <array>
#include <vector>
#include <queue>
#include <limits.h>
#include <stdio.h>

class minCostAlgorithm {
    int flow = 0, cost = 0;
    std:: vector<int> pairs;
    std:: vector<char> used;
    size_t vertex_count;
    size_t source;
    size_t target;
    int n, m, k;
    struct edge {
        int to, capacity, cost, flow, number;
        size_t back, doubled;
        edge(int t, int c, int f, int num, size_t b){
            to = t;
            capacity = c;
            flow = f;
            number = num;
            back = b;
        }
    };

    void add_rib(std::vector<std::vector<edge> > &g, int new_from, int new_to, int new_capacity, int num) {
        size_t a = g[new_to].size();
        size_t b = g[new_from].size();
        edge r1 = {new_to, new_capacity, 0, num, a};
        edge r2 = {new_from, 0, 0, -1,  b};
        g[new_from].push_back(r1);
        g[new_to].push_back(r2);
    }

    std::vector<std::vector<edge>> g;
    std::vector<int> costs;

public:
    minCostAlgorithm() {}

    ~minCostAlgorithm() {}

    void readNetwork() {
        int a, b, c, e;
        scanf("%d %d %d", &n, &m, &e);
        vertex_count = n + m;
        g.resize(vertex_count);
        costs.resize(vertex_count);
        source = 0;
        target = 1;
        for(int i = 0 ; i < n + m ; i ++){
            scanf("%d", &a);
            costs[i+2] = a;
        }

        for (int i = 0; i < e; i++) {
            scanf("%d %d", &a, &b);
            add_rib(g, a + 1, n + b + 1, 1, i);
        }
    }
    bool try_kuhn (int vertex) {
        if (used[vertex]){
            return false;
        }
        used[vertex] = true;
        for (size_t i = 0; i < g[vertex].size(); i++) {
            int to = g[vertex][i].to;
            if ((pairs[to] == -1) || (try_kuhn ( pairs[to]))){
                 pairs[to] = vertex;
                return true;
            }
        }
        return false;
    }
public:
    void getSolution(){
        readNetwork();
        pairs.assign (n, -1);
        for (int v = 0; v < n; ++v) {
            used.assign (n, false);
            try_kuhn(v);
        }
        int w;
        for (int i=0; i<k; ++i)
            if (pairs[i] != -1)
                printf ("%d %d\n",  pairs[i]+1, i+1);
        }
};

int main() {
    minCostAlgorithm myAl;
    myAl.getSolution();

    return 0;
}
