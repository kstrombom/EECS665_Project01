/*
 * Kate Strombom
 * EECS 665
 * Assignment 1: nfa2dfa
 */

#include "nfa.h"

// Constructor
NFA :: NFA()
{
    mInitialState = parseInitialState();
    mFinalStates = parseFinalStates();
    mTotalNumStates = parseTotalNumStates();
    mTransitionCharacters = parseTransitionCharacters();
    mNumTransitionCharacters = mTransitionCharacters.size();
    mTransitionTable = parseTransitionTable();
}

// Deconstructor
NFA :: ~NFA()
{
}

// Parses the 1st line input to get initial state
int NFA :: parseInitialState()
{
    // Get inital state line.
    string lInput;
    getline ( cin, lInput );

    // Extract and return the initial state number.
    string lWord;
    istringstream iss ( lInput, istringstream::in );
    int lWordCount = 1;
    while( iss >> lWord )
    {
        if( lWordCount == 3 )
        {
            string lInitialState = lWord.substr( lWord.find( '{' ) + 1, lWord.find( '}' ) - 1 );
            return stoi( lInitialState );
        }
        lWordCount++;
    }

    return 0;
}

// Parses the 2nd line of console input to get a vector of final state(s).
vector <int> NFA :: parseFinalStates ()
{
    vector<int> lFinalStates;

    // Get final states line.
    string lInput;
    getline (cin, lInput);

    // Extract and return the final states.
    string lWord;
    istringstream iss( lInput, istringstream::in );
    int lWordCount = 1;
    while( iss >> lWord )
    {
        if( lWordCount == 3 )
        {
            string lFinalStatesString = lWord.substr( lWord.find( '{' ) + 1, lWord.find( '}' ) - 1  );

            // Break string into a vector of integers.
            stringstream ss( lFinalStatesString );
            int i;
            while( ss >> i )
            {
                lFinalStates.push_back( i );
                if( ss.peek() == ',' )
                {
                    ss.ignore();
                }
            }
        }
        lWordCount++;
    }

    return lFinalStates;
}

// Parses the 3rd input line to get total number of states in NFA.
int NFA :: parseTotalNumStates ()
{
    // Get the total number of states line.
    string lInput;
    getline ( cin, lInput );

    // Extract and return the total number of states.
    string lWord;
    istringstream iss ( lInput, istringstream::in );
    int lWordCount = 1;
    while( iss >> lWord )
    {
        if( lWordCount == 3 )
        {
            return stoi( lWord );
        }
        lWordCount++;
    }

    return 0;
}

// Parses 4th input line to get the character transitions.
vector <char> NFA :: parseTransitionCharacters()
{
    vector <char> lTransitionCharacters;

    // Get the transition characters line.
    string lInput;
    getline ( cin, lInput );

    // Extract and return the total number of states.
    string lWord;
    istringstream iss ( lInput, istringstream::in );
    int lWordCount = 1;
    while( iss >> lWord )
    {
        if( lWordCount > 1 )
        {
            lTransitionCharacters.push_back( lWord[0] );
        }
        lWordCount++;
    }

    return lTransitionCharacters;
}

// Parses the rows in the transition table and stores then in the appropriate vector.
vector<vector<vector<int>>> NFA :: parseTransitionTable ()
{
    vector<vector<vector<int>>> lTransitionTable;

    // Iterate through the transition table rows.
	string lInput;
	while (getline (cin, lInput))
	{
        vector<vector<int>> lStateRow;

        string lWord;
        istringstream iss ( lInput, istringstream::in );
        int lWordCount = 1;
        while (iss >> lWord)
        {
            if (lWordCount > 1)
            {
                vector<int> lTransitions;

                string lStatesString = lWord.substr( lWord.find( '{' ) + 1, lWord.find( '}' ) - 1  );

                int i;
                stringstream ss( lStatesString );
                while( ss >> i )
                {
                    lTransitions.push_back( i );
                    if( ss.peek() == ',' )
                    {
                        ss.ignore ();
                    }
                }

                lStateRow.push_back( lTransitions );
            }

            lWordCount++;
        }

        lTransitionTable.push_back( lStateRow );
    }

    return lTransitionTable;
}
