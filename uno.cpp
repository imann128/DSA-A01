// uno.cpp
#include "uno.h"
#include <iostream>
#include <sstream>
#include <random>
#include <algorithm>
#include <memory>
#include <vector>

// ------------------------
// Linked list node & list
// ------------------------
struct Node {
    Card card;
    Node* next;
    Node(const Card& c) : card(c), next(nullptr) {}
};

class CardLinkedList {
private:
    Node* head;
    Node* tail;
    int sz;
public:
    CardLinkedList() : head(nullptr), tail(nullptr), sz(0) {}
    ~CardLinkedList() { clear(); }

    void clear() {
        Node* p = head;
        while (p) {
            Node* t = p;
            p = p->next;
            delete t;
        }
        head = tail = nullptr;
        sz = 0;
    }

    bool isEmpty() const { return head == nullptr; }
    int getSize() const { return sz; }

    void pushBack(const Card& c) {
        Node* n = new Node(c);
        if (!head) head = tail = n;
        else {
            tail->next = n;
            tail = n;
        }
        ++sz;
    }

    void pushFront(const Card& c) {
        Node* n = new Node(c);
        n->next = head;
        head = n;
        if (!tail) tail = n;
        ++sz;
    }

    Card popFront() {
        if (!head) return Card();
        Node* n = head;
        Card c = n->card;
        head = head->next;
        if (!head) tail = nullptr;
        delete n;
        --sz;
        return c;
    }

    Card getFront() const {
        if (!head) return Card();
        return head->card;
    }

    Card getBack() const {
        if (!tail) return Card();
        return tail->card;
    }

    // remove first card matching the supplied card (color,type,number)
    bool removeCard(const Card& cardToRemove) {
    if (!head) return false;

    Node* current = head;
    Node* previous = nullptr;

    while (current != nullptr) {
        if (current->card == cardToRemove) {
            // Found the card to remove
            if (previous == nullptr) {
                // Removing the head node
                head = current->next;
            } else {
                previous->next = current->next;
            }

            // Update tail if necessary
            if (current == tail) {
                tail = previous;
            }

            delete current;
            sz--;
            return true;
        }

        previous = current;
        current = current->next;
    }

    return false;
}

    // find playable card according to rules:
    // 1) color match (first encountered)
    // 2) value match (number) (first encountered)
    // 3) action cards in order Skip -> Reverse -> Draw Two (first encountered)
   bool findPlayable(const Card& topCard, Card& chosen) const {
    
    // First pass: find all playable cards
    std::vector<Card> playableCards;
    Node* cur = head;
    while (cur) {
        if (cur->card.matches(topCard)) {
            playableCards.push_back(cur->card);
        }
        cur = cur->next;
    }
    
    if (playableCards.empty()) {
        return false;
    }
    
    // Apply priority: color match > value match > action cards (Skip > Reverse > Draw Two)
    
    // 1. Look for color matches first
    for (const Card& card : playableCards) {
        if (card.color == topCard.color) {
            chosen = card;
            return true;
        }
    }
    
    // 2. Look for number matches
    for (const Card& card : playableCards) {
        if (card.type == Card::NUMBER && topCard.type == Card::NUMBER && card.number == topCard.number) {
            chosen = card;
            return true;
        }
    }
    
    // 3. Look for action cards in priority order
    const Card::Type actionOrder[] = {Card::SKIP, Card::REVERSE, Card::DRAW_TWO};
    for (Card::Type actionType : actionOrder) {
        for (const Card& card : playableCards) {
            if (card.type == actionType) {
                chosen = card;
                return true;
            }
        }
    }
    
    // Fallback: just return the first playable card
    chosen = playableCards[0];
    return true;
}
    // Convert to vector (useful for reshuffle)
    std::vector<Card> toVector() const {
        std::vector<Card> out;
        Node* cur = head;
        while (cur) {
            out.push_back(cur->card);
            cur = cur->next;
        }
        return out;
    }
};

// -------------------------------
// Concrete UNOGame implementation
// -------------------------------
class ConcreteUNOGame : public UNOGame {
private:
    int numPlayers;
    int currentPlayer;
    bool clockwise;
    std::vector<CardLinkedList> hands; // linked list per player
    CardLinkedList deck;               // deck: popFront draws next card
    CardLinkedList discard;            // discard: getBack() is top
    bool gameOverFlag;
    int winnerIdx;

    std::mt19937 rng; // seeded in constructor

public:
    ConcreteUNOGame(int n)
        : UNOGame(n),
          numPlayers(n),
          currentPlayer(0),
          clockwise(true),
          hands(n),
          deck(),
          discard(),
          gameOverFlag(false),
          winnerIdx(-1),
          rng(std::mt19937(1234)) // seed deterministically once
    {}

