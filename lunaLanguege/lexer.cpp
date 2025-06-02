// lexer.cpp

#include <iostream>
#include <string>
#include <vector>

// Token types
enum TokenType {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_LITERAL,
    TOKEN_SYMBOL,
    TOKEN_EOF,
	TOKEN_ERROR,
	TOKEN_IMPORT,
	TOKEN_STRING,
};

// Token structure
struct Token {
    TokenType type;
    std::string value;
};

// Lexer class
class Lexer {
public:
    Lexer(const std::string& sourceCode) : sourceCode_(sourceCode), pos_(0) {}

    // Get the next token from the source code
    Token getNextToken() {
        // Skip whitespace
        while (pos_ < sourceCode_.size() && isspace(sourceCode_[pos_])) {
            pos_++;
        }

        // Check for end of file
        if (pos_ >= sourceCode_.size()) {
            return Token{ TOKEN_EOF, "" };
        }

        // Check for keywords
        if (isKeyword(sourceCode_.substr(pos_))) {
            return Token{ TOKEN_KEYWORD, getKeyword(sourceCode_.substr(pos_)) };
        }

        // Check for identifiers
        if (isIdentifier(sourceCode_.substr(pos_))) {
            return Token{ TOKEN_IDENTIFIER, getIdentifier(sourceCode_.substr(pos_)) };
        }

        // Check for literals
        if (isLiteral(sourceCode_.substr(pos_))) {
            return Token{ TOKEN_LITERAL, getLiteral(sourceCode_.substr(pos_)) };
        }

        // Check for symbols
        if (isSymbol(sourceCode_.substr(pos_))) {
            return Token{ TOKEN_SYMBOL, getSymbol(sourceCode_.substr(pos_)) };
        }

		// Check for import statements
		if (isImport(sourceCode_.substr(pos_))) {
			return Token{ TOKEN_IMPORT, getImport(sourceCode_.substr(pos_)) };
		}

		// Check for string literals
		if (isString(sourceCode_.substr(pos_))) {
			return Token{ TOKEN_STRING, getLiteral(sourceCode_.substr(pos_)) };
		}

		

        

        // Error: unknown token
        std::cerr << "Error: unknown token at position " << pos_ << std::endl;
        exit(1);
    }

private:
    // Helper functions for checking token types
    bool isKeyword(const std::string& str) {
        // Implement keyword checking logic here
        
        // For example:
        return str == "if" || str == "while" || str == "func" || str == "return" || str == "end" || str == "true" || str == "false" || str == "null" || str == "print"  || str == "input" || str == "var" || str == "let" || str == "const" || str == "break" || str == "continue";
    }

    bool isImport(const std::string& str) {
        // Implement import statement checking logic here
        return str.substr(0, 6) == "import";
	}

	std::string getImport(const std::string& str) {
		// Implement import statement extraction logic here
		return str.substr(7);
	}

    bool isString(const std::string& str) {
        // Implement string literal checking logic here
        return str[0] == '"' || str[0] == '\'';
	}

    



    std::string getKeyword(const std::string& str) {
        // Implement keyword extraction logic here
        // For example:
        return str;
    }

    bool isIdentifier(const std::string& str) {
        // Implement identifier checking logic here
        // For example:
        return str[0] == '_' || isalpha(str[0]);
    }

    std::string getIdentifier(const std::string& str) {
        // Implement identifier extraction logic here
        // For example:
        return str;
    }

    bool isLiteral(const std::string& str) {
        // Implement literal checking logic here
        // For example:
        return str[0] == '"' || str[0] == '\'';
    }

    std::string getLiteral(const std::string& str) {
        // Implement literal extraction logic here
        // For example:
        return str.substr(1, str.size() - 2);
    }

    bool isSymbol(const std::string& str) {
        // Implement symbol checking logic here
        // For example:
        return str == "+" || str == "-" || str == "*" || str == "/";
    }

    std::string getSymbol(const std::string& str) {
        // Implement symbol extraction logic here
        // For example:
        return str;
    }

    std::string sourceCode_;
    size_t pos_;
};


