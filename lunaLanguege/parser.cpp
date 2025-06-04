// parser.cpp
#include <stdexcept>
#include <iostream>
#include <vector>
#include <filesystem>
#include <stack> // Add this for std::stack
#include "lexer.cpp"

// TokenType extension for custom statements
enum ExtendedTokenType {
    TOKEN_IMPORT = 1000, // Use a value outside the base enum to avoid collision
    TOKEN_STRING
};

// Parser class
class Parser {
public:
    Parser(const std::string& sourceCode) : lexer_(sourceCode), loopExit_(false), loop_(0) {} // initialize new members

    // Parse the source code and generate an abstract syntax tree (AST)
    void parse() {
        // Get the first token from the lexer
        Token token = lexer_.getNextToken();

        // Start parsing the program
        program(token);
    }

    // Program -> Statement*
    void program(Token token) {
        while (token.type != TOKEN_EOF) {
            statement(token);
            token = lexer_.getNextToken();
        }
    }

    // Statement -> IfStatement | WhileStatement | FunctionDeclaration | ExpressionStatement
    void statement(Token token) {
        if (token.type == TOKEN_KEYWORD && token.value == "if") {
            ifStatement(token);
        }
        else if (token.type == TOKEN_KEYWORD && token.value == "while") {
            whileStatement(token);
        }
        else if (token.type == TOKEN_KEYWORD && token.value == "func") {
            functionDeclaration(token);
        }
        else if (token.type == TOKEN_KEYWORD && token.value == "import") {
            importStatement(token);
        }
        else if (token.type == TOKEN_KEYWORD && (token.value == "true" || token.value == "false")) {
            boolStatement(token);
        }
        else if (token.type == TOKEN_KEYWORD && token.value == "export") {
            exportStatement(token);
        }
        else if (token.type == TOKEN_KEYWORD && token.value == "let") {
            letStatement(token);

        }
        else if (token.type == TOKEN_KEYWORD && token.value == "var") {
            varStatement(token);
        }
        else if (token.type == TOKEN_KEYWORD && token.value == "break") {
            breakStatement(token);
        }
        else if (token.type == TOKEN_KEYWORD && token.value == "loop") {
            loopStatement(token);
        }
        else if (token.type == TOKEN_KEYWORD && token.value == "print") {
            printstament(token);
        }
        else if (token.type == TOKEN_STRING) {
            stringStatement(token);
        }
        else if (token.type == TOKEN_NUMBER) {
            numberStatement(token);
        }
        else if (token.type == TOKEN_IDENTIFIER) {
            callStatement(token); // Handle function calls or variable access
        }
        else {
            expressionStatement(token);
        }
    }

	// letStatement -> "let" IDENTIFIER "=" Expression ";"
	void letStatement(Token token) {
		// Consume the "let" keyword
		token = lexer_.getNextToken();

		// Check for identifier
		if (token.type != TOKEN_IDENTIFIER) {
			error("Expected identifier");
		}

		// Get the identifier name
		std::string identifier = token.value;

		// Consume the "=" operator
		token = lexer_.getNextToken();

		// Parse the expression
		expression(token);

		// Consume the ";" terminator
		token = lexer_.getNextToken();

		// Check for syntax error
		if (token.type != TOKEN_SYMBOL || token.value != ";") {
			error("Expected ';' terminator");
		}
	}

    // IfStatement -> "if" Expression "then" Statement "end"
    void ifStatement(Token token) {
        // Consume the "if" keyword
        token = lexer_.getNextToken();

        // Parse the condition expression
        expression(token);

        // Consume the "then" keyword
        token = lexer_.getNextToken();

        // Parse the then statement
        statement(token);

        // Consume the "end" keyword
        token = lexer_.getNextToken();

        // Check for syntax error
        if (token.type != TOKEN_KEYWORD || token.value != "end") {
            error("Expected 'end' keyword");
        }
    }

    // WhileStatement -> "while" Expression "do" Statement "end"
    void whileStatement(Token token) {
        // Consume the "while" keyword
        token = lexer_.getNextToken();

        // Parse the condition expression
        expression(token);

        // Consume the "do" keyword
        token = lexer_.getNextToken();

        // Parse the loop body statement
        statement(token);

        // Consume the "end" keyword
        token = lexer_.getNextToken();

        // Check for syntax error
        if (token.type != TOKEN_KEYWORD || token.value != "end") {
            error("Expected 'end' keyword after while statement");
        }
    }

    // FunctionDeclaration -> "func" IDENTIFIER Statement "end"
    void functionDeclaration(Token token) {
        // Consume the "func" keyword
        token = lexer_.getNextToken();

        // Parse function name (identifier)
        if (token.type != TOKEN_IDENTIFIER) {
            error("Expected function name after 'func'");
        }
        token = lexer_.getNextToken();

        // Optionally parse parameters, etc. (not implemented)

        // Parse function body
        statement(token);

        // Consume the "end" keyword
        token = lexer_.getNextToken();

        if (token.type != TOKEN_KEYWORD || token.value != "end") {
            error("Expected 'end' keyword after function declaration");
        }
    }


