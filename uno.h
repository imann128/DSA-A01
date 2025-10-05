#ifndef UNO_H
#define UNO_H

#include <string>
#include <memory>
#include <vector>

// ------------------------------
// Card Definition
// ------------------------------
struct Card {
    enum Color { RED, GREEN, BLUE, YELLOW, NONE };
    enum Type { NUMBER, SKIP, REVERSE, DRAW_TWO };

    Color color;
    Type type;
    int number;  // valid only if type == NUMBER

    Card(Color c = NONE, Type t = NUMBER, int n = -1)
        : color(c), type(t), number(n) {}

    // Add equality operator
    bool operator==(const Card& other) const {
        return color == other.color && 
               type == other.type && 
               number == other.number;
    }

    // Add inequality operator for completeness
    bool operator!=(const Card& other) const {
        return !(*this == other);
    }

    // Check if two cards can match
    bool matches(const Card& other) const {
        return (color == other.color) ||
               (type == other.type && type != NUMBER) ||
               (type == NUMBER && other.type == NUMBER && number == other.number);
    }

    // Convert card to readable string like "Blue 5" or "Yellow Skip"
    std::string toString() const {
        std::string colorStr, typeStr;

        switch (color) {
            case RED: colorStr = "Red"; break;
            case GREEN: colorStr = "Green"; break;
            case BLUE: colorStr = "Blue"; break;
            case YELLOW: colorStr = "Yellow"; break;
            default: colorStr = "None"; break;
        }

        switch (type) {
            case NUMBER: typeStr = std::to_string(number); break;
            case SKIP: typeStr = "Skip"; break;
            case REVERSE: typeStr = "Reverse"; break;
            case DRAW_TWO: typeStr = "Draw Two"; break;
        }

        return colorStr + " " + typeStr;
    }
};

// ------------------------------
// Abstract UNOGame Class
// ------------------------------
class UNOGame {
public:
    UNOGame(int numPlayers) {}
    virtual ~UNOGame() = default;

    // Core virtual methods (to be overridden in ConcreteUNOGame)
    virtual void initialize() = 0;
    virtual void playTurn() = 0;
    virtual bool isGameOver() const = 0;
    virtual int getWinner() const = 0;
    virtual std::string getState() const = 0;

    // Factory method (implemented in .cpp)
    static std::unique_ptr<UNOGame> create(int numPlayers);
};

#endif // UNO_H
