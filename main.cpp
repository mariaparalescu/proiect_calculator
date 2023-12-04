#include <iostream>


using namespace std;

class Token {
public:
    enum Type {
        NUMBER,
        OPERATOR,
        OPEN_PAREN,
        CLOSE_PAREN
    };

    Type type;
    string value;

    Token() : type(NUMBER), value("") {}
    Token(Type t, const string& v) : type(t), value(v) {}
    Token(const string& v) : type(NUMBER), value(v) {}

    Type getType() const {
        return type;
    }

    void setType(Type t) {
        type = t;
    }

    const string& getValue() const {
        return value;
    }

    void setValue(const string& v) {
        value = v;
    }
};

class ExpressionParser {
private:
    Token* tokens;
    int numTokens;

    void tokenize(const string& expression);

public:
    ExpressionParser(const string& expression);
    ~ExpressionParser();
    Token* getTokens();
    int getNumTokens() const;
};

ExpressionParser::ExpressionParser(const string& expression) : tokens(nullptr), numTokens(0) {
    tokenize(expression);
}

ExpressionParser::~ExpressionParser() {
    delete[] tokens;
}

Token* ExpressionParser::getTokens() {
    return tokens;
}

int ExpressionParser::getNumTokens() const {
    return numTokens;
}

void ExpressionParser::tokenize(const string& expression) {
    tokens = new Token[expression.size()];

    for (size_t i = 0; i < expression.size(); ++i) {
        char currentChar = expression[i];

        if (isspace(currentChar)) {
            continue;
        }

        if (isdigit(currentChar)) {
            tokens[numTokens++] = Token(Token::NUMBER, string(1, currentChar));
        } else if (currentChar == '+') {
            tokens[numTokens++] = Token(Token::OPERATOR, "+");
        } else if (currentChar == '-') {
            tokens[numTokens++] = Token(Token::OPERATOR, "-");
        } else if (currentChar == '*') {
            tokens[numTokens++] = Token(Token::OPERATOR, "*");
        } else if (currentChar == '/') {
            tokens[numTokens++] = Token(Token::OPERATOR, "/");
        } else if (currentChar == '(') {
            tokens[numTokens++] = Token(Token::OPEN_PAREN, "(");
        } else if (currentChar == ')') {
            tokens[numTokens++] = Token(Token::CLOSE_PAREN, ")");
        }
    }
}

int main() {

}
