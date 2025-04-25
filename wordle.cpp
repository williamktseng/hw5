#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;

// Helper prototype
static void wordleHelper(const string& in, string& current,
                         size_t pos, multiset<char>& floating_set,
                         int blanks_remaining,
                         const set<string>& dict, set<string>& results) {
    if (pos == in.size()) {
        if (floating_set.empty() && dict.count(current)) {
            results.insert(current);
        }
        return;
    }
    if (in[pos] != '-') {
        wordleHelper(in, current, pos + 1, floating_set, blanks_remaining, dict, results);
    } else {
        int f = floating_set.size();
        if (blanks_remaining == f) {
            char prev = '\0';
            for (auto it = floating_set.begin(); it != floating_set.end(); ++it) {
                char c = *it;
                if (c == prev) continue;
                prev = c;
                current[pos] = c;
                auto eraseIt = floating_set.find(c);
                floating_set.erase(eraseIt);
                wordleHelper(in, current, pos + 1, floating_set, blanks_remaining - 1, dict, results);
                floating_set.insert(c);
            }
        } else {
            char prev = '\0';
            for (auto it = floating_set.begin(); it != floating_set.end(); ++it) {
                char c = *it;
                if (c == prev) continue;
                prev = c;
                current[pos] = c;
                auto eraseIt = floating_set.find(c);
                floating_set.erase(eraseIt);
                wordleHelper(in, current, pos + 1, floating_set, blanks_remaining - 1, dict, results);
                floating_set.insert(c);
            }
            for (char c = 'a'; c <= 'z'; ++c) {
                if (floating_set.count(c) == 0) {
                    current[pos] = c;
                    wordleHelper(in, current, pos + 1, floating_set, blanks_remaining - 1, dict, results);
                }
            }
        }
    }
}

std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    std::set<std::string> results;
    std::multiset<char> floating_set;
    for (char c : floating) {
        floating_set.insert(c);
    }
    int blanks = 0;
    for (char c : in) {
        if (c == '-') blanks++;
    }
    std::string current = in;
    wordleHelper(in, current, 0, floating_set, blanks, dict, results);
    return results;
}