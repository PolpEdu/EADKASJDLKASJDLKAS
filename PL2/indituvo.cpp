#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

// prove that f really calculates the factorial of a number
int f(int n) {
    if (n == 0) { // base de case (n = 0)
        return 1;
    }
    else {
        return n * f(n - 1);
    }
}

// Base Case
// n = 0, f(0) = 1, 0! = 1 

// Inductive Hypothesis
// f(n) = n! for all n >= 0

// Inductive Step
// f(n+1) = (n+1)! (=)
// (n+1) * f(n) = (n+1)!, Inductive Hypothesis f(n) = n!
// (n+1) * n! = (n+1)!
// (n+1)! = (n+1)!, True, therefore f(n+1) = (n+1)!

// by all of the above, f(n) = n! for all n >= 0

int main() {
    
}
