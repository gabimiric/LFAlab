# Lexer

### Course: Formal Languages & Finite Automata
### Author: Miricinschi Gabriel, FAF-233

----

## Theory

A lexer (short for lexical analyzer) is a fundamental component of a compiler or interpreter responsible for breaking down input text into meaningful units called tokens. It is the first phase of a language processing pipeline, converting a raw sequence of characters into structured elements that a parser can work with. Lexers are commonly used in programming languages, scripting engines, and even command-line interpreters to understand user input.

Lexical analysis involves scanning an input string and categorizing substrings based on predefined rules. These rules define the syntax of tokens, such as keywords, identifiers, numbers, operators, and punctuation. For example, in a programming language, int x = 10; would be broken into tokens like "int" (keyword), "x" (identifier), "=" (operator), and "10" (integer). Regular expressions and finite automata are often used to define and implement these tokenization rules efficiently.

A well-designed lexer must handle whitespace, comments, and errors gracefully. Whitespace and comments are typically ignored unless they are significant in the language (e.g., indentation in Python). Error handling ensures that invalid sequences, such as an unexpected character in a number, are flagged early in the processing pipeline. In some cases, lexers support lookahead, where they peek at upcoming characters to determine token boundaries more accurately.

Lexers can be implemented in various ways, including handwritten code using conditional statements or lexer generators like Lex, Flex, or ANTLR, which automate the process based on a formal grammar. A state machine approach is also common, where different states represent different token types, and transitions occur based on character input. The choice of implementation depends on factors like performance, maintainability, and language complexity.

## Objectives:

1. Understand what lexical analysis [1] is.
   
3. Get familiar with the inner workings of a lexer/scanner/tokenizer.
   
5. Implement a sample lexer and show how it works. 
    

## Implementation description
The following part of my lexer defines regular expressions to identify different types of tokens in a command-line input. The patterns array consists of three regex patterns: R"(^/\w+)" matches a command that starts with / (e.g., /roll), R"(\b\w+\b)" captures general words as arguments (e.g., Alice, STR), and R"(\b\d+\b)" detects numeric values (e.g., 20, 18). The types array assigns corresponding TokenType values (COMMAND, ARG, NUMBER) to each pattern, ensuring that when the lexer scans input, it can categorize tokens correctly. This structure enables efficient tokenization of user commands, allowing the parser to interpret them based on their roles in the syntax.
```c++
regex patterns[] = {regex(R"(^/\w+)"), regex(R"(\b\w+\b)"), regex(R"(\b\d+\b)")};
TokenType types[] = {TokenType::COMMAND, TokenType::ARG, TokenType::NUMBER};
```
This code snippet processes an input string by iterating through each character and attempting to match predefined patterns using regular expressions. The loop begins by setting pos to 0, which indicates the starting position in the input string. It then enters a while loop that continues as long as pos is less than the input string size. Inside the loop, the for loop tries to match each of the three regex patterns defined earlier (e.g., for commands, arguments, and numbers). If a match is found, the corresponding token is created and added to the tokens vector, along with its associated type from the types array. The position pos is then updated to skip over the matched part of the input. If no match is found, the code increments pos by 1 to skip the current character and continue searching for valid tokens. This ensures that every character is processed, and any unrecognized characters are ignored.
```c++
size_t pos = 0;
        while (pos < input.size()) // Iterate over the input string
        {
            bool matched = false;
            for (int i = 0; i < 3; i++) // Try to match each pattern
            {
                smatch match;
                if (regex_search(input.begin() + pos, input.end(), match, patterns[i])) // If a match is found
                {
                    tokens.push_back({types[i], match.str()}); // Create token and add to vector
                    pos += match.position() + match.length(); // Move position to the end of the matched part
                    matched = true;
                    break;
                }
            }
            if (!matched) pos++; // Skip unknown characters
        }
```
This snippet handles the /roll command by determining the number of sides on the dice and rolling it accordingly. By default, it assumes a six-sided die (d6). If the user provides a specific die type (e.g., /roll d20), the lexer checks if the second token exists and starts with 'd'. If so, it extracts the number after 'd' using substr(1) and converts it to an integer with stoi(). Finally, it calls rollDice(sides), which simulates rolling a die with the specified number of sides, and prints the result. This ensures flexibility while maintaining a default behavior if no die type is specified.
```c++
if (command == "/roll")
        {
            int sides = 6; // Default dice is d6
            if (tokens.size() > 1 && tokens[1].value[0] == 'd') // Check if a specific dice is mentioned (e.g., d20)
            {
                sides = stoi(tokens[1].value.substr(1)); // Extract the number of sides (e.g., 20 from d20)
            }
            cout << "Dice rolled: " << rollDice(sides) << "\n"; // Output the result of the dice roll
        }
```
## Conclusions
In this lab, I developed a lexer to tokenize command-line input for a DnD campaign system. I implemented regular expressions to identify commands, arguments, and numbers, ensuring accurate parsing of user input. The lexer processes input efficiently by iterating through characters and matching predefined token patterns. Additionally, I handled specific commands like /roll by extracting relevant arguments and executing the appropriate actions. This lab strengthened my understanding of lexical analysis and its role in command processing, reinforcing key concepts in text parsing, tokenization, and structured input interpretation.

### Output

![Results](https://github.com/user-attachments/assets/d6141b75-02b5-40a3-a5be-4d899a4bd6c3)

Results after running the code.
