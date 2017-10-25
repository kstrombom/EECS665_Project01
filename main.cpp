/*
 * Kate Strombom
 * EECS 665
 * Assignment 1: nfa2dfa
 */

#include <algorithm>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <vector>

#include "nfa.h"

using namespace std;

// Struct to store data to create a DFA from an NFA
struct gDFADataStruct
{
    vector<int> mClosureOnVector;
    vector<int> mNewStateVector;
    bool mMarked;
    vector<tuple<char,vector<int>>> mTransitionsVector;
};

// Function Declarations
int VectorExistsAtIndex( vector<gDFADataStruct> aContainingVector, vector<int> aNewState );
int getTransitionCharacterIndex( NFA aNFA, char aTransitionCharacter );
vector<int> Move( NFA aNFA, vector<int> aStates, char aTransitionCharacter );
bool UnmarkedStateExists(vector<gDFADataStruct> aDStates);
int getNextUnmarkedStateIndex( vector<gDFADataStruct> aDStates );
void printTransition( char aTransitionCharacter, vector<int> aInputVector, vector<int> aOutputVector );
void printEClosure( vector<int> aStartStates, vector<int> aEClosure, int aState);
vector<int> EClosure( NFA aNFA, vector<int> aStates );
void printDFA( vector<gDFADataStruct> aDFAData, NFA aNFA );
vector<int> getEndPointStates( vector<gDFADataStruct> aDFA, vector<int> aOldStates );
int getClosureOnIndex( vector<gDFADataStruct> aDFAData, vector<int> aMatchVector );

int main(int argc, char* argv[])
{
    // The NFA initalization
    NFA lNFA;
    // The entire DFA data vector
    vector<gDFADataStruct> lDFADataVector;

    // Make transition template vector<tuple>
    vector<tuple<char,vector<int>>> lTransitionTemplate;
    for( int i = 0; i < lNFA.mNumTransitionCharacters - 1; i++ )
    {
        tuple<char,vector<int>> lTuple = {lNFA.mTransitionCharacters[i], vector<int>()};
        lTransitionTemplate.push_back( lTuple );
    }

    // Initial E-closure
    int lInitialState = lNFA.mInitialState;
    vector<int> lInitialStateVector = {lInitialState};
    vector<int> lInitialNewStateVector = EClosure( lNFA, lInitialStateVector );
    // Create DFA data struct for inital and place in the DFA data vector
    gDFADataStruct lInitialDFAStruct = {lInitialStateVector, lInitialNewStateVector, false, lTransitionTemplate};
    lDFADataVector.push_back( lInitialDFAStruct );

    // Print IO E closure
    cout << "E-closure(IO) = {";
    for( int i = 0; i < lInitialNewStateVector.size(); i++ )
    {
        if( i == lInitialNewStateVector.size() - 1 )
        {
            cout << lInitialNewStateVector[i] << "} = " << 1 << endl;
        }
        else
        {
            cout << lInitialNewStateVector[i] << ",";
        }
    }

    // Variable to track the mark
    int iMark = 1;

    // NFA to DFA Conversion. Continue if any states remain unmarked.
    while ( UnmarkedStateExists(lDFADataVector) )
    {
        int lNextUnmarkedIndex = getNextUnmarkedStateIndex(lDFADataVector);

        // Mark it and print
        lDFADataVector[lNextUnmarkedIndex].mMarked = true;
        cout << "\nMark " << iMark << endl;
        iMark++;

        // Loop through each transition character
        for ( int iChar = 0; iChar < lNFA.mNumTransitionCharacters - 1; iChar++ )
        {
            vector<int> lMoveResults = Move( lNFA, lDFADataVector[lNextUnmarkedIndex].mNewStateVector, lNFA.mTransitionCharacters[iChar] );
            vector<int> lNewStateVector = EClosure( lNFA, lMoveResults );
            int lVectorExistsIndex = VectorExistsAtIndex( lDFADataVector, lNewStateVector );

            // If the new state is not an empty set add the transition
            if( !lNewStateVector.empty() )
            {
                //store the transition in current vector
                vector<tuple<char,vector<int>>> & lTupleVector = lDFADataVector[lNextUnmarkedIndex].mTransitionsVector;
                for ( tuple<char,vector<int>> & iTuple : lTupleVector )
                {
                    if (get<0>(iTuple) == lNFA.mTransitionCharacters[iChar] )
                    {
                        get<1>(iTuple) = lMoveResults;
                    }
                }
            }
            else
            {
                continue;
            }

            printTransition( lNFA.mTransitionCharacters[iChar], lDFADataVector[lNextUnmarkedIndex].mNewStateVector, lMoveResults );

            // If the move state exists add it
            if( lVectorExistsIndex == -1 )
            {
                gDFADataStruct lNewStruct = {lMoveResults, lNewStateVector, false, lTransitionTemplate};
                lDFADataVector.push_back( lNewStruct );
                printEClosure( lMoveResults, lNewStateVector, lDFADataVector.size() );
            }
            else
            {
                printEClosure( lMoveResults, lNewStateVector, lVectorExistsIndex + 1 );
            }

            //////DEBUG
            /*
            cout<<"-------Struct at state "<<lNextUnmarkedIndex<<"----------"<<endl;
            for (gDFADataStruct iStruct : lDFADataVector )
            {
                cout<<"Closureonvector: ";
                for(int iCO: iStruct.mClosureOnVector)
                {
                    cout<<iCO<<", ";
                }
                cout<<endl;

                cout<<"newstatevector: ";
                for(int iNS: iStruct.mNewStateVector)
                {
                    cout<<iNS<<", ";
                }
                cout<<endl;

                cout<<"marked: "<< ((iStruct.mMarked) ? "true" : "false");
                cout<<endl;

                cout<<"tuples:"<<endl;

                for(tuple<char,vector<int>> iTuple : iStruct.mTransitionsVector)
                {
                    cout<<"     trans "<<get<0>(iTuple)<<": ";
                    for(int itrans: get<1>(iTuple))
                    {
                        cout<<itrans<<", ";
                    }
                    cout<<endl;
                }
                cout<<endl;
            }
            cout<<"-----------------------------------------\n";
            */
            //////DEBUG


        }
    }

    cout << endl;
    printDFA( lDFADataVector, lNFA );

    return 0;
}

