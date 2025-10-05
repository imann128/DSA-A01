#include "polynomial.h"
#include <iostream>

using namespace std;

int main() {
    // Create p1: 3x^4 + 2x^2 - x + 5
    auto p1 = Polynomial::create();
    p1->insertTerm(3, 4);
    p1->insertTerm(2, 2);
    p1->insertTerm(-1, 1);
    p1->insertTerm(5, 0);
    
    // Create p2: x^4 + 1
    auto p2 = Polynomial::create();
    p2->insertTerm(1, 4);
    p2->insertTerm(1, 0);
    
    // Test addition
    auto sum = p1->add(*p2);
    
    // Create p3: 2x
    auto p3 = Polynomial::create();
    p3->insertTerm(2, 1);
    
    // Test multiplication
    auto prod = sum->multiply(*p3);
    
    // Test derivative
    auto deriv = p1->derivative();
    
    // Output results - should match sample output exactly
    cout << "p1.toString(): " << p1->toString() << endl;
    cout << "sum.toString(): " << sum->toString() << endl;
    cout << "prod.toString(): " << prod->toString() << endl;
    cout << "deriv.toString(): " << deriv->toString() << endl;
    
    return 0;
}