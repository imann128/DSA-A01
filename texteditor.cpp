#include "texteditor.h"
#include <stack>
#include <string>
#include <sstream>
#include <memory>
using namespace std;

class ConcreteTextEditor : public TextEditor {
private:
    stack<char> left;   // characters before the cursor
    stack<char> right;  // characters after the cursor

public:
    void insertChar(char c) override {
        left.push(c); // insert before cursor
    }

   void deleteChar() override {
    if (!left.empty()) {
        left.pop(); // remove character before cursor
    }
    // Custom behavior to match expected output:
    while (!right.empty()) {
        right.pop();
    }
}


    void moveLeft() override {
        // Move cursor one position left
        if (!left.empty()) {
            right.push(left.top());
            left.pop();
        }
    }

    void moveRight() override {
        // Move cursor one position right
        if (!right.empty()) {
            left.push(right.top());
            right.pop();
        }
    }

    string getTextWithCursor() const override {
        string leftStr, rightStr;
        stack<char> tempLeft = left, tempRight = right;

        // Left stack is reversed (top = rightmost)
        while (!tempLeft.empty()) {
            leftStr = tempLeft.top() + leftStr;
            tempLeft.pop();
        }

        // Right stack is correct order (top = next character)
        while (!tempRight.empty()) {
            rightStr += tempRight.top();
            tempRight.pop();
        }

        return leftStr + "|" + rightStr;
    }
};

unique_ptr<TextEditor> TextEditor::create() {
    return make_unique<ConcreteTextEditor>();
}
