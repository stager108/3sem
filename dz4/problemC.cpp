
#include <iostream>
#include <thread>
#include <atomic>
#include <array>
#include <vector>
#include <queue>
#include <limits.h>
#include <algorithm>
#include <stdio.h>

class minCostAlgorithm {
    long long int flow = 0, cost = 0;
    std::vector <int> start;
    std::vector <int> finish;
    std::vector <int> pribil;
    size_t vertex_count;
    size_t source;
    size_t target;
    int n, m;
    struct edge {
        int to, capacity, cost, flow;
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
        edge r2 = {new_from, 0, -new_cost, 0, b };
        g[new_from].push_back(r1);
        g[new_to].push_back(r2);
    }

    std::vector<std::vector<edge>> g;

public:
    minCostAlgorithm() {}

    ~minCostAlgorithm() {}

    void readNetwork() {
        int a, b, c;
        scanf("%d",&n);
        vertex_count = n*2 + 2;
        g.resize(vertex_count);
        source = 0;
        target = 1;

        for (int i = 0; i < n; i++) {
            scanf("%d",&a);
            add_rib(g, i+2 ,n+i+2 , 1, a);
        }
        for (int i = 0; i < n; i++) {
            scanf("%d",&b);
            for(int j = 0; j < b ; j ++){
                scanf("%d",&c);
                add_rib(g, n+i+2 ,c+2 , INT_MAX/2, 0);
            }
            if(b == 0){
                add_rib(g, source, i+2, INT_MAX/2, 0);
            }
        }
        for(int i = 0; i < n ; i ++){
            add_rib(g, n+i+2, target, INT_MAX/2, 0);
        }


    }

    int findMFMC() {
        cost = 0;
        while (true) {
            std::vector<int> used(vertex_count, 0);
            std::vector<long long int> distance(vertex_count, INT_MAX/2);
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
                for (int i = 0; i < g[vertex].size(); i++){
                    if (g[vertex][i].capacity <= g[vertex][i].flow)
                        continue;
                    if (distance[g[vertex][i].to] > distance[vertex] + g[vertex][i].cost){
                        distance[g[vertex][i].to] = distance[vertex] + g[vertex][i].cost;
                        parent[g[vertex][i].to] = vertex;
                        p_rib[g[vertex][i].to] = i;
                        if (!used[g[vertex][i].to]){
                            my_queue.push(g[vertex][i].to);
                            used[g[vertex][i].to] = 1;
                        }
                    }
                }
            }


            if (distance[target] == INT_MAX/2) return cost;
            int addflow = INT_MAX/2;
            for (int nextVertex = target; nextVertex != source; nextVertex = parent[nextVertex]) {
                int pv = parent[nextVertex];
                size_t pr = p_rib[nextVertex];
                addflow = std::min(addflow, g[pv][pr].capacity - g[pv][pr].flow);
            }
            int dcost = 0;
            for (int vert = target; vert != source; vert = parent[vert]) {
                int pv = parent[vert];
                int pr = p_rib[vert], r = g[pv][pr].back;
                g[pv][pr].flow += addflow;
                g[vert][r].flow -= addflow;
                dcost += g[pv][pr].cost * addflow;
            }
            if((addflow >0)&&(dcost > 0)){
                flow += addflow;
                cost += dcost;
                std::cout<<"abacaba ";
            }
            else{
                    break;
            }

        }

        return cost;
    }

public:
    void getSolution(){
        readNetwork();
        int r = findMFMC();
        std::cout << r;

    }
};

int main() {
    minCostAlgorithm myAl;
    myAl.getSolution();

    return 0;
}
