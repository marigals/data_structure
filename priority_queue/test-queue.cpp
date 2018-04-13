#include <iostream>
#include <cstdlib>
#include <vector>
#include <list>
#include <deque>
#include <exception>

#include "priority-queue.hpp"

int main()
{
    priority_queue<float, std::list> pq;

    pq.push(9.432, 4);

    pq.push(150.3, 2);

    std::cout << pq.peek() << std::endl;
    pq.pop();
    std::cout << pq.peek() << std::endl;
    pq.pop();
    try
    {
        pq.pop();
    }
    catch (std::bad_alloc e)
    {
        std::cerr << "Error! " << e.what() << std::endl;
    }
    std::cout << "I'm okay" << std::endl;

    return 0;
}

