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
    TOKEN_NUMBER, // Added for number literals
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
        skipWhitespaceAndComments();

        // Check for end of file
        if (pos_ >= sourceCode_.size()) {
            return Token{ TOKEN_EOF, "" };
        }

        char current = sourceCode_[pos_];

        // String literal
        if (current == '"' || current == '\'') {
            return getStringLiteral();
        }

        // Number literal
        if (isdigit(current)) {
            return getNumberLiteral();
        }

        // Identifier or keyword
        if (isalpha(current) || current == '_') {
            return getIdentifierOrKeyword();
        }

        // Symbols and operators
        if (isSymbolStart(current)) {
            return getSymbol();
        }

        // Error: unknown token
        std::cerr << "Error: unknown token at position " << pos_ << std::endl;
        exit(1);
    }

private:
    // Skip whitespace and comments
    void skipWhitespaceAndComments() {
        while (pos_ < sourceCode_.size()) {
            char c = sourceCode_[pos_];
            // Skip whitespace
            if (isspace(c)) {
                ++pos_;
                continue;
            }
            // Single-line comment
            if (c == '/' && pos_ + 1 < sourceCode_.size() && sourceCode_[pos_ + 1] == '/') {
                pos_ += 2;
                while (pos_ < sourceCode_.size() && sourceCode_[pos_] != '\n') ++pos_;
                continue;
            }
            // Multi-line comment
            if (c == '/' && pos_ + 1 < sourceCode_.size() && sourceCode_[pos_ + 1] == '*') {
                pos_ += 2;
                while (pos_ + 1 < sourceCode_.size() && !(sourceCode_[pos_] == '*' && sourceCode_[pos_ + 1] == '/')) ++pos_;
                pos_ += 2; // skip */
                continue;
            }
            break;
        }
    }

    // String literal with escape support
    Token getStringLiteral() {
        char quote = sourceCode_[pos_];
        size_t start = ++pos_;
        std::string value;
        bool closed = false;
        while (pos_ < sourceCode_.size()) {
            char c = sourceCode_[pos_];
            if (c == '\\' && pos_ + 1 < sourceCode_.size()) {
                char next = sourceCode_[pos_ + 1];
                if (next == quote || next == '\\' || next == 'n' || next == 't') {
                    if (next == 'n') value += '\n';
                    else if (next == 't') value += '\t';
                    else value += next;
                    pos_ += 2;
                    continue;
                }
            }
            if (c == quote) {
                closed = true;
                ++pos_;
                break;
            }
            value += c;
            ++pos_;
        }
        if (!closed) {
            std::cerr << "Error: Unterminated string literal\n";
            exit(1);
        }
        return Token{ TOKEN_STRING, value };
    }

    // Number literal (integer or float)
    Token getNumberLiteral() {
        size_t start = pos_;
        bool isFloat = false;
        while (pos_ < sourceCode_.size() && (isdigit(sourceCode_[pos_]) || sourceCode_[pos_] == '.')) {
            if (sourceCode_[pos_] == '.') {
                if (isFloat) break; // only one dot allowed
                isFloat = true;
            }
            ++pos_;
        }
        std::string value = sourceCode_.substr(start, pos_ - start);
        return Token{ TOKEN_NUMBER, value };
    }

    // Identifier or keyword
    Token getIdentifierOrKeyword() {
        size_t start = pos_;
        while (pos_ < sourceCode_.size() && (isalnum(sourceCode_[pos_]) || sourceCode_[pos_] == '_')) {
            ++pos_;
        }
        std::string value = sourceCode_.substr(start, pos_ - start);
        if (isKeyword(value)) {
            if (value == "import") return Token{ TOKEN_IMPORT, value };
            return Token{ TOKEN_KEYWORD, value };
        }
        return Token{ TOKEN_IDENTIFIER, value };
    }

    // Symbol recognition (multi-char and single-char)
    Token getSymbol() {
        static const std::vector<std::string> multiCharSymbols = {
            "==", "!=", "<=", ">=", "&&", "||"
        };
        for (const auto& sym : multiCharSymbols) {
            if (sourceCode_.substr(pos_, sym.size()) == sym) {
                pos_ += sym.size();
                return Token{ TOKEN_SYMBOL, sym };
            }
        }
        char c = sourceCode_[pos_++];
        return Token{ TOKEN_SYMBOL, std::string(1, c) };
    }

    bool isSymbolStart(char c) {
        static const std::string symbols = "+-*/=<>!&|;:,.(){}[]";
        return symbols.find(c) != std::string::npos;
    }

    // Helper functions for checking token types
    bool isKeyword(const std::string& str) {
        static const std::vector<std::string> keywords = {
            "if", "while", "func", "return", "end", "true", "false", "null",
            "print", "input", "var", "let", "const", "break", "continue", "import", "export", "loop"
        };
        for (const auto& kw : keywords) {
            if (str == kw) return true;
        }
        return false;
    }

    std::string sourceCode_;
    size_t pos_;
};


