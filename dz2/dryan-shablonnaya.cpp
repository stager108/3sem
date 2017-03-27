//#include "../stdafx.h"
//#include "Dinic.h"
//#include "../stdafx.h"
#include <vector>
#include <limits>

using uint = unsigned int;
#define private public

// Remember: vertices are unsigned, edges are signed!
template<typename vtype, typename etype, typename FlowType>
class Network {
private:

    // Forward declaration.
    class NetworkEdgeIterator;

    struct Edge;
    struct Vertex;
public:
    Network() = default;

    Network(uint n, vtype s, vtype t) {
        init(n, s, t);
    }

    void init(uint n_, vtype s_, vtype t_) {
        n = n_;
        s = s_;
        t = t_;
        vertices.resize(n);
        edges.clear();
    }

    vtype getSource() { return s; }
    vtype getTarget() { return t; }
    uint getVertexCount() { return n; }
    void setSource(vtype s_) { s = s_; }
    void setTarget(vtype t_) { t = t_; }
    void insertUndirectedEdge(vtype u, vtype v, FlowType c) {
        insertEdgeLocal(u, v, c, 0);
        insertEdgeLocal(v, u, c, 0);
    }
    void insertDirectedEdge(vtype u, vtype v, FlowType c) {
        insertEdgeLocal(u, v, c, 0);
        insertEdgeLocal(v, u, 0, 0);
    }
    void insertRemainEdge(vtype u, vtype v, FlowType c) {
        insertEdgeLocal(u, v, c, 0);
        insertEdgeLocal(v, u, -c, 0);
    }

    void resetDeleted() {
        for (auto&& v: vertices) {
            v.first = v.backup_first;
        }
    }

    // Returns iterator of vertex v edges.
    NetworkEdgeIterator begin(vtype v) { return NetworkEdgeIterator(*this, v, true); }
    NetworkEdgeIterator end() { return NetworkEdgeIterator(*this, 0, false); }
    NetworkEdgeIterator getEdgeByNum(etype num) { return num != -1? NetworkEdgeIterator(*this, num):end(); }
private:
    static const etype NullEdge = std::numeric_limits<etype>::max();
    uint n;
    vtype s, t;
    std::vector<Edge> edges;
    std::vector<Vertex> vertices; // First and last (according to time of addition) edges of vertex.

    struct Edge {
        bool deleted = false;
        vtype u, v;
        FlowType c, f;
        etype prev = NullEdge, next = NullEdge;
        Edge(vtype u, vtype v, FlowType c, FlowType f)
                : u(u), v(v), c(c), f(f) {}
    };

    struct Vertex {
        etype first = NullEdge;
        etype last = NullEdge;
        etype backup_first = NullEdge;
    };

    void insertEdgeLocal(vtype u, vtype v, FlowType c, FlowType f) {
        edges.push_back(Edge(u, v, c, f));
        // Set first if necessary.
        if (vertices[u].first == NullEdge) {
            vertices[u].backup_first = vertices[u].first = edges.size() - 1;
        } else {
            etype tmp = vertices[u].last;
            // Set previous and next.
            edges[tmp].next = edges.size() - 1;
            edges.back().prev = tmp;
        }
        // Set last.
        vertices[u].last = edges.size() - 1;
    }

    class NetworkEdgeIterator : public std::iterator<std::forward_iterator_tag, Edge> {
    public:
        NetworkEdgeIterator(Network& graph_, vtype v_, bool first) {
            graph = &graph_;
            if (first) {
                v = v_;
                e = graph->vertices[v].first;
                edge = &graph->edges[e];
            } else {
                e = graph->NullEdge;
            }
        }

        NetworkEdgeIterator(Network& graph_, etype num) {
            graph = &graph_;
            e = num;
            edge = &graph->edges[e];
        }

        bool hasNext() { return edge->next != graph->NullEdge; }

        bool operator==(const NetworkEdgeIterator& other) const { return graph == other.graph && e == other.e; }
        bool operator!=(const NetworkEdgeIterator& other) const { return !(*this == other); }

        // Set start pointer to the next edge or to NullEdge if no other left.
        void remove() {
            /*if (graph->vertices[v].first == graph->vertices[v].last)
                graph->vertices[v].last = edge->next;*/
            graph->vertices[v].first = edge->next;

        }

