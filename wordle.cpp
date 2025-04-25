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
                         const set<string>& dict, set<string>& results) {
    if (pos == in.size()) {
        if (floating_set.empty() && dict.count(current)) {
            results.insert(current);
        }
        return;
    }
    if (in[pos] != '-') {
        current[pos] = in[pos];
        wordleHelper(in, current, pos + 1, floating_set, dict, results);
    } else {
        for (char c = 'a'; c <= 'z'; ++c) {
            current[pos] = c;
            bool used = false;
            auto it = floating_set.find(c);
            if (it != floating_set.end()) {
                used = true;
                floating_set.erase(it);
            }
            wordleHelper(in, current, pos + 1, floating_set, dict, results);
            if (used) {
                floating_set.insert(c);
            }
        }
    }
}

set<string> wordle(
    const string& in,
    const string& floating,
    const set<string>& dict)
{
    set<string> results;
    multiset<char> floating_set;
    for (char c : floating) {
        floating_set.insert(c);
    }
    string current = in;
    wordleHelper(in, current, 0, floating_set, dict, results);
    return results;
}