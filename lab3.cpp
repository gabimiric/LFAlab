#include <iostream>
#include <vector>
#include <unordered_map>
#include <regex>
#include <random>

using namespace std;

// Enum to define the type of token
enum class TokenType { COMMAND, ARG, NUMBER, UNKNOWN };

// Structure to hold information about a token (its type and value)
struct Token {
    TokenType type;
    string value;
};

// Lexer class to tokenize the input string
class Lexer {
public:
    // Function to tokenize input string into a vector of tokens
    static vector<Token> tokenize(const string &input)
    {
        vector<Token> tokens;
        // Regular expressions for different patterns
        regex patterns[] = {regex(R"(^/\w+)"), regex(R"(\b\w+\b)"), regex(R"(\b\d+\b)")};
        TokenType types[] = {TokenType::COMMAND, TokenType::ARG, TokenType::NUMBER}; // Assign corresponding token types

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
        return tokens;
    }
};

// Main class for the DnD game
class DnDGame {
    unordered_map<string, unordered_map<string, int> > players; // Players data
    unordered_map<string, unordered_map<string, int> > npcs; // NPCs data
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

    // Function to execute a command based on the parsed tokens
    void executeCommand(const vector<Token> &tokens)
    {
        if (tokens.empty()) return; // If no tokens, do nothing

        string command = tokens[0].value; // Extract the command (first token)

        // Handling the "/roll" command (for rolling dice)
        if (command == "/roll")
        {
            int sides = 6; // Default dice is d6
            if (tokens.size() > 1 && tokens[1].value[0] == 'd') // Check if a specific dice is mentioned (e.g., d20)
            {
                sides = stoi(tokens[1].value.substr(1)); // Extract the number of sides (e.g., 20 from d20)
            }
            cout << "Dice rolled: " << rollDice(sides) << "\n"; // Output the result of the dice roll
        }
        // Handling the "/stats" command (for displaying or setting stats for players or NPCs)
        else if (command == "/stats")
        {
            if (tokens.size() < 2) // If not enough tokens are present, ask for more details
            {
                cout << "Error: Incomplete command. Possible continuation: show, set.\n";
                return;
            }

            string action = tokens[1].value; // Extract the action (show/set)

            if (tokens.size() < 3) // If a name is not provided, ask for one
            {
                cout << "Error: Please specify a player or NPC name.\n";
                return;
            }

            string name = tokens[2].value; // Extract the name of the player or NPC

            // Check if it's a player
            if (players.find(name) != players.end())
            {
                if (action == "show") // Show player's stats
                {
                    cout << name << "'s stats:\n";
                    for (const auto &[stat, value]: players[name]) // Iterate through player's stats
                    {
                        cout << "  " << stat << ": " << value << "\n";
                    }
                }
                else if (action == "set") // Set player's stat
                {
                    if (tokens.size() < 4) // If stat is missing, ask for it
                    {
                        cout << "Error: Please specify a stat to change.\n";
                        return;
                    }
                    if (tokens.size() < 5) // If stat value is missing, ask for it
                    {
                        cout << "Error: Please specify a value for the stat.\n";
                        return;
                    }

                    string stat = tokens[3].value; // Extract the stat name
                    int value;

                    try
                    {
                        value = stoi(tokens[4].value); // Convert stat value to integer
                    } catch (const invalid_argument &)
                    {
                        cout << "Error: Stat value must be a number.\n";
                        return;
                    }

                    if (players[name].find(stat) == players[name].end()) // Check if stat exists
                    {
                        cout << "Error: Invalid stat name. Possible stats: str, dex, con, int, wis, cha.\n";
                        return;
                    }

                    if (value > 20) // Check if stat value is valid (max 20)
                    {
                        cout << "Error: Stat value too high (max 20).\n";
                        return;
                    }

                    players[name][stat] = value; // Update the stat
                    cout << "Success: " << name << "'s " << stat << " set to " << value << ".\n";
                }
                else
                {
                    cout << "Error: Invalid stats command for player. Possible continuation: show, set.\n";
                }
            }
            // Check if it's an NPC
            else if (npcs.find(name) != npcs.end())
            {
                if (action == "show") // Show NPC's stats
                {
                    cout << name << "'s stats:\n";
                    for (const auto &[stat, value]: npcs[name]) // Iterate through NPC's stats
                    {
                        cout << "  " << stat << ": " << value << "\n";
                    }
                }
                else if (action == "set") // Set NPC's stat
                {
                    if (tokens.size() < 4) // If stat is missing, ask for it
                    {
                        cout << "Error: Please specify a stat to change.\n";
                        return;
                    }
                    if (tokens.size() < 5) // If stat value is missing, ask for it
                    {
                        cout << "Error: Please specify a value for the stat.\n";
                        return;
                    }

                    string stat = tokens[3].value; // Extract the stat name
                    int value;

                    try
                    {
                        value = stoi(tokens[4].value); // Convert stat value to integer
                    } catch (const invalid_argument &)
                    {
                        cout << "Error: Stat value must be a number.\n";
                        return;
                    }

                    if (npcs[name].find(stat) == npcs[name].end()) // Check if stat exists
                    {
                        cout <<
                                "Error: Invalid stat name for NPC. Possible stats: str, dex, con, int, wis, cha, alignment.\n";
                        return;
                    }

                    if (value > 20 && stat != "alignment") // Check if stat value is valid (max 20 for most stats)
                    {
                        cout << "Error: Stat value too high (max 20).\n";
                        return;
                    }
                    if (stat == "alignment" && (value < 0 || value > 100)) // Check if alignment is within valid range
                    {
                        cout << "Error: Alignment value must be between 0 and 100.\n";
                        return;
                    }

                    npcs[name][stat] = value; // Update the stat
                    cout << "Success: " << name << "'s " << stat << " set to " << value << ".\n";
                }
                else
                {
                    cout << "Error: Invalid stats command for NPC. Possible continuation: show, set.\n";
                }
            }
            else
            {
                cout << "Error: Player or NPC '" << name << "' not found.\n";
            }
        }
        // Handling the "/player" command (for managing players)
        else if (command == "/player")
        {
            if (tokens.size() < 2) // If no action is specified, ask for one
            {
                cout << "Error: Incomplete command. Possible continuation: add, remove, name, show.\n";
                return;
            }

            string action = tokens[1].value; // Extract the action (add/remove/name/show)

            if (action == "add") // Add a player
            {
                if (tokens.size() < 3) // If player name is missing, ask for it
                {
                    cout << "Error: Please provide a player name to add.\n";
                    return;
                }
                string username = tokens[2].value; // Extract the player name
                if (players.find(username) != players.end()) // Check if player already exists
                {
                    cout << "Error: Player '" << username << "' already exists.\n";
                }
                else
                {
                    // Create a new player with random stats (rolling dice for each stat)
                    players[username] = {
                        {"str", rollDice(20)},
                        {"dex", rollDice(20)},
                        {"con", rollDice(20)},
                        {"int", rollDice(20)},
                        {"wis", rollDice(20)},
                        {"cha", rollDice(20)}
                    };
                    cout << "Success: Player '" << username << "' added.\n";
                }
            }
            else if (action == "remove") // Remove a player
            {
                if (tokens.size() < 3) // If player name is missing, ask for it
                {
                    cout << "Error: Please provide a player name to remove.\n";
                    return;
                }
                string username = tokens[2].value; // Extract the player name
                if (players.find(username) == players.end()) // Check if player exists
                {
                    cout << "Error: Player '" << username << "' not found.\n";
                }
                else
                {
                    players.erase(username); // Remove the player
                    cout << "Success: Player '" << username << "' removed.\n";
                }
            }
            else if (action == "name") // Rename a player
            {
                if (tokens.size() < 4) // If both old and new names are missing, ask for them
                {
                    cout << "Error: Please provide both old and new player names.\n";
                    return;
                }
                string oldName = tokens[2].value; // Extract old name
                string newName = tokens[3].value; // Extract new name
                if (players.find(oldName) == players.end()) // Check if the old player exists
                {
                    cout << "Error: Player '" << oldName << "' not found.\n";
                }
                else if (players.find(newName) != players.end()) // Check if new name is already taken
                {
                    cout << "Error: Player '" << newName << "' already exists.\n";
                }
                else
                {
                    players[newName] = players[oldName]; // Rename the player by copying data
                    players.erase(oldName); // Remove the old name entry
                    cout << "Success: Player '" << oldName << "' renamed to '" << newName << "'.\n";
                }
            }
            else if (action == "show") // Show the list of all NPCs
            {
                if (npcs.empty()) // If there are no NPCs, display an appropriate message
                {
                    cout << "No NPCs available.\n"; // No NPCs present in the system
                }
                else
                {
                    cout << "Current NPCs:\n"; // Indicate that the list of NPCs will follow
                    for (const auto &[name, stats]: npcs) // Loop through all NPCs
                    {
                        cout << "- " << name << "\n"; // Display each NPC's name
                    }
                }
            }
            else
            {
                cout << "Error: Invalid player command. Possible continuation: add, remove, name.\n";
            }
        }
        else if (command == "/npc") // Check for NPC-related commands
        {
            if (tokens.size() < 2) // If the command lacks an action, prompt for the next possible action
            {
                cout << "Error: Incomplete command. Possible continuation: add, remove, show, interact.\n";
                return;
            }

            string action = tokens[1].value; // Extract the action (add, remove, show, interact)

            // Add a new NPC
            if (action == "add")
            {
                if (tokens.size() < 3) // If the NPC name is missing, prompt for it
                {
                    cout << "Error: Please provide an NPC name to add.\n";
                    return;
                }
                string npcName = tokens[2].value; // Extract the NPC name

                if (npcs.find(npcName) != npcs.end()) // Check if the NPC already exists
                {
                    cout << "Error: NPC '" << npcName << "' already exists.\n";
                }
                else
                {
                    // Create NPC with random stats and alignment (from 1 to 20 for stats, 0-100 for alignment)
                    unordered_map<string, int> npcStats;
                    npcStats["str"] = rollDice(20); // Strength stat
                    npcStats["dex"] = rollDice(20); // Dexterity stat
                    npcStats["int"] = rollDice(20); // Intelligence stat
                    npcStats["cha"] = rollDice(20); // Charisma stat
                    npcStats["wis"] = rollDice(20); // Wisdom stat
                    npcStats["aln"] = rollDice(100); // Alignment from 1 to 100

                    npcs[npcName] = npcStats; // Add the NPC to the map
                    cout << "Success: NPC '" << npcName << "' added.\n";
                }
            }
            // Remove an NPC
            else if (action == "remove")
            {
                if (tokens.size() < 3) // If the NPC name is missing, prompt for it
                {
                    cout << "Error: Please specify the name of the NPC to remove.\n";
                    return;
                }
                string npcName = tokens[2].value; // Extract the NPC name

                if (npcs.erase(npcName)) // Attempt to remove the NPC from the map
                {
                    cout << "Success: NPC '" << npcName << "' removed.\n";
                }
                else
                {
                    cout << "Error: NPC '" << npcName << "' not found.\n";
                }
            }
            // Show a list of all NPCs
            else if (action == "show")
            {
                if (npcs.empty()) // Check if there are no NPCs to show
                {
                    cout << "No NPCs available.\n";
                }
                else
                {
                    cout << "Current NPCs:\n";
                    for (const auto &[name, stats]: npcs) // Display each NPC's name
                    {
                        cout << "- " << name << "\n";
                    }
                }
            }
            // Interact with an NPC
            else if (action == "interact")
            {
                if (tokens.size() < 3) // If the NPC name is missing, prompt for it
                {
                    cout << "Error: Please provide an NPC name for interaction.\n";
                    return;
                }

                string npcName = tokens[2].value; // Extract the NPC name

                if (npcs.find(npcName) == npcs.end()) // Check if the NPC exists
                {
                    cout << "Error: NPC '" << npcName << "' not found.\n";
                    return;
                }

                if (tokens.size() < 4) // If the interaction type is missing, prompt for it
                {
                    cout << "Error: Please specify an interaction (e.g., attack, seduce, outsmart).\n";
                    return;
                }

                string interaction = tokens[3].value; // Extract the interaction type
                string playerName = tokens.back().value; // Last token is always the player's name

                if (players.find(playerName) == players.end()) // Check if the player exists
                {
                    cout << "Error: Player '" << playerName << "' not found.\n";
                    return;
                }

                // Retrieve the NPC's and player's stats
                unordered_map<string, int> npcStats = npcs[npcName];
                unordered_map<string, int> playerStats = players[playerName];

                int npcAlignment = npcStats["aln"]; // Get NPC's alignment
                int npcStat, playerStat;

                // Set the stats for the interaction based on the type
                if (interaction == "bargain")
                {
                    npcStat = npcStats["wis"];
                    playerStat = playerStats["cha"];
                    // Apply buffs/nerfs to player stat based on NPC's alignment
                    if (npcAlignment < 35) playerStat -= 3; // Nerf for evil NPCs
                    else if (npcAlignment > 90) playerStat += 2; // Buff for friendly NPCs
                }
                else if (interaction == "attack")
                {
                    npcStat = npcStats["str"];
                    playerStat = playerStats["str"];
                }
                else if (interaction == "outsmart")
                {
                    npcStat = npcStats["int"];
                    playerStat = playerStats["int"];
                }
                else if (interaction == "threaten")
                {
                    npcStat = npcStats["str"];
                    playerStat = playerStats["wis"];
                    if (npcAlignment < 20) playerStat -= 5; // Nerf for evil NPCs
                }
                else if (interaction == "seduce")
                {
                    npcStat = npcStats["cha"];
                    playerStat = playerStats["cha"];
                    // Apply buffs/nerfs to player stat based on NPC's alignment
                    if (npcAlignment < 35) playerStat -= 2; // Nerf for evil NPCs
                    else if (npcAlignment > 75) playerStat += 3; // Buff for friendly NPCs
                }
                else
                {
                    cout << "Error: Unknown interaction type.\n";
                    return;
                }

                // Display the result of the interaction
                cout << "Interaction (" << interaction << "): " << npcName << " vs " << playerName << "\n";
                cout << playerName << "'s stat: " << playerStat << "\n";
                cout << npcName << "'s response stat: " << npcStat << "\n";

                // Compare the stats to determine the winner
                if (npcStat > playerStat)
                {
                    cout << npcName << " wins the interaction!\n";
                }
                else if (npcStat < playerStat)
                {
                    cout << playerName << " wins the interaction!\n";
                }
                else
                {
                    cout << "It's a tie!\n";
                }
            }
            else // If the action is invalid, show the available options
            {
                cout << "Error: Invalid NPC command. Possible continuation: add, remove, show, interact.\n";
            }
        }
        else
        {
            cout << "Error: Invalid command. Possible commands: /roll, /stats, /player, /npc.\n";
        }
    }
};

int main()
{
    DnDGame game;

    string input;
    while (true)
    {
        cout << "> ";
        getline(cin, input); // Get input from the user
        if (input == "exit") break; // Exit the loop if the user types "exit"

        vector<Token> tokens = Lexer::tokenize(input); // Tokenize the input
        game.executeCommand(tokens); // Execute the command based on the tokens
    }

    return 0;
}
