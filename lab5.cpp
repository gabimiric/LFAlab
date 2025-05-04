#include <bits/stdc++.h>

using namespace std;

class Grammar {
public:
    // Structures to hold the grammar components
    set<string> nonTerminals;
    set<string> terminals;
    map<string, set<string> > productions;
    string startSymbol;

    // Constructor to initialize the grammar from the given rules
    Grammar(set<string> nonTerminals, set<string> terminals,
            map<string, set<string> > productions, string startSymbol)
        : nonTerminals(nonTerminals), terminals(terminals), productions(productions), startSymbol(startSymbol) {}

    vector<string> tokenize(const string &str)
    {
        vector<string> result;
        for (char c: str)
        {
            string s(1, c);
            result.push_back(s);
        }
        return result;
    }

    void eliminateEpsilon() {
    set<string> nullable;

    // Step 1: Find nullable non-terminals
    for (auto &nonTerminal : nonTerminals) {
        for (auto &prod : productions[nonTerminal]) {
            if (prod == "ε") {
                nullable.insert(nonTerminal);
            }
        }
    }

    // Step 2: Remove epsilon productions (C -> ε)
    for (auto &nonTerminal : nonTerminals) {
        for (auto &prod : productions[nonTerminal]) {
            if (prod == "ε") {
                productions[nonTerminal].erase(prod);
            }
        }
    }

    // Step 3: Completely remove nullable non-terminals from all productions
    map<string, set<string>> newProductions;

    for (auto &nonTerminal : nonTerminals) {
        set<string> updatedProds;  // Set to store the updated productions for this non-terminal
        for (auto &prod : productions[nonTerminal]) {
            // Tokenize production into parts and remove any nullable non-terminals
            vector<string> prodParts = tokenize(prod);
            string updatedProd = "";
            for (const auto &part : prodParts) {
                // Only add the part if it's not a nullable non-terminal
                if (nullable.find(part) == nullable.end()) {
                    updatedProd += part;
                }
            }

            // If the production is non-empty, add it to the set of updated productions
            if (!updatedProd.empty()) {
                updatedProds.insert(updatedProd);
            }
        }

        // Store the updated productions in the new production set
        newProductions[nonTerminal] = updatedProds;
    }

    // Step 4: Remove nullable non-terminals from the grammar
    for (const auto& nonTerminal : nullable) {
        nonTerminals.erase(nonTerminal);  // Remove nullable non-terminal
        newProductions.erase(nonTerminal);  // Remove its productions
    }

    // Update the productions after removing nullable non-terminals
    productions = newProductions;
}



    // Method to eliminate unit productions
    void eliminateUnitProductions()
    {
        // Step 1: Find unit productions (A → B where both A and B are non-terminals)
        for (auto &nonTerminal: nonTerminals)
        {
            set<string> unitProductions;
            for (auto &prod: productions[nonTerminal])
            {
                if (nonTerminals.find(prod) != nonTerminals.end())
                {
                    unitProductions.insert(prod);
                }
            }

            // Step 2: Replace unit productions with their corresponding productions
            for (auto &unit: unitProductions)
            {
                for (auto &unitProd: productions[unit])
                {
                    productions[nonTerminal].insert(unitProd);
                }
                productions[nonTerminal].erase(unit); // Remove the unit production
            }
        }
    }

