#include "substring.hpp"

int main()
{
    std::vector<size_t> test;

    //std::string target = "hello, little cute World! space";
    //std::string pattern = "e ";
    std::string target = "aaaabaabaaaaa";
    std::string pattern = "aabaa";

    test = Rabin_Karp(target, pattern);
    std::cerr << "Rabin-Karp: " << std::endl;
    for(size_t i = 0; i < test.size(); i++)
        std::cout << test.at(i) << std::endl;
    std::cout << std::endl;

    test = Boyer_Moore(target, pattern);
    std::cerr << "Boyer-Moore: " << std::endl;
    for(size_t i = 0; i < test.size(); i++)
        std::cout << test.at(i) << std::endl;
    std::cout << std::endl;

    test = Knuth_Morris_Pratt(target, pattern);
    std::cerr << "Knuth-Morris-Pratt: " << std::endl;
    for(size_t i = 0; i < test.size(); i++)
        std::cout << test.at(i) << std::endl;
    std::cout << std::endl;

    test = automation(target, pattern);
    std::cerr << "automation: " << std::endl;
    for(size_t i = 0; i < test.size(); i++)
        std::cout << test.at(i) << std::endl;
    std::cout << std::endl;



    return 0;
}
