
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
        long long int a, b, c;
        scanf("%d",&n);
        scanf("%d",&m);
        vertex_count = n*2 + 2;
        g.resize(vertex_count);
        start.resize(vertex_count);
        finish.resize(vertex_count);
        pribil.resize(vertex_count);
        std::vector <std::pair<int, int>> myVector(n);
        source = 0;
        target = 1;

        for (int i = 0; i < n; i++) {
            scanf("%lld",&a);
            scanf("%lld",&b);
            scanf("%lld",&c);
            start[i] = a;
            finish[i] = a + b;
            pribil[i] = c;
            myVector[i].first = start[i];
            myVector[i].second = i;
        }
        std::sort(myVector.begin(), myVector.end(), [](std::pair<int, int> a, std::pair<int,int> b){ return a.first < b.first; });
        for(int i = 0; i < n - 1; i ++){
            add_rib(g, myVector[i].second + 2, myVector[i+1].second + 2, 2*m, 0);//ne berem
            for(int j = i+1; j < n; j ++){
                if((finish[myVector[i].second] <= start[myVector[j].second])){
                    add_rib(g, n + myVector[i].second + 2, myVector[j].second + 2, 2*m, 0);//berem, idem dalshe
                    break;
                }
            }


        }

        add_rib(g, myVector[n - 1].second + 2, target, 2 * m, 0);

        for(int i = 0; i < n ; i ++){
            add_rib(g, i+2, n+i+2, 1, -pribil[i]);
        }
        for(int i = 0; i < n ; i ++){
            add_rib(g, source, i+2, 2*m, 0);
        }
        for(int i = 0; i < n ; i ++){
            add_rib(g, n+i+2, target, 2*m, 0);
        }


    }

    int findMFMC() {
        cost = 0;
        while (flow < m) {
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


            if (distance[target] == INT_MAX/2) return -1;
            int addflow = INT_MAX/2;
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
        for(int i = 0 ; i < n; i++){
            int r = 0;
            for(int j = 0; j < g[n+i+2].size(); j++){
                if(g[n+i+2][j].flow == 1){
                    r = 1;
                    break;
                }
            }
            printf("%d ",r);
        }
    }
public:
    void getSolution(){
        readNetwork();
        int r = findMFMC();
        printWays();

    }
};

int main() {
    minCostAlgorithm myAl;
    myAl.getSolution();

    return 0;
}
