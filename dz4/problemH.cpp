#include <stdio.h>
#include <vector>
#include <iostream>
#include <limits.h>
class CMatrixGraph {

    int CountVertexes;
    std::vector < std::vector < int > > Matrix;

public:

    CMatrixGraph() {
        CountVertexes = 0;
        Matrix.clear();
    }

    CMatrixGraph( int n ) {
        CountVertexes = n;
        Matrix.clear();
        Matrix.resize(n);
        for(int i = 0; i < CountVertexes; i++) {
             Matrix[i].resize( n , 0 );
         }
    }
    void CMatrixGraphResize( int n ) {
        CountVertexes = n;
        Matrix.clear();
        Matrix.resize(n);
        for(int i = 0; i < CountVertexes; i++) {
             Matrix[i].resize( n , 0 );
         }
    }

	void AddEdge(int from, int to, int weight) {
	    Matrix[from][to] = weight;
	}
	int getEdge(int from, int to) {
	    return Matrix[from][to];
	}

	int VertexCount() const {
	    return CountVertexes;
	}

    std::vector < int > GetNextVertexes(int vertex) const {
         std::vector <int> Vertexes;
         Vertexes.clear();
         for(int i = 0; i < CountVertexes; i++) {
            if(Matrix[vertex][i] == 1) {
                Vertexes.push_back(i);
            }
         }
         return Vertexes;
    }

    std::vector < int > GetPrevVertexes(int vertex) const {
        std::vector <int> Vertexes;
         Vertexes.clear();
         for( int i = 0; i < CountVertexes; i++) {
            if(Matrix[i][vertex] == 1) {
                Vertexes.push_back(i);
            }
         }
         return Vertexes;

    }

    ~CMatrixGraph() {
        /* for( int i = 0; i < CountVertexes; i++ ) {
            Matrix[i].clear();
         }*/
         Matrix.clear();
    }
};

class Hungary {
public:
    Hungary(){}
    ~Hungary(){}
    void findAnswer() {
        std::vector<int> minv(m + 1, INT_MAX);
        std::vector<char> used(m + 1, false);
        int i0, delta, minimumColumn , freeColumn;
        strochka.resize(n + 1);
        column.resize(m + 1);
        pairs.resize(m + 1); // Matching.
        predecessor.resize(m + 1); // Array of predecessors.
        for (int i = 1; i <= n; ++i) {
            pairs[0] = i;
            freeColumn = 0;
            minv.assign(m + 1, INT_MAX);
            used.assign(m + 1, false);
            used[freeColumn] = true;
            i0 = pairs[freeColumn];
            delta = INT_MAX;
            for (int j = 1; j <= m; ++j){
                if (!used[j]) {
                    int cur = myGraph.getEdge(i0,j) - strochka[i0] - column[j];
                    if (cur < minv[j]) {
                        predecessor[j] = freeColumn;
                        minv[j] = cur;
                    }
                    if (minv[j] < delta) {
                        minimumColumn = j;
                        delta = minv[j];
                    }
                }
            }
            for (int j = 0; j <= m; ++j){
                if (used[j]){
                    strochka[pairs[j]] += delta;
                    column[j] -= delta;
                }
                else{
                    minv[j] -= delta;
                }
            }
            freeColumn = minimumColumn;
            while (pairs[freeColumn] != 0){
                used[freeColumn] = true;
                i0 = pairs[freeColumn];
                delta = INT_MAX;
                for (int j = 1; j <= m; ++j){
                    if (!used[j]) {
                        int cur = myGraph.getEdge(i0,j) - strochka[i0] - column[j];
                        if (cur < minv[j]) {
                            predecessor[j] = freeColumn;
                            minv[j] = cur;
                        }
                        if (minv[j] < delta) {
                            minimumColumn = j;
                            delta = minv[j];
                        }
                    }
                }
                for (int j = 0; j <= m; ++j){
                    if (used[j]){
                        strochka[pairs[j]] += delta;
                        column[j] -= delta;
                    }
                    else{
                        minv[j] -= delta;
                    }
                }
                freeColumn = minimumColumn;
            }
            int minimumColumn = predecessor[freeColumn];
            pairs[freeColumn] = pairs[minimumColumn];
            freeColumn = minimumColumn;
            while (freeColumn != 0) {
                int minimumColumn = predecessor[freeColumn];
                pairs[freeColumn] = pairs[minimumColumn];
                freeColumn = minimumColumn;
            }
        }
    }

private:
    int n, m;
    std::vector<std::vector<int>> a;
    CMatrixGraph myGraph;
    void recoverAnswer() {
        int cost = -column[0];
    }
public:
    void solve() {
        readNetwork();
        findAnswer();
        printAnswer();
    }
private:

    std::vector<int> strochka, column, pairs, predecessor;
    void readNetwork() {
        std::cin >> n;
        m = n;
        myGraph.CMatrixGraphResize(n+1);
        int w;
        for (int i = 0; i < n; i++) {
            for(int j = 0; j < n; j ++){
                scanf("%d",&w);
                myGraph.AddEdge(i+1, j+1, w);
            }
        }
    }

    void printAnswer() {
        int cost = -column[0];
        std::cout << cost << std::endl;
        for (int i = 1; i <= m; ++i) {
            printf("%d %d\n",pairs[i],i );
        }
    }
};
int main(){
    Hungary myAl;
    myAl.solve();
    return 0;
}
