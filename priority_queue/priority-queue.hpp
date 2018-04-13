#ifndef PROIRITY_QUEUE_HPP
#define PRIORITY_QUEUE_HPP

#include <iostream>
#include <cstdlib>
#include <vector>
#include <list>
#include <deque>
#include <exception>

template <typename D, template<typename, class> class C = std::vector>
class priority_queue
{
private:
    struct elem
    {
        D val;
        uint32_t prior;
    };

    elem& _at(size_t ind)
    {
        if (ind >= cont.size())
            throw std::out_of_range("priority_queue::_at()");
        for(auto i = cont.begin(); i != cont.end(); i++, ind--)
            if(ind == 0)
                return *i;
    }

    //recover heap's property
    void _rise()
    {
        size_t ind = cont.size() - 1;
        while(ind != 0)
        {
            if(_at(ind).prior < _at((ind-1)/2).prior)// /2 == >>1
            {
                std::swap(_at(ind), _at((ind-1)/2));
                ind = (ind-1)/2;
            }
            else
                break;
        }
    }

    void _seed()
    {
        size_t ind = 0;
        while(2*ind+1 < cont.size())
        {
            size_t max_child_i = 2*ind+1;
            if (2*ind+2 < cont.size())
                if (_at(2*ind+2).prior < _at(2*ind+1).prior)
                    max_child_i = 2*ind+2;

            if(_at(ind).prior > _at(max_child_i).prior)
            {
                std::swap(_at(ind), _at(max_child_i));
                ind = max_child_i;
            }
            else
                break;
        }
    }

public:
    C<elem, std::allocator<elem> > cont;

    priority_queue() {}
    priority_queue(const priority_queue<D, C>& src) : cont(src.cont)
    {}

    bool empty() const
    { return cont.empty(); }

    size_t size() const
    { return cont.size(); }

    void push(D val, uint32_t priority)
    {
        elem e;
        e.val = val;
        e.prior = priority;

        cont.push_back(e);
        _rise();
    }

    void pop()
    {
        if(cont.empty())
            throw std::bad_alloc();
        std::swap(cont.front(), cont.back());
        cont.pop_back();
        _seed();
    }

    D peek() const
    { return cont.front().val; }

};

#endif

