#include <iostream>
#include <vector>
#include <unordered_map>
#include <regex>
#include <random>
#include <memory>

using namespace std;

// Enum to define the type of token
enum class TokenType { COMMAND, ACTION, NAME, STAT, NUMBER, UNKNOWN };

// Structure to hold information about a token (its type and value)
struct Token {
    TokenType type;
    string value;
};

// Abstract Syntax Tree Node types
enum class NodeType { 
    COMMAND_NODE,
    ROLL_NODE, 
    STATS_NODE, 
    PLAYER_NODE, 
    NPC_NODE,
    INTERACTION_NODE
};

// Base AST Node class
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual NodeType getType() const = 0;
    virtual void print(int indent = 0, bool isLast = true) const = 0;
    static string makePadding(int indent, bool isLast) {
        string pad;
        for (int i = 0; i < indent - 1; ++i)
            pad += "|   ";
        if (indent > 0)
            pad += isLast ? "`-- " : "|-- ";
        return pad;
    }


};

// Command Node - Base node for all command types
class CommandNode : public ASTNode {
protected:
    string commandName;

public:
    CommandNode(const string& name) : commandName(name) {}

    NodeType getType() const override { return NodeType::COMMAND_NODE; }

    virtual string getCommandName() const { return commandName; }

    void print(int indent = 0, bool isLast = true) const override {
        cout << makePadding(indent, isLast) << "Command: " << commandName << '\n';
    }
};

// Roll Node - For dice rolling commands
class RollNode : public CommandNode {
private:
    int sides;
    
public:
    RollNode(int s) : CommandNode("/roll"), sides(s) {}
    
    NodeType getType() const override { return NodeType::ROLL_NODE; }
    int getSides() const { return sides; }
    
    void print(int indent = 0, bool isLast = true) const override {
        cout << makePadding(indent, isLast) << "Roll Command: d" << sides << '\n';
    }
};

// Stats Node - For viewing or setting stats
class StatsNode : public CommandNode {
private:
    string action; // "show" or "set"
    string name;   // Character name
    string stat;   // Optional - stat name if setting
    int value;     // Optional - value if setting
    
public:
    StatsNode(const string& a, const string& n, const string& s = "", int v = 0)
        : CommandNode("/stats"), action(a), name(n), stat(s), value(v) {}
    
    NodeType getType() const override { return NodeType::STATS_NODE; }
    
    string getAction() const { return action; }
    string getName() const { return name; }
    string getStat() const { return stat; }
    int getValue() const { return value; }
    
    void print(int indent = 0, bool isLast = true) const override {
        cout << makePadding(indent, isLast) << "Command: stats\n";

        // Print show branch
        bool isShowCalled = (action == "show");
        cout << makePadding(indent + 1, false) << "show: " << (isShowCalled ? "called" : "NULL") << '\n';
        if (isShowCalled) {
            cout << makePadding(indent + 2, true) << "Name: " << (name.empty() ? "NULL" : name) << '\n';
        }

        // Print set branch
        bool isSetCalled = (action == "set");
        cout << makePadding(indent + 1, true) << "set: " << (isSetCalled ? "called" : "NULL") << '\n';
        if (isSetCalled) {
            cout << makePadding(indent + 2, false) << "Name: " << (name.empty() ? "NULL" : name) << '\n';
            cout << makePadding(indent + 2, false) << "Stat: " << stat << '\n';
            cout << makePadding(indent + 2, true) << "Value: " << value << '\n';
        }
    }

};

// Player Node - For player management
class PlayerNode : public CommandNode {
private:
    string action;     // "add", "remove", "name", "show"
    string playerName; // Player name
    string newName;    // Optional - for rename action
    
public:
    PlayerNode(const string& a, const string& name, const string& newN = "")
        : CommandNode("/player"), action(a), playerName(name), newName(newN) {}
    
    NodeType getType() const override { return NodeType::PLAYER_NODE; }
    
    string getAction() const { return action; }
    string getPlayerName() const { return playerName; }
    string getNewName() const { return newName; }
    
