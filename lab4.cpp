#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>

using namespace std;

const int SLEEP = 300;                // Delay between characters
const int MAX_REPETITIONS = 10;      // Max repeats for *, +, and ^*
const int N_STRINGS = 3;            // Number of strings to be generated for each pattern

string generateFromRegex(const string &pattern, size_t &i);

// Handles group patterns: (...) with a|b alternation
string parseGroup(const string &pattern, size_t &i) {
    vector<string> options;
    string current;

    ++i; // Skip '('
    while (i < pattern.size() && pattern[i] != ')') {
        if (pattern[i] == '|') {
            options.push_back(current);
            current.clear();
            ++i;
        } else if (pattern[i] == '(') {
            current += parseGroup(pattern, i); // Nested group
        } else {
            current += pattern[i++];
        }
    }
    ++i; // Skip ')'
    options.push_back(current); // Add last option

    return options[rand() % options.size()]; // Random choice
}

// Parses and generates string from regex-like pattern
string generateFromRegex(const string &pattern, size_t &i) {
    string result;
    string lastToken;

    while (i < pattern.size()) {
        char c = pattern[i];

        if (c == '(') {
            string group = parseGroup(pattern, i);
            result += group;
            lastToken = group;
        } else if (c == '|') {
            ++i; // Skip, handled in parseGroup
        } else if (c == '*') {
            int reps = rand() % MAX_REPETITIONS;
            for (int j = 0; j < reps; ++j) result += lastToken;
            ++i;
        } else if (c == '+') {
            int reps = 1 + rand() % MAX_REPETITIONS;
            for (int j = 1; j < reps; ++j) result += lastToken;
            ++i;
        } else if (c == '?') {
            if (rand() % 2 == 0) result.erase(result.size() - lastToken.size());
            ++i;
        } else if (c == '^') {
            ++i;
            if (i < pattern.size() && pattern[i] == '*') {
                ++i;
                int reps = rand() % MAX_REPETITIONS;
                for (int j = 0; j < reps; ++j) result += lastToken;
            } else {
                int count = 0;
                while (i < pattern.size() && isdigit(pattern[i])) {
                    count = count * 10 + (pattern[i] - '0');
                    ++i;
                }
                for (int j = 1; j < count; ++j) result += lastToken; // ^N includes 1 original + N-1 more
            }
        } else if (c == ' ') {
            ++i; // Ignore spaces
        } else {
            result += c;
            lastToken = string(1, c);
            ++i;
        }
    }

    return result;
}

// Generates and prints N_STRINGS strings from a pattern
void buildFromPattern(const string &pattern) {
    for (int i = 0; i < N_STRINGS; ++i) {
        size_t index = 0;
        string result = generateFromRegex(pattern, index);
        for (char c : result) {
            cout << c;
            this_thread::sleep_for(chrono::milliseconds(SLEEP));
        }
        cout << endl;
    }
}

int main() {
    srand(time(0)); // Seed randomness

    vector<string> regexPatterns = {
        "(a|b)(c|d)E+G?",
        "P(Q|R|S)T(UV|W|X)*Z+",
        "1(0|1)*2(3|4)^5 36",
        // "(Ha|He|Ho)^* (wo|wa)^3 !+(x|y|z)?"
    };

    for (const string &pattern : regexPatterns) {
        cout << "Generated strings for: " << pattern << endl;
        buildFromPattern(pattern);
    }

    return 0;
}