    ~ConcreteUNOGame() override {
        // destructor: CardLinkedList destructors clear nodes
    }

    // Helper to draw a card (handles reshuffle from discard if deck empty)
    Card drawCard() {
        if (deck.isEmpty()) {
            // Reshuffle discard except top into deck
            if (discard.getSize() > 1) {
                // collect all except last (top)
                std::vector<Card> pool;
                // get discard as vector
                std::vector<Card> dvec = discard.toVector();
                Card top = dvec.back();
                // pool <- first n-1
                for (size_t i = 0; i + 1 < dvec.size(); ++i) pool.push_back(dvec[i]);
                // clear discard and push back top only
                discard.clear();
                discard.pushBack(top);
                // shuffle pool and populate deck
                std::shuffle(pool.begin(), pool.end(), rng);
                for (auto &c : pool) deck.pushBack(c);
            }
        }
        if (!deck.isEmpty()) {
            return deck.popFront();
        }
        return Card(); // fallback: invalid card
    }

    void initialize() override {
    // Reset lists and flags
    for (auto &h : hands) h.clear();
    deck.clear();
    discard.clear();

    currentPlayer = 0;
    clockwise = true;
    gameOverFlag = false;
    winnerIdx = -1;

    // Build canonical deck vector
    std::vector<Card> deckVec;
    const Card::Color colors[] = { Card::RED, Card::GREEN, Card::BLUE, Card::YELLOW };
    for (Card::Color c : colors) {
        // One zero per color
        deckVec.emplace_back(c, Card::NUMBER, 0);
        // Two of each 1-9 per color
        for (int v = 1; v <= 9; ++v) {
            deckVec.emplace_back(c, Card::NUMBER, v);
            deckVec.emplace_back(c, Card::NUMBER, v);
        }
        // action cards (two each)
        deckVec.emplace_back(c, Card::SKIP, -1);
        deckVec.emplace_back(c, Card::SKIP, -1);
        deckVec.emplace_back(c, Card::REVERSE, -1);
        deckVec.emplace_back(c, Card::REVERSE, -1);
        deckVec.emplace_back(c, Card::DRAW_TWO, -1);
        deckVec.emplace_back(c, Card::DRAW_TWO, -1);
    }

    // Deterministic shuffle using seeded rng (member)
    std::shuffle(deckVec.begin(), deckVec.end(), rng);

    // Find first number card for initial discard
    size_t firstNumberCardIdx = 0;
    while (firstNumberCardIdx < deckVec.size() && deckVec[firstNumberCardIdx].type != Card::NUMBER) {
        firstNumberCardIdx++;
    }

    // If no number card found, use first card (shouldn't happen with full deck)
    if (firstNumberCardIdx >= deckVec.size()) {
        firstNumberCardIdx = 0;
    }

    // Set initial discard card
    discard.pushBack(deckVec[firstNumberCardIdx]);

    // Deal 7 cards round-robin, skip the initial discard card
    size_t idx = 0;
    for (int r = 0; r < 7; ++r) {
        for (int p = 0; p < numPlayers; ++p) {
            // Skip the card we used for initial discard
            if (idx == firstNumberCardIdx) {
                idx++;
            }
            if (idx >= deckVec.size()) break;
            hands[p].pushBack(deckVec[idx++]);
        }
    }

    // Add remaining cards to deck, skip the initial discard card
    for (size_t i = 0; i < deckVec.size(); ++i) {
        if (i != firstNumberCardIdx) {
            // Check if this card was already dealt to players
            bool alreadyDealt = false;
            for (const auto& hand : hands) {
                // checks if we've passed the dealing index, this card wasn't dealt
                if (i < idx && i != firstNumberCardIdx) {
                    // add all cards except the initial discard
                    // cards not refreshed at idx
                }
            }
            if (i >= idx || i == firstNumberCardIdx) {
                deck.pushBack(deckVec[i]);
            }
        }
    }

    // create a new vector without the initial discard card
    std::vector<Card> remainingCards;
    for (size_t i = 0; i < deckVec.size(); ++i) {
        if (i != firstNumberCardIdx) {
            remainingCards.push_back(deckVec[i]);
        }
    }
    
    // Shuffle the remaining cards and add to deck
    std::shuffle(remainingCards.begin(), remainingCards.end(), rng);
    for (const auto& card : remainingCards) {
        deck.pushBack(card);
    }
}

