#include "texteditor.h"
#include <iostream>
using namespace std;

void printState(const TextEditor& ed, const string& description) {
    cout << description << ": " << ed.getTextWithCursor() << endl;
}

int main() {
    auto ed = TextEditor::create();

    ed->insertChar('a');
    printState(*ed, "After insert 'a'");

    ed->insertChar('b');
    printState(*ed, "After insert 'b'");

    ed->moveLeft();
    printState(*ed, "After move left once");

    ed->insertChar('c');
    printState(*ed, "After insert 'c'");

    ed->deleteChar();
    printState(*ed, "After delete");

    ed->moveLeft();
    printState(*ed, "After move left once");

    ed->moveLeft();
    printState(*ed, "After move left twice");

    ed->moveRight();
    printState(*ed, "After move right once");

    ed->moveRight();
    printState(*ed, "After move right twice");

    return 0;
}
