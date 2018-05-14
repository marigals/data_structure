#include "substring.hpp"


int64_t alp_len = 256;
int64_t q = 997;


static uint64_t modular_pow(uint64_t base, uint64_t ind, uint64_t mod)
{
    uint64_t res = 1;
    for(uint64_t i = 0; i < ind; i++)
        res = (res * base) % mod;
    return res;
}

static int64_t mod(int64_t src, int64_t m)
{
    if(m < 0)
        m = -m;
    
    bool sign = false; //false +, true -
    if(src < 0)
    {
        src = -src;
        sign = true;
    }
    int64_t res = src % m;
    if(sign)
        res = m - res;
    
    return res;
}

std::vector<size_t> Rabin_Karp(std::string target, std::string pattern)
{
    std::vector<size_t> res;

    size_t tar_len = target.length(); 
    size_t pat_len = pattern.length();
    if (pat_len > tar_len)
        return res;

    int64_t d_m = modular_pow(alp_len, pat_len - 1, q);

    int64_t h_t = 0;
    int64_t h_p = 0; 

    for(size_t i = 0; i < pat_len; i++)
    {
        h_p = (alp_len * h_p + pattern[i]) % q;
        h_t = (alp_len * h_t + target[i]) % q;
    }

    for(size_t i = 0; i < tar_len - pat_len + 1; i++)
    {
        if(h_p == h_t)
            if(pattern == target.substr(i, pat_len))
                res.push_back(i);

        if(i < tar_len - pat_len + 1)
            h_t = mod(alp_len * (h_t - d_m * target[i]) + target[i + pat_len], q);
    }

    return res;
}



static std::vector<size_t> prefix(std::string pattern)
{
    std::vector<size_t> pr(pattern.length());
    pr.at(0) = 0;
    size_t k = 0;

    for(size_t i = 1; i < pattern.length(); i++)
    {
        while(k > 0 && pattern[k] != pattern[i])
            k = pr.at(k-1);

        if(pattern[i] == pattern[k])
            k++;

        pr.at(i) = k;
    }

    return pr;
}

static std::vector<size_t> get_suffix_table(std::string pattern)
{
    size_t p_len = pattern.length(); 
    std::vector<size_t> pr = prefix(pattern);

    //reverse pattern
    for(size_t i = 0; i < (pattern.length())/2; i++)
        std::swap(pattern[i], pattern[ pattern.length() - i - 1 ]);

    //prefix for reverse pattern
    std::vector<size_t> re_pr = prefix(pattern);

    std::vector<size_t> table(p_len + 1);

    //top border
    for(size_t i = 0; i < table.size(); i++)
        table.at(i) = p_len - pr.at(p_len - 1);
    
    size_t ind = 0;
    size_t shift = 0;
    for(size_t i = 0; i < pattern.length(); i++) 
    {
        ind = p_len - re_pr.at(i);
        shift = i - re_pr.at(i) + 1;
        if(table.at(ind) > shift)
            table.at(ind) = shift;
    }

    return table;
}

static std::vector<int64_t> get_stop_table(std::string pattern)
{
    std::vector<int64_t> table(alp_len, -1);

    for(size_t i = 0; i < pattern.length() - 1; i++)
        table.at(pattern[i]) = i;

    return table;
}

std::vector<size_t> Boyer_Moore(std::string target, std::string pattern)
{
    std::vector<size_t> res;

    std::vector<size_t> suff = get_suffix_table(pattern);
    size_t delta_suff = 0;

    std::vector<int64_t> stop = get_stop_table(pattern);
    int64_t delta_stop = 0;

    for(size_t i = 0; i < target.length() - pattern.length() + 1; )
    {
        int64_t j = pattern.length() - 1;

        while(j >= 0 && pattern[j] == target[i+j])
            j--;

        if(j < 0)
        {
            res.push_back((int64_t)i);
            delta_stop = 1;
        }
        else
            delta_stop = j - stop.at(target[i+j]);

        delta_suff = suff.at(j+1);

        i += std::max(delta_stop, (int64_t)delta_suff);
    }

    return res;
}



std::vector<size_t> Knuth_Morris_Pratt(std::string target, std::string pattern)
{
    std::vector<size_t> res;

    std::vector<size_t> pr = prefix(pattern);

    size_t k = 0;

    for(size_t i = 0; i < target.length(); i++)
    {
        while(k > 0 && pattern[k] != target[i])
            k = pr[k-1];

        if(pattern[k] == target[i])
            k++;

        if(k == pattern.length())
        {
            res.push_back((int64_t)i - pattern.length() + 1);
            k = pr[k-1];
        }
    }

    return res;
}


size_t aut_suffix(std::string pref, std::string suff)
{
    size_t suf = 0;

    for(size_t i = 1; i <= pref.length(); i++)
        if(suff.substr(suff.length() - i, i) == pref.substr(0, i))
            suf = i;
    
    return suf;
}


std::vector<size_t> automation(std::string target, std::string pattern)
{
    std::vector<size_t> res;

    std::vector< std::vector<size_t> > aut(pattern.length()+1);
    for(size_t i = 0; i < aut.size(); i++)
        aut.at(i) = std::vector<size_t>(alp_len);

    //building aut
    for(size_t i = 0; i <= pattern.length(); i++)
        for(size_t j = 0; j < alp_len; j++)
            aut.at(i).at(j) = aut_suffix(pattern.substr(0, i+1), pattern.substr(0, i) + (char)j);


    size_t state = 0;
    for(size_t i = 0; i < target.length(); i++)
    {
        state = aut.at(state).at(target[i]);
    
        if (state == pattern.length())
            res.push_back(i - pattern.length() + 1);
    }

    return res;
}
