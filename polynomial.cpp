#include "polynomial.h"
#include <memory>
#include <sstream>
#include <iostream>

using namespace std;

// Term node for linked list
struct TermNode {
    int coefficient;
    int exponent;
    unique_ptr<TermNode> next;
    
    TermNode(int coeff, int exp) : coefficient(coeff), exponent(exp), next(nullptr) {}
};

class ConcretePolynomial : public Polynomial {
private:
    unique_ptr<TermNode> head;
    
    // Helper to insert term maintaining sorted order (highest exponent first)
    void insertSorted(int coefficient, int exponent) {
        if (coefficient == 0) return;
        
        // Create new node
        auto newNode = make_unique<TermNode>(coefficient, exponent);
        
        // If list is empty or new node has higher exponent than head
        if (!head || exponent > head->exponent) {
            newNode->next = move(head);
            head = move(newNode);
            return;
        }
        
        // Find insertion point
        TermNode* current = head.get();
        TermNode* prev = nullptr;
        
        while (current && current->exponent > exponent) {
            prev = current;
            current = current->next.get();
        }
        
        // Combine like terms if found
        if (current && current->exponent == exponent) {
            current->coefficient += coefficient;
            if (current->coefficient == 0) {
                // Remove the term if coefficient becomes zero
                if (prev) {
                    prev->next = move(current->next);
                } else {
                    head = move(head->next);
                }
            }
            return;
        }
        
        // Insert new term
        if (prev) {
            newNode->next = move(prev->next);
            prev->next = move(newNode);
        }
    }
    
public:
    ConcretePolynomial() : head(nullptr) {}
    
    // Copy constructor for operations
    ConcretePolynomial(const ConcretePolynomial& other) : head(nullptr) {
        TermNode* current = other.head.get();
        while (current) {
            insertTerm(current->coefficient, current->exponent);
            current = current->next.get();
        }
    }
    
    void insertTerm(int coefficient, int exponent) override {
        insertSorted(coefficient, exponent);
    }
    
    string toString() const override {
        if (!head) return "0";
        
        ostringstream oss;
        TermNode* current = head.get();
        bool firstTerm = true;
        
        while (current) {
            int coeff = current->coefficient;
            int exp = current->exponent;
            
            if (!firstTerm) {
                if (coeff > 0) {
                    oss << " + ";
                } else {
                    oss << " - ";
                    coeff = -coeff;
                }
            } else {
                firstTerm = false;
                if (coeff < 0) {
                    oss << "-";
                    coeff = -coeff;
                }
            }
            
            if (exp == 0) {
                oss << coeff;
            } else if (exp == 1) {
                if (coeff == 1) {
                    oss << "x";
                } else {
                    oss << coeff << "x";
                }
            } else {
                if (coeff == 1) {
                    oss << "x^" << exp;
                } else {
                    oss << coeff << "x^" << exp;
                }
            }
            
            current = current->next.get();
        }
        
        return oss.str();
    }
    
    unique_ptr<Polynomial> add(const Polynomial& other) const override {
        const ConcretePolynomial& otherPoly = dynamic_cast<const ConcretePolynomial&>(other);
        auto result = make_unique<ConcretePolynomial>();
        
        // Merge two sorted linked lists
        TermNode* current1 = head.get();
        TermNode* current2 = otherPoly.head.get();
        
        while (current1 || current2) {
            if (current1 && current2) {
                if (current1->exponent > current2->exponent) {
                    result->insertTerm(current1->coefficient, current1->exponent);
                    current1 = current1->next.get();
                } else if (current1->exponent < current2->exponent) {
                    result->insertTerm(current2->coefficient, current2->exponent);
                    current2 = current2->next.get();
                } else {
                    int sum = current1->coefficient + current2->coefficient;
                    if (sum != 0) {
                        result->insertTerm(sum, current1->exponent);
                    }
                    current1 = current1->next.get();
                    current2 = current2->next.get();
                }
            } else if (current1) {
                result->insertTerm(current1->coefficient, current1->exponent);
                current1 = current1->next.get();
            } else {
                result->insertTerm(current2->coefficient, current2->exponent);
                current2 = current2->next.get();
            }
        }
        
        return result;
    }
    
    unique_ptr<Polynomial> multiply(const Polynomial& other) const override {
        const ConcretePolynomial& otherPoly = dynamic_cast<const ConcretePolynomial&>(other);
        auto result = make_unique<ConcretePolynomial>();
        
        // Multiply each term from this polynomial with each term from other polynomial
        for (TermNode* term1 = head.get(); term1; term1 = term1->next.get()) {
            for (TermNode* term2 = otherPoly.head.get(); term2; term2 = term2->next.get()) {
                int newCoeff = term1->coefficient * term2->coefficient;
                int newExp = term1->exponent + term2->exponent;
                result->insertTerm(newCoeff, newExp);
            }
        }
        
        return result;
    }
    
    unique_ptr<Polynomial> derivative() const override {
        auto result = make_unique<ConcretePolynomial>();
        
        for (TermNode* current = head.get(); current; current = current->next.get()) {
            if (current->exponent > 0) {
                int newCoeff = current->coefficient * current->exponent;
                int newExp = current->exponent - 1;
                result->insertTerm(newCoeff, newExp);
            }
        }
        
        return result;
    }
};

// Factory function
unique_ptr<Polynomial> Polynomial::create() {
    return make_unique<ConcretePolynomial>();
}