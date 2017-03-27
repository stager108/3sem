
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
        edge(int t, int c, int cs, int f, int num, size_t b){
            to = t;
            capacity = c;
            cost = cs;
            flow = f;
            number = num;
            back = b;
        }
    };

    void add_rib(std::vector<std::vector<edge> > &g, int new_from, int new_to, int new_capacity, int new_cost, int num) {
        size_t a = g[new_to].size();
        size_t b = g[new_from].size();
        edge r1 = {new_to, new_capacity, new_cost, 0, num,  a};
        edge r2 = {new_from, 0, -new_cost, 0, num, b };
        g[new_from].push_back(r1);
        g[new_to].push_back(r2);
    }

    std::vector<std::vector<edge>> g;

public:
    minCostAlgorithm() {}

    ~minCostAlgorithm() {}

    void readNetwork() {
        int a, b, c;
        std::cin >> n >> m >> k;
        vertex_count = n;
        g.resize(vertex_count);
        source = 0;
        target = n-1;

        for (int i = 0; i < m; i++) {
            std::cin >> a >> b >> c;
            add_rib(g, a - 1, b - 1, 1, c, i);
            add_rib(g, b - 1, a - 1, 1, c, i);
        }

    }

    int findMFMC() {
        cost = 0;
        while (flow < k) {
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


            if (distance[target] == INT_MAX/2) return -1;
            int addflow = INT_MAX;
            for (int nextVertex = target; nextVertex != source; nextVertex = parent[nextVertex]) {
                int pv = parent[nextVertex];
                size_t pr = p_rib[nextVertex];
                addflow = std::min(addflow, g[pv][pr].capacity - g[pv][pr].flow);
            }
            for (int vert = target; vert != source; vert = parent[vert]) {
                int pv = parent[vert];
                int pr = p_rib[vert], r = g[pv][pr].back;
                g[pv][pr].flow += addflow;
                g[vert][r].flow -= addflow;
                cost += g[pv][pr].cost * addflow;
            }
            flow += addflow;
        }
        return flow;
    }
    void printWays(){
        std::vector<bool> closed_roads(m, 0);
        for(int i = 0 ; i < k; i++){
            int j = 0;
            int next = source;
            std::vector<int> towns;
            std::vector<int> used(n,0);
            used[source] = 1;
            while((g[source][j].flow == 0)||(closed_roads[g[source][j].number] == 1))
                j++;
            towns.push_back(g[source][j].number);
            closed_roads[g[source][j].number] = 1;
            next = g[source][j].to;
            while(next != target){
                int e = next;
                for(int h = 0; h < g[e].size(); h++){
                    if((closed_roads[g[e][h].number] == 0)&&(g[e][h].flow > 0)&&(used[g[e][h].to] == 0)){
                        towns.push_back(g[e][h].number);
                        closed_roads[g[e][h].number] = 1;
                        used[g[e][h].to] = 1;
                        next = g[e][h].to;
                        break;
                    }
                }
            }
            std::cout << towns.size()<<" ";
            for(int w = 0; w < towns.size(); w++){
                std::cout<< towns[w] + 1<<" ";
            }
            std::cout << std::endl;
        }
    }
public:
    void getSolution(){
        readNetwork();
        int r = findMFMC();
        if( r < k ){
            std::cout << "-1";
        }
        else{
            double d = cost;
            d = d/k;
            std::cout.setf(std::cout.fixed);
            std::cout.precision(6);
            std::cout << d << std::endl;

            //printf("%.5d\n",d);
            printWays();
        }

    }
};

int main() {
    minCostAlgorithm myAl;
    myAl.getSolution();

    return 0;
}