        NetworkEdgeIterator& operator++() {
            if (!hasNext()) {
                e = graph->NullEdge;
            } else {
                e = edge->next;
                edge = &graph->edges[e];
            }
            return *this;
        }

        bool isDeleted() { return edge->deleted; }
        void markAsDeleted() { edge->deleted = true; }
        void markReverseAsDeleted() { graph->edges[BackEdge(e)].deleted = true; }

        FlowType getFlow() { return edge->f; }
        int getCapacity() { return edge->c; }
        vtype getStart() { return edge->u; }
        vtype getFinish() { return edge->v; }
        etype getEdgeNumber() { return e; }
        FlowType getResidualCapacity() { return getCapacity() - getFlow(); }
        void pushFlow(FlowType f) {
            edge->f += f;
            graph->edges[BackEdge(e)].f -= f;
        }
        void pushCapacity(FlowType c) {
            edge->c += c;
            graph->edges[BackEdge(e)].c -= c;
        }

    private:
        vtype v; // Vertex that iterator belongs to.
        etype e;
        Network* graph;
        Edge* edge;

        etype BackEdge(etype e) {
            return e ^ 1;
        }

        bool isBackward(etype edge) { return (edge & 1) == 1; }

        friend class Network;
    };
};
#include <climits>
#include <queue>
#include <memory>
#include <algorithm>
#include <iostream>

template<typename vtype, typename etype, typename FlowType>
class EdmondsKarp {
public:
    size_t findMaxFlow(Network<vtype, etype, FlowType>* net_) {
        size_t maxFlow = 0;
        net = net_;
        pred.resize(net->getVertexCount());
        while (bfs()) {
            FlowType df = std::numeric_limits<FlowType>::max();
            for (auto it = net->getEdgeByNum(pred[net->getTarget()]);
                 it != net->end(); it = net->getEdgeByNum(pred[it.getStart()])) {
                df = std::min(df, it.getResidualCapacity());
                //std::cout << it.getEdgeNumber() <<std::endl;
            }
            for (auto it = net->getEdgeByNum(pred[net->getTarget()]);
                it != net->end(); it = net->getEdgeByNum(pred[it.getStart()])) {
                it.pushFlow(df);
            }

            maxFlow += df;
        }
        return maxFlow;
    }
private:
    std::vector<etype> pred;
    Network<vtype, etype, FlowType>* net;

    bool bfs() {
        std::queue<vtype> q;
        q.push(net->getSource());
        pred.assign(net->getVertexCount(), -1);
        while (!q.empty()) {
            vtype u = q.front();
            q.pop();
            for (auto it = net->begin(u); it != net->end(); ++it) {
                if (pred[it.getFinish()] == -1
                        && it.getFinish() != net->getSource()
                        && it.getResidualCapacity()>0) {
                    pred[it.getFinish()] = it.getEdgeNumber();
                    q.push(it.getFinish());
                }
            }
        }
        return pred[net->getTarget()] != -1;
    }
};

#include <iostream>

template<typename vtype, typename etype>
class MaximalFlow {
public:
    void find() {
        while (true) {
            Network<vtype, etype, int> net;
            uint n;
            vtype s, t;
            std::cin >> n;
            if (n == 0)
                break;
            int cnt;
            std::cin >> s >> t >> cnt;

            net.init(n, s - 1, t - 1);
            for (int j = 0; j < cnt; ++j) {
                vtype u, v;
                int c;
                std::cin >> u >> v >> c;
                net.insertUndirectedEdge(u - 1, v - 1, c);
            }

            EdmondsKarp<vtype, etype, int> dinic;
            std::cout << dinic.findMaxFlow(&net) << std::endl;
        }
    }
};

#include <stdlib.h>
#include <fstream>
//#define MY_CIN_REDIR

int main() {
    srand(static_cast<unsigned int>(time(0)));
#ifdef MY_CIN_REDIR
    std::ifstream ifs( "inFlow.txt", std::ios_base::in );
    std::streambuf* cinbuf = std::cin.rdbuf();
    std::cin.rdbuf( ifs.rdbuf() );
#endif

    MaximalFlow<uint, int> flow;
    flow.find();

#ifdef MY_CIN_REDIR
    std::cin.rdbuf( cinbuf );
#endif
    //std::cin.get();
    return 0;
}
