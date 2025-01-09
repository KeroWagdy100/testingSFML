#include <iostream> // cout, cin
#include <conio.h> // _getch()
#include <limits> // numeric_limits 
#include <iomanip> // for setprecision
#include <LinkedList.hpp>

// TODO: 1. Use A namespace to wrap all functions in a single place
// TODO: 2. Set A Menu To make the user able to change settings or exit calculator
// TODO: 3. Find A way to represent the calculator Graphically (GUI)

enum char_t {
    NON = 0, DIGIT, SPACE, ENTER, BACKSPACE, EQUAL, DOT, ADD, SUB, MUL, DIV
//         '0':'9'  ' '    '\n'   '='         '+'  '-'  '*'  '/'
};

char_t defineType(char c) {
    if (c >= '0' && c <= '9')
        return char_t::DIGIT;
    switch (c) {
        case ' ':  return char_t::SPACE;
        case '\n': case 13: return char_t::ENTER;
        case 8: return char_t::BACKSPACE;
        case '=':  return char_t::EQUAL;
        case '.':  return char_t::DOT;
        case '+':  return char_t::ADD;
        case '-':  return char_t::SUB;
        case '*':  return char_t::MUL;
        case '/':  return char_t::DIV;
        default: return char_t::NON;
    }
}


class MyString {
public:
    MyString(): m_len(0){}
    MyString& operator+=(char c) {
        if (pushable())
            m_data[m_len++] = c;
        return *this;
    }
    float toFloat() const {
        char temp[CAPACITY];
        for (unsigned short i = 0; i < m_len; i++)
            temp[i] = m_data[i];
        return atof(temp);
    }
    unsigned short len() const {
        return m_len;
    }
    bool empty() const {
        return m_len == 0;
    }
    bool pushable() const {
        return m_len < CAPACITY;
    }
    void clear() {
        m_len = 0;
    }
private:
    static const unsigned short CAPACITY = 8;
    char m_data[CAPACITY];
    unsigned short m_len {0};
};


bool isop(char_t cType) {
    bool flag = (cType == ADD || cType == SUB || cType == MUL || cType == DIV);
    return flag;
}


float evaluate(float leftOperand, char_t op, float rightOperand) {
    switch (op) {
        case MUL: return leftOperand * rightOperand;
        case DIV: return rightOperand == 0 ? 0.0f : leftOperand / rightOperand;
        case ADD: return leftOperand + rightOperand;
        case SUB: return leftOperand - rightOperand;
        default: return 0.0f;
    }
}


float evaluate(LinkedList<float>& operands, LinkedList<char_t>& operators) {
    if (operands.length() != operators.length() + 1)
        return 0.0f;

    /* HANDLING MUL & DIV */
    const char_t largestPrecedence[] = {MUL, DIV};
    while (true) {
        int i = operators.findAny(largestPrecedence, 2);

        /* NO MUL NOR DIV REMAINING */
        if (i == -1)
            break;
        
        float res = evaluate(operands[i], operators[i], operands[i + 1]);
        operands[i] = res;
        operands.removeAt(i + 1);
        operators.removeAt(i);
    }
    // ---------------------------

    /* HANDLING ADD & SUB */
    const char_t smallestPrecedence[] = {ADD, SUB};
    while (true) {
        int i = operators.findAny(smallestPrecedence, 2);

        /* NO ADD NOR SUB REMAINING */
        if (i == -1)
            break;
        
        float res = evaluate(operands[i], operators[i], operands[i + 1]);
        operands[i] = res;
        operands.removeAt(i + 1);
        operators.removeAt(i);
    }
    if (operands.length() == 1)
        return operands[0];
    return 0.0;
}

void reader() {
    char c;
    char_t cType = NON;
    char_t prev_cType = NON;
    LinkedList<float> operands;
    LinkedList<char_t> operators;
    MyString operand; // stream for the current/last number
    do {
        /* READ CHARACTER AND DEFINE ITS TYPE */
        c = _getch(); // NOTE: keystrokes are not printed by default
        cType = defineType(c);

        if (cType == char_t::SPACE || cType == char_t::NON)
            continue;
        
        if (cType == char_t::BACKSPACE) {
            operands.clear();
            operators.clear();
            operand.clear();
            prev_cType = NON;
            cType = NON;
            std::cout << " [Line Removed]\n";
            continue;
        }

        /* USER ENTERS DIGIT */
        if (cType == char_t::DIGIT || cType == char_t::DOT) {
            if (!operand.pushable() ||
                (cType == char_t::DOT && prev_cType == char_t::DOT))
                continue;
            operand += c;
        }
        
        /* USER ENTERS OPERATOR */
        else if (isop(cType)) {
            if (isop(prev_cType))
                continue;
            operands.append(operand.toFloat());
            operand.clear();
            operators.append(cType);
        }

        /* PRINT CHARACTER IF VALID */
        if (isop(cType))
            std::cout << " " << c << " ";
        else if (cType == char_t::DIGIT || cType == char_t::DOT)
            std::cout << c;
        
        prev_cType = cType;
    } while (cType != char_t::EQUAL && cType != char_t::ENTER);
    
    operands.append(operand.toFloat());
    
    float result = evaluate(operands, operators);

    std::cout << " = " << std::fixed << std::setprecision(3) << result << "\n";
}


int main() {
    reader();
}