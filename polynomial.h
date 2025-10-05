#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <string>
#include <memory>

class Polynomial {
public:
    virtual ~Polynomial() = default;
    
    // Insert a term into the polynomial
    virtual void insertTerm(int coefficient, int exponent) = 0;

    // Return polynomial as a human-readable string
    virtual std::string toString() const = 0;

    // Return a new polynomial that is the sum of this and other
    virtual std::unique_ptr<Polynomial> add(const Polynomial& other) const = 0;

    // Return a new polynomial that is the product of this and other
    virtual std::unique_ptr<Polynomial> multiply(const Polynomial& other) const = 0;

    // Return a new polynomial that is the derivative of this polynomial
    virtual std::unique_ptr<Polynomial> derivative() const = 0;

    // Create a concrete instance
    static std::unique_ptr<Polynomial> create();
};

#endif