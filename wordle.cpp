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

static void wordleHelper(const string& in, string& current,
                         size_t pos, multiset<char>& floating_set,
                         const set<string>& dict, set<string>& results) {
    if (pos == in.length()) {
        // Check if all floating letters are used
        if (floating.empty() && dict.find(current) != dict.end()) {
            results.insert(current);
        }
        return;
    }

    if (in[pos] != '-') {
        // Fixed letter position
        current[pos] = in[pos];
        generateWords(in, floating, current, pos + 1, results, dict);
    } else {
        // Try all possible letters, but need to ensure floating letters are used
        vector<bool> used(26, false);
        for (size_t i = 0; i < 26; ++i) {
            char c = 'a' + i;
            
            // Check if this is a floating letter that needs to be placed
            size_t float_pos = floating.find(c);
            if (float_pos != string::npos) {
                string new_floating = floating;
                new_floating.erase(float_pos, 1);
                current[pos] = c;
                generateWords(in, new_floating, current, pos + 1, results, dict);
                used[i] = true;
            }
        }
        
        // Try other letters only if we have remaining positions to place floating letters
        size_t remaining_positions = in.length() - pos - 1;
        if (remaining_positions >= floating.length()) {
            for (size_t i = 0; i < 26; ++i) {
                if (!used[i]) {
                    char c = 'a' + i;
                    current[pos] = c;
                    generateWords(in, floating, current, pos + 1, results, dict);
                }
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
    string current = in;
    generateWords(in, floating, current, 0, results, dict);
    return results;
}