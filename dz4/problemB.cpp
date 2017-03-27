
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
    int getC(int a, int b){
        if( a == b ){
            return INT_MAX/2;
        }
        else{
            return 1;
        }
    }
	int readCoordinate() {
		int a, b;
		scanf("%i %i", &a, &b);
		//std::cin >> a >> b;
		a--;
		b--;
		return a*N + b;

	}
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
	bool dop_bfs() {
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
				if (((g[u][i].capacity - g[u][i].flow) > 0) && (distance[g[u][i].to] == INT_MAX)&&(myMap[g[u][i].to] == 1)) {
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
	int rEnd(int v){
        if(myMap[v] == 2){
            return v + N*M;
        }
        else{
            return v;
        }
	}
	void readNetwork() {
		int k, l, a, b;
		scanf("%i %i", &M, &N);
		//std::cin >> N >> M;
		vertex_count = N * M * 2;
		g.resize(vertex_count);
		myMap.resize(vertex_count, 1);
        distance.resize(vertex_count,INT_MAX);
		int r = N*M;

		scanf("%i %i", &k, &l);
		for (int i = 0; i < k; i++) {
			myMap[readCoordinate()] = 0;
		}
		for (int i = 0; i < l; i++) {
			myMap[readCoordinate()] = 2;
		}
		source = readCoordinate();
		target = readCoordinate();

        for (int i = 0; i < r; i++) {
			myMap[i+r] = myMap[i];
		}
		for(int i = 0; i < r; i++){
            if(myMap[i] == 2){
                insertEdge(g,i,i+r,1);
            }
		}
        int p = -1;
		for (int i = 0; i < M; i++) {
			for (int j = 0; j < N; j++) {
				p++;
				if ((myMap[p] != 0)) {
					if ((i != 0) && (myMap[p - N] != 0)) {
						insertEdge(g,rEnd(p), p - N, getC(myMap[p], myMap[p-N]));
					}
					if ((i != M - 1) && (myMap[p + N] != 0)) {
						insertEdge(g,rEnd(p), p + N, getC(myMap[p], myMap[p+N]));
					}
					if ((j != N - 1) && (myMap[p + 1] != 0)) {
						insertEdge(g,rEnd(p), p + 1, getC(myMap[p], myMap[p+1]));
					}
					if ((j != 0) && (myMap[p - 1] != 0)) {
						insertEdge(g,rEnd(p), p - 1, getC(myMap[p], myMap[p-1]));
					}
				}

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


	void printAnswer() {
		std::queue <int> myQueue;
		std::vector <bool> used(vertex_count, 0);
		std::vector <bool> ans(vertex_count, 0);
		myQueue.push(source);
		int r = N*M;
		while (!myQueue.empty()) {
			int u = myQueue.front();
			myQueue.pop();
			used[u] = 1;
			for( int i = 0; i < g[u].size(); i++) {
				if ((g[u][i].capacity > 0)&&(distance[g[u][i].to] == INT_MAX)) {
                    if(myMap[g[u][i].to] == 1){
                        ans[u % r] = 1;
                    }
                    else{
                        ans[(g[u][i].to)%r] = 1;
                    }
				}
				else {
					if ((g[u][i].capacity > 0)&&(!used[g[u][i].to])){
						myQueue.push(g[u][i].to);
                        used[g[u][i].to] = 1;
					}
				}
			}
		}

		int res = 0;
		for (int i = 0; i < r; i++) {
			res += ans[i];
		}
		printf("%i\n", res);
		for (int i = 0; i < r; i++) {
			if (ans[i]) {
				int a = i / N + 1;
				int b = i % N + 1;
				printf("%i %i\n", a, b);
			}
		}
	}
public:
	OptimalFind() {}
	~OptimalFind() {}

	void findAnswer() {
		readNetwork();
		if (dop_bfs()) {
			printf("%i", -1);
		}
		else {
            int a = findMaxFlow();
				if (a > 0) {
					printAnswer();
				}
				else {
					printf("%i", 0);
				}

			}
	}
};

int main() {
	OptimalFind myAl;
	myAl.findAnswer();

	return 0;
}