    void print(int indent = 0, bool isLast = true) const override {
        cout << makePadding(indent, isLast) << "Command: player\n";

        // add
        bool isAddCalled = (action == "add");
        cout << makePadding(indent + 1, false) << "add: " << (isAddCalled ? "called" : "NULL") << '\n';
        if (isAddCalled) {
            cout << makePadding(indent + 2, true) << "PlayerName: " << (playerName.empty() ? "NULL" : playerName) << '\n';
        }

        // remove
        bool isRemoveCalled = (action == "remove");
        cout << makePadding(indent + 1, false) << "remove: " << (isRemoveCalled ? "called" : "NULL") << '\n';
        if (isRemoveCalled) {
            cout << makePadding(indent + 2, true) << "PlayerName: " << (playerName.empty() ? "NULL" : playerName) << '\n';
        }

        // name (rename)
        bool isNameCalled = (action == "name");
        cout << makePadding(indent + 1, false) << "name: " << (isNameCalled ? "called" : "NULL") << '\n';
        if (isNameCalled) {
            cout << makePadding(indent + 2, false) << "OldName: " << (playerName.empty() ? "NULL" : playerName) << '\n';
            cout << makePadding(indent + 2, true) << "NewName: " << (newName.empty() ? "NULL" : newName) << '\n';
        }

        // show
        bool isShowCalled = (action == "show");
        cout << makePadding(indent + 1, true) << "show: " << (isShowCalled ? "called" : "NULL") << '\n';
    }

};

// NPC Node - For NPC management
class NPCNode : public CommandNode {
private:
    string action;  // "add", "remove", "show", "interact"
    string npcName; // NPC name
    string interaction; // Optional - for interaction type
    string playerName;  // Optional - for interaction player
    
public:
    NPCNode(const string& a, const string& name, const string& interact = "", const string& player = "")
        : CommandNode("/npc"), action(a), npcName(name), interaction(interact), playerName(player) {}
    
    NodeType getType() const override { return NodeType::NPC_NODE; }
    
    string getAction() const { return action; }
    string getNPCName() const { return npcName; }
    string getInteraction() const { return interaction; }
    string getPlayerName() const { return playerName; }
    
    void print(int indent = 0, bool isLast = true) const override {
        cout << makePadding(indent, isLast) << "Command: npc\n";

        // add
        bool isAddCalled = (action == "add");
        cout << makePadding(indent + 1, false) << "add: " << (isAddCalled ? "called" : "NULL") << '\n';
        if (isAddCalled) {
            cout << makePadding(indent + 2, true) << "NPCName: " << (npcName.empty() ? "NULL" : npcName) << '\n';
        }

        // remove
        bool isRemoveCalled = (action == "remove");
        cout << makePadding(indent + 1, false) << "remove: " << (isRemoveCalled ? "called" : "NULL") << '\n';
        if (isRemoveCalled) {
            cout << makePadding(indent + 2, true) << "NPCName: " << (npcName.empty() ? "NULL" : npcName) << '\n';
        }

        // show
        bool isShowCalled = (action == "show");
        cout << makePadding(indent + 1, false) << "show: " << (isShowCalled ? "called" : "NULL") << '\n';

        // interact
        bool isInteractCalled = (action == "interact");
        cout << makePadding(indent + 1, true) << "interact: " << (isInteractCalled ? "called" : "NULL") << '\n';
        if (isInteractCalled) {
            cout << makePadding(indent + 2, false) << "NPCName: " << (npcName.empty() ? "NULL" : npcName) << '\n';
            cout << makePadding(indent + 2, false) << "Interaction: " << (interaction.empty() ? "NULL" : interaction) << '\n';
            cout << makePadding(indent + 2, true) << "PlayerName: " << (playerName.empty() ? "NULL" : playerName) << '\n';
        }
    }
};

