# Grammar

### Course: Formal Languages & Finite Automata
### Author: Miricinschi Gabriel, FAF-233

----

## Theory
_**Formal languages**_ consist of words whose letters are taken from an alphabet and are well-formed according to a specific set of rules called a formal grammar.

A _**formal grammar**_ describes a set of rules to follow in order to create strings that are valid for the language.

A _**finite automata**_ is a model used to recognize patterns in a formal language, consisting of states and transitions. It processes a string one symbol at a time in order to determine its validity.
## Objectives:

1. Implement a type/class for your grammar;

2. Add one function that would generate 5 valid strings from the language expressed by your given grammar;

3. Implement some functionality that would convert and object of type Grammar to one of type Finite Automaton;

4. For the Finite Automaton, please add a method that checks if an input string can be obtained via the state transition from it;

## Implementation description

This loop in the Grammar class generates random strings based on the production rules defined in rules. It repeatedly scans the result string, checking if any character matches a non-terminal in the rules. If a match is found, it randomly selects one of the possible productions for that non-terminal and replaces it in result. The process continues until no more replacements are possible, meaning all non-terminals have been expanded into terminal symbols. This ensures that the generated string conforms to the grammar.

The class also includes a method for converting the grammar into a finite automaton (FA), allowing formal language processing. The generateFiveStrings() method simply calls this string generation logic five times, printing the results. This provides a way to observe how the grammar expands and generates different valid strings based on its rules.

```c++
        while (true) {
            bool replaced = false;
            for (int i = 0; i < result.size(); i++) {
                if (rules.count(result[i])) {
                    string replacement = rules[result[i]][rand() % rules[result[i]].size()];
                    result = result.substr(0, i) + replacement + result.substr(i + 1);
                    replaced = true;
                    break;
                }
            }
            if (!replaced) break;
        }
```

The FiniteAutomaton class models a finite automaton with a set of states, an alphabet, transitions, a start state, and final states. The validateString() method determines whether a given input string is accepted by the automaton by simulating its behavior. It starts from the initial state and processes each symbol in the input. If a valid transition exists for the current symbol, it moves to the next state; otherwise, it immediately returns false, indicating rejection. If, after processing all symbols, the automaton is in a final state, the function returns true, confirming that the string is accepted. This method effectively checks if the input belongs to the language defined by the automaton.

```c++
        char currentState = startState;
        for (char symbol : input) {
            if (transitions.find({currentState, symbol}) == transitions.end())
                return false;
            currentState = transitions[{currentState, symbol}];
        }
        return finalStates.count(currentState) > 0;
```
## Conclusions
In this lab, I implemented a Grammar class that defines a formal grammar using production rules. I added functionality to generate five valid strings based on the grammar and created a method to convert the grammar into a FiniteAutomaton. The FiniteAutomaton class was then enhanced with a method to validate if an input string is accepted by the automaton. This exercise helped reinforce the concepts of formal languages, grammars, and finite automata, as well as their interrelation in language generation and validation. Through this lab, I deepened my understanding of basic formal languages and finite automaton, as well as learning how to create valid words/strings, according to a specified grammar, through programming.

### Output

![Results](https://github.com/user-attachments/assets/435a52f6-cbc9-4a34-85fb-43f64fb41c61)

Results after running the code.
