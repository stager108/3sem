
#include <iostream>
#include <thread>
#include <atomic>
#include <array>
#include <vector>
#include <queue>
#include <limits.h>
#include <stdio.h>



class OptimalFind {
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

    std::vector<std::vector<edge>> g;

	size_t vertex_count;
	int N;
	int M;
	int source = 0;
	int target = 1;
	int e_count = 0;
	int max_flow = 0;
    std::vector<int> distance;
    std::vector<int> begins;
	std::vector<int> myMap;
	std::vector <int> numb;

private:

	bool bfs() {
		std::queue <int> myQueue;
		begins.clear();
		begins.resize(vertex_count, 0);
		distance.assign(vertex_count, INT_MAX);
		distance[source] = 0;
		myQueue.push(source);
		while (!myQueue.empty()) {
			int u = myQueue.front();
			myQueue.pop();
			for(int i = 0; i < g[u].size(); i++) {
				if (((g[u][i].capacity - g[u][i].flow) > 0) && (distance[g[u][i].to] == INT_MAX)) {
					distance[g[u][i].to] = distance[u] + 1;
					myQueue.push(g[u][i].to);
				}
			}
		}
		return distance[target] != INT_MAX;
	}

	int dfs(int u, int minC) {

		if ((u == target) || (minC == 0)) {
			return minC;
		}
		int delta;
		for( int  i = begins[u]; i < g[u].size(); i++) {
			if (distance[g[u][i].to] == distance[u] + 1) {
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

	void readNetwork() {
        int w, r, v;
		std::cin >> v;
		N = v;
		vertex_count = v + 2;
		g.resize(vertex_count);
		source = 0;
		target = 1;

		for (int i = 0; i < v; i++) {
            std::cin >> w;
            insertEdge(g,source, i + 2, -w + 2000);
        }
        for (int i = 0; i < v; i++) {
            insertEdge(g, i + 2, target, 2000);
        }
		for (int i = 0; i < v; i++) {
            std::cin >> w;
            for(int j = 0; j < w; j++){
                std::cin >> r;
                insertEdge(g, r+1, i+2, INT_MAX/2);
            }
        }
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
		max_flow = maxFlow;
		return maxFlow;
	}
public:
	OptimalFind() {}
	~OptimalFind() {}

	void findAnswer() {
		readNetwork();
        int a = findMaxFlow();
        a = a - N*2000;
        std::cout << -a;
	}
};

int main() {
	OptimalFind myAl;
	myAl.findAnswer();

	return 0;
}

