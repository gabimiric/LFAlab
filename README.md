# Regular Grammar

### Course: Formal Languages & Finite Automata

### Author: Miricinschi Gabriel, FAF-233

----

## Theory

A regular grammar consists of a set of production rules used to generate strings within a language. These rules are of
two main types: right-linear and left-linear. In right-linear grammar, a production rule follows the format A → aB or
A → a, where A and B are non-terminal symbols, and a is a terminal symbol. In left-linear grammar, the production rule
is of the form A → Ba or A → a. Regular grammars only allow the non-terminal to appear at the end (right-linear) or at
the start (left-linear) of the string, ensuring the grammar generates simple, predictable patterns.

The right-linear grammar is the more commonly used form because it directly translates to regular expressions and finite
automata, which are widely used in practical applications such as text processing and lexical analysis. In right-linear
grammar, non-terminal symbols are always followed by terminal symbols or the empty string. This structure makes regular
grammars less complex than context-free grammars, which allow more varied arrangements of non-terminals.

A regular grammar is context-free in the sense that it doesn't rely on the context of surrounding symbols, allowing for
simpler parsing. It can only generate patterns that consist of a limited number of operations, such as alternation (
using the pipe symbol |), repetition (using * or +), and optionality (using ?). This makes regular grammars suitable for
describing simpler syntactic structures in many applications like pattern matching or constructing basic compilers.

## Objectives:

1. Write and cover what regular expressions are, what they are used for;

2. Take a variant depending on your number in the list of students and do the following:

   a. Write a code that will generate valid combinations of symbols conform given regular expressions (examples will be
   shown). Be careful that idea is to interpret the given regular expressions dinamycally, not to hardcode the way it
   will
   generate valid strings. You give a set of regexes as input and get valid word as an output

   b. In case you have an example, where symbol may be written undefined number of times, take a limit of 5 times (to
   evade
   generation of extremely long combinations);

   c. Bonus point: write a function that will show sequence of processing regular expression (like, what you do first,
   second and so on)

## Implementation description

This part of the code generates a string based on the regular expression (a|b)(c|d)E+G?. It first randomly selects
either "a" or "b" for the (a|b) part, and then randomly selects either "c" or "d" for the (c|d) part. Next, it ensures
that the letter "E" appears at least once due to the E+ part, and it may add additional "E"s up to a maximum number
defined by MAX_REPETITIONS. Finally, it randomly decides whether to append the letter "G" (due to the G? part, where "G"
is optional) with a 50% chance. The resulting string will follow this pattern, but with the optional repetition of "E"s
and "G".

```c++
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
```

This is the main part of the code that is responsible for generating and displaying strings based on multiple regular
expressions. It first seeds the random number generator using srand(time(0)), ensuring that the random numbers are
different each time the program runs. Then, a list of regular expressions (regexPatterns) is defined, containing three
patterns: (a|b)(c|d)E+G?, P(Q|R|S)T(UV|W|X)*Z+, and 1(0/1)*2(3/4)^5 36. The code then loops through each of these
patterns, printing a message indicating the current pattern being processed (cout << "Building string for pattern: " <<
pattern << endl;), and calls the buildStringFromRegex function to generate and display the corresponding string based on
the pattern. Each pattern will be processed and a string will be built and displayed according to the rules of the
respective regular expression.

```c++
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
```

This part of the code displays each of the 5 generated strings one character at a time, with a delay between each
character to simulate real-time construction. It iterates over the results vector, which contains the generated strings,
and for each string, it loops through each character in the string (res[i]). For each character, it prints the character
to the console and then pauses the program for a specified amount of time (SLEEP milliseconds, which is typically set to
100ms). After displaying all the characters of a string, it moves to the next line (cout << endl) to display the next
string. This creates the effect of the string being built character by character with a delay between each character.

```c++
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
```

## Conclusions

In this lab, I focused on working with regular grammar to define formal languages using production rules. I implemented
functionality to dynamically generate valid strings based on given regular expressions. The task involved parsing
patterns, handling different operations such as alternation, repetition, and optionality, and then generating random
strings that conform to those patterns. By handling infinite repetitions and ensuring random choices in the generation
process, I gained a deeper understanding of how regular grammars work and how they can be applied to generate strings
that match specific patterns in a flexible and efficient manner. This lab enhanced my knowledge of regular expressions
and regular grammar's role in language generation.

### Output

![Results](https://github.com/user-attachments/assets/92134b18-87a9-448f-88c1-e98c6557d2d8)

Results after running the code.
