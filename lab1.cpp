#include <bits/stdc++.h>

using namespace std;

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

int main() {
    Grammar grammar;
    cout<<"Generated Strings:\n";
    grammar.generateFiveStrings();

    FiniteAutomaton fa = grammar.toFiniteAutomaton();

    cout<<"\nFinite Automata Checking:\n";
    vector<string> testStrings = {"ab", "aba", "abaabb", "aabaabb", "aabb"};
    for (const string& str : testStrings) {
        cout << "Checking string '" << str << "': " << (fa.validateString(str) ? "Accepted" : "Rejected") << endl;
    }

    return 0;
}
