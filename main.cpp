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

class MathExpression {
private:
    Token* tokens;
    int numTokens;

    double evaluate(int& index) const;

public:
    MathExpression(const string& expression);
    ~MathExpression();
    double calculate() const;

    MathExpression operator+(const MathExpression& other) const;
    MathExpression operator-(const MathExpression& other) const;
    MathExpression operator*(const MathExpression& other) const;
    MathExpression operator/(const MathExpression& other) const;

    friend ostream& operator<<(ostream& os, const MathExpression& mathExpr);
    friend istream& operator>>(istream& is, MathExpression& mathExpr);
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

MathExpression::MathExpression(const string& expression) : tokens(nullptr), numTokens(0) {
    ExpressionParser parser(expression);
    numTokens = parser.getNumTokens();
    tokens = new Token[numTokens];
    Token* parserTokens = parser.getTokens();
    for (int i = 0; i < numTokens; ++i) {
        tokens[i] = parserTokens[i];
    }
}

MathExpression::~MathExpression() {
    delete[] tokens;
}

double MathExpression::calculate() const {
    int index = 0;
    return evaluate(index);
}

double MathExpression::evaluate(int& index) const {
    double result = 0.0;
    double currentNumber = 0.0;
    char currentOperator = '+';

    while (index < numTokens) {
        const Token& currentToken = tokens[index];

        if (currentToken.type == Token::NUMBER) {
            currentNumber = stod(currentToken.value);
        } else if (currentToken.type == Token::OPERATOR) {
            currentOperator = currentToken.value[0];
        } else if (currentToken.type == Token::OPEN_PAREN) {
            currentNumber = evaluate(++index); // Evaluate recursively inside parentheses
        } else if (currentToken.type == Token::CLOSE_PAREN) {
            break; // Return from recursive call when closing parenthesis is encountered
        }

        switch (currentOperator) {
            case '+':
                result += currentNumber;
                break;
            case '-':
                result -= currentNumber;
                break;
            case '*':
                result *= currentNumber;
                break;
            case '/':
                if (currentNumber != 0.0) {
                    result /= currentNumber;
                } else {
                    cerr << "Error: Division by zero." << endl;
                    return 0.0; // Handle division by zero
                }
                break;
            default:
                break;
        }

        ++index;
    }

    return result;
}

MathExpression MathExpression::operator+(const MathExpression& other) const {
    return MathExpression(to_string(calculate() + other.calculate()));
}

MathExpression MathExpression::operator-(const MathExpression& other) const {
    return MathExpression(to_string(calculate() - other.calculate()));
}

MathExpression MathExpression::operator*(const MathExpression& other) const {
    return MathExpression(to_string(calculate() * other.calculate()));
}

MathExpression MathExpression::operator/(const MathExpression& other) const {
    double result = calculate();
    double otherResult = other.calculate();

    if (otherResult != 0.0) {
        return MathExpression(to_string(result / otherResult));
    } else {
        cerr << "Error: Division by zero." << endl;
        return MathExpression("0.0"); // Handle division by zero
    }
}

ostream& operator<<(ostream& os, const MathExpression& mathExpr) {
    os << mathExpr.calculate();
    return os;
}

istream& operator>>(istream& is, MathExpression& mathExpr) {
    string expression;
    is >> expression;
    mathExpr = MathExpression(expression);
    return is;
}

int main() {
    MathExpression expr1("[40+(5-1)]*2");
    MathExpression expr2("5 / 0");
    MathExpression expr3("[(2*3)^2]/4-(6+2)#3");
    MathExpression expr4("2 * 3.2");
    MathExpression expr5("24342343.0002-3");
    MathExpression expr6("2 * 3 - 8");

    cout << expr1 << " = " << expr1.calculate() << endl;
    cout << expr2 << " = " << expr2.calculate() << endl;
    cout << expr3 << " = " << expr3.calculate() << endl;
    cout << expr4 << " = " << expr4.calculate() << endl;
    cout << expr5 << " = " << expr5.calculate() << endl;
    cout << expr6 << " = " << expr6.calculate() << endl;

    return 0;
}
