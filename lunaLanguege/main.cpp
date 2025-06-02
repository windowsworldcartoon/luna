#include <iostream>
#include "parser.cpp"

int main() {
    // Create a Luna parser
    Parser parser;

    // Define a Luna script
    std::string script = R"(
        print("Hello, World!")
        x = 5
        y = 10
        print(x + y)
    )";

    // Parse and execute the Luna script
    parser.parse(script);
    parser.execute();

    return 0;
}

