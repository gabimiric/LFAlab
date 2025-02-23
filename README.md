# The title of the work

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

The Grammar class defines the set of rules for my grammar and includes methods to generate random strings based on those rules. It also has a method to convert the grammar into a finite automaton (FA). The generateFiveStrings() method generates and prints five random strings using the defined rules.

```c++
class Grammar {
public:
    map<char, vector<string>> rules;

    Grammar() {
        rules['S'] = {"aA"};
        rules['A'] = {"bS", "aB"};
        rules['B'] = {"bC"};
        rules['C'] = {"aA", "b"};
    }

    string generateString() {
        string result = "S";
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
        return result;
    }

    void generateFiveStrings() {
        srand(time(0));
        for (int i = 0; i < 5; i++) {
            cout << generateString() << endl;
        }
    }

    FiniteAutomaton toFiniteAutomaton() {
        FiniteAutomaton fa;
        fa.startState = 'S';

        for (auto& rule : rules) {
            char fromState = rule.first;
            for (string production : rule.second) {
                if (production.size() == 2) {
                    fa.transitions[{fromState, production[0]}] = production[1];
                    fa.states.insert(fromState);
                    fa.states.insert(production[1]);
                    fa.alphabet.insert(production[0]);
                } else {
                    fa.transitions[{fromState, production[0]}] = production[0];
                    fa.finalStates.insert(production[0]);
                    fa.states.insert(production[0]);
                }
            }
        }
        return fa;
    }
};
```

The FiniteAutomaton class represents a finite automaton with states, an alphabet, transitions, a start state, and final states. The validateString() method checks if a given string is accepted by the automaton by following the transitions from the start state and ensuring the string ends in a valid final state.

```c++
class FiniteAutomaton {
public:
    set<char> states;
    set<char> alphabet;
    map<pair<char, char>, char> transitions;
    char startState;
    set<char> finalStates;

    FiniteAutomaton() {}

    bool validateString(const string& input) {
        char currentState = startState;
        for (char symbol : input) {
            if (transitions.find({currentState, symbol}) == transitions.end())
                return false;
            currentState = transitions[{currentState, symbol}];
        }
        return finalStates.count(currentState) > 0;
    }
};
```
## Conclusions
In this lab, I implemented a Grammar class that defines a formal grammar using production rules. I added functionality to generate five valid strings based on the grammar and created a method to convert the grammar into a FiniteAutomaton. The FiniteAutomaton class was then enhanced with a method to validate if an input string is accepted by the automaton. This exercise helped reinforce the concepts of formal languages, grammars, and finite automata, as well as their interrelation in language generation and validation. Through this lab, I deepened my understanding of basic formal languages and finite automaton, as well as learning how to create valid words/strings, according to a specified grammar, through programming.

### Output

![Results](https://github.com/user-attachments/assets/435a52f6-cbc9-4a34-85fb-43f64fb41c61)

Results after running the code.
