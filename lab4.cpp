#include <iostream>
#include <vector>
#include <regex>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <sstream>

using namespace std;

// Max repetitions and sleep timer
const int MAX_REPETITIONS = 10;
const int SLEEP = 200;

// Function to generate a random element from a given set of options
string getRandomChoice(const vector<string> &choices)
{
    return choices[rand() % choices.size()];
}

// Function to generate a random string based on specific patterns
string generateStringFromPattern(const string &pattern)
{
    string result;

    // Handle specific parts of the regex
    if (pattern == "(a|b)(c|d)E+G?")
    {
        // First part (a|b)
        result += getRandomChoice({"a", "b"});
        // Second part (c|d)
        result += getRandomChoice({"c", "d"});
        // E+ (E at least once, so we add one 'E' and can add more)
        result += "E"; // E must be present
        for (int i = 0; i < rand() % MAX_REPETITIONS; ++i)
        {
            // Add 0-MAX_REPETITIONS more Es
            result += "E";
        }
        // G? (G is optional, can appear 0 or 1 time)
        if (rand() % 2)
        {
            result += "G";
        }
    }
    else if (pattern == "P(Q|R|S)T(UV|W|X)*Z+")
    {
        // P(Q|R|S)
        result += "P" + getRandomChoice({"Q", "R", "S"});

        // T(UV|W|X)*
        result += "T";
        string choice = getRandomChoice({"UV", "W", "X"});
        int repetitions = rand() % MAX_REPETITIONS; // Repeat 0-MAX_REPETITIONS times
        for (int i = 0; i < repetitions; ++i)
        {
            result += choice; // Append the same choice each time
        }

        // Z+ (Z at least once)
        result += "Z";
        for (int i = 0; i < rand() % MAX_REPETITIONS; ++i)
        {
            // Add 0-MAX_REPETITIONS more Zs
            result += "Z";
        }
    }

    else if (pattern == "1(0/1)*2(3/4)^5 36")
    {
        // 1(0/1)*2(3/4)^5
        result += "1";
        string choice = getRandomChoice({"0", "1"});
        for (int i = 0; i < rand() % MAX_REPETITIONS; ++i)
        {
            // Repeat the same choice for 0-MAX_REPETITIONS times
            result += choice;
        }
        result += "2";
        choice = getRandomChoice({"3", "4"});
        for (int i = 0; i < 5; ++i)
        {
            // Repeat the same choice for exactly 5 times (for (3/4)^5)
            result += choice;
        }
        // 36
        result += "36";
    }
    return result;
}

// Function to simulate the construction of strings from a regex pattern
void buildStringFromRegex(const string &pattern)
{
    vector<string> results;

    // Generate 5 random strings based on the pattern
    for (int i = 0; i < 5; ++i)
    {
        string result = generateStringFromPattern(pattern);
        results.push_back(result);
    }

    // Display the results with a delay between each string for real-time construction
    for (const auto &res: results)
    {
        for (size_t i = 0; i < res.length(); ++i)
        {
            cout << res[i];
            this_thread::sleep_for(chrono::milliseconds(SLEEP)); // Delay of 100ms between characters
        }
        cout << endl;
    }
}

int main()
{
    srand(time(0)); // Seed for random generation

    // Regular expressions as input
    vector<string> regexPatterns = {
        "(a|b)(c|d)E+G?",
        "P(Q|R|S)T(UV|W|X)*Z+",
        "1(0/1)*2(3/4)^5 36"
    };

    // Process each pattern dynamically
    for (const string &pattern: regexPatterns)
    {
        cout << "Building string for pattern: " << pattern << endl;
        buildStringFromRegex(pattern);
    }

    return 0;
}
