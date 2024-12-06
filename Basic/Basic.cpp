/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <string>
#include "program.hpp"
#include "Utils/error.hpp"
#include "Utils/tokenScanner.hpp"
#include "statement.hpp"
#include "evalstate.hpp"
#include "exp.hpp"
#include "parser.hpp"


/* Function prototypes */

void processLine(std::string line, Program &program, EvalState &state);

/* Main program */

bool check_varname(std::string varName) {
    if (varName == "REM" || varName == "LET" || varName == "PRINT" || varName == "INPUT" || varName == "END" || varName == "GOTO" || varName == "IF" ||varName == "THEN" || varName == "RUN"||varName == "LIST" || varName == "CLEAR" ||varName == "QUIT" || varName == "HELP") {
        return false;
    }
    for (int i = 0; i < varName.size(); i++) {
        if (!((varName[i] <= '9' && varName[i] >= '0') || (varName[i] >= 'a' && varName[i] <= 'z') || (varName[i] >= 'A' && varName[i] <= 'Z'))) {
            return false;
        }
    }
    return true;
}

int main() {
    EvalState state;
    Program program;
    //std::cout << "Stub implementation of BASIC" << std::endl;
    while (true) {
        try {
            std::string input;
            getline(std::cin, input);
            if (input.empty())
                continue;
            processLine(input, program, state);
        } catch (ErrorException &ex) {
            std::cout << ex.getMessage() << std::endl;
        }
    }
    return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(std::string line, Program &program, EvalState &state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.scanStrings();
    scanner.setInput(line);
    if (scanner.hasMoreTokens()) {
        std::string firstToken = scanner.nextToken();
        if (isdigit(firstToken[0])) {
            int lineNumber = std::stoi(firstToken);
            program.addSourceLine(lineNumber, line);
            if (scanner.hasMoreTokens()) {
                std::string command = scanner.nextToken();
                if (command == "REM") {
                    Statement* stmt = new RemStatement();
                    program.setParsedStatement(lineNumber, stmt);
                    return;
                }
                else if (command == "LET") {
                    std::string VarName = scanner.nextToken();
                    if (!check_varname(VarName)) {
                        error("SYNTAX ERROR");
                    }
                    scanner.nextToken();
                    Expression* expression = parseExp(scanner);
                    Statement* stmt = new LetStatement(VarName, expression);
                    program.setParsedStatement(lineNumber,stmt);
                    return;
                }
                else if (command == "PRINT") {
                    Expression* expr = nullptr;
                    if (scanner.hasMoreTokens()) {
                        expr = parseExp(scanner);
                    }
                    else {
                        error("SYNTAX ERROR");
                    }
                    Statement *stmt = new PrintStatement(expr);
                    program.setParsedStatement(lineNumber,stmt);
                    return;
                }
                else if (command == "INPUT") {
                    std::string name = scanner.nextToken();
                    Statement*stmt =new InputStatement(name);
                    program.setParsedStatement(lineNumber, stmt);
                    return;
                }
                else if (command == "END") {
                    if (scanner.hasMoreTokens()) {
                        error("SYNTAX ERROR");
                    }
                    else {
                        Statement* stmt = new EndStatement();
                        program.setParsedStatement(lineNumber,stmt);
                    }
                    return;
                }
                else if (command == "IF") {
                    std::string condition = "";
                    while (scanner.hasMoreTokens()) {
                        std::string s = scanner.nextToken();
                        if (s == "THEN") break;
                        condition += s;
                        condition += " ";
                    }
                    std::string str_num = "";
                    while (scanner.hasMoreTokens()) {
                        std::string s = scanner.nextToken();
                        str_num += s;
                    }
                    int number = std::stoi(str_num);
                    Statement* stmt = new IfStatement(condition, number);
                    program.setParsedStatement(lineNumber,stmt);
                }
                else if (command == "GOTO") {
                    int number = std::stoi(scanner.nextToken());
                    Statement*stmt = new GotoStatement(number);
                    program.setParsedStatement(lineNumber,stmt);
                    return;
                }
                else {
                    program.removeSourceLine(lineNumber);
                    error("SYNTAX ERROR");
                }
                return;
            }
            else {
                program.removeSourceLine(lineNumber);
            }
        }
        else {
            if (firstToken == "LIST") {
                int cur = program.getFirstLineNumber();
                while (cur != -1) {
                    std::cout << program.getSourceLine(cur) << std::endl;
                    cur = program.getNextLineNumber(cur);
                }
                return;
            }
            else if (firstToken == "QUIT") {
                program.clear();
                state.Clear();
                exit(0);
            }
            else if (firstToken == "CLEAR") {
                program.clear();
                state.Clear();
                return;
            }
            else if (firstToken == "RUN") {
                program.run();
                int cur = program.getFirstLineNumber();
                program.goto_line(cur);
                while (cur != -1 && !(program.if_end())) {
                    Statement *stmt = program.getParsedStatement(cur);
                    if (stmt != nullptr) {
                        stmt->execute(program, state);
                    }
                    cur  = program.get_cur_linenumber();
                    if (!program.check_jump()) {
                        cur = program.getNextLineNumber(cur);
                    }
                    else {
                        program.not_jump();
                    }
                    program.goto_line(cur);
                    cur = program.get_cur_linenumber();
                }
                return;
            }
            else if (firstToken == "PRINT") {
                Expression* expr = nullptr;
                if (scanner.hasMoreTokens()) {
                    expr = parseExp(scanner);
                }
                Statement *stmt = new PrintStatement(expr);
                try {
                    stmt->execute(program, state);  
                } catch (ErrorException &ex) {
                    std::cout << ex.getMessage() << std::endl;
                }
                delete stmt;
                return;
            }
            else if (firstToken == "LET") {
                std::string varname = scanner.nextToken();
                if(varname =="LET" || varname =="IF" || varname =="REM" || varname =="GOTO" || varname =="PRINT" || varname =="INPUT" || varname =="END" || varname =="THEN" || varname =="RUN" || varname =="LIST" ||varname =="CLEAR"||varname =="QUIT") {
                    error("SYNTAX ERROR");
                }
                scanner.nextToken();
                Expression*expr = parseExp(scanner);
                Statement* stmt = new LetStatement(varname,expr);
                try {
                    stmt->execute(program, state);
                } catch (ErrorException &ex) {
                    std::cout << ex.getMessage() << std::endl;
                }
                delete stmt;
                return;
            }
            else if (firstToken == "INPUT") {
                std::string name = scanner.nextToken();
                Statement*stmt =new InputStatement(name);
                stmt->execute(program, state);
                delete stmt;
                return;
            }
        }
    }
    else return;
}
    //todo
