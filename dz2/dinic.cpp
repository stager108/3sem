
#include <iostream>
#include <thread>
#include <atomic>
#include <array>
#include <vector>
#include <queue>
#include "network.h"


class DinicAlgorihm{
    std::vector <int> distance;
    size_t vertex_count;
    size_t source;
    size_t target;
    Network myNetwork;
public:
    DinicAlgorihm(){}
    ~DinicAlgorihm(){}
    bool bfs(){
        std:: queue <int> myQueue;
        distance.resize(vertex_count, INT_MAX);
        distance[source] = 0;
        myQueue.push(source);
        while (!myQueue.empty()){

            int u = myQueue.back();
            myQueue.pop();
            Network::NetworkEdgesIterator myIt = myNetwork.begin(u);
            while ( !myIt.isNull()) {
                if (( myIt.getResidualCapacity() > 0) && (distance[ myIt.getFinish()] == INT_MAX)) {
                    distance[myIt.getFinish()] = distance[u] + 1;
                    myQueue.push(myIt.getFinish());
                }
                myIt++;
            }
        }
        return distance[target] != INT_MAX;
    }

    int dfs(int u, int minC){
       if ((u == target)||(minC == 0)){
           return minC;
       }
       int delta;
       Network::NetworkEdgesIterator myIt = myNetwork.begin(u);
       while(!myIt.isNull()){
          if (distance[myIt.getFinish()] == distance[u] + 1){
              delta = dfs(myIt.getFinish(), std::min(minC,myIt.getResidualCapacity()));
              if (delta != 0){
                  myIt.pushFlow(delta); // �������� ����� �� ���� dfs
                  return delta;
              }
          }
          if( myIt.getResidualCapacity()== 0){
              myIt.makeRemark();
          }
          myIt++;
       }
       return 0;
    }
    int findMaxFlow(int n){

        size_t s, t, cnt;
        int a, b, c;
        std::cin >> s >> t >> cnt ;
        std::cout << "i read \n";
        myNetwork.setSize(n);
        source = s;
        target = t;

        for(int i = 0; i < cnt; i++){
            std::cin >> a >> b >> c;
            std::cout << "i read \n";
            myNetwork.insertEdge(a,b,c);
        }

        int maxFlow = 0;
        int flow;
        while( bfs() ){
            flow = dfs(source, INT_MAX);
            while (flow != 0){
                maxFlow += flow;
                flow = dfs(source, INT_MAX);
            }
        }

      return maxFlow;
    }

};

int main(){

    int n = -1;
    std::cin >> n;
    DinicAlgorihm myAl;
    while( n != 0){
        myAl.findMaxFlow(n);
        std::cin >> n;
    }


    return 0;
}
