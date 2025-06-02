// runtime.cpp

#include <iostream>
#include <map>
#include <vector>
                            #include <string>

// Minimal AST node and enums to resolve missing identifiers
enum AstNodeType {
    AST_NODE_IF_STATEMENT,
    AST_NODE_WHILE_STATEMENT,
    AST_NODE_FUNCTION_DECLARATION,
    AST_NODE_EXPRESSION_STATEMENT,
    AST_NODE_LITERAL,
    AST_NODE_IDENTIFIER,
    AST_NODE_FUNCTION_CALL
};

struct AstNode {
    AstNodeType type;
    std::string value;
    AstNode* left;
    AstNode* right;
};

// Forward declarations
class Value;
class Function;
class Scope;

// Value class
class Value {
public:
    // Default constructor
    Value() : value_("") {}
    // Constructor
    Value(const std::string& value) : value_(value) {}

    // Get the value
    std::string getValue() const {
        return value_;
    }

    // Conversion to bool for control flow
    explicit operator bool() const {
        return value_ != "0" && value_ != "" && value_ != "false";
    }

private:
    std::string value_;
};

// Function class
class Function {
public:
    // Constructor
    Function(const std::string& name, AstNode* body) : name_(name), body_(body) {}

    // Get the function name
    std::string getName() const {
        return name_;
    }

    // Get the function body
    AstNode* getBody() const {
        return body_;
    }

private:
    std::string name_;
    AstNode* body_;
};

// Scope class
class Scope {
public:
    // Constructor
    Scope() : lastValue_() {}

    // Destructor
    ~Scope() {}

    // Add a variable to the scope
    void addVariable(const std::string& name, Value value) {
        variables_[name] = value;
    }

    // Get a variable from the scope
    Value getVariable(const std::string& name) {
        auto it = variables_.find(name);
        if (it != variables_.end()) {
            return it->second;
        }
        else {
            std::cerr << "Error: variable not found" << std::endl;
            exit(1);
        }
    }

    // Add a function to the scope
    void addFunction(Function* function) {
        functions_[function->getName()] = function;
    }

    // Get a function from the scope
    Function* getFunction(const std::string& name) {
        auto it = functions_.find(name);
        if (it != functions_.end()) {
            return it->second;
        }
        else {
            std::cerr << "Error: function not found" << std::endl;
            exit(1);
        }
    }

    // Get the last value in the scope
    Value getLastValue() {
        return lastValue_;
    }

    // Set the last value in the scope
    void setLastValue(Value value) {
        lastValue_ = value;
    }

private:
    std::map<std::string, Value> variables_;
    std::map<std::string, Function*> functions_;
    Value lastValue_;
};

// Runtime class
class Runtime {
public:
    // Constructor
    Runtime() {}

    // Destructor
    ~Runtime() {}

    // Run the program
    void run(AstNode* program) {
        // Create a scope for the program
        Scope* scope = new Scope();

        // Run the program
        runProgram(program, scope);

        // Delete the scope
        delete scope;
    }

    // Run a program
    void runProgram(AstNode* program, Scope* scope) {
        // Run each statement in the program
        for (AstNode* statement = program->left; statement != nullptr; statement = statement->right) {
            runStatement(statement, scope);
        }
    }

    // Run a statement
    void runStatement(AstNode* statement, Scope* scope) {
        // Check the type of the statement
        switch (statement->type) {
        case AST_NODE_IF_STATEMENT:
            runIfStatement(statement, scope);
            break;
        case AST_NODE_WHILE_STATEMENT:
            runWhileStatement(statement, scope);
            break;
        case AST_NODE_FUNCTION_DECLARATION:
            runFunctionDeclaration(statement, scope);
            break;
        case AST_NODE_EXPRESSION_STATEMENT:
            runExpressionStatement(statement, scope);
            break;
        default:
            std::cerr << "Error: unknown statement type" << std::endl;
            exit(1);
        }
    }

    // Run an if statement
    void runIfStatement(AstNode* statement, Scope* scope) {
        // Evaluate the condition
        bool condition = static_cast<bool>(evaluateExpression(statement->left, scope));

        // Run the then or else branch
        if (condition) {
            runStatement(statement->right->left, scope);
        }
        else {
            runStatement(statement->right->right, scope);
        }
    }

    // Run a while statement
    void runWhileStatement(AstNode* statement, Scope* scope) {
        // Evaluate the condition
        bool condition = static_cast<bool>(evaluateExpression(statement->left, scope));

        // Run the loop body
        while (condition) {
            runStatement(statement->right, scope);
            condition = static_cast<bool>(evaluateExpression(statement->left, scope));
        }
    }

    // Run a function declaration
    void runFunctionDeclaration(AstNode* statement, Scope* scope) {
        // Create a new function
        Function* function = new Function(statement->left->value, statement->right);

        // Add the function to the scope
        scope->addFunction(function);
    }

    // Run an expression statement
    void runExpressionStatement(AstNode* statement, Scope* scope) {
        // Evaluate the expression
        evaluateExpression(statement->left, scope);
    }

    // Evaluate an expression
    Value evaluateExpression(AstNode* expression, Scope* scope) {
        // Check the type of the expression
        switch (expression->type) {
        case AST_NODE_LITERAL:
            return Value(expression->value);
        case AST_NODE_IDENTIFIER:
            return scope->getVariable(expression->value);
        case AST_NODE_FUNCTION_CALL:
            return callFunction(expression, scope);
        default:
            std::cerr << "Error: unknown expression type" << std::endl;
            exit(1);
        }
    }

    // Call a function
    Value callFunction(AstNode* expression, Scope* scope) {
        // Get the function
        Function* function = scope->getFunction(expression->left->value);

        // Create a new scope for the function
        Scope* functionScope = new Scope();

        // Add the function's parameters to the scope
        for (AstNode* parameter = expression->right; parameter != nullptr; parameter = parameter->right) {
            functionScope->addVariable(parameter->left->value, evaluateExpression(parameter->right, scope));
        }

        // Run the function's body
        Value returnValue = runFunctionBody(function->getBody(), functionScope);

        // Delete the function scope
        delete functionScope;

        return returnValue;
    }

    // Run a function body
    Value runFunctionBody(AstNode* body, Scope* scope) {
        // Run each statement in the body
        for (AstNode* statement = body->left; statement != nullptr; statement = statement->right) {
            runStatement(statement, scope);
        }

        // Return the last value
        return scope->getLastValue();
    }
};

