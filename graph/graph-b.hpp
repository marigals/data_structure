#ifndef GRAPH_A_HPP
#define GRAPH_A_HPP

#include <vector>
#include <set>
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
        edge();
        edge(size_t i1, size_t i2, size_t l);
        bool operator < (const edge& e2) const;
        size_t contains(size_t vert) const; // returns other index or -1, if not contains vert
    };

private:
    std::vector<vertex> vert;
    std::set<edge> edges;

    size_t _get_len(size_t i1, size_t i2) const;

    void _add_edge(Graph& g, size_t i1, size_t i2, bool* visited = NULL);

    void _dfs_inv(std::vector<size_t>& res, size_t ind, bool* visited) const;

    void _find_euler_path(size_t start, std::vector<size_t>& res);

public:
    Graph();
    Graph(const std::vector<std::vector<size_t> >& matr);

    void set_values(std::vector<int> values);

    void print();
    //LABA
    std::vector<size_t> tarjan(size_t start) const;

    Graph transposed() const;

    std::vector< std::vector<size_t> > kosaraiju(size_t start) const;

    std::vector<size_t> find_euler_path(size_t start) const;

    void modify_edge(std::set<edge>::iterator& i, size_t new_len = 0, bool remove_from_linked = false);

    std::vector<edge> fleury (size_t start) const;
};

#endif

