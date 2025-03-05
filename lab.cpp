#include <bits/stdc++.h>

using namespace std;

class FiniteAutomaton;  // Forward declaration

class Grammar {
public:
    map<char, vector<string>> rules;

    Grammar() {
        rules['S'] = {"aA"};
        rules['A'] = {"bS", "aB"};
        rules['B'] = {"bC"};
        rules['C'] = {"aA", "b"};
    }

    FiniteAutomaton toFiniteAutomaton();
};

class FiniteAutomaton {
public:
    set<string> states;
    set<char> alphabet;
    map<pair<string, char>, set<string>> transitions;  // Allows multiple transitions for NFA
    string startState;
    set<string> finalStates;

    bool isDeterministic() {
        for (auto& [key, value] : transitions) {
            if (value.size() > 1) return false;  // Multiple states for (state, symbol) → NFA
        }
        return true;
    }

    FiniteAutomaton convertToDFA() {
    FiniteAutomaton dfa;
    queue<set<string>> newStates;  // Queue to manage the sets of NFA states
    set<string> processedStates;   // Set to track processed DFA states (state sets)
    map<string, set<string>> stateMapping;  // Map to map state names to NFA state sets

    set<string> initialState = {startState};  // Initial DFA state is the start state of the NFA
    newStates.push(initialState);
    stateMapping["q0"] = initialState;  // Initial state name mapping

    int stateCount = 1;  // Start with "q0"

    while (!newStates.empty()) {
        set<string> currentSet = newStates.front();
        newStates.pop();

        // Create state name by concatenating sorted elements from currentSet
        string stateName = "";
        for (const string& s : currentSet) {
            stateName += s;  // Concatenate each string from the set
        }

        if (processedStates.count(stateName)) continue;  // Skip already processed sets
        processedStates.insert(stateName);

        // For each symbol in the alphabet, compute transitions
        for (char symbol : alphabet) {
            set<string> newStateSet;
            for (const string& s : currentSet) {
                if (transitions.count({s, symbol})) {
                    newStateSet.insert(transitions[{s, symbol}].begin(), transitions[{s, symbol}].end());
                }
            }

            if (!newStateSet.empty()) {
                // Create new state name by concatenating NFA states in the set
                string newStateName = "";
                for (const string& s : newStateSet) {
                    newStateName += s;  // Concatenate each string from the set
                }

                // Add transition in DFA from current state to new state
                dfa.transitions[{stateName, symbol}].insert(newStateName);

                // If this new state hasn't been processed, enqueue it
                if (!processedStates.count(newStateName)) {
                    newStates.push(newStateSet);
                    stateMapping[newStateName] = newStateSet;
                }
            }
        }
    }

    // Assign final states in DFA
    for (auto& [stateName, stateSet] : stateMapping) {
        for (const string& s : stateSet) {
            if (finalStates.count(s)) {
                dfa.finalStates.insert(stateName);
                break;
            }
        }
    }

    // Set DFA states and start state
    dfa.states.insert("q0");  // Add initial state q0
    for (auto& [stateName, stateSet] : stateMapping) {
        dfa.states.insert(stateName);  // Add all the new DFA states
    }
    dfa.startState = "q0";  // Start state of the DFA is always q0

    return dfa;
}

    void display() {
        cout << "Finite Automaton States: { ";
        for (const auto& state : states) cout << state << " ";
        cout << "}\n";

        cout << "Alphabet: { ";
        for (char c : alphabet) cout << c << " ";
        cout << "}\n";

        cout << "Transitions:\n";
        for (auto& [key, value] : transitions) {
            for (const string& next : value) {
                cout << "6(" << key.first << ", " << key.second << ") -> " << next << endl;
            }
        }

        cout << "Start State: " << startState << endl;
        cout << "Final States: { ";
        for (const string& state : finalStates) cout << state << " ";
        cout << "}\n";
    }
};

// Converts Grammar to Finite Automaton
FiniteAutomaton Grammar::toFiniteAutomaton() {
    FiniteAutomaton fa;
    int stateCount = 0;
    map<char, string> stateMapping;

    // Add the start state
    fa.startState = "q0";
    stateMapping['S'] = "q0";  // Start state is mapped to S

    // For each non-terminal, create a state and handle transitions
    for (auto& rule : rules) {
        char nonTerminal = rule.first;
        if (stateMapping.find(nonTerminal) == stateMapping.end()) {
            stateMapping[nonTerminal] = "q" + to_string(stateCount++);
        }

        for (const string& production : rule.second) {
            char symbol = production[0];  // Terminal symbol
            string nextState;

            if (production.size() == 2) {  // Non-terminal transition (i.e., second character)
                char nextNonTerminal = production[1];
                if (stateMapping.find(nextNonTerminal) == stateMapping.end()) {
                    stateMapping[nextNonTerminal] = "q" + to_string(stateCount++);
                }
                nextState = stateMapping[nextNonTerminal];
            } else {  // Terminal transition
                nextState = stateMapping[nonTerminal];
                fa.finalStates.insert(nextState);  // This should be marked as final for terminal symbols
            }

            // Add the transition for the terminal symbol
            fa.transitions[{stateMapping[nonTerminal], symbol}].insert(nextState);
            fa.alphabet.insert(symbol);
        }
    }

    // Add all states generated
    for (const auto& entry : stateMapping) {
        fa.states.insert(entry.second);
    }

    return fa;
}

int main() {
    Grammar grammar;
    FiniteAutomaton fa = grammar.toFiniteAutomaton();

    cout << "Converted Finite Automaton from Grammar:\n";
    fa.display();

    cout << "\nChecking if Finite Automaton is Deterministic: "
         << (fa.isDeterministic() ? "DFA" : "NDFA") << endl;

    if (!fa.isDeterministic()) {
        cout << "\nConverting NDFA to DFA...\n";
        FiniteAutomaton dfa = fa.convertToDFA();

        cout << "DFA Transitions:\n";
        for (auto& [key, value] : dfa.transitions) {
            for (const string& next : value) {
                cout << "6(" << key.first << ", " << key.second << ") -> " << next << endl;
            }
        }

        cout << "DFA Start State: " << dfa.startState << endl;
        cout << "DFA Final States: { ";
        for (string state : dfa.finalStates) {
            cout << state << " ";
        }
        cout << "}\n";
        cout << "\nChecking if Finite Automaton is Deterministic: "
         << (dfa.isDeterministic() ? "DFA" : "NDFA") << endl;
    }

    return 0;
}
