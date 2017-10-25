/*
 * Kate Strombom
 * EECS 665
 * Assignment 1: nfa2dfa
 */

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

#ifndef NFA_H
#define NFA_H

class NFA
{
    public:
        NFA();
        ~NFA();

        int mInitialState;
        vector<int> mFinalStates;
        int mTotalNumStates;
        vector<char> mTransitionCharacters;
        unsigned int mNumTransitionCharacters;
        vector<vector<vector<int>>> mTransitionTable;

	private:
        int parseInitialState();
        vector<int> parseFinalStates();
        int parseTotalNumStates();
        vector<char> parseTransitionCharacters();
        vector<vector<vector<int>>> parseTransitionTable();
};

#endif
