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

// Helper function prototype
void wordleHelper(
    const string& pattern,
    string& current,
    const string& floating,
    const set<string>& dict,
    set<string>& results,
    size_t pos,
    string& remainingFloating);

std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    set<string> results;
    string current = in;
    string remainingFloating = floating;
    
    wordleHelper(in, current, floating, dict, results, 0, remainingFloating);
    
    return results;
}

void wordleHelper(
    const string& pattern,
    string& current,
    const string& floating,
    const set<string>& dict,
    set<string>& results,
    size_t pos,
    string& remainingFloating)
{
    if (pos == pattern.size()) {
        if (dict.find(current) != dict.end()) {
            if (remainingFloating.empty()) {
                results.insert(current);
            }
        }
        return;
    }
    
    if (pattern[pos] != '-') {
        current[pos] = pattern[pos];
        
        wordleHelper(pattern, current, floating, dict, results, pos+1, remainingFloating);
    }
    else {
        for (size_t i = 0; i < remainingFloating.size(); ++i) {
            char floatingChar = remainingFloating[i];
            current[pos] = floatingChar;
            
            string newRemainingFloating = remainingFloating.substr(0, i) + 
                                         remainingFloating.substr(i+1);
            
            wordleHelper(pattern, current, floating, dict, results, pos+1, newRemainingFloating);
        }
        
        for (char c = 'a'; c <= 'z'; ++c) {
            if (remainingFloating.find(c) != string::npos) continue;
            
            current[pos] = c;
            
            wordleHelper(pattern, current, floating, dict, results, pos+1, remainingFloating);
        }
    }
}