// Lexer class to tokenize the input string
class Lexer {
public:
    // Function to tokenize input string into a vector of tokens
    static vector<Token> tokenize(const string &input)
    {
        vector<Token> tokens;

        // First, find any command (starts with /)
        regex commandPattern(R"(^/\w+)");
        smatch commandMatch;
        string processedInput = input;

        if (regex_search(processedInput, commandMatch, commandPattern)) {
            tokens.push_back({TokenType::COMMAND, commandMatch.str()});
            processedInput = processedInput.substr(commandMatch.length());
        } else {
            return tokens; // No command found, return empty tokens
        }

        // Then tokenize the rest of the input
        regex actionPattern(R"(\b(?:show|set|add|remove|name|interact|bargain|attack|outsmart|threaten|seduce)\b)");
        regex statPattern(R"(\b(?:str|dex|con|int|wis|cha|aln|alignment)\b)");
        regex dicePattern(R"(\bd\d+\b)");
        regex numberPattern(R"(\b\d+\b)");
        regex namePattern(R"(\b\w+\b)");

        string remaining = processedInput;
        smatch match;

        while (!remaining.empty()) {
            // Skip whitespace
            if (remaining[0] == ' ' || remaining[0] == '\t') {
                remaining = remaining.substr(1);
                continue;
            }

            // Try to match each pattern
            bool matched = false;

            if (regex_search(remaining, match, actionPattern, regex_constants::match_continuous)) {
                tokens.push_back({TokenType::ACTION, match.str()});
                matched = true;
            } else if (regex_search(remaining, match, statPattern, regex_constants::match_continuous)) {
                tokens.push_back({TokenType::STAT, match.str()});
                matched = true;
            } else if (regex_search(remaining, match, dicePattern, regex_constants::match_continuous)) {
                // Extract the number part from the dice notation (d20 -> 20)
                string sides = match.str().substr(1);
                tokens.push_back({TokenType::NUMBER, sides});
                matched = true;
            } else if (regex_search(remaining, match, numberPattern, regex_constants::match_continuous)) {
                tokens.push_back({TokenType::NUMBER, match.str()});
                matched = true;
            } else if (regex_search(remaining, match, namePattern, regex_constants::match_continuous)) {
                tokens.push_back({TokenType::NAME, match.str()});
                matched = true;
            }

            if (matched) {
                remaining = remaining.substr(match.length());
            } else {
                // Skip unrecognized character
                remaining = remaining.substr(1);
            }
        }

        return tokens;
    }
};

// Parser class to create AST from tokens
class Parser {
private:
    vector<Token> tokens;
    size_t currentPos;

    Token consume() {
        if (currentPos < tokens.size()) {
            return tokens[currentPos++];
        }
        return {TokenType::UNKNOWN, ""};
    }

    Token peek(int ahead = 0) {
        size_t pos = currentPos + ahead;
        if (pos < tokens.size()) {
            return tokens[pos];
        }
        return {TokenType::UNKNOWN, ""};
    }

    bool match(TokenType type) {
        if (currentPos < tokens.size() && tokens[currentPos].type == type) {
            currentPos++;
            return true;
        }
        return false;
    }

    bool check(TokenType type) {
        if (currentPos < tokens.size()) {
            return tokens[currentPos].type == type;
        }
        return false;
    }

public:
    Parser(const vector<Token>& t) : tokens(t), currentPos(0) {}

    // Parse tokens into an AST
    unique_ptr<ASTNode> parse() {
        if (tokens.empty()) {
            return nullptr;
        }

        // Get the first token which should be a command
        Token commandToken = consume();
        if (commandToken.type != TokenType::COMMAND) {
            return nullptr; // Not a valid command
        }

        string command = commandToken.value;

        // Parse based on command type
        if (command == "/roll") {
            return parseRollCommand();
        } else if (command == "/stats") {
            return parseStatsCommand();
        } else if (command == "/player") {
            return parsePlayerCommand();
        } else if (command == "/npc") {
            return parseNPCCommand();
        }

        return make_unique<CommandNode>(command); // Generic command
    }

    unique_ptr<ASTNode> parseRollCommand() {
        int sides = 6; // Default dice is d6

        // Check if next token is a dice specification
        if (currentPos < tokens.size() && tokens[currentPos].type == TokenType::NUMBER) {
            sides = stoi(tokens[currentPos].value);
            consume(); // Consume the dice token
        }

        return make_unique<RollNode>(sides);
    }

