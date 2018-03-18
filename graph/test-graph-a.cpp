#include <iostream>
#include <vector>
#include <cstdlib>

#include "graph-a.hpp"

#define N (5)

int main()
{
    //paths from [i] vertex t0 [j]
    std::vector<std::vector<size_t> > matr =  { {0, 2, 1, 3, 0, 6, 0},
                                                {2, 0, 0, 1, 3, 0, 0},
                                                {1, 0, 0, 2, 0, 3, 0},
                                                {3, 1, 2, 0, 0, 0, 1},
                                                {0, 3, 0, 0, 0, 0, 4},
                                                {6, 0, 3, 0, 0, 0, 7},
                                                {0, 0, 0, 1, 4, 7, 0} };
/*    std::vector<std::vector<size_t> > matr =  { {0,0,3,0,1},
                                                {0,0,0,2,5},
                                                {3,0,0,5,3},
                                                {0,2,5,0,0},
                                                {1,5,3,0,0} };*/
    std::vector<int> values =  {1,3,5,7,9, 2, 6};

    Graph g(matr);
    g.set_values(values);
    g.print();
    std::cout << std::endl;

    std::cout << "searching 4: " << g.search_deep(4, 0) << std::endl;
    std::cout << "searching 5: " << g.search_wide(5, 0) << std::endl;

    std::cout << "dijkstra from 2 vertex: " << std::endl;
    auto d = g.dijkstra(2);
    for(size_t i = 0; i < d.size(); i++)
        std::cout << i << ": " << d.at(i) << std::endl;

    std::cout << "floyd_warshall: " << std::endl;
    auto fw = g.floyd_warshall();

    std::cout << "  | ";
    for(size_t i = 0; i < fw.size(); i++)
        std::cout << i << " ";
    std::cout << std::endl;

    for(size_t i = 0; i < fw.size(); i++)
    {
        std::cout << i << " | ";
        for(size_t j = 0; j < fw.at(i).size(); j++)
            std::cout << fw.at(i).at(j) << " ";
        std::cout << std::endl;
    }

    Graph p = g.prim(4);
    std::cout << std::endl << "prim from 4:" << std::endl;
    p.print();

    p = g.kruskal();
    std::cout << std::endl << "kruskal from 4:" << std::endl;
    p.print();
    return 0;
}

