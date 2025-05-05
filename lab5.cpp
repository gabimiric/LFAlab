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

    void eliminateEpsilon()
    {
        set<string> nullable;
        set<string> fullyNullable;

        // Step 1: Find nullable non-terminals (those that produce ε directly)
        for (const auto &[nt, prods]: productions)
        {
            if (prods.count("ε"))
            {
                nullable.insert(nt);
            }
        }

        // Identify fully nullable non-terminals (only ε as production)
        for (const string &nt: nullable)
        {
            if (productions[nt].size() == 1 && productions[nt].count("ε"))
            {
                fullyNullable.insert(nt);
            }
        }

        // Step 2: Remove ε productions (we'll handle replacements in step 3)
        for (auto &[nt, prods]: productions)
        {
            prods.erase("ε");
        }

        // Step 3: For each production, generate new forms by omitting nullable symbols
        map<string, set<string> > newProductions;

        for (const auto &[nt, prods]: productions)
        {
            for (const string &prod: prods)
            {
                vector<string> parts = tokenize(prod);
                int n = parts.size();

                // Remove fully nullable symbols entirely
                vector<string> filtered;
                for (const string &p: parts)
                {
                    if (!fullyNullable.count(p))
                        filtered.push_back(p);
                }

                if (filtered.empty())
                    continue;

                // Generate all subsets by optionally removing other nullable symbols
                int total = 1 << filtered.size();
                for (int mask = 0; mask < total; ++mask)
                {
                    vector<string> newParts;
                    for (int i = 0; i < filtered.size(); ++i)
                    {
                        if (!(mask & (1 << i)) || nullable.count(filtered[i]) == 0)
                        {
                            newParts.push_back(filtered[i]);
                        }
                    }
                    if (!newParts.empty())
                    {
                        newProductions[nt].insert(join(newParts, ""));
                    }
                }
            }
        }

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

    void printGrammar(const map<string, set<string> > &prods)
    {
        cout << "\nCurrent Grammar:\n";
        for (const auto &[lhs, rhsSet]: prods)
        {
            for (const string &rhs: rhsSet)
            {
                cout << lhs << " -> " << rhs << endl;
            }
        }
    }

    void toCNF() {
    cout << "Original Grammar:";
    printGrammar(productions);

    eliminateEpsilon();
    cout << "\nAfter eliminateEpsilon:";
    printGrammar(productions);

    eliminateUnitProductions();
    cout << "\nAfter eliminateUnitProductions:";
    printGrammar(productions);

    eliminateInaccessibleSymbols();
    cout << "\nAfter eliminateInaccessibleSymbols:";
    printGrammar(productions);

    eliminateNonProductiveSymbols();
    cout << "\nAfter eliminateNonProductiveSymbols:";
    printGrammar(productions);

    // Create terminal-to-nonterminal mappings and add productions
    map<string, string> terminalToNonTerminal;
    for (const auto& terminal : terminals) {
        string newNT = "T_" + terminal;
        terminalToNonTerminal[terminal] = newNT;
        nonTerminals.insert(newNT);
    }

    // Map to store patterns of right-hand sides to avoid duplicate X variables
    map<string, string> patternToVar;
    int varCounter = 0;

    // Create a new set of productions in CNF
    map<string, set<string>> cnfProductions;

    // First, add the terminal productions
    for (const auto& [terminal, nt] : terminalToNonTerminal) {
        cnfProductions[nt].insert(terminal);
    }

    // Process each original production
    for (const auto& [lhs, rhsSet] : productions) {
        for (const auto& rhs : rhsSet) {
            vector<string> symbols = tokenize(rhs);

            // Case 1: A -> a (single terminal)
            if (symbols.size() == 1 && terminals.count(symbols[0])) {
                cnfProductions[lhs].insert(terminalToNonTerminal[symbols[0]]);
                continue;
            }

            // Case 2: A -> B (single non-terminal)
            if (symbols.size() == 1 && nonTerminals.count(symbols[0])) {
                // This will be handled in the unit production elimination
                cnfProductions[lhs].insert(symbols[0]);
                continue;
            }

            // Case 3: Replace terminals with their non-terminals
            for (size_t i = 0; i < symbols.size(); ++i) {
                if (terminals.count(symbols[i])) {
                    symbols[i] = terminalToNonTerminal[symbols[i]];
                }
            }

            // Case 4: A -> BC (already in correct form)
            if (symbols.size() == 2) {
                cnfProductions[lhs].insert(symbols[0] + " " + symbols[1]);
                continue;
            }

            // Case 5: A -> BCD... (need to introduce new non-terminals)
            if (symbols.size() > 2) {
                string currentLHS = lhs;
                vector<string> currentRHS = symbols;

                while (currentRHS.size() > 2) {
                    // Take first two symbols
                    vector<string> firstTwo = {currentRHS[0], currentRHS[1]};
                    string firstTwoStr = firstTwo[0] + " " + firstTwo[1];
                    string newNT;

                    // Check if we've seen this pattern before
                    if (patternToVar.count(firstTwoStr)) {
                        newNT = patternToVar[firstTwoStr];
                    } else {
                        newNT = "X" + to_string(varCounter++);
                        nonTerminals.insert(newNT);
                        patternToVar[firstTwoStr] = newNT;
                        cnfProductions[newNT].insert(firstTwoStr);
                    }

                    // Update current RHS: remove first two, insert new NT at beginning
                    currentRHS.erase(currentRHS.begin(), currentRHS.begin() + 2);
                    currentRHS.insert(currentRHS.begin(), newNT);
                }

                // After the loop, currentRHS should have exactly 2 symbols
                cnfProductions[currentLHS].insert(currentRHS[0] + " " + currentRHS[1]);
            }
        }
    }

    // Handle any remaining unit productions
    map<string, set<string>> finalProductions = cnfProductions;
    bool changed;

    do {
        changed = false;
        map<string, set<string>> tempProductions = finalProductions;

        for (const auto& [lhs, rhsSet] : finalProductions) {
            for (const string& rhs : rhsSet) {
                vector<string> parts = tokenize(rhs);

                if (parts.size() == 1 && nonTerminals.count(parts[0])) {
                    // This is a unit production A -> B
                    if (finalProductions.count(parts[0])) {
                        for (const string& replacement : finalProductions[parts[0]]) {
                            vector<string> replacementParts = tokenize(replacement);

                            // Only add if it's not another unit production
                            if (replacementParts.size() != 1 || !nonTerminals.count(replacementParts[0])) {
                                if (tempProductions[lhs].insert(replacement).second) {
                                    changed = true;
                                }
                            }
                        }
                    }

                    // Remove the unit production
                    tempProductions[lhs].erase(rhs);
                    if (tempProductions[lhs].empty()) {
                        tempProductions.erase(lhs);
                    }
                    changed = true;
                }
            }
        }

        if (changed) {
            finalProductions = tempProductions;
        }
    } while (changed);

    // Update the original productions with our CNF productions
    productions = finalProductions;

    cout << "\nFinal CNF Grammar:";
    printGrammar(productions);
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

    // set<string> nonTerminals = {"S", "A", "B", "C", "E"};
    // set<string> terminals = {"a", "b"};
    // map<string, set<string> > productions = {
    //     {"S", {"bA", "B"}},
    //     {"A", {"a", "aS", "bAaAb"}},
    //     {"B", {"AC", "bS", "aAa"}},
    //     {"C", {"ε", "AB"}},
    //     {"E", {"BA"}}
    // };
    // string startSymbol = "S";

    // Create a Grammar object
    Grammar grammar(nonTerminals, terminals, productions, startSymbol);

    // Convert the grammar to CNF
    grammar.toCNF();

    return 0;
}