    unique_ptr<ASTNode> parseStatsCommand() {
        // Check for action (show/set)
        if (currentPos >= tokens.size() || tokens[currentPos].type != TokenType::ACTION) {
            return nullptr; // Missing action
        }
        string action = consume().value;

        // Check for name
        if (currentPos >= tokens.size() || tokens[currentPos].type != TokenType::NAME) {
            return nullptr; // Missing name
        }
        string name = consume().value;

        // If action is "set", expect stat and value
        if (action == "set") {
            // Get stat name
            if (currentPos >= tokens.size() ||
                (tokens[currentPos].type != TokenType::STAT &&
                 tokens[currentPos].type != TokenType::NAME)) {
                return nullptr; // Missing stat
            }
            string stat = consume().value;

            // Get value
            if (currentPos >= tokens.size() || tokens[currentPos].type != TokenType::NUMBER) {
                return nullptr; // Missing value
            }
            int value = stoi(consume().value);

            return make_unique<StatsNode>(action, name, stat, value);
        }

        return make_unique<StatsNode>(action, name);
    }

    unique_ptr<ASTNode> parsePlayerCommand() {
        // Check for action (add/remove/name/show)
        if (currentPos >= tokens.size() || tokens[currentPos].type != TokenType::ACTION) {
            return nullptr; // Missing action
        }
        string action = consume().value;

        // For "show" action, no name is needed
        if (action == "show") {
            return make_unique<PlayerNode>(action, "");
        }

        // Check for player name
        if (currentPos >= tokens.size() || tokens[currentPos].type != TokenType::NAME) {
            return nullptr; // Missing name
        }
        string name = consume().value;

        // If action is "name", expect new name
        if (action == "name") {
            if (currentPos >= tokens.size() || tokens[currentPos].type != TokenType::NAME) {
                return nullptr; // Missing new name
            }
            string newName = consume().value;

            return make_unique<PlayerNode>(action, name, newName);
        }

        return make_unique<PlayerNode>(action, name);
    }

    unique_ptr<ASTNode> parseNPCCommand() {
        // Check for action (add/remove/show/interact)
        if (currentPos >= tokens.size() || tokens[currentPos].type != TokenType::ACTION) {
            return nullptr; // Missing action
        }
        string action = consume().value;

        // For "show" action, no name is needed
        if (action == "show") {
            return make_unique<NPCNode>(action, "");
        }

        // Check for NPC name
        if (currentPos >= tokens.size() || tokens[currentPos].type != TokenType::NAME) {
            return nullptr; // Missing NPC name
        }
        string npcName = consume().value;

        // If action is "interact", expect interaction type and player name
        if (action == "interact") {
            // Get interaction type
            if (currentPos >= tokens.size() || tokens[currentPos].type != TokenType::ACTION) {
                return nullptr; // Missing interaction type
            }
            string interaction = consume().value;

            // Get player name
            if (currentPos >= tokens.size() || tokens[currentPos].type != TokenType::NAME) {
                return nullptr; // Missing player name
            }
            string playerName = consume().value;

            return make_unique<NPCNode>(action, npcName, interaction, playerName);
        }

        return make_unique<NPCNode>(action, npcName);
    }
};

// Main class for the DnD game
class DnDGame {
    unordered_map<string, unordered_map<string, int>> players; // Players data
    unordered_map<string, unordered_map<string, int>> npcs; // NPCs data
    random_device rd; // For generating random numbers
    mt19937 rng; // Random number generator

public:
    DnDGame() : rng(rd()) {} // Constructor to initialize random number generator

    // Function to roll a dice with a specified number of sides
    int rollDice(int sides)
    {
        uniform_int_distribution<int> dist(1, sides); // Create a uniform distribution
        return dist(rng); // Return a random number within the specified range
    }

    // Execute a command based on the AST node
    void executeCommand(const unique_ptr<ASTNode>& node) {
        if (!node) {
            cout << "Error: Invalid command syntax." << endl;
            return;
        }

        switch (node->getType()) {
            case NodeType::ROLL_NODE:
                executeRollCommand(static_cast<const RollNode&>(*node));
                break;
            case NodeType::STATS_NODE:
                executeStatsCommand(static_cast<const StatsNode&>(*node));
                break;
            case NodeType::PLAYER_NODE:
                executePlayerCommand(static_cast<const PlayerNode&>(*node));
                break;
            case NodeType::NPC_NODE:
                executeNPCCommand(static_cast<const NPCNode&>(*node));
                break;
            default:
                cout << "Error: Unknown command type." << endl;
                break;
        }
    }

