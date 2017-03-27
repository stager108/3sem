
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

    std::vector<int> distance;
    std::vector<int> begins;
	std::vector<int> myMap;
	std::vector <int> numb;
	size_t vertex_count;
    int N;
    int M;
    int blackValentnost = 0;
    int whiteValentnost = 0;
	int source = 0;
	int target = 1;
	int e_count = 0;
	int max_flow = 0;

public:
	OptimalFind() {}
	~OptimalFind() {}

	void findAnswer(){
        readNetwork();
        if( (whiteValentnost == blackValentnost)&&(whiteValentnost!= 0)){
            max_flow = findMaxFlow();
            if(( max_flow == whiteValentnost)&&(max_flow == blackValentnost))
                std::cout<< "Valid";
            else{
                std::cout << "Invalid";
            }
        }
        else{
            std::cout << "Invalid";
        }

	}
private:
int element(char d){
    int ans;
    switch (d){
        case 'H': ans = 1; break;
        case 'O': ans = 2;break;
        case 'N': ans  = 3; break;
        case 'C': ans = 4; break;
        default: ans = 0; break;
    }
    return ans;
}
int isConnected(){
        int cnt = -1;
        std::queue <int> myQueue;
		distance.clear();
		begins.clear();
		begins.resize(vertex_count, 0);
		distance.resize(vertex_count, 0);
		distance[2] = 0;
		myQueue.push(2);
		while (!myQueue.empty()) {

			int u = myQueue.front();
			myQueue.pop();
			cnt++;
            while (!myQueue.empty()) {
                int u = myQueue.front();
                myQueue.pop();
                for(int i = 0; i < g[u].size(); i++) {
                    if (distance[g[u][i].to] == 0) {
                        distance[g[u][i].to] = 1;
                        myQueue.push(g[u][i].to);
                    }
                }
            }


		}
		return (cnt == (vertex_count - 2));
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
	void readNetwork (){

	    std:: string S;
		std::cin >> N >> M ;
		vertex_count = N * M + 2;
		g.resize(vertex_count);
        std::vector< std::vector<char> > H(N);
        for(int j = 0; j < N; j++){
            H[j].resize(M);
        }
		source = 0;
		target = 1;
		char d = ' ';
		for(int i = 0; i < N; i++){
            for(int j = 0; j < M; j++){
                    while ((d!= '.')&&(d!='O')&&(d!='H')&&(d!='C')&&(d!='N'))
                        d = getchar();
                    H[i][j] = d;
                    d =' ';
            }
		}
		for( int i = 0 ; i < N; i++){
            for(int j = 0 ; j < M; j++){

                if( H[i][j] != '.'){
                    e_count++;
                    if( ( i + j ) % 2){
                        insertEdge(g, source, i*M + j + 2, element(H[i][j]));
                        if((i != 0 )&&(H[i-1][j] != '.')){
                            insertEdge(g, i*M + j+2, (i - 1)*M + j+2, 1);
                        }
                        if((i != N - 1 )&&(H[i+1][j] != '.')){
                            insertEdge(g, i*M + j+2, (i + 1)*M + j+2, 1);
                        }
                        if( (j != M - 1 )&&(H[i][j+1] !='.')){
                            insertEdge(g, i*M + j+2, i * M  + j+2 + 1, 1);
                        }
                        if(( j != 0 )&&(H[i][j-1]!= '.')){
                            insertEdge(g, i*M + j+2, i * M + j+2 - 1, 1);
                        }
                        whiteValentnost+= element(H[i][j]);
                    }
                else{
                    insertEdge(g, i*M + j + 2, target, element(H[i][j]));
                    blackValentnost+= element(H[i][j]);
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
		return maxFlow;
	}
};

int main() {
    OptimalFind myAl;
	myAl.findAnswer();

	return 0;
}
