
#include <map>
#include <memory>
#include <set>

template<typename keyT, typename valueT>
struct node {
    keyT key;
    valueT value;
};

template<typename keyT, typename nodeT, typename edgeT>
struct edge {
    std::shared_ptr<node<keyT, nodeT>> source;
    std::shared_ptr<node<keyT, nodeT>> target;
    edgeT value;
};

template<typename keyT, typename nodeT, typename edgeT = nodeT>
struct graph {
    typedef node<keyT, nodeT> node_type;
    typedef std::shared_ptr<node_type> node_pointer;
    typedef edge<keyT, edgeT, nodeT> edge_type;
    typedef std::shared_ptr<edge_type> edge_pointer;

    std::map<keyT, node_pointer> nodes;
    std::map<keyT, std::set<edge_pointer>> outbound;
    std::map<keyT, std::set<edge_pointer>> inbound;
};