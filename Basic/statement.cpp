/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"
#include "Utils/error.hpp"
#include <iostream>
#include <stdexcept>

/* Implementation of the Statement class */

int stringToInt(std::string str) {
  try {
    return std::stoi(str);
  } catch (std::invalid_argument& e) {
    throw std::invalid_argument("SYNTAX ERROR");
  }
}
Statement::Statement() = default;
Statement::~Statement() = default;
void Statement::execute(Program& program, EvalState& state) {}
//REM
void RemStatement::execute(Program &program, EvalState &state) {}
RemStatement::RemStatement() {}
RemStatement::~RemStatement() {}
statement_type RemStatement::getType() {
    return REM;
}

//LET
void LetStatement::execute(Program &program, EvalState &state) {
    int var_value = expr->eval(state);
    state.setValue(varname, var_value);
}
LetStatement::LetStatement(std::string varname, Expression* expr) {
    this->expr = expr;
    this->varname = varname;
}
LetStatement::~LetStatement() {
    delete expr;
}
statement_type LetStatement::getType() {
    return LET;
}

//PRINT
void PrintStatement::execute(Program &program, EvalState &state) {
    int print_value = expr->eval(state);
    std::cout << print_value << std::endl;
}
PrintStatement::PrintStatement(Expression* expr) {
    this->expr = expr;
}
PrintStatement::~PrintStatement() {
    delete expr;
}
statement_type PrintStatement::getType() {
    return PRINT;
}

//INPUT
void InputStatement::execute(Program &program, EvalState &state) {
    std::string input;
    while (true) {
        std::cout<<" ?"<<" ";
        bool check = true;
        bool if_negative = false;
        std::getline(std::cin, input);
        TokenScanner input_scanner;
        input_scanner.ignoreWhitespace();
        input_scanner.scanNumbers();
        input_scanner.scanStrings();
        input_scanner.setInput(input);
        std::string num = input_scanner.nextToken();
        try {
            if (input_scanner.hasMoreTokens() && input.front() != '-') {
                error("INVALID NUMBER");
            }
            if (input.front() == '-') {
                state.setValue(varname,std::stoi(input));
                return;
            }
            for (int i = 0; i < num.size(); i++) {
                if (num[i] != '-' && (!(isdigit(num[i])))) {
                    error("INVALID NUMBER");
                }
            }
            state.setValue(varname,std::stoi(num));
            return;
        } catch (ErrorException& ex) {
            std::cout << ex.getMessage() << std::endl;
        }
    }
}
InputStatement::InputStatement(std::string varname) {
    this->varname = varname;
}
InputStatement::~InputStatement() {}
statement_type InputStatement::getType() {
    return INPUT;
}

//END
void EndStatement::execute(Program &program, EvalState &state) {
    program.endprogram();
}
EndStatement::EndStatement() {}
EndStatement::~EndStatement() {}
statement_type EndStatement::getType() {
    return END;
}

//GOTO
void GotoStatement::execute(Program &program, EvalState &state) {
    if (program.check_line(number)) {
        program.goto_line(number);
        program.jump();
    }
    else {
        std::cout << "LINE NUMBER ERROR" << std::endl;
    }
}
GotoStatement::GotoStatement(int x) {
    number = x;
}
GotoStatement::~GotoStatement() {}
statement_type GotoStatement::getType() {
    return GOTO;
}

//IF
void IfStatement::execute(Program &program, EvalState &state) {
    TokenScanner condition_scanner;
    condition_scanner.ignoreWhitespace();
    condition_scanner.scanNumbers();
    condition_scanner.scanStrings();
    condition_scanner.setInput(condition);
    std::string left = "";
    std::string s;
    while (condition_scanner.hasMoreTokens()) {
        s = condition_scanner.nextToken();
        if (s == ">" || s == "<" || s=="=") {
            break;
        }
        else {
            left += s;
            left += " ";
        }
    }
    char op = s[0];
    std::string right = "";
    while (condition_scanner.hasMoreTokens()) {
        s = condition_scanner.nextToken();
        right += s;
        right += " ";
    }
    TokenScanner left_scanner;
    left_scanner.ignoreWhitespace();
    left_scanner.scanNumbers();
    left_scanner.scanStrings();
    left_scanner.setInput(left);
    Expression* left_expr = parseExp(left_scanner);
    int left_value = left_expr->eval(state);
    TokenScanner right_scanner;
    right_scanner.ignoreWhitespace();
    right_scanner.scanNumbers();
    right_scanner.scanStrings();
    right_scanner.setInput(right);
    Expression* right_expr = parseExp(right_scanner);
    int right_value = right_expr->eval(state);
    bool ans = false;
    switch(op) {
        case '=':
            ans = (left_value == right_value);
            break;
        case '<':
            ans = (left_value < right_value);
            break;
        case '>':
            ans = (left_value > right_value);
            break;
        default:
            ans = false;
            break;
    }
    if (ans) {
        if (program.check_line(linenumber)) {
            program.goto_line(linenumber);
            program.jump();
        }
        else {
            std::cout << "LINE NUMBER ERROR" << std::endl;
        }
    }
    else {
        program.not_jump();
    }
    delete left_expr;
    delete right_expr;
}
IfStatement::IfStatement(std::string condition, int linenumber) {
    this->condition = condition; 
    this->linenumber = linenumber;
}
IfStatement::~IfStatement() {}
statement_type IfStatement::getType() {
    return IF;
}

//todo
