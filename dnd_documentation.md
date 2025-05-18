# D&D Game Command Line Interface Documentation

## Overview

This document provides a comprehensive guide to all commands available in the D&D Command Line Interface. The system allows you to manage players and NPCs, roll dice, modify character stats, and simulate character interactions.

## Command Syntax

All commands follow this general syntax:
```
/command action arguments
```

## Available Commands

### 1. Dice Rolling

**Command**: `/roll`

**Description**: Roll a dice with a specified number of sides.

**Syntax**:
```
/roll [dice]
```

**Arguments**:
- `dice` (optional): The dice to roll, in the format `dN` where N is the number of sides (e.g., d20, d6). If omitted, defaults to d6.

**Examples**:
```
/roll        # Rolls a d6 (6-sided dice)
/roll d20    # Rolls a d20 (20-sided dice)
/roll d100   # Rolls a d100 (100-sided dice)
```

---

### 2. Character Stats

**Command**: `/stats`

**Description**: View or modify character statistics.

**Syntax**:
```
/stats show Name
/stats set Name stat value
```

**Actions**:
- `show`: Display all stats for a character
- `set`: Modify a specific stat for a character

**Arguments**:
- `Name`: Name of the player or NPC
- `stat`: The stat to modify (when using `set`)
  - For players: str, dex, con, int, wis, cha
  - For NPCs: str, dex, con, int, wis, cha, aln (alignment)
- `value`: New value for the stat (when using `set`). Maximum is 20 for normal stats, 0-100 for alignment.

**Examples**:
```
/stats show Gandalf     # Shows all stats for player/NPC named Gandalf
/stats set Thor str 18  # Sets Thor's strength to 18
/stats set Loki aln 20  # Sets Loki's alignment to 20 (evil)
```

---

### 3. Player Management

**Command**: `/player`

**Description**: Add, remove, rename, or list players.

**Syntax**:
```
/player add PlayerName
/player remove PlayerName
/player name OldName NewName
/player show
```

**Actions**:
- `add`: Create a new player with random stats
- `remove`: Delete an existing player
- `name`: Rename an existing player
- `show`: List all existing players

**Arguments**:
- `PlayerName`: Name of the player to add or remove
- `OldName`: Current name of the player to rename
- `NewName`: New name for the player

**Examples**:
```
/player add Aragorn      # Creates a new player named Aragorn
/player remove Frodo     # Removes player named Frodo
/player name Bilbo Bilbo_Baggins  # Renames Bilbo to Bilbo_Baggins
/player show             # Lists all players
```

---

### 4. NPC Management

**Command**: `/npc`

**Description**: Add, remove, list, or interact with NPCs (Non-Player Characters).

**Syntax**:
```
/npc add NPCName
/npc remove NPCName
/npc show
/npc interact NPCName interaction PlayerName
```

**Actions**:
- `add`: Create a new NPC with random stats
- `remove`: Delete an existing NPC
- `show`: List all existing NPCs
- `interact`: Simulate an interaction between an NPC and a player

**Arguments**:
- `NPCName`: Name of the NPC
- `interaction`: Type of interaction:
  - `bargain`: Negotiation (NPC wis vs Player cha)
  - `attack`: Combat (NPC str vs Player str)
  - `outsmart`: Intelligence contest (NPC int vs Player int)
  - `threaten`: Intimidation (NPC str vs Player wis)
  - `seduce`: Charisma contest (NPC cha vs Player cha)
- `PlayerName`: Name of the player interacting with the NPC

**Examples**:
```
/npc add Smaug           # Creates a new NPC named Smaug
/npc remove Gollum       # Removes NPC named Gollum
/npc show                # Lists all NPCs
/npc interact Sauron attack Gandalf  # Simulates Gandalf attacking Sauron
```

## Game Mechanics

### Stat System

Characters have the following stats:
- `str` (Strength): Physical power
- `dex` (Dexterity): Agility and reflexes
- `con` (Constitution): Health and stamina
- `int` (Intelligence): Knowledge and reasoning
- `wis` (Wisdom): Perception and intuition
- `cha` (Charisma): Social influence

NPCs also have:
- `aln` (Alignment): A value from 0-100, where:
  - 0-30: Evil
  - 31-70: Neutral
  - 71-100: Good

### Player/NPC Creation

When creating a player or NPC:
- Each stat is randomly rolled on a d20 (1-20 scale)
- For NPCs, alignment is randomly rolled on a d100 (1-100 scale)

### Interaction Mechanics

For NPC interactions:
1. The relevant stats for the player and NPC are compared
2. NPC alignment may affect the player's stat (buffing or nerfing it)
3. The character with the higher final stat value wins the interaction

## Examples of Full Command Sequences

Here's an example of a complete game session:

```
/player add Legolas
/player add Gimli
/npc add Orc_Chief
/stats show Legolas
/stats set Legolas dex 20
/stats show Orc_Chief
/npc interact Orc_Chief attack Gimli
/roll d20
/player name Gimli Gimli_Son_of_Gloin
/npc remove Orc_Chief
/player show
```

## Error Handling

The system will display error messages when:
- Using invalid commands or actions
- Referencing non-existent players or NPCs
- Attempting to set invalid stat values
- Missing required arguments

## Notes

- Stats cannot exceed 20 (except NPC alignment, which ranges from 0-100)
- Player and NPC names must be unique
- All commands are case-sensitive