    // Method to eliminate inaccessible symbols
    void eliminateInaccessibleSymbols()
    {
        set<string> accessible;
        accessible.insert(startSymbol);

        // Step 1: Find all accessible symbols
        bool changed = true;
        while (changed)
        {
            changed = false;
            for (auto &nonTerminal: nonTerminals)
            {
                for (auto &prod: productions[nonTerminal])
                {
                    vector<string> prodParts = tokenize(prod);
                    for (auto &part: prodParts)
                    {
                        if (nonTerminals.find(part) != nonTerminals.end() && accessible.find(part) == accessible.end())
                        {
                            accessible.insert(part);
                            changed = true;
                        }
                    }
                }
            }
        }

        // Step 2: Remove inaccessible symbols
        for (auto it = nonTerminals.begin(); it != nonTerminals.end();)
        {
            if (accessible.find(*it) == accessible.end())
            {
                it = nonTerminals.erase(it);
            }
            else
            {
                ++it;
            }
        }

        for (auto it = productions.begin(); it != productions.end();)
        {
            if (accessible.find(it->first) == accessible.end())
            {
                it = productions.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    // Method to eliminate non-productive symbols
    void eliminateNonProductiveSymbols()
    {
        set<string> productive;

        // Step 1: Find productive non-terminals (those that can eventually derive terminals)
        bool changed = true;
        while (changed)
        {
            changed = false;
            for (auto &nonTerminal: nonTerminals)
            {
                for (auto &prod: productions[nonTerminal])
                {
                    vector<string> prodParts = tokenize(prod);
                    bool isProductive = true;
                    for (auto &part: prodParts)
                    {
                        if (nonTerminals.find(part) != nonTerminals.end() && productive.find(part) == productive.end())
                        {
                            isProductive = false;
                            break;
                        }
                    }
                    if (isProductive)
                    {
                        if (productive.find(nonTerminal) == productive.end())
                        {
                            productive.insert(nonTerminal);
                            changed = true;
                        }
                    }
                }
            }
        }

        // Step 2: Remove non-productive symbols
        for (auto it = nonTerminals.begin(); it != nonTerminals.end();)
        {
            if (productive.find(*it) == productive.end())
            {
                it = nonTerminals.erase(it);
            }
            else
            {
                ++it;
            }
        }

        for (auto it = productions.begin(); it != productions.end();)
        {
            if (productive.find(it->first) == productive.end())
            {
                it = productions.erase(it);
            }
            else
            {
                for (auto &prod: it->second)
                {
                    vector<string> prodParts = tokenize(prod);
                    bool removeProd = false;
                    for (auto &part: prodParts)
                    {
                        if (nonTerminals.find(part) != nonTerminals.end() && productive.find(part) == productive.end())
                        {
                            removeProd = true;
                            break;
                        }
                    }
                    if (removeProd)
                    {
                        it->second.erase(prod);
                    }
                }
                ++it;
            }
        }
    }

    string join(const vector<string> &parts, const string &delim = " ")
    {
        stringstream ss;
        for (size_t i = 0; i < parts.size(); ++i)
        {
            if (i > 0) ss << delim;
            ss << parts[i];
        }
        return ss.str();
    }

    void toCNF() {
    eliminateEpsilon();
    eliminateUnitProductions();
    eliminateInaccessibleSymbols();
    eliminateNonProductiveSymbols();

    map<string, set<string>> updatedProductions;
    map<string, string> terminalToNonTerminal;
    int varCounter = 1;

    // Step 1: Replace terminals inside longer rules
    for (auto& [lhs, rhsSet] : productions) {
        for (string rule : rhsSet) {
            vector<string> parts = tokenize(rule);

            // Step 1.1: Replace terminals in longer rules with non-terminals
            for (int i = 0; i < parts.size(); ++i) {
                string symbol = parts[i];
                if (terminals.count(symbol) && parts.size() > 1) {
                    // Create a non-terminal for this terminal if it doesn't exist yet
                    if (!terminalToNonTerminal.count(symbol)) {
                        string tempNT = "T_" + symbol;
                        terminalToNonTerminal[symbol] = tempNT;
                        nonTerminals.insert(tempNT);
                        updatedProductions[tempNT].insert(symbol);  // T_a -> a
                    }
                    parts[i] = terminalToNonTerminal[symbol];  // Replace terminal with its non-terminal
                }
            }

            // Step 2: Convert to binary productions if necessary
            if (parts.size() == 1) {
                updatedProductions[lhs].insert(parts[0]);
            } else if (parts.size() == 2) {
                updatedProductions[lhs].insert(join(parts, " "));
            } else {
                // Convert long productions into binary form
                string prev = lhs;
                for (size_t i = 0; i < parts.size() - 2; ++i) {
                    string newNT = "X" + to_string(varCounter++);
                    updatedProductions[prev].insert(parts[i] + " " + newNT);
                    nonTerminals.insert(newNT);
                    prev = newNT;
                }
                updatedProductions[prev].insert(parts[parts.size() - 2] + " " + parts[parts.size() - 1]);
            }
        }
    }

    // Assign the updated productions back to the main production map
    productions = updatedProductions;
}



    void printGrammar()
    {
        for (const auto &nonTerminal: nonTerminals)
        {
            if (productions.count(nonTerminal) && !productions[nonTerminal].empty())
            {
                cout << nonTerminal << " -> ";
                const auto &rules = productions[nonTerminal];
                size_t count = 0;
                for (const auto &prod: rules)
                {
                    cout << prod;
                    if (++count < rules.size()) cout << " | ";
                }
                cout << endl;
            }
        }
    }
};

int main()
{
    // Define the grammar as per Variant 19
    set<string> nonTerminals = {"S", "A", "B", "C", "E"};
    set<string> terminals = {"a", "b", "d"};
    map<string, set<string> > productions = {
        {"S", {"dB", "B"}},
        {"A", {"d", "dS", "aAdCB"}},
        {"B", {"aC", "bA", "AC"}},
        {"C", {"ε"}},
        {"E", {"AS"}}
    };
    string startSymbol = "S";


    // Create a Grammar object
    Grammar grammar(nonTerminals, terminals, productions, startSymbol);

    // Convert the grammar to CNF
    grammar.toCNF();

    // Print the resulting CNF grammar
    grammar.printGrammar();

    return 0;
}
