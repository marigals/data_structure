#ifndef HASH_MAP
#define HASH_MAP

#include <iostream>
#include <utility>
#include <vector>
#include <list>
#include <cmath>
#include <functional>
#include <cstdlib>

template <class K, class V, /*template <typename> class H =*/ class H = std::hash<K> >
class hash_map
{
    typedef std::pair</*const */K, V> _elem;
    private:
/*        struct _elem
        {
            K key;
            V val;

            _elem() {}
            _elem(K k, V v) : key(k), val(v) {}
        };*/
        
        std::vector< std::list<_elem> > _hash_table;

        size_t _elem_count = 0;
        //size_t _list_count; //_hash_table.size()

        double _max_load = 2.0;

        H _h;

        void _rehash(size_t list_count)
        {
            std::cerr << "rehashing from " << _hash_table.size() << " to " << list_count;
            std::cerr << ", elements: " << _elem_count << std::endl; 
            if(list_count < 10)
                list_count = 10;

            if(list_count < _hash_table.size())
                if((double)_elem_count / list_count > get_max_load())
                {
                    list_count = ceil((double)_elem_count / get_max_load());
                    std::cerr << "can't use this value, resizing to " << list_count << std::endl;
                }
            

            std::vector< std::list<_elem> > old = _hash_table;

            clear();
            _hash_table.resize(list_count);

            for(size_t i = 0; i < old.size(); i++)
                for(auto j = old.at(i).begin(); j != old.at(i).end(); j++)
                {
                    insert(j->first, j->second);
                }
        }

    public:
        hash_map() : _hash_table(10) {}
        
        hash_map(const hash_map& src):  _hash_table(src._hash_table), 
                                        _elem_count(src._elem_count),
                                        _max_load(src._max_load)
        {}

        void clear()
        {
            /*for(size_t i = 0; i < _hash_table.size(); i++)
                for(auto j = _hash_table.at(i).begin(); j != _hash_table.at(i).end(); j++)
                    j->deleted = true;*/
            _hash_table.clear();
            _hash_table.resize(10);

            _elem_count = 0;
        }

        void insert(K key, V val)
        {
            if(_hash_table.size() == 0)
                throw std::out_of_range("insert error");
            
            size_t hash = _h(key) % _hash_table.size();

/*            bool added = false;
            for(auto i = _hash_table.at(hash).begin(); i != _hash_table.at(hash).end(); i++)
            {
                if(i->deleted)
                {
                    i->first = key;
                    i->second = val;
                    
                    added = true;
                    break;
                }
            }

            if(!added)*/
    
            for(auto j = _hash_table.at(hash).begin(); j != _hash_table.at(hash).end(); j++)
                if(j->first == key)
                    throw std::invalid_argument("adding existing key");



            _hash_table.at(hash).push_back(_elem(key, val));
            
            _elem_count++;

            if(get_current_load() > get_max_load())
                _rehash(2 * _hash_table.size() + 1);
        }

        V& operator [] (K key)
        {
            if(_hash_table.size() == 0)
                throw std::out_of_range("operator [] error");
            
            size_t hash = _h(key) % _hash_table.size();
            for(auto i = _hash_table.at(hash).begin(); i != _hash_table.at(hash).end(); i++)
                if(i->first == key)
                    return i->second;
            throw std::out_of_range("operator [] error");
        }

        V& operator [] (K key) const
        {
            if(_hash_table.size() == 0)
                throw std::out_of_range("operator [] error");
            
            size_t hash = _h(key) % _hash_table.size();
            for(auto i = _hash_table.at(hash).begin(); i != _hash_table.at(hash).end(); i++)
                if(i->first == key)
                    return i->second;
            throw std::out_of_range("operator [] error");
        }

        void erase(K key)
        {
            if(_hash_table.size() == 0)
                throw std::out_of_range("erase error");

            size_t hash = _h(key) % _hash_table.size();
            for(auto i = _hash_table.at(hash).begin(); i != _hash_table.at(hash).end(); i++)
                if(i->first == key)
                {
                    _hash_table.at(hash).erase(i);
                    _elem_count--;

                    if(get_current_load() < (get_max_load() / 3))
                        _rehash(_hash_table.size() / 2);

                    return;
                }
            throw std::out_of_range("erase error");
        }
        
        size_t get_elem_count() const
        { return _elem_count; }

        double get_max_load() const
        { return _max_load; }

        void set_max_load(double max_load)
        { 
            if(max_load < 1)
                throw std::out_of_range("incorrect max_load (< 1)");
            _max_load = max_load; 
        }

        double get_current_load() const
        { 
            if(_hash_table.size() == 0)
                return 0;
            return (double)_elem_count / _hash_table.size(); 
        }
};

#endif