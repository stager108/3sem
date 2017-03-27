
#include <iostream>
#include <thread>
#include <atomic>
#include <array>
#include <vector>
#include <queue>
#include <limits.h>

class Network {


	static const int MYNULL = -1;
	struct MyEdge {
		int from;
		int to;
		int capacity;
		int flow = 0;
		int previous_edge = MYNULL;
		int next_edge = MYNULL;
		bool forbidden = false;

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
		void changeCapacity(int v){
		    myNetwork->allEdges[position].capacity += v;
		}
        void changeFlow( int v){
		    myNetwork->allEdges[position].flow += v;
		    myNetwork->allEdges[myNetwork->backEdge(position)].flow -= v;
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
		// subInsertEdge(new_to, new_from, -new_capacity);
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
class DinicAlgorithm {
	std::vector <int> distance;
	size_t vertex_count;
	size_t source;
	size_t target;
	int max_flow;
	Network myNetwork;
public:
	DinicAlgorithm() {}
	~DinicAlgorithm() {}
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
					myIt.pushFlow(delta); // �������� ����� �� ���� dfs
					return delta;
				}
			}
			myNetwork.shiftBegin(myIt.getStart());
			myIt++;
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
        max_flow = maxFlow;
		return maxFlow;
	}

	int tryIncreaseFlow(int from, int to){
        Network::NetworkEdgesIterator myIt = myNetwork.begin(from);
        while ((!myIt.isNull())&&(myIt.getFinish()!= to)){
            myIt++;
        }
        if(myIt.isNull()){
            myNetwork.insertEdge(from, to, 1);
        }
        else{
            myIt.changeCapacity( + 1 );
        }
	    if(bfs()){
            int flow = dfs(source, INT_MAX);
            if(flow != 0) {
                max_flow += flow;
            }
	    }
        return max_flow;
	}

    int tryDecreaseFlow(int from, int to){

        Network::NetworkEdgesIterator myIt = myNetwork.begin(from);
        while ((!myIt.isNull())&&(myIt.getFinish()!= to)){
            myIt++;
        }
        if( myIt.getResidualCapacity()> 0){
            myIt.changeCapacity( -1 );
            return max_flow;
        }
        else{
            myIt.changeCapacity( -1 );
            myIt.changeFlow( -1 );
            max_flow--;
            max_flow = std::max(0, max_flow);
        }
	    if(bfs()){
            int flow = dfs(source, INT_MAX);
            if(flow != 0) {
                max_flow += flow;
            }
	    }
        return max_flow;
    }
	void readNetwork(){
		int a, b, c, n, m;
		std::cin >> n >> m;
        vertex_count = n;
        myNetwork.setSize(n);
		source = 0;
		target = n - 1;

		for (int i = 0; i < m; i++) {
			std::cin >> a >> b >> c;
			myNetwork.insertEdge(a - 1, b - 1, c);
		}
	}
    void getAnswer(){
        int cnt, nmb, from, to;
        readNetwork();
        max_flow = findMaxFlow();
        std::cout << max_flow << std::endl;
        std::cin >> cnt;
        for(int i = 0; i < cnt; i++){
            std::cin>> nmb >> from >> to;
            if(nmb == 1){
                std:: cout << tryIncreaseFlow(from - 1, to - 1 ) << std::endl;;
            }
            else{
                std::cout << tryDecreaseFlow(from - 1, to - 1) << std::endl;
            }
        }
    }
};

int main() {

    DinicAlgorithm myAl;
    myAl.getAnswer();
	return 0;
}
