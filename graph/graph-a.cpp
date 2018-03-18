#include <iostream>
#include <vector>
#include <cstdlib>
#include <queue>
#include <algorithm>

#include "graph-a.hpp"


Graph::edge::edge(size_t i1, size_t i2, size_t l) : len(l)
{
    //if (i_low > i_high) std::swap(i_low, i_high);
    i_low = (i1 < i2) ? i1 : i2;
    i_high = (i1 > i2) ? i1 : i2;
}

bool Graph::edge::operator < (const edge& e2) const
{ return len < e2.len; }



size_t Graph::get_len(size_t i1, size_t i2) const
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
            if(i < j)
                edges.push_back( edge(i, j, matr.at(i).at(j)) );
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
            std::cout << "(" << *j << ":" << get_len(i,*j) << ") ";
        std::cout << std::endl;
    }
}



//LABA
int Graph::search_deep(int value, size_t start) //size_t
{
    bool visited[vert.size()];
    for(size_t i = 0; i < vert.size(); i++)
        visited[i] = false;

    std::vector<size_t> stack;
    stack.push_back(start);

    while(!stack.empty())
    {
        size_t temp = stack.back();
        stack.pop_back(); //pop_back returns void => read value before

        visited[temp] = true;

        //if found
        if(vert.at(temp).val == value)
            return temp;

        //for(int i = 0; i < graph.at(temp).linked.size(); i++)
        /*for(auto i = graph.at(temp).linked.begin(); i != graph.at(temp).linked.end(); i++)
            if (!visited[*i]) ...*/

        //push all linked unvisited vertexes to stack
        for(auto i = vert.at(temp).linked.begin(); i != vert.at(temp).linked.end(); i++) //auto i
            if(!visited[*i])
                stack.push_back(*i);
    }

    return -1;
}

//same as deep except queue
int Graph::search_wide(int value, size_t start) //size_t
{
    bool visited[vert.size()];
    for(size_t i = 0; i < vert.size(); i++)
        visited[i] = false;

    std::queue<size_t> queue;
    queue.push(start);

    while(!queue.empty())
    {
        size_t temp = queue.front();
        queue.pop();
        visited[temp] = true;
        if(vert.at(temp).val == value)
            return temp;

        for(auto i = vert.at(temp).linked.begin(); i != vert.at(temp).linked.end(); i++) //auto i
            if(!visited[*i])
                queue.push(*i);
    }

    return -1;
}

std::vector<size_t> Graph::dijkstra(size_t start)
{
    bool* visited = new bool[vert.size()];
    for(size_t i = 0; i < vert.size(); i++)
        visited[i] = false;

    //length of paths
    std::vector<size_t> res(vert.size());
    for(size_t i = 0; i < vert.size(); i++)
        res[i] = -1;
    res[start] = 0;

    //while not all vertexes checked
    while(1)
    {
        /*for(int i = 0; i < graph.size(); i++)
            std::cout << visited[i] << " ";
        std::cout << std::endl;*/
        bool all_visited = true;
        for(size_t i = 0; i < vert.size(); i++)
            //if any unvisited vertex - not all visited
            if(!visited[i])
            {
                all_visited = false;
                break; //to avoid ckecking of rest array values
            }
        if(all_visited)
            break;


        //find closest to start
        size_t min_i = 0;
        while (visited[min_i])
            min_i++;

        for(size_t i = 0; i < vert.size(); i++)
        {
            if (res[i] < res[min_i] && !visited[i])
                min_i = i;
        }
        visited[min_i] = true;

        //for each linked try to build path through [min_i]. if shorter - remember it
        //const vector<edge>& edges = graph.at(min_i).linked;
        for(auto i = vert.at(min_i).linked.begin(); i != vert.at(min_i).linked.end(); i++)
            res[*i] = std::min(get_len(min_i, *i) + res[min_i], res[*i]);
    }

    delete[] visited;
    return res;
}

std::vector<std::vector<size_t> > Graph::floyd_warshall()
{
    std::vector<std::vector<size_t> > res(vert.size());
    for(size_t i = 0; i < vert.size(); i++)
    {
        res.at(i).resize(vert.size());
        for(size_t j = 0; j < vert.size(); j++)
            res.at(i).at(j) = get_len(i, j);
    }

    for(size_t k = 0; k < vert.size(); k++)
        for(size_t j = 0; j < vert.size(); j++)
            for(size_t i = 0; i < vert.size(); i++)
                if(res.at(i).at(k) != (size_t)-1 && res.at(k).at(j) != (size_t)-1)
                    res.at(i).at(j) = std::min(res.at(i).at(j), res.at(i).at(k) + res.at(k).at(j));
    return res;
}

void Graph::_add_edge(Graph& g, size_t i1, size_t i2, bool* visited)
{
    g.vert.at(i1).linked.push_back(i2);
    g.vert.at(i2).linked.push_back(i1);
    g.edges.push_back(edge(i1, i2, get_len(i1, i2)));
    if (visited != NULL)
    {
        visited[i1] = true;
        visited[i2] = true;
    }
}

Graph Graph::prim(size_t start)
{
    Graph res;
    res.vert.resize(vert.size());

    bool* visited = new bool[vert.size()];
    for(size_t i = 0; i < vert.size(); i++)
        visited[i] = false;
    visited[start] = true;

    size_t min_i = vert.at(start).linked.front();

    for(auto i = vert.at(start).linked.begin(); i != vert.at(start).linked.end(); i++)
        if(get_len(start, *i) < get_len(start, min_i))
            min_i = *i;

    _add_edge(res, start, min_i, visited);

    while (1)
    {
        bool all_visited = true;
        for(size_t i = 0; i < vert.size(); i++)
            //if any unvisited vertex - not all visited
            if(!visited[i])
            {
                all_visited = false;
                break; //to avoid ckecking of rest array values
            }
        if(all_visited)
            break;

        edge min(-1,-1,-1);
        //for each vertex that already in new graph
        for(size_t i = 0; i < vert.size(); i++)
        {
            if(!visited[i])
                continue;

            //for each vertex that isn't in new graph yet
            for(auto j = vert.at(i).linked.begin(); j != vert.at(i).linked.end(); j++)
            {
                if (visited[*j])
                    continue;

                //find min edge from current new graph to other vertexes
                if (get_len(i, *j) < min.len)
                    min = edge(i, *j, get_len(i, *j));
            }
        }

        //add min edge (so vertex) to new graph
        _add_edge(res, min.i_low, min.i_high, visited);
    }

    delete[] visited;
    return res;
}

Graph Graph::kruskal()
{
    std::vector<edge> edges_copy = edges;
    std::sort(edges_copy.begin(), edges_copy.end());

    Graph res;
    res.vert.resize(vert.size());

    std::vector<size_t> d(vert.size());
    for(size_t i = 0; i < d.size(); i++)
        d.at(i) = i;

    for(auto i = edges_copy.begin(); i != edges_copy.end(); i++)
        if(d.at(i->i_low) != d.at(i->i_high))
        {
            _add_edge(res, i->i_low, i->i_high);
            size_t temp = d.at(i->i_low);
            for(size_t j = 0; j < d.size(); j++)
                if(d.at(j) == temp)
                    d.at(j) = d.at(i->i_high);
        }

    return res;
}