int VectorExistsAtIndex( vector<gDFADataStruct> aContainingVector, vector<int> aNewState )
{
    for( int iIndex = 0; iIndex < aContainingVector.size(); iIndex++ )
    {
        if( aContainingVector[iIndex].mNewStateVector.size() == aNewState.size() )
        {
            bool lVectorFound = true;
            for( int iState : aNewState )
            {
                // Did not find a value in vector
                if( find( aContainingVector[iIndex].mNewStateVector.begin(),
                          aContainingVector[iIndex].mNewStateVector.end(),
                          iState ) == aContainingVector[iIndex].mNewStateVector.end() )
                {
                    lVectorFound = false;
                    break;
                }
            }
            if ( lVectorFound )
            {
                return iIndex;
            }
        }
    }
    return -1;
}

vector<int> Move( NFA aNFA, vector<int> aStates, char aTransitionCharacter )
{
    vector<int> lMoveStates;
    int lTransitionCharacterIndex = getTransitionCharacterIndex( aNFA, aTransitionCharacter );

    for( int iNewStateVal : aStates )
    {
        vector<int> lTransitionStates = aNFA.mTransitionTable.at(iNewStateVal - 1).at( lTransitionCharacterIndex );

        for( int iTransitionState : lTransitionStates )
        {
            if( find(lMoveStates.begin(), lMoveStates.end(), iTransitionState) == lMoveStates.end())
            {
                lMoveStates.push_back( iTransitionState );
            }
        }
    }

    sort( lMoveStates.begin(),lMoveStates.end() );
    return lMoveStates;
}

int getTransitionCharacterIndex( NFA aNFA, char aTransitionCharacter )
{
    for( int i = 0; i < aNFA.mTransitionCharacters.size(); i++ )
    {
        if ( aNFA.mTransitionCharacters[i] == aTransitionCharacter )
        {
            return i;
        }
    }
    return -1;
}

int getNextUnmarkedStateIndex( vector<gDFADataStruct> aDStates )
{
    for ( int i = 0; i < aDStates.size(); i++ )
    {
        if ( !aDStates[i].mMarked )
        {
            return i;
        }
    }

    return -1;
}

bool UnmarkedStateExists(vector<gDFADataStruct> aDStates)
{
    for ( gDFADataStruct iStruct : aDStates )
    {
        if ( !iStruct.mMarked )
        {
            return true;
        }
    }

    return false;
}

void printEClosure( vector<int> aClosureOnVector, vector<int> aNewStateVector, int aState)
{
    cout << "E-closure{";
    for( int i = 0; i < aClosureOnVector.size(); i++ )
    {
        if( i == aClosureOnVector.size() - 1 )
        {
            cout << aClosureOnVector[i] << "} = {";
        }
        else
        {
            cout << aClosureOnVector[i] << ",";
        }
    }

    for( int i = 0; i < aNewStateVector.size(); i++ )
    {
        if( i == aNewStateVector.size() - 1 )
        {
            cout << aNewStateVector[i] << "} = " << aState<< endl;
            return;
        }
        else
        {
            cout << aNewStateVector[i] << ", ";
        }
    }
}