    // Execute roll command
    void executeRollCommand(const RollNode& node) {
        cout << "Dice rolled: " << rollDice(node.getSides()) << "\n";
    }

    // Execute stats command
    void executeStatsCommand(const StatsNode& node) {
        string name = node.getName();
        string action = node.getAction();

        // Check if it's a player
        if (players.find(name) != players.end()) {
            if (action == "show") {
                cout << name << "'s stats:\n";
                for (const auto &[stat, value]: players[name]) {
                    cout << "  " << stat << ": " << value << "\n";
                }
            }
            if (action == "set") {
                string stat = node.getStat();
                int value = node.getValue();

                if (players[name].find(stat) == players[name].end()) {
                    cout << "Error: Invalid stat name. Possible stats: str, dex, con, int, wis, cha.\n";
                    return;
                }

                if (value > 20) {
                    cout << "Error: Stat value too high (max 20).\n";
                    return;
                }

                players[name][stat] = value;
                cout << "Success: " << name << "'s " << stat << " set to " << value << ".\n";
            }
        }
        // Check if it's an NPC
        else if (npcs.find(name) != npcs.end()) {
            if (action == "show") {
                cout << name << "'s stats:\n";
                for (const auto &[stat, value]: npcs[name]) {
                    cout << "  " << stat << ": " << value << "\n";
                }
            } else if (action == "set") {
                string stat = node.getStat();
                int value = node.getValue();

                if (npcs[name].find(stat) == npcs[name].end()) {
                    cout << "Error: Invalid stat name for NPC. Possible stats: str, dex, con, int, wis, cha, aln.\n";
                    return;
                }

                if (value > 20 && stat != "aln") {
                    cout << "Error: Stat value too high (max 20).\n";
                    return;
                }
                if (stat == "aln" && (value < 0 || value > 100)) {
                    cout << "Error: Alignment value must be between 0 and 100.\n";
                    return;
                }

                npcs[name][stat] = value;
                cout << "Success: " << name << "'s " << stat << " set to " << value << ".\n";
            }
        } else {
            cout << "Error: Player or NPC '" << name << "' not found.\n";
        }
    }

    // Execute player command
    void executePlayerCommand(const PlayerNode& node) {
        string action = node.getAction();
        string name = node.getPlayerName();

        if (action == "add") {
            if (players.find(name) != players.end()) {
                cout << "Error: Player '" << name << "' already exists.\n";
            } else {
                players[name] = {
                    {"str", rollDice(20)},
                    {"dex", rollDice(20)},
                    {"con", rollDice(20)},
                    {"int", rollDice(20)},
                    {"wis", rollDice(20)},
                    {"cha", rollDice(20)}
                };
                cout << "Success: Player '" << name << "' added.\n";
            }
        } else if (action == "remove") {
            if (players.find(name) == players.end()) {
                cout << "Error: Player '" << name << "' not found.\n";
            } else {
                players.erase(name);
                cout << "Success: Player '" << name << "' removed.\n";
            }
        } else if (action == "name") {
            string newName = node.getNewName();
            if (players.find(name) == players.end()) {
                cout << "Error: Player '" << name << "' not found.\n";
            } else if (players.find(newName) != players.end()) {
                cout << "Error: Player '" << newName << "' already exists.\n";
            } else {
                players[newName] = players[name];
                players.erase(name);
                cout << "Success: Player '" << name << "' renamed to '" << newName << "'.\n";
            }
        } else if (action == "show") {
            if (players.empty()) {
                cout << "No players available.\n";
            } else {
                cout << "Current players:\n";
                for (const auto &[playerName, stats]: players) {
                    cout << "- " << playerName << "\n";
                }
            }
        } else {
            cout << "Error: Invalid player command.\n";
        }
    }