	// PrintStatement -> "print" Expression = () IDENTIFIER ";"
    void printstament(Token token) {
        // Consume the "print" keyword
        token = lexer_.getNextToken();
        // Expect an identifier or literal for printing
        if (token.type != TOKEN_IDENTIFIER && token.type != TOKEN_LITERAL) {
            error("Expected identifier or literal after 'print'");
        }
        // Print the value
        std::cout << token.value << std::endl;
        // Consume the ";" terminator
        token = lexer_.getNextToken();
        if (token.type != TOKEN_SYMBOL || token.value != ";") {
            error("Expected ';' terminator after print statement");
		}
		
        

    }

    // ImportStatement -> "import" IDENTIFIER
    void importStatement(Token token) {
        // Consume the "import" keyword
        token = lexer_.getNextToken();
        // Expect an identifier for the module name
        if (token.type != TOKEN_IDENTIFIER) {
            error("Expected module name after 'import'");
        }
        // Construct the directory path
        std::string dir = "lina_modules/" + token.value;

        // Check if the directory exists
        if (std::filesystem::exists(dir)) {
            std::cout << "Module found: " << token.value << std::endl;
        } else {
            error("Module not found");
        }
    }

	// VarStatement -> "var" IDENTIFIER "=" Expression ";"
	void varStatement(Token token) {
		// Consume the "var" keyword
		token = lexer_.getNextToken();
        // Expect an identifier for the variable name
        if (token.type != TOKEN_IDENTIFIER) {
            error("Expected variable name after 'var'");
        }
        // Get the variable name
        std::string varName = token.value;
        // Consume the "=" operator
        token = lexer_.getNextToken();
        // Parse the expression
        expression(token);
        // Consume the ";" terminator
        token = lexer_.getNextToken();
        // Check for syntax error
        if (token.type != TOKEN_SYMBOL || token.value != ";") {
            error("Expected ';' terminator after variable declaration");
        }
	}

    // ExpressionStatement -> Expression
    void expressionStatement(Token token) {
        // Parse the expression
        expression(token);
    }

    // BoolStatement -> "true" | "false"
    void boolStatement(Token token) {
        if (token.type == TOKEN_KEYWORD && token.value == "true") {
            // set stuff to true

        }
        else if (token.type == TOKEN_KEYWORD && token.value == "false") {
			// set stuff to false
            
        }
        else {
            error("Expected 'true' or 'false' keyword");
        }
    }

    // Handle a break statement
    void breakStatement(Token token) {
        // Check if the token is a break keyword
        if (token.type == TOKEN_KEYWORD && token.value == "break") {
            // Exit the loop
            loopStack_.pop();
            // Set the loop exit flag
            loopExit_ = true;
        }
        else {
            // Error: expected break keyword
            error("Expected 'break' keyword");
        }
    }
    // Handle a loop statement
    void loopStatement(Token token) {
        // Check if the token is a loop keyword
        if (token.type == TOKEN_KEYWORD && token.value == "loop") {
            // Push the loop onto the loop stack
            loopStack_.push(loop_);
            // Set the loop exit flag to false
            loopExit_ = false;
            // Execute the loop body
            while (!loopExit_) {
                // Execute the loop body
                statement(token);
            }
            // Pop the loop from the loop stack
            loopStack_.pop();
        }
        else {
            // Error: expected loop keyword
            error("Expected 'loop' keyword");
        }
    }

    // exportStatement -> "export" IDENTIFIER
    void exportStatement(Token token) {
        // Consume the "export" keyword
        token = lexer_.getNextToken();

		// Expect an identifier for the variable name
		if (token.type != TOKEN_IDENTIFIER) {
			error("Expected variable name after 'export'");
		}

		// Get the variable name
		std::string varName = token.value;

		std::cout << "Exporting variable: " << varName << std::endl;

		// Consume the token
		token = lexer_.getNextToken();

		// Check for syntax error (optional, depending on your language design)
		if (token.type != TOKEN_SYMBOL || token.value != ";") {
			error("Expected ';' terminator");
		}

    }
    // CallStatement -> IDENTIFIER (arguments not implemented)
    void callStatement(Token token) {
        if (token.type == TOKEN_IDENTIFIER) {
            std::cout << "Function call: " << token.value << std::endl;
            // Consume the identifier token
            token = lexer_.getNextToken();
            // Optionally handle arguments (not implemented)
        }
        else {
            error("Expected function name for call statement");
        }
    }

    // Handle a string literal statement
    void stringStatement(Token token) {
        // For demonstration, just print the string literal
        std::cout << "String literal: \"" << token.value << "\"" << std::endl;
    }

    // Handle a number literal statement
    void numberStatement(Token token) {
        // For demonstration, just print the number literal
        std::cout << "Number literal: " << token.value << std::endl;
    }

    // Expression -> LITERAL | IDENTIFIER | NUMBER | STRING
    void expression(Token token) {
        if (token.type != TOKEN_LITERAL &&
            token.type != TOKEN_IDENTIFIER &&
            token.type != TOKEN_NUMBER &&
            token.type != TOKEN_STRING) {
            error("Expected expression");
        }
    }

    // Error handling function
    void error(const std::string& message) {
        std::cerr << "Error: " << message << std::endl;
        exit(1);
    }

    // Lexer instance
    Lexer lexer_;

    // Add missing members for loop handling
    std::stack<int> loopStack_; // or use appropriate type if needed
    bool loopExit_;
    int loop_; // Placeholder type, adjust as needed
};