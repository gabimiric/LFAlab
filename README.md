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
   shown). Be careful that idea is to interpret the given regular expressions dynamically, not to hardcode the way it
   will
   generate valid strings. You give a set of regexes as input and get valid word as an output

   b. In case you have an example, where symbol may be written undefined number of times, take a limit of 5 times (to
   evade
   generation of extremely long combinations);

   c. Bonus point: write a function that will show sequence of processing regular expression (like, what you do first,
   second and so on)

## Implementation description

This part of the generateFromRegex function handles the custom repetition operator ```^```, allowing either random or fixed
repetitions of the previous token. If followed by ```*```, it repeats lastToken a random number of times up to
MAX_REPETITIONS. If followed by digits (e.g., ```^5```), it repeats the token exactly that number of times, adding ```count - 1```
copies since one instance already exists. This is just a small segment of the full generateFromRegex logic, which parses
and builds a string based on the full regex-like pattern.

```c++
        else if (c == '^') {
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
        }
```

This section of parseGroup handles the parsing of group expressions enclosed in parentheses, such as ```(a|b|c)```. It first
skips the opening parenthesis, then iterates through the characters until it finds the closing one. Inside the loop, it
collects characters into a current option, splitting them at each ```|```, and recursively processes nested groups if
another ```(``` is encountered. Once the closing ```)``` is found, it adds the final option and returns a randomly selected one from
the collected list.

```c++
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
    options.push_back(current); // Add last option buildFromPattern(pattern);
    }
```

This part in main initializes a list of regex-like patterns and generates example strings from each. It stores several
pattern strings in the regexPatterns vector, demonstrating different features like alternation ```(a|b)```, repetition ```*```, ```+```,
optional elements ```?```, and custom repetition using ```^```. For each pattern, it prints a heading and calls buildFromPattern,
which generates and displays five randomized strings based on that pattern. This serves as a testbed to visually confirm
the pattern parser and generator work correctly.

```c++
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
```

## Conclusions

In this lab, I focused on working with regular grammar to define formal languages using production rules and implemented a parser to interpret and process simplified regular expressions. I developed functionality to dynamically generate valid strings based on given patterns by parsing them and handling operations such as alternation, repetition, optionality, and custom repetition counts. The parser played a key role in breaking down complex expressions, managing nested groups, and applying randomization to reflect the flexible nature of regular grammars. Through building this parser and string generator, I gained a deeper understanding of how regular grammars operate and how they can be effectively used for language generation.

### Output

![Results](https://github.com/user-attachments/assets/92134b18-87a9-448f-88c1-e98c6557d2d8)

Results after running the code.