void printTransition( char aTransitionCharacter, vector<int> aInputVector, vector<int> aOutputVector )
{
    cout << "{";
    for( int i = 0; i < aInputVector.size(); i++ )
    {
        if( i == aInputVector.size() - 1 )
        {
            cout << aInputVector[i] << "}";
        }
        else
        {
            cout << aInputVector[i] << ", ";
        }
    }

    cout << " --" << aTransitionCharacter << "--> {";

    for( int i = 0; i < aOutputVector.size(); i++ )
    {
        if( i == aOutputVector.size() - 1 )
        {
            cout << aOutputVector[i] << "}\n";
        }
        else
        {
            cout << aOutputVector[i] << ", ";
        }
    }
}

vector<int> EClosure( NFA aNFA, vector<int> aStates )
{
    unsigned int lNumTransitionCharacters = aNFA.mNumTransitionCharacters;

    vector<int> lEClosure = aStates;

    stack<int> lStack;

    for( int i : aStates )
    {
        lStack.push( i );
    }

    while ( !lStack.empty() )
    {
        int lState = lStack.top();
        lStack.pop();

        vector<int> lEStates = aNFA.mTransitionTable.at( lState - 1 )
                                                    .at( lNumTransitionCharacters - 1 );
        for ( int iNewStateVal : lEStates )
        {
            if ( find( lEClosure.begin(), lEClosure.end(), iNewStateVal) == lEClosure.end())
            {
                lEClosure.push_back( iNewStateVal );
                lStack.push( iNewStateVal );
            }
        }
    }

    sort( lEClosure.begin(), lEClosure.end() );
    return lEClosure;
}

void printDFA( vector<gDFADataStruct> aDFAData, NFA aNFA )
{
    vector<int> lOldInitialState = {aNFA.mInitialState};
    vector<int> lNewInitialState = getEndPointStates( aDFAData, lOldInitialState );
    vector<int> lNewFinalState = getEndPointStates( aDFAData, aNFA.mFinalStates );

    cout << "Initial State: {";
    for( int i = 0; i < lNewInitialState.size(); i++ )
    {
        if( i == lNewInitialState.size() - 1 )
        {
            cout << lNewInitialState[i] << "}" << endl;
        }
        else
        {
            cout << lNewInitialState[i] << ",";
        }
    }

    cout << "Final States: {";
    for( int i = 0; i < lNewFinalState.size(); i++ )
    {
        if( i == lNewFinalState.size() - 1 )
        {
            cout << lNewFinalState[i] << "}" << endl;
        }
        else
        {
            cout << lNewFinalState[i] << ",";
        }
    }

    cout << "State  ";
    for( int i = 0; i < aNFA.mTransitionCharacters.size() - 1; i++ )
    {
        if( i == aNFA.mTransitionCharacters.size() - 2 )
        {
            cout << aNFA.mTransitionCharacters[i] << endl;
        }
        else
        {
            cout << aNFA.mTransitionCharacters[i] << "      ";
        }
    }

    // Print DFA table
    // Iterates through each new state data
    for( int i = 0; i < aDFAData.size(); i++ )
    {
        cout << i + 1 << "      ";
        // Iterates through each transition in state
        for( tuple<char,vector<int>> iTrans : aDFAData[i].mTransitionsVector )
        {
            // New state
            int lNewState = getClosureOnIndex( aDFAData, get<1>( iTrans ) ) + 1;
            if( lNewState == 0 )
            {
                cout << "{}     ";
            }
            else
            {
                cout << "{" << lNewState << "}    ";
            }
        }
        cout << endl;
    }
}

int getClosureOnIndex( vector<gDFADataStruct> aDFAData, vector<int> aMatchVector )
{
    for( int iIndex = 0; iIndex < aDFAData.size(); iIndex++ )
    {
        if( aDFAData[iIndex].mClosureOnVector.size() == aMatchVector.size() )
        {
            bool lVectorFound = true;
            for( int iState : aMatchVector )
            {
                // Did not find a value in vector
                if( find( aDFAData[iIndex].mClosureOnVector.begin(),
                          aDFAData[iIndex].mClosureOnVector.end(),
                          iState ) == aDFAData[iIndex].mClosureOnVector.end() )
                {
                    lVectorFound = false;
                    break;
                }
            }
            if ( lVectorFound )
            {
                return iIndex;
            }
        }
    }
    return -1;
}

vector<int> getEndPointStates( vector<gDFADataStruct> aDFAData, vector<int> aOldStates )
{
    vector<int> lEndPoints;
    for( int iState : aOldStates )
    {
        for( int iIndex = 0; iIndex < aDFAData.size(); iIndex++ )
        {
            if( find( aDFAData[iIndex].mNewStateVector.begin(),
                aDFAData[iIndex].mNewStateVector.end(),
                iState ) != aDFAData[iIndex].mNewStateVector.end() )
            {
                lEndPoints.push_back( iIndex + 1 );
            }
        }
    }

    return lEndPoints;
}