/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include "program.hpp"
#include "Utils/error.hpp"
#include "statement.hpp"


void Program::clear() {
    // Replace this stub with your own code
    //todo
    for (auto& pair: parsedStatements) {
        delete pair.second;
    }
    sourceLines.clear();
    parsedStatements.clear();//只删除，不释放内存
    lineNumbers.clear();
    variables.clear();
}

void Program::addSourceLine(int lineNumber, std::string line) {
    // Replace this stub with your own code
    //todo
    if (sourceLines.find(lineNumber) != sourceLines.end()) {
        sourceLines[lineNumber] = line;
    }
    else {
        sourceLines.insert({lineNumber, line});
        lineNumbers.insert(lineNumber); 
    }
}

void Program::removeSourceLine(int lineNumber) {
    // Replace this stub with your own code
    //todo
    auto it = sourceLines.find(lineNumber);
    if (it != sourceLines.end()) {
        sourceLines.erase(it);
        lineNumbers.erase(lineNumber);
        auto stmtIt = parsedStatements.find(lineNumber);
        if (stmtIt != parsedStatements.end()) {
            delete stmtIt->second;
            parsedStatements.erase(stmtIt);
        }
    }
    else {
        error("LINE NUMBER ERROR ");
    }
}

std::string Program::getSourceLine(int lineNumber) {
    // Replace this stub with your own code
    //todo
    auto it = sourceLines.find(lineNumber);
    if (it != sourceLines.end()) {
        return it->second; 
    } else {
        error("LINE NUMBER ERROR ");
        return "";
    }
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    // Replace this stub with your own code
    //todo
    if(lineNumbers.find(lineNumber) == lineNumbers.end()) {
        error("SYNTAX ERROR");
    }
    if(parsedStatements[lineNumber] != nullptr) {
        delete parsedStatements[lineNumber];
    }
    parsedStatements[lineNumber] = stmt;
}

Statement *Program::getParsedStatement(int lineNumber) {
   // Replace this stub with your own code
   //todo
   auto it = parsedStatements.find(lineNumber);
    if (it != parsedStatements.end()) {
        return it->second; 
    } else {
        return nullptr; 
    }
}

int Program::getFirstLineNumber() {
    // Replace this stub with your own code
    //todo
    if (lineNumbers.empty()) {
        return -1;  
    }
    else {
        return *lineNumbers.begin();
    }
}

int Program::getNextLineNumber(int lineNumber) {
    // Replace this stub with your own code
    //todo
    auto it = lineNumbers.find(lineNumber); 
    if (it != lineNumbers.end()) {
        it++;
        if (it != lineNumbers.end()) return *it;  
        else return -1;
    } else {
        return -1; 
    }
}

//more func to add
//todo
int Program::getVariable(std::string vname) {
    auto it = variables.find(vname);
    if (it != variables.end()) {
        return it->second; 
    } else {
        error("VARIABLE NOT DEFINED");
        return 0;
    }
}

bool Program::if_exist(std::string vname) {
    if (variables.find(vname) == variables.end()) {
        error("VARIABLE NOT DEFINED");
    }
    return true;
}

bool Program::check_line(int check_linenumber) {
    return (lineNumbers.find(check_linenumber) != lineNumbers.end());
}

void Program::setvariable(std::string varname, int value) {
    variables.insert({varname, value});
}

void Program::gotoLine(int x) {
    if(lineNumbers.find(x) != lineNumbers.end()) currentLineNumber = x;
    else error("LINE NUMBER ERROR");
}

void Program::goto_line(int goto_number) {
    currentLineNumber = goto_number;
}

int Program::get_cur_linenumber() {
    return currentLineNumber;
}