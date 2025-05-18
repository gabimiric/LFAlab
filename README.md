# Parser & Building an Abstract Syntax Tree

### Course: Formal Languages & Finite Automata

### Author: Miricinschi Gabriel, FAF-233

----

## Theory

A parser is a crucial component in the process of translating source code into a format that a computer can understand and manipulate. It operates by taking a stream of tokens—typically produced by a lexical analyzer or lexer—and organizing them into a structure that reflects the grammatical rules of a programming language. This structure, called a parse tree, shows the syntactic arrangement of the input according to the language's grammar. Parsers are commonly used in compilers, interpreters, and static analysis tools to ensure that code follows valid syntax before further processing.

An Abstract Syntax Tree (AST) is a simplified, high-level representation of a program's structure derived from the parse tree. Unlike a parse tree, which reflects every detail of the syntax, an AST omits redundant syntactic elements such as parentheses and semicolons, focusing instead on the essential hierarchical structure of the code. Each node in the AST represents a language construct (e.g., expressions, statements, declarations), and its tree-like shape captures how these constructs are nested within one another. This abstraction allows for more efficient analysis and manipulation of code during later phases of compilation or interpretation.

Parsers typically build ASTs after performing syntactic validation. This AST becomes the foundation for subsequent operations such as semantic analysis, optimization, and code generation. Since ASTs provide a structured and easily traversable representation of code, they are also widely used in tools like linters, formatters, and source-to-source compilers (transpilers). Understanding the relationship between parsers and ASTs is essential for anyone involved in language design, compiler construction, or tooling development for programming languages.

## Objectives:

1. Get familiar with parsing, what it is and how it can be programmed [1].
2. Get familiar with the concept of AST [2].
3. In addition to what has been done in the 3rd lab work do the following:
   1. In case you didn't have a type that denotes the possible types of tokens you need to:
      1. Have a type __*TokenType*__ (like an enum) that can be used in the lexical analysis to categorize the tokens. 
      2. Please use regular expressions to identify the type of the token.
   2. Implement the necessary data structures for an AST that could be used for the text you have processed in the 3rd lab work.
   3. Implement a simple parser program that could extract the syntactic information from the input text.

## Implementation description

ASTNode is the abstract base class for all nodes in the abstract syntax tree, enforcing a consistent interface through getType() and print(). It also includes a utility function makePadding() to help visually structure the tree output. CommandNode extends ASTNode and serves as a common base for specific command-related nodes (like /roll, /player, etc.). It stores the command name and provides a basic implementation of print() to display it. Together, these classes form the structural foundation of the D&D-style command parser, enabling polymorphic behavior and clear, tree-based visualization.
```c++
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual NodeType getType() const = 0;
    virtual void print(int indent = 0, bool isLast = true) const = 0;
    static string makePadding(int indent, bool isLast) {
        string pad;
        for (int i = 0; i < indent - 1; ++i) pad += "|   ";
        if (indent > 0) pad += isLast ? "`-- " : "|-- ";
        return pad;
    }
};
class CommandNode : public ASTNode {
protected:
    string commandName;
public:
    CommandNode(const string &name) : commandName(name) {}
    NodeType getType() const override { return NodeType::COMMAND_NODE; }
    virtual string getCommandName() const { return commandName; }
    void print(int indent = 0, bool isLast = true) const override {
        cout << makePadding(indent, isLast) << "Command: " << commandName << '\n';
    }
};
```

This code defines types and structures used in parsing and representing commands in a simplified language. TokenType is an enumeration that categorizes lexical tokens such as commands, actions, names, stats, and numbers. The Token struct holds a token's type and its string value, serving as a basic unit of parsed input. NodeType is another enumeration that defines different types of Abstract Syntax Tree (AST) nodes, such as commands, rolls, and various entities like players or NPCs. Together, these components support the lexical and syntactic analysis of input for further processing.
```c++
enum class TokenType { COMMAND, ACTION, NAME, STAT, NUMBER, UNKNOWN }; // Token types

