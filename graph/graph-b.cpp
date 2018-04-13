#include <iostream>
#include <vector>
#include <cstdlib>
#include <queue>
#include <set>
#include <algorithm>

#include "graph-b.hpp"


Graph::edge::edge() : len(0), i_low(-1), i_high(-1) {}

Graph::edge::edge(size_t i1, size_t i2, size_t l) : len(l)
{
    //if (i_low > i_high) std::swap(i_low, i_high);
    i_low = (i1 < i2) ? i1 : i2;
    i_high = (i1 > i2) ? i1 : i2;
}

bool Graph::edge::operator < (const edge& e2) const
{
    if (i_low < e2.i_low)
        return true;
    if (i_low > e2.i_low)
        return false;
    return i_high < e2.i_high;
}

size_t Graph::edge::contains(size_t vert) const
{
    size_t other = -1;
    if(i_low == vert)
        other = i_high;
    if(i_high == vert)
        other = i_low;
    return other;
}

size_t Graph::_get_len(size_t i1, size_t i2) const
{
    if(i1 == i2)
        return 0;

    if(i1 > i2)
        std::swap(i1, i2);

    for(auto i : edges)
        if(i.i_low == i1 && i.i_high == i2)
            return i.len;
    return -1;
}

Graph::Graph() {}
Graph::Graph(const std::vector<std::vector<size_t> >& matr)
{
    vert.resize(matr.size());
    for(size_t i = 0; i < matr.size(); i++)
        for(size_t j = 0; j < matr.at(i).size(); j++)
        {
            if(matr.at(i).at(j) == 0)
                continue;
            vert.at(i).linked.push_back(j);
            //if(i < j)
            edges.insert( edge(i, j, matr.at(i).at(j)) );
        }
}

void Graph::set_values(std::vector<int> values)
{
    std::cerr << "val... ";
    for(size_t i = 0; i < vert.size(); i++)
        vert.at(i).val = values.at(i);
    std::cerr << "ok!" << std::endl;
}

void Graph::print()
{
    for(size_t i = 0; i < vert.size(); i++)
    {
        std::cout << i << ":  ";
        for(auto j = vert.at(i).linked.begin(); j != vert.at(i).linked.end(); j++)
            std::cout << "(" << *j << ":" << _get_len(i,*j) << ") ";
        std::cout << std::endl;
    }
}



//LABA
void Graph::_dfs_inv(std::vector<size_t>& res, size_t ind, bool* visited) const
{
    visited[ind] = true;

    for(size_t i : vert.at(ind).linked)
        if(!visited[i] && _get_len(ind, i) != 0)
            _dfs_inv(res, i, visited);
    res.push_back(ind);
}

std::vector<size_t> Graph::tarjan(size_t start) const
{
    bool* visited = new bool[vert.size()];
    for(size_t i = 0; i < vert.size(); i++)
        visited[i] = false;

    std::vector<size_t> res(0);

    _dfs_inv(res, start, visited);

    for(size_t i = 0; i < vert.size(); i++)
        if(!visited[i])
            _dfs_inv(res, i, visited);

    delete[] visited;
    return res;
}

Graph Graph::transposed() const
{
    Graph res = *this;

    res.vert = std::vector<vertex>(vert.size());
    //std::vector<vertex> newv = vector<vertex>(vert.size());
    //std::vector<vertex> newv(vert.size());
    for(size_t i = 0; i < vert.size(); i++)
        for(size_t j : vert.at(i).linked)
            res.vert.at(j).linked.push_back(i);
    return res;
}

std::vector< std::vector<size_t> > Graph::kosaraiju(size_t start) const
{
    bool* visited = new bool[vert.size()];
    for(size_t i = 0; i < vert.size(); i++)
        visited[i] = false;

    //visited[start] = true;
    std::vector<size_t> stack;
    std::vector< std::vector<size_t> > res;

    Graph transp = transposed();
    transp._dfs_inv(stack, start, visited);
    for(size_t i = 0; i < transp.vert.size(); i++)
        if(!visited[i])
            transp._dfs_inv(stack, i, visited);

    for(size_t i = 0; i < vert.size(); i++)
        visited[i] = false;
    while(!stack.empty())
    {
        size_t v = stack.back();
        stack.pop_back();
        if(!visited[v])
        {
            res.push_back( std::vector<size_t>() );
            _dfs_inv(res.back(), v, visited);
        }
    }
    delete[] visited;
    return res;
}

//std::vector< size_t> fleury
std::vector<size_t> Graph::find_euler_path(size_t start) const
{
    Graph temp = *this;
    std::vector<size_t> res(0);
    temp._find_euler_path(start, res);
    return res;
}


void Graph::_find_euler_path(size_t start, std::vector<size_t>& res)
{
    for(auto i = edges.begin(); i != edges.end(); i++)
    {
        if (i->len == 0)
            continue;

        //save 2nd vertex
        size_t other = i->contains(start);

        if (other == -1)
            continue;

        //can't edit element in std::set, so delete and add updated edge
        modify_edge(i);
        /*edge temp = *i;
        edges.erase(i);
        temp.len = 0;
        i = edges.insert(temp).first; //insert returns std::pair<iterator, bool>, .first is iterator of the inserted element*/
        //i->len = 0; //_delete_edge(start, other);
        _find_euler_path(other, res);
    }
    res.push_back(start);
}

void Graph::modify_edge(std::set<edge>::iterator& i, size_t new_len, bool remove_from_linked)
{
        edge temp = *i;
        edges.erase(i);
        temp.len = new_len;
        i = edges.insert(temp).first;

        if (remove_from_linked)
        {
            for(auto j = vert.at(temp.i_low).linked.begin(); j != vert.at(temp.i_low).linked.end(); j++)
                if(*j == temp.i_high)
                {
                    vert.at(temp.i_low).linked.erase(j);
                    break;
                }

            for(auto j = vert.at(temp.i_high).linked.begin(); j != vert.at(temp.i_high).linked.end(); j++)
                    if(*j == temp.i_low)
                    {
                        vert.at(temp.i_high).linked.erase(j);
                        break;
                    }
        }
}

std::vector<Graph::edge> Graph::fleury (size_t start) const
{
    Graph temp = *this;
    std::vector<edge> res(0);

    size_t v = start;
    size_t edges_count = edges.size();
    for (size_t k = 0; k < edges_count; k++)
    {
        for(auto i = temp.edges.begin(); i != temp.edges.end(); i++)
        {
            //std::cerr << "edge:
            if(i->len == 0)
                continue;

            size_t other = i->contains(v);

            if(other == -1)
                continue;

            edge copy = *i;
            if(temp.vert.at(v).linked.size() != 1)
            {
                size_t cur_strong_count = temp.kosaraiju(v).size();

                size_t edge_len = i->len;

                temp.modify_edge(i);

                //temp.edges.erase(i);
                //copy.len = 0;
              //  i = temp.edges.insert(copy).first;

                //if removed edge was a bridge
                if(temp.kosaraiju(v).size() > cur_strong_count)
                {
                    //put it back
                    temp.modify_edge(i,edge_len);
                    //temp.edges.erase(i);
                    //copy.len = edge_len;
                    //i = temp.edges.insert(copy).first;
                    continue;
                }
            }

            res.push_back(copy);
            temp.modify_edge(i, 0, true);
            /*temp.edges.erase(*i);
            copy.len = 0;
            i = temp.edges.insert(copy).first;*/
            v = other;
            break;
        }
    }
    return res;
}




