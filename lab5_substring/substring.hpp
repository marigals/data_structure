#ifndef SUBSTRING_HPP
#define SUBSTRING_HPP

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <utility>


std::vector<size_t> Rabin_Karp(std::string target, std::string pattern);

std::vector<size_t> Boyer_Moore(std::string target, std::string pattern);

std::vector<size_t> Knuth_Morris_Pratt(std::string target, std::string pattern);

std::vector<size_t> automation(std::string target, std::string pattern);

#endif