    // Execute NPC command
    void executeNPCCommand(const NPCNode& node) {
        string action = node.getAction();
        string npcName = node.getNPCName();

        if (action == "add") {
            if (npcs.find(npcName) != npcs.end()) {
                cout << "Error: NPC '" << npcName << "' already exists.\n";
            } else {
                npcs[npcName] = {
                    {"str", rollDice(20)},
                    {"dex", rollDice(20)},
                    {"con", rollDice(20)},
                    {"int", rollDice(20)},
                    {"wis", rollDice(20)},
                    {"cha", rollDice(20)},
                    {"aln", rollDice(100)}
                };
                cout << "Success: NPC '" << npcName << "' added.\n";
            }
        } else if (action == "remove") {
            if (npcs.erase(npcName)) {
                cout << "Success: NPC '" << npcName << "' removed.\n";
            } else {
                cout << "Error: NPC '" << npcName << "' not found.\n";
            }
        } else if (action == "show") {
            if (npcs.empty()) {
                cout << "No NPCs available.\n";
            } else {
                cout << "Current NPCs:\n";
                for (const auto &[name, stats]: npcs) {
                    cout << "- " << name << "\n";
                }
            }
        } else if (action == "interact") {
            string interaction = node.getInteraction();
            string playerName = node.getPlayerName();

            if (npcs.find(npcName) == npcs.end()) {
                cout << "Error: NPC '" << npcName << "' not found.\n";
                return;
            }

            if (players.find(playerName) == players.end()) {
                cout << "Error: Player '" << playerName << "' not found.\n";
                return;
            }

            // Retrieve the NPC's and player's stats
            unordered_map<string, int> npcStats = npcs[npcName];
            unordered_map<string, int> playerStats = players[playerName];

            int npcAlignment = npcStats["aln"];
            int npcStat, playerStat;

            // Set the stats for the interaction based on the type
            if (interaction == "bargain") {
                npcStat = npcStats["wis"];
                playerStat = playerStats["cha"];
                // Apply buffs/nerfs to player stat based on NPC's alignment
                if (npcAlignment < 35) playerStat -= 3; // Nerf for evil NPCs
                else if (npcAlignment > 90) playerStat += 2; // Buff for friendly NPCs
            } else if (interaction == "attack") {
                npcStat = npcStats["str"];
                playerStat = playerStats["str"];
            } else if (interaction == "outsmart") {
                npcStat = npcStats["int"];
                playerStat = playerStats["int"];
            } else if (interaction == "threaten") {
                npcStat = npcStats["str"];
                playerStat = playerStats["wis"];
                if (npcAlignment < 20) playerStat -= 5; // Nerf for evil NPCs
            } else if (interaction == "seduce") {
                npcStat = npcStats["cha"];
                playerStat = playerStats["cha"];
                // Apply buffs/nerfs to player stat based on NPC's alignment
                if (npcAlignment < 35) playerStat -= 2; // Nerf for evil NPCs
                else if (npcAlignment > 75) playerStat += 3; // Buff for friendly NPCs
            } else {
                cout << "Error: Unknown interaction type.\n";
                return;
            }

            // Display the result of the interaction
            cout << "Interaction (" << interaction << "): " << npcName << " vs " << playerName << "\n";
            cout << playerName << "'s stat: " << playerStat << "\n";
            cout << npcName << "'s response stat: " << npcStat << "\n";

            // Compare the stats to determine the winner
            if (npcStat > playerStat) {
                cout << npcName << " wins the interaction!\n";
            } else if (npcStat < playerStat) {
                cout << playerName << " wins the interaction!\n";
            } else {
                cout << "It's a tie!\n";
            }
        } else {
            cout << "Error: Invalid NPC command.\n";
        }
    }
};

// Example usage (optional)
void printASTTree(const unique_ptr<ASTNode>& node) {
    if (node) {
        node->print();
    } else {
        cout << "Empty AST node\n";
    }
}

int main()
{
    DnDGame game;

    cout << "D&D Game with Abstract Syntax Tree Parser\n";
    cout << "----------------------------------------\n";
    cout << "Commands: /roll, /stats, /player, /npc\n";
    cout << "Type 'exit' to quit\n\n";

    string input;
    while (true)
    {
        cout << "> ";
        getline(cin, input); // Get input from the user
        if (input == "exit") break; // Exit the loop if the user types "exit"

        // Tokenize the input
        vector<Token> tokens = Lexer::tokenize(input);

        // Parse tokens into AST
        Parser parser(tokens);
        unique_ptr<ASTNode> ast = parser.parse();

        // Debug: Print the AST (uncomment to see the tree structure)
        cout << "AST Structure:" << endl;
        printASTTree(ast);
        
        // Execute the command
        game.executeCommand(ast);
    }

    return 0;
}