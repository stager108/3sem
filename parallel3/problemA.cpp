
#include <iostream>
#include <thread>
#include <atomic>
#include <array>
#include <vector>
#include <queue>
#include <limits.h>
#include <stdio.h>
class Network {


	static const int MYNULL = -1;
	struct MyEdge {
		int from;
		int to;
		int capacity = 0;
		int flow = 0;
		int previous_edge = MYNULL;
		int next_edge = MYNULL;

		MyEdge(int f, int t, int c) {
			from = f;
			to = t;
			capacity = c;
		}
	};

	struct MyVertex {
		int first_edge = MYNULL;
		int last_edge = MYNULL;
	};

	std::vector <MyEdge> allEdges;
	std::vector <MyVertex> allVertexes;
	std::vector <int> iteratorBegins;
	size_t nw_size = 0;
	size_t edges_count = 0;
	size_t source;
	size_t target;


public:

	class NetworkEdgesIterator {
		size_t EdgeNum;
		Network* myNetwork;

		int vertex;
	public:
		int position;
	public:
		//friend class Network;
		NetworkEdgesIterator() {}

		NetworkEdgesIterator(Network* newNetwork) {
			myNetwork = newNetwork;
		}
		NetworkEdgesIterator(Network* newNetwork, int new_vertex, int new_position) {
			myNetwork = newNetwork;
			position = new_position;
			vertex = new_vertex;
		}

		void pushFlow(int flow_inc) {
			myNetwork->allEdges[position].flow += flow_inc;
			myNetwork->allEdges[myNetwork->backEdge(position)].flow -= flow_inc;
		}

		MyEdge getEdge() const {
			return myNetwork->allEdges[position];
		}

		int getStart() const {
			return myNetwork->allEdges[position].from;
		}
		int getFinish() const {
			return myNetwork->allEdges[position].to;
		}
		int getCapacity() const {
			return myNetwork->allEdges[position].capacity;
		}
		int getFlow() const {
			return myNetwork->allEdges[position].flow;
		}
		int getResidualCapacity() const {
			return myNetwork->allEdges[position].capacity - myNetwork->allEdges[position].flow;
		}
		bool isNull() {
			return (position == MYNULL);
		}
		NetworkEdgesIterator& operator ++ () {
			if(position != MYNULL)
				position = myNetwork->allEdges[position].next_edge;
			return *this;
		}
		NetworkEdgesIterator& operator ++ (int) {
			if (position != MYNULL)
			    position = myNetwork->allEdges[position].next_edge;
			return *this;
		}

		bool operator == (const NetworkEdgesIterator &other) {
			return (position == other.position);
		}
		bool operator != (const NetworkEdgesIterator &other) {
			return (position != other.position);
		}

		bool isBackEdge() {
			return position % 2;
		}
	};

	Network() {}

	Network(size_t network_size) {
		nw_size = network_size;
		allVertexes.resize(nw_size);
		iteratorBegins.resize(nw_size, MYNULL);
	}

	Network(size_t network_size, size_t s, size_t t) {
		nw_size = network_size;
		source = s;
		target = t;
		allVertexes.resize(nw_size);
		iteratorBegins.resize(nw_size, MYNULL);
	}

	void insertEdge(size_t new_from, size_t new_to, int new_capacity) {
		subInsertEdge(new_from, new_to, new_capacity);
		subInsertEdge(new_to, new_from, 0);
	}


	void shiftBegin(int vertex) {
		if (vertex != MYNULL) {
			if (iteratorBegins[vertex] != MYNULL) {
				iteratorBegins[vertex] = allEdges[iteratorBegins[vertex]].next_edge;
			}
		}
	}

	NetworkEdgesIterator shiftedBegin(size_t vertex_number) {
		return NetworkEdgesIterator(this, vertex_number, iteratorBegins[vertex_number]);
	}
	NetworkEdgesIterator begin(size_t vertex_number) {
		return NetworkEdgesIterator(this, vertex_number, allVertexes[vertex_number].first_edge);
	}
	NetworkEdgesIterator end(size_t vertex_number) {
		return NetworkEdgesIterator(this, vertex_number, allVertexes[vertex_number].last_edge);
	}

	void setSize(int network_size) {
		nw_size = network_size;
		allEdges.clear();
		allVertexes.resize(nw_size);
		edges_count = 0;
		iteratorBegins.clear();
		iteratorBegins.resize(nw_size, -1);
	}
	void clearIterator(){
		iteratorBegins.clear();
		iteratorBegins.resize(nw_size, -1);
		for (int i = 0; i < nw_size; i++) {
			iteratorBegins[i] = allVertexes[i].first_edge;
		}
	}
	~Network() {}

private:

	void subInsertEdge(size_t new_from, size_t new_to, int new_capacity) {

		MyEdge NewEdge(new_from, new_to, new_capacity);
		int tmp = allVertexes[new_from].first_edge;

		if (allVertexes[new_from].first_edge == MYNULL) {
			allVertexes[new_from].last_edge = edges_count;
		}

		allVertexes[new_from].first_edge = edges_count;
		NewEdge.next_edge = tmp;

		if (tmp != MYNULL) {
			allEdges[tmp].previous_edge = edges_count;
		}

		allEdges.push_back(NewEdge);
		edges_count++;
	}

	size_t backEdge(size_t number) {
		if (number % 2 == 0) {
			return number + 1;
		}
		else {
			return number - 1;
		}
	}

};
class OptimalFind {
	std::vector <int> distance;
	size_t vertex_count;
    int N;
    int M;
    int blackValentnost = 0;
    int whiteValentnost = 0;
	int source = 0;
	int target = 1;
	int hole_count = 0;
	int max_flow = 0;
	Network myNetwork;

public:
	OptimalFind() {}
	~OptimalFind() {}

	void findAnswer(){
        readNetwork();
        if( (whiteValentnost == blackValentnost)&&(whiteValentnost!= 0)){
            max_flow = findMaxFlow();
            if(( max_flow == whiteValentnost)&&(max_flow == blackValentnost))
                std::cout<< "Valid";
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
bool bfs() {
		std::queue <int> myQueue;
		distance.clear();
		myNetwork.clearIterator();
		distance.resize(vertex_count, INT_MAX);
		distance[source] = 0;
		myQueue.push(source);
		while (!myQueue.empty()) {

			int u = myQueue.front();
			myQueue.pop();
			Network::NetworkEdgesIterator myIt = myNetwork.shiftedBegin(u);
			while (!myIt.isNull()) {
				if ((myIt.getResidualCapacity() > 0) && (distance[myIt.getFinish()] == INT_MAX)) {
					distance[myIt.getFinish()] = distance[u] + 1;
					myQueue.push(myIt.getFinish());
				}
				if (myIt.getResidualCapacity() == 0) {
				}
				myIt++;
			}
		}
		return distance[target] != INT_MAX;
	}

	int dfs(int u, int minC) {

		if ((u == target) || (minC == 0)) {
			return minC;
		}
		int delta;
		Network::NetworkEdgesIterator myIt = myNetwork.shiftedBegin(u);
		while (!myIt.isNull()) {
			if (distance[myIt.getFinish()] == distance[u] + 1) {
				delta = dfs(myIt.getFinish(), std::min(minC, myIt.getResidualCapacity()));
				if (delta != 0) {
					myIt.pushFlow(delta); // насыщаем ребра по пути dfs
					return delta;
				}
			}
			myNetwork.shiftBegin(myIt.getStart());
			myIt++;
		}
		return 0;
	}
	void readNetwork (){

	    std:: string S;
		std::cin >> N >> M ;
		vertex_count = N * M + 2;
        myNetwork.setSize(N * M + 2);
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
                    if( ( i + j ) % 2){
                        myNetwork.insertEdge(source, i*M + j + 2, element(H[i][j]));

                    if((i != 0 )&&(H[i-1][j] != '.')){
                        myNetwork.insertEdge(i*M + j+2, (i - 1)*M + j+2, 1);
                    }
                    if((i != N - 1 )&&(H[i+1][j] != '.')){
                        myNetwork.insertEdge(i*M + j+2, (i + 1)*M + j+2, 1);
                    }
                    if( (j != M - 1 )&&(H[i][j+1] !='.')){
                        myNetwork.insertEdge(i*M + j+2, i * M  + j+2 + 1, 1);
                    }
                    if(( j != 0 )&&(H[i][j-1]!= '.')){
                        myNetwork.insertEdge(i*M + j+2, i * M + j+2 - 1, 1);
                    }
                    whiteValentnost+= element(H[i][j]);

                }
                else{
                    myNetwork.insertEdge(i*M + j + 2, target, element(H[i][j]));
                    blackValentnost+= element(H[i][j]);
                }
            }
        }
    }
   /* std::cout << std::endl;
    for(int i = 0; i<N; i++){
        for(int j = 0; j <M; j++){
            std::cout<<H[i][j];
        }
        std::cout<<std::endl;
    }*/
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