struct Token { TokenType type; string value; }; // Represents a token with its type and value

enum class NodeType { COMMAND_NODE, ROLL_NODE, STATS_NODE, PLAYER_NODE, NPC_NODE, INTERACTION_NODE }; // AST node types

```

This code snippet is a specialized AST node for handling dice roll commands like /roll d20. It inherits from CommandNode and stores the number of dice sides (e.g., 6, 20) in the sides variable. The getType() method identifies it as a ROLL_NODE, and getSides() exposes the number of sides for use in execution or display. The print() method outputs the command in a formatted tree structure, useful for debugging or visualization. This class supports structured parsing of roll commands in a D&D command interpreter.
```c++
class RollNode : public CommandNode {
    int sides;
public:
    RollNode(int s) : CommandNode("/roll"), sides(s) {}
    NodeType getType() const override { return NodeType::ROLL_NODE; }
    int getSides() const { return sides; }
    void print(int indent = 0, bool isLast = true) const override {
        cout << makePadding(indent, isLast) << "Roll Command: d" << sides << '\n';
    }
};
```

This print() method outputs a tree structure representing a /player command and its sub-actions. The root node is labeled "Command: player", with child branches for subcommands: add, remove, name, and show. Each branch checks if its respective action (stored in the action string) is being invoked and prints "called" if so, or "NULL" otherwise. For add and remove, it shows the target player's name if provided; for name, it displays both the old and new names involved in renaming. This structure visually maps the command and its parameters in a clear, hierarchical way for debugging or analysis.
```c++
    cout << makePadding(indent, isLast) << "Command: player\n";
    bool isAddCalled = (action == "add");
    cout << makePadding(indent + 1, false) << "add: " << (isAddCalled ? "called" : "NULL") << '\n';
    if (isAddCalled)
        cout << makePadding(indent + 2, true) << "PlayerName: " << (playerName.empty() ? "NULL" : playerName) << '\n';
    bool isRemoveCalled = (action == "remove");
    cout << makePadding(indent + 1, false) << "remove: " << (isRemoveCalled ? "called" : "NULL") << '\n';
    if (isRemoveCalled)
        cout << makePadding(indent + 2, true) << "PlayerName: " << (playerName.empty() ? "NULL" : playerName) << '\n';
    bool isNameCalled = (action == "name");
    cout << makePadding(indent + 1, false) << "name: " << (isNameCalled ? "called" : "NULL") << '\n';
    if (isNameCalled) {
        cout << makePadding(indent + 2, false) << "OldName: " << (playerName.empty() ? "NULL" : playerName) << '\n';
        cout << makePadding(indent + 2, true) << "NewName: " << (newName.empty() ? "NULL" : newName) << '\n';
    }
    bool isShowCalled = (action == "show");
    cout << makePadding(indent + 1, true) << "show: " << (isShowCalled ? "called" : "NULL") << '\n';

```

## Conclusions

In this lab, I focused on working with abstract syntax trees (ASTs) and parsers to define and interpret the structure of simplified command languages. I implemented a parser that converts input into a hierarchical AST, capturing the syntactic structure of expressions and commands. The AST allowed for organized representation of nested elements and operations such as commands, actions, and modifiers. The parser played a key role in breaking down input into meaningful components, building nodes that represent the grammar of the language, and enabling further processing or visualization. Through constructing the parser and AST, I gained a deeper understanding of how language structure can be represented programmatically and how ASTs are essential for tasks like interpretation, validation, and transformation.

### Output

![/roll, /npc & /player add commands](https://github.com/user-attachments/assets/de18ff31-3e69-47a0-918a-9b36deb90ced)
![/stats commands](https://github.com/user-attachments/assets/7d51c36e-ebe4-4cbc-b1c1-b73ab11bee55)
![/player name change and /npc interact commands](https://github.com/user-attachments/assets/c7bee770-4c62-4785-97b5-b62537f6a1c5)









Results after running the code.
