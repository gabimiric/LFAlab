# Finite Automata

### Course: Formal Languages & Finite Automata
### Author: Miricinschi Gabriel, FAF-233

----

## Theory
A _finite automaton_ is a mechanism used to represent processes of different kinds. It can be compared to a state machine as they both have similar structures and purpose as well. The word finite signifies the fact that an automaton comes with a starting and a set of final states. In other words, for process modeled by an automaton has a beginning and an ending.

Based on the structure of an automaton, there are cases in which with one transition multiple states can be reached which causes non determinism to appear. In general, when talking about systems theory the word determinism characterizes how predictable a system is. If there are random variables involved, the system becomes stochastic or non deterministic.

That being said, the automata can be classified as non-/deterministic, and there is in fact a possibility to reach determinism by following algorithms which modify the structure of the automaton.
## Objectives:

1. Understand what an automaton is and what it can be used for.

2. Continuing the work in the same repository and the same project, the following need to be added:
    a. Provide a function in your grammar type/class that could classify the grammar based on Chomsky hierarchy.

    b. For this you can use the variant from the previous lab.

3. According to your variant number (by universal convention it is register ID), get the finite automaton definition and do the following tasks:

    a. Implement conversion of a finite automaton to a regular grammar.

    b. Determine whether your FA is deterministic or non-deterministic.

    c. Implement some functionality that would convert an NDFA to a DFA.
    

## Implementation description

I added a function in the Grammar class which converts a grammar to an FA. The function returns a FA object once the conversion is done.

```c++
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
```

In the FiniteAutomaton class I added a function to check if the FA is determinstic or not. On top of this I also added a function which converts an NFA to a DFA and returns an FA object which is a DFA.

```c++
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
```
## Conclusions
In this lab, I enhanced the Grammar class to define formal grammars using production rules and integrated functionality to convert the grammar into a Finite Automaton (FA). Additionally, I implemented methods in the FiniteAutomaton class to check for determinism and convert an NFA to a DFA. This work deepened my understanding of formal languages, grammars, and finite automata, particularly in their application for language generation, validation, and automaton conversions. The tasks reinforced how these theoretical concepts are connected and provided practical experience in working with them through programming.

### Output

![Results](https://github.com/user-attachments/assets/92134b18-87a9-448f-88c1-e98c6557d2d8)

Results after running the code.
