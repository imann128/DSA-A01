# DSA Assignment 1 - ADT Implementations

### Approach

### Problem 1: Polynomial ADT
- **Data Structure**: Used linked list of Term nodes to store coefficients and exponents
- **Operations**: Implemented insertion with automatic like-term combination, addition, multiplication, and differentiation
- **Display**: Handled special cases for coefficients (1, -1) and exponents (0, 1) for mathematical formatting
- **Linked List Benefits**: Efficient insertion/deletion and natural ordering of polynomial terms

### Problem 2: Text Editor with Cursor Simulation  
- **Data Structure**: Used two stacks (left and right of cursor) for efficient cursor operations
- **Operations**: Stack-based approach for O(1) insert/delete at cursor position
- **Cursor Management**: Left stack stores text before cursor, right stack stores text after cursor
- **Challenges**: Initially struggled with cursor positioning logic; stack approach provided elegant solution

### Problem 3: UNO Card Game Simulation
- **Data Structure**: Used custom linked lists for deck, discard pile, and player hands
- **Game Logic**: Implemented card matching with priority system, action card effects, and turn management
- **Linked List Benefits**: Efficient card removal and traversal for playable card search
- **Randomness**: Used mt19937 with fixed seed 1234 for reproducible results

## GitHub Repository
https://github.com/imann128/DSA-A01

## Challenges Faced

1. **Polynomial Linked List Management**: Maintaining sorted order while combining like terms required careful pointer manipulation and memory management.

2. **Text Editor Cursor Positioning**: Initially used string-based approach which was inefficient. Switched to dual-stack design which provided O(1) operations but required rethinking the cursor logic.

3. **UNO Card Matching Logic**: Implementing the priority system (color > value > action type) in linked list traversal was complex, especially with action card effects on turn order.

4. **Linked List Memory Management**: Ensuring proper cleanup of all nodes in destructors to prevent memory leaks across all three problems.

5. **UNO Game State Consistency**: Debugging issues with card removal and turn progression required extensive testing of linked list operations.

6. **Cursor Placement**: Faulty cursor placement did not correctly delete characters.

7. **Fixed Seed**: Fixed seed e.g. 1234 did not match sample output. Could be due to system/ library differences.

## Compilation Instructions

```bash
# Polynomial
g++ -std=c++14 -o polynomial polynomial.cpp test_main.cpp 

# Text Editor  
g++ -std=c++14 -o texteditor texteditor.cpp test_texteditor.cpp 

# UNO Game
g++ -std=c++14 -o uno uno.cpp uno_test.cpp
