// Phase 1: Console Calculator (pure C++, no Qt yet).
// New ideas practiced here: variables & types, reading input with
// std::cin/std::getline, if/else if branching, writing our own function,
// and basic error handling (unknown operator, divide-by-zero).
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

// All the math logic lives in this one function, separate from the
// "ask the user, then print" flow in main(). `ok` is set to false when
// something goes wrong (unknown operator or divide-by-zero) so main()
// knows to print an error instead of a bogus number.
double calculate(double first, char op, double second, bool &ok)
{
    ok = true;

    if (op == '+')
    {
        return first + second;
    }
    else if (op == '-')
    {
        return first - second;
    }
    else if (op == '*')
    {
        return first * second;
    }
    else if (op == '/')
    {
        if (second == 0.0)
        {
            ok = false;
            return 0.0;
        }
        return first / second;
    }

    ok = false; // not one of + - * /
    return 0.0;
}

int main()
{
    // Default cout precision is only 6 significant digits, which makes
    // larger results print in scientific notation (e.g. 1.33333e+07)
    // and lose digits. Bumping it to 15 covers the full precision a
    // double can hold, so results print as plain, exact numbers.
    std::cout << std::setprecision(15);

    std::cout << "=== Console Calculator ===" << std::endl;
    std::cout << "Enter a calculation like: 3 + 4" << std::endl;
    std::cout << "Operators: +  -  *  /    (type q to quit)" << std::endl
               << std::endl;

    std::string line;
    while (true)
    {
        std::cout << "Enter calculation (e.g. 3 + 4): ";

        if (!std::getline(std::cin, line))
        {
            break; // input stream closed (e.g. Ctrl+Z then Enter)
        }

        if (line == "q" || line == "Q")
        {
            break;
        }

        // Pull "first op second" out of the typed line, e.g. "3 + 4"
        // becomes first=3, op='+', second=4.
        std::istringstream lineStream(line);
        double first = 0.0;
        char op = '\0';
        double second = 0.0;

        if (!(lineStream >> first >> op >> second))
        {
            std::cout << "Sorry, I couldn't read that. Try: 3 + 4" << std::endl
                       << std::endl;
            continue;
        }

        bool ok = false;
        double result = calculate(first, op, second, ok);

        if (!ok)
        {
            if (op == '/')
            {
                std::cout << "Error: can't divide by zero!" << std::endl;
            }
            else
            {
                std::cout << "Error: '" << op
                           << "' isn't an operator I know (+ - * /)." << std::endl;
            }
        }
        else
        {
            std::cout << "= " << result << std::endl;
        }

        std::cout << std::endl;
    }

    std::cout << "Goodbye!" << std::endl;
    return 0;
}