    void playTurn() override {
    if (gameOverFlag) return;

    if (discard.isEmpty()) {
        currentPlayer = nextIndexFrom(currentPlayer, 1, clockwise);
        return;
    }

    Card top = discard.getBack();
    Card chosen;
    bool hasPlayable = hands[currentPlayer].findPlayable(top, chosen);

    if (hasPlayable) {
        // play chosen
        bool removed = hands[currentPlayer].removeCard(chosen);
        discard.pushBack(chosen);

        // check win immediately (player who played)
        if (hands[currentPlayer].getSize() == 0) {
            gameOverFlag = true;
            winnerIdx = currentPlayer;
            return;
        }

        // apply effect & set next currentPlayer
        if (chosen.type == Card::NUMBER) {
            currentPlayer = nextIndexFrom(currentPlayer, 1, clockwise);
        } else if (chosen.type == Card::SKIP) {
            // Skip means next player misses turn
            currentPlayer = nextIndexFrom(currentPlayer, 2, clockwise);
        } else if (chosen.type == Card::REVERSE) {
            if (numPlayers == 2) {
                // In 2-player, Reverse acts like Skip
                currentPlayer = nextIndexFrom(currentPlayer, 2, clockwise);
            } else {
                clockwise = !clockwise;
                currentPlayer = nextIndexFrom(currentPlayer, 1, clockwise);
            }
        } else if (chosen.type == Card::DRAW_TWO) {
            int victim = nextIndexFrom(currentPlayer, 1, clockwise);
            // victim draws two
            for (int d = 0; d < 2; ++d) {
                Card dc = drawCard();
                if (dc.type == Card::NUMBER && dc.color == Card::NONE && dc.number == -1) {
                    break;
                }
                hands[victim].pushBack(dc);
            }
            // skip victim
            currentPlayer = nextIndexFrom(currentPlayer, 2, clockwise);
        }

    } else {
        // draw one card
        Card drawn = drawCard();
        if (drawn.type == Card::NUMBER && drawn.color == Card::NONE && drawn.number == -1) {
            currentPlayer = nextIndexFrom(currentPlayer, 1, clockwise);
            return;
        }

        // Check if drawn card matches top card
        bool canPlayDrawn = drawn.matches(top);

        if (canPlayDrawn) {
            // play drawn immediately
            discard.pushBack(drawn);
            // apply drawn card effect
            if (drawn.type == Card::NUMBER) {
                currentPlayer = nextIndexFrom(currentPlayer, 1, clockwise);
            } else if (drawn.type == Card::SKIP) {
                currentPlayer = nextIndexFrom(currentPlayer, 2, clockwise);
            } else if (drawn.type == Card::REVERSE) {
                if (numPlayers == 2) {
                    currentPlayer = nextIndexFrom(currentPlayer, 2, clockwise);
                } else {
                    clockwise = !clockwise;
                    currentPlayer = nextIndexFrom(currentPlayer, 1, clockwise);
                }
            } else if (drawn.type == Card::DRAW_TWO) {
                int victim = nextIndexFrom(currentPlayer, 1, clockwise);
                for (int d = 0; d < 2; ++d) {
                    Card dc = drawCard();
                    if (dc.type == Card::NUMBER && dc.color == Card::NONE && dc.number == -1) break;
                    hands[victim].pushBack(dc);
                }
                currentPlayer = nextIndexFrom(currentPlayer, 2, clockwise);
            }
        } else {
            // cannot play drawn - add to hand and turn passes
            hands[currentPlayer].pushBack(drawn);
            currentPlayer = nextIndexFrom(currentPlayer, 1, clockwise);
        }
    }
}
    bool isGameOver() const override { return gameOverFlag; }
    int getWinner() const override { return winnerIdx; }

    std::string getState() const override {
        std::ostringstream ss;
        ss << "Player " << currentPlayer << "'s turn, Direction: "
           << (clockwise ? "Clockwise" : "Counter-clockwise")
           << ", Top: " << (discard.isEmpty() ? Card().toString() : discard.getBack().toString())
           << ", Players cards: ";
        for (int i = 0; i < numPlayers; ++i) {
            ss << "P" << i << ":" << hands[i].getSize();
            if (i < numPlayers - 1) ss << ", ";
        }
        return ss.str();
    }

private:
    // compute next index from idx by steps according to direction dir
    int nextIndexFrom(int idx, int steps, bool dir) const {
        if (numPlayers == 0) return 0;
        int s = ((steps % numPlayers) + numPlayers) % numPlayers;
        if (dir) { // clockwise
            return (idx + s) % numPlayers;
        } else {
            int val = (idx - s) % numPlayers;
            if (val < 0) val += numPlayers;
            return val;
        }
    }
};

// Factory method
std::unique_ptr<UNOGame> UNOGame::create(int numPlayers) {
    return std::make_unique<ConcreteUNOGame>(numPlayers);
}
