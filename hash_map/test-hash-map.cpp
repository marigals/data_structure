#include "hash-map.hpp"

int main()
{
    hash_map<double, bool> hm;

    for(size_t i = 0; i < 30; i++)
        hm.insert(i, i & 2);

    try
    {
        for(size_t i = 26; i < 31; i++)
            std::cout << "val at " << i << ": " << hm[i] << std::endl;
    }
    catch(std::out_of_range ex)
    {
        std::cerr << "exception: " << ex.what() << std::endl;
    }

    std::cout << "max load: " << hm.get_max_load() << std::endl;

    hm.set_max_load(5);

    std::cout << "max load is set to 5, erasing element" << std::endl;
    hm.erase(7);

    std::cout << "current load: " << hm.get_current_load() << std::endl;
    
    return 0;
}