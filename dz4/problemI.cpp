
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
    size_t vertex_count;
    size_t source;
    size_t target;
    int n, m, k;
    struct edge {
        int to, capacity, cost, flow, number;
        size_t back, doubled;
        edge(int t, int c, int cs, int f, size_t b){
            to = t;
            capacity = c;
            cost = cs;
            flow = f;
            back = b;
        }
    };

    void add_rib(std::vector<std::vector<edge> > &g, int new_from, int new_to, int new_capacity, int new_cost) {
        size_t a = g[new_to].size();
        size_t b = g[new_from].size();
        edge r1 = {new_to, new_capacity, new_cost, 0, a};
        edge r2 = {new_from, 0, -new_cost, 0, b};
        g[new_from].push_back(r1);
        g[new_to].push_back(r2);
    }

    std::vector<std::vector<edge>> g, h;

public:
    minCostAlgorithm() {}

    ~minCostAlgorithm() {}

    void readNetwork() {
        int a, b, c, e;
        scanf("%d %d", &n, &k);
        vertex_count = 2*n + 2;
        g.resize(vertex_count);
        source = 0;
        target = 1;

        for (int i = 0; i < n; i++) {
            for(int j = 0; j < n; j ++){
                scanf("%d", &a);
                add_rib(g, i + 2, n + j + 2, 1, a);
            }
        }
        for(int i = 0 ; i < n; i ++){
            add_rib(g, source, i + 2, k, 0);
        }
        for(int i = 0 ; i < n; i ++){
            add_rib(g, n+i+2, target, k, 0);
        }
    }

    int findMFMC(  std::vector<std::vector<edge>> &u, int q ) {
        cost = 0;
        flow = 0;
        while (flow < q) {
            std::vector<int> used(vertex_count, 0);
            std::vector<int> distance(vertex_count, INT_MAX/2);
            std::queue <int> my_queue;
            std::vector<int> parent(vertex_count, -1);
            std::vector<size_t> p_rib(vertex_count, -1);
            my_queue.push(source);
            distance[source] = 0;
            my_queue.push(source);
            used[source] = 1;
            while (!my_queue.empty()){
                int vertex =  my_queue.front();
                my_queue.pop();
                used[vertex] = 0;
                for (int i = 0; i < u[vertex].size(); i++){
                    if (u[vertex][i].capacity <= u[vertex][i].flow)
                        continue;
                    if (distance[u[vertex][i].to] > distance[vertex] + u[vertex][i].cost){
                        distance[u[vertex][i].to] = distance[vertex] + u[vertex][i].cost;
                        parent[u[vertex][i].to] = vertex;
                        p_rib[u[vertex][i].to] = i;
                        if (!used[u[vertex][i].to]){
                            my_queue.push(u[vertex][i].to);
                            used[u[vertex][i].to] = 1;
                        }
                    }
                }
            }
            if (distance[target] == INT_MAX/2) return flow;
            int addflow = 1;
            int dcost = distance[target];
            for (int vert = target; vert != source; vert = parent[vert]) {
                int pv = parent[vert];
                int pr = p_rib[vert], r = u[pv][pr].back;
                u[pv][pr].flow += addflow;
                u[vert][r].flow -= addflow;
            }
            cost += dcost;
            flow += addflow;
        }
        return flow;
    }

      void createNewNetwork() {
        h.resize(vertex_count);
        source = 0;
        target = 1;

        for (int i = 0; i < n; i++) {
            for(int j = 0; j < g[i+2].size(); j++){
                if(g[i+2][j].flow == 1){
                    add_rib(h, i+2, g[i+2][j].to , 1, 0);
                }
            }
        }
        for(int i = 0 ; i < n; i ++){
            add_rib(h, source, i + 2, 1, 0);
        }
        for(int i = 0 ; i < n; i ++){
            add_rib(h, n+i+2, target, 1, 0);
        }
    }
    void findNextBipartite() {
        int l = findMFMC(h, n);
        int w;
        for(int i = 0; i < n; i++){
            for(int j = 0; j < h[i+2].size(); j++){
                if((h[i+2][j].flow == 1)&&(h[i+2][j].to != source)){
                    w = h[i+2][j].to - n - 1;
                    printf("%d ", w);
                    h[i+2][j].capacity = 0;
                    h[h[i+2][j].to][h[i+2][j].back].capacity = 0;

                    break;
                }
            }
        }
        printf("\n");
        for(int i = 0; i < vertex_count; i++){
            for(int j = 0; j < h[i].size(); j++){
                h[i][j].flow = 0;
            }
        }
    }
public:
    void getSolution(){
        readNetwork();
        int w;
        int r = findMFMC(g, k*n);
        printf("%d\n", cost);
        createNewNetwork();
        for(int i = 0; i < k; i++){
            findNextBipartite();
        }

    }
};

int main() {
    minCostAlgorithm myAl;
    myAl.getSolution();

    return 0;
}
