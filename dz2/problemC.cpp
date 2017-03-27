
#include <iostream>
#include <thread>
#include <atomic>
#include <array>
#include <vector>
#include <queue>
#include <limits.h>
#include <string>
#include <cmath>
class Network {


	static const int MYNULL = -1;
	struct MyEdge {
		int from;
		int to;
		int capacity;
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
		NetworkEdgesIterator& operator -- () {
			if (position != MYNULL)
				position = myNetwork->allEdges[position].previous_edge;
			return *this;
		}

		NetworkEdgesIterator& operator -- (int) {
			if (position != MYNULL)
				position = myNetwork->allEdges[position].previous_edge;
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
		//subInsertEdge(new_to, new_from, new_capacity);
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
class DinicAlgorithm {
	std::vector <int> distance;
	size_t vertex_count;
	size_t source;
	size_t target;
	Network myNetwork;
	std::string v, u;
public:
	DinicAlgorithm() {}
	~DinicAlgorithm() {}
    int findCapacity( char a, char b){
	    int ans = 0;
	    if(b != '?'){
            ans = 1;
	    }
	    return ans;
	}
    int findCapacityBetweenVertices(char a, char b){
	    int ans = 0;
	    if((b == '?')&&(a=='?')){
            ans = 1;
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
					myIt.pushFlow(delta);
					return delta;
				}
			}
			myNetwork.shiftBegin(myIt.getStart());
			myIt++;
		}
		return 0;
	}
	void findMaxFlow() {
		int maxFlow = 0;
		int flow;
		while (bfs()) {
			flow = dfs(source, INT_MAX);
			while (flow != 0) {
				maxFlow += flow;
				flow = dfs(source, INT_MAX);
			}
		}
	}


	void readNetwork(){
		std::cin >> v >> u ;
        //v = "00?";
        //u = "1?";
		size_t vlen = v.length();
		size_t ulen = u.length();
		vertex_count = vlen + ulen + 2;
        myNetwork.setSize(vlen + ulen + 2);
		source = 0;
		target = 1;
		std::vector <int> cap11(vlen,0);
		std::vector <int> cap21(ulen,0);
		std::vector <int> cap12(vlen,0);
		std::vector <int> cap22(ulen,0);

        for(int j = 0 ; j < ulen; j++){
            for (int i = j ; i < vlen - ulen + j + 1; i++){
                cap11[i]+= (u[j] == '0');
                cap21[j]+= (v[i] == '0');
                cap12[i]+= (u[j] == '1');
                cap22[j]+= (v[i] == '1');
            }
        }

		for (int i = 0; i < vlen; i++) {
			if( (cap11[i]!=0)&&(v[i]=='?')){
                myNetwork.insertEdge(source, i + 2, cap11[i]);
            }
        }

		for (int i = 0; i < ulen; i++) {
			if(( cap21[i]!=0)&&(u[i]=='?')){
                myNetwork.insertEdge(source, vlen + i + 2, cap21[i]);
            }
        }
		for (int i = 0; i < vlen; i++) {
			if( (cap12[i]!=0)&&(v[i]=='?')){
                myNetwork.insertEdge(i + 2, target, cap12[i]);
            }
        }

		for (int i = 0; i < ulen; i++) {
			if(( cap22[i]!=0)&&(u[i]=='?')){
                myNetwork.insertEdge( vlen + i + 2, target, cap22[i]);
            }
        }

        for(int j = 0 ; j < ulen; j++){
            for (int i = j ; i < vlen - ulen + j + 1; i++){
                myNetwork.insertEdge( i + 2, vlen + j + 2, findCapacityBetweenVertices(v[i], u[j]));
                myNetwork.insertEdge(vlen + j + 2, i + 2, findCapacityBetweenVertices(u[j], v[i]));
            }
        }
	}
	void findAnswer(){

		size_t vlen = v.length();
		size_t ulen = u.length();
        int a = bfs();
		for(int i = 0; i < vlen ; i++){
            if(( distance[i + 2]!= INT_MAX)&&( v[i] == '?' )){
                v[i] = '0';
            }
		}
        for(int i = 0; i < ulen ; i++){
            if(( distance[i + vlen + 2] != INT_MAX)&&( u[i] == '?' )){
                u[i] = '0';
            }
		}
        for(int i = 0; i < vlen; i++){
            if (v[i] == '?')
                v[i] = '1';
        }
        for(int i = 0; i < ulen; i++){
            if (u[i] == '?')
                u[i] = '1';
        }

	}
	void printAnswer(){
        std::cout << v << std::endl;
        std::cout << u << std::endl;
	}
    void findDistance(){
        int a = v.length();
        int b = u.length();
        int ans = 0;
        for(int i = 0 ; i < b; i++){
            for (int j = i ; j < a - b + i + 1; j++){
                ans += (v[j] != u[i]);
            }
        }
        std::cout<< ans << std::endl;
	}
	void getAnswer(){
	    readNetwork();
	    //std::cout << " readNetwork "<< std::endl;
	    findMaxFlow();
	    //	    std::cout << " findMaxFlow "<< std::endl;
	    findAnswer();
	    	//    std::cout << " findAnswer "<< std::endl;
	    findDistance();
	    	  //  std::cout << "  findDistance "<< std::endl;
	    printAnswer();
	    	    //std::cout << "  printAnswer "<< std::endl;
	}
};

int main() {

	DinicAlgorithm myAl;
	myAl.getAnswer();
	return 0;
}
