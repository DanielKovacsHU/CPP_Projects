// this project showcases some of what i learned in learncpp's Chapter 1 Basics

#include <iostream>     // to use input/output functions
#include <cmath>        // to do math

// this is a simple calculator, asks 2 times for a number, than shows the most common operations and their results
int main()
{
    std::cout << "Enter the first number: ";
    double num1 {};
    std::cin >> num1;

    std::cout << "Enter the second number: ";
    double num2 {};
    std::cin >> num2;

    std::cout << '\n' << num1 << " + " << num2 << " is " << num1 + num2 << '\n';
    std::cout << num1 << " - " << num2 << " is " << num1 - num2 << '\n';
    std::cout << num1 << " * " << num2 << " is " << num1 * num2 << '\n';
    std::cout << num1 << " / " << num2 << " is " << num1 / num2 << '\n';
    std::cout << num1 << " raised to the exponent " << num2 << " is " << std::pow(num1,num2) << '\n';
    std::cout << num1 << " under the " << num2 << "nth root is " << std::pow(num1,1/num2) << '\n';

    return 0;
}

