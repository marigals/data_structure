#ifndef GRAPH_A_HPP
#define GRAPH_A_HPP

#include <vector>
#include <cstdlib>

class Graph
{
public:
    struct vertex
    {
        int val;
        std::vector<size_t> linked;
    };

    struct edge
    {
        size_t len;
        size_t i_low, i_high;

        //1st vertex is always less than 2nd
        edge(size_t i1, size_t i2, size_t l);
        bool operator < (const edge& e2) const;
    };

private:
    std::vector<vertex> vert;
    std::vector<edge> edges;

    size_t get_len(size_t i1, size_t i2) const;
    void _add_edge(Graph& g, size_t i1, size_t i2, bool* visited = NULL);

public:
    Graph();
    Graph(const std::vector<std::vector<size_t> >& matr);

    void set_values(std::vector<int> values);

    void print();
    //LABA
    int search_deep(int value, size_t start);//size_t

    //same as deep except queue
    int search_wide(int value, size_t start); //size_t

    std::vector<size_t> dijkstra(size_t start);

    std::vector<std::vector<size_t> > floyd_warshall();

    Graph prim(size_t start= 0);

    Graph kruskal();
};

#endif

