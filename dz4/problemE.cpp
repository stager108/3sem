
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
    struct edge {
        int to, capacity,flow;
        size_t back, doubled;
        edge(int t, int c, int f, size_t b){
            to = t;
            capacity = c;
            flow = f;
            back = b;
        }
    };

    void insertEdge(std::vector<std::vector<edge> > &g, int new_from, int new_to, int new_capacity) {
        size_t a = g[new_to].size();
        size_t b = g[new_from].size();
        edge r1 = {new_to, new_capacity, 0,  a};
        edge r2 = {new_from, 0,  0,  b };
        g[new_from].push_back(r1);
        g[new_to].push_back(r2);
    }

	size_t vertex_count;
	int n, k;

	int source = 0;
	int target = 1;
	int max_flow = 0;
    std::vector<std::vector<edge>> g;
    std::vector<std::vector<int>> girls;
    std::vector <int> stopped;
    std::vector<int> distance;
    std::vector<int> begins;

public:
    minCostAlgorithm() {}

    ~minCostAlgorithm() {}

    void readNetwork() {
        int a, b, c;
        scanf("%d %d",&n, &k);
        vertex_count = n + k + 2;
        g.resize(vertex_count);
        girls.resize(n);
        source = 0;
        target = 1;

        for(int i = 0; i < n; i ++){
            scanf("%d", &a);
            girls[i].resize(a);
            for(int j = 0; j < a; j++){
                scanf("%d", &b);
                insertEdge(g, b+1, k+i+2, 1);
                girls[i][j] = b+1;
            }
        }
        for(int i = 0; i < k ; i++){
            insertEdge(g, source, i+2, 1);
        }
        for(int i = 0; i < n ; i++){
            insertEdge(g, k+i+2, target, 1);
        }


    }
    bool bfs() {
		std::queue <int> myQueue;
		begins.clear();
		begins.resize(vertex_count, 0);
		distance.assign(vertex_count, INT_MAX/2);
		distance[source] = 0;
		myQueue.push(source);
		while (!myQueue.empty()) {
			int u = myQueue.front();
			myQueue.pop();
			for(int i = 0; i < g[u].size(); i++) {
				if (((g[u][i].capacity - g[u][i].flow) > 0) && (distance[g[u][i].to] == INT_MAX/2)&&(g[u][i].to < stopped[u])) {
					distance[g[u][i].to] = distance[u] + 1;
					myQueue.push(g[u][i].to);
				}
			}
		}
		return distance[target] != INT_MAX/2;
	}

	int dfs(int u, int minC) {

		if ((u == target) || (minC == 0)) {
			return minC;
		}
		int delta;
		for( int  i = begins[u]; i < g[u].size(); i++) {
			if ((distance[g[u][i].to] == distance[u] + 1)) {
				delta = dfs(g[u][i].to, std::min(minC, (g[u][i].capacity - g[u][i].flow)));
				if (delta != 0) {
					g[u][i].flow += delta;
					g[g[u][i].to][g[u][i].back].flow -= delta; // насыщаем ребра по пути dfs
					return delta;
				}
			}
			begins[u]++;
		}
		return 0;
	}

	int findMaxFlow() {
		int maxFlow = 0;
		int flow;
		while (bfs()) {
			flow = dfs(source, INT_MAX);
			while (flow != 0) {
				maxFlow += flow;
				flow = dfs(source, INT_MAX);
			}
		}
		return maxFlow;
	}

public:
    void getSolution(){
        readNetwork();
        stopped.resize(n+k+2);
        int q, s, t, v;
        scanf("%d", &q);//сколько компашек подсовывают
        for(int i = 0; i < q; i++){
            scanf("%d", &s); // сколько парней в компашке
            stopped.assign(n+k+2, 10000);
            for(int j = 0; j < s; j++){
                scanf("%d", &v);//текущий парень
                v = v - 1;
                for(int z = 0; z < girls[v].size();z++){//смотрим всех девушек
                    if(stopped[girls[v][z]] > v + k + 2){
                        stopped[girls[v][z]]= v + k + 2;
                    }
                }
            }
            t = 0;
                for(int h = 0 ; h < g[source].size(); h++){
                    if(stopped[g[source][h].to] < 10000){
                        g[source][h].capacity = 1;
                        t++;
                    }
                    else{
                        g[source][h].capacity = 0;
                    }
                }
                for(int h = 2; h < k +2; h ++){
                    for(int y = 0; y < g[h].size(); y++){
                        if((g[h][y].to < stopped[h] )){
                            g[h][y].capacity = 1;
                        }
                        else{
                             g[h][y].capacity = 0;
                        }
                    }

                }
            int r = findMaxFlow();
            if(r == t){
                printf("Yes\n");
            }
            else{
                printf("No\n");
            }
            for(int j = 0; j < vertex_count; j++){
                for(int f = 0 ; f < g[j].size(); f++ ){
                    g[j][f].flow = 0;
                }
            }
        }
    }
};

int main() {
    minCostAlgorithm myAl;
    myAl.getSolution();

    return 0;
}
