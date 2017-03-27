#pragma once
#include <iostream>
#include <thread>
#include <atomic>
#include <array>
#include <vector>
#include <stack>


class Network {


    static const int MYNULL = -1;
    struct MyEdge{
       int from;
       int to;
       int capacity;
       int flow = 0;
       int previous_edge = MYNULL;
       int next_edge = MYNULL;

       MyEdge(int f, int t, int c){
           from = f;
           to = t;
           capacity = c;
       }
    };

    struct MyVertex{
        int first_edge = MYNULL;
        int last_edge = MYNULL;
    };

    std::vector <MyEdge> allEdges;
    std::vector <MyVertex> allVertexes;
    std::vector <bool> isGood;
    size_t nw_size = 0;
    size_t edges_count = 0;
    size_t source;
    size_t target;


public:

    class NetworkEdgesIterator{
        size_t EdgeNum;
        Network* myNetwork;
        int position;
        int vertex;
    public:
        //friend class Network;
        NetworkEdgesIterator(){}

        NetworkEdgesIterator(Network* newNetwork){
            myNetwork = newNetwork;
        }
        NetworkEdgesIterator(Network* newNetwork, int new_vertex, int new_position){
            myNetwork = newNetwork;
            position = new_position;
            vertex = new_vertex;
        }

        void pushFlow(int flow_inc){
            myNetwork->allEdges[position].flow += flow_inc;
            myNetwork->allEdges[myNetwork->backEdge(position)].flow -= flow_inc;
        }

        void makeRemark(){
            myNetwork->isGood[position] = false;
        }

        MyEdge getEdge() const{
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
        bool isNull(){
            return (position == MYNULL);
        }
        NetworkEdgesIterator& operator ++ (){
            position = myNetwork->allEdges[position].next_edge;
            return *this;
        }
        NetworkEdgesIterator& operator ++ (int){
            position = myNetwork->allEdges[position].next_edge;
            return *this;
        }
        NetworkEdgesIterator& operator -- (){
            position = myNetwork->allEdges[position].previous_edge;
            return *this;
        }

        NetworkEdgesIterator& operator -- (int){
            position = myNetwork->allEdges[position].previous_edge;
            return *this;
        }

        bool operator == (const NetworkEdgesIterator &other){
            return (position == other.position);
        }
        bool operator != (const NetworkEdgesIterator &other){
            return (position != other.position);
        }

        bool isBackEdge(){
            return position % 2;
        }
    };

    Network(){}

    Network( size_t network_size){
        nw_size = network_size;
        allVertexes.resize( nw_size );
    }

    Network( size_t network_size, size_t s, size_t t){
        nw_size = network_size;
        source = s;
        target = t;
        allVertexes.resize( nw_size );
    }

    void insertEdge (size_t new_from, size_t new_to, int new_capacity) {
        subInsertEdge(new_from, new_to, new_capacity);
        subInsertEdge(new_to, new_from, -new_capacity);
    }


    NetworkEdgesIterator begin (size_t vertex_number) {
        return NetworkEdgesIterator(this, vertex_number, allVertexes[vertex_number].first_edge);
    }
    NetworkEdgesIterator end (size_t vertex_number){
        return NetworkEdgesIterator(this, vertex_number, allVertexes[vertex_number].last_edge);
    }

    void setSize(int network_size){
        nw_size = network_size;
        allVertexes.resize( nw_size );
    }
    ~Network(){}

private:

    void subInsertEdge(size_t new_from, size_t new_to, int new_capacity) {

        MyEdge NewEdge(new_from, new_to, new_capacity);
        int tmp = allVertexes[new_from].first_edge;

        if(allVertexes[new_from].first_edge == MYNULL){
            allVertexes[new_from].last_edge = edges_count;
        }

        allVertexes[new_from].first_edge = edges_count;
        NewEdge.next_edge = tmp;

        if(tmp != MYNULL){
            allEdges[tmp].previous_edge = edges_count;
        }

        allEdges.push_back( NewEdge );
        isGood.push_back(true);
        edges_count++;
    }

    size_t backEdge( size_t number){
        return number ^ 1;
    }

};
