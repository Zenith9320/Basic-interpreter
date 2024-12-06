/*
 * File: program.h
 * ---------------
 * This interface exports a Program class for storing a BASIC
 * program.
 */

#ifndef _program_h
#define _program_h

#include <string>
#include <set>
#include <unordered_map>
#include "statement.hpp"

class Statement;

/*
 * This class stores the lines in a BASIC program.  Each line
 * in the program is stored in order according to its line number.
 * Moreover, each line in the program is associated with two
 * components:
 *
 * 1. The source line, which is the complete line (including the
 *    line number) that was entered by the user.
 *
 * 2. The parsed representation of that statement, which is a
 *    pointer to a Statement.
 */

class Program {

public:

/*
 * Constructor: Program
 * Usage: Program program;
 * -----------------------
 * Constructs an empty BASIC program.
 */

    Program(): currentLineNumber(0) {}

/*
 * Destructor: ~Program
 * Usage: usually implicit
 * -----------------------
 * Frees any heap storage associated with the program.
 */

    ~Program() {
        for (auto& pairx : parsedStatements) {
            delete pairx.second;
        }
    }

/*
 * Method: clear
 * Usage: program.clear();
 * -----------------------
 * Removes all lines from the program.
 */

    void clear();

/*
 * Method: addSourceLine
 * Usage: program.addSourceLine(lineNumber, line);
 * -----------------------------------------------
 * Adds a source line to the program with the specified line number.
 * If that line already exists, the text of the line replaces
 * the text of any existing line and the parsed representation
 * (if any) is deleted.  If the line is new, it is added to the
 * program in the correct sequence.
 */

    void addSourceLine(int lineNumber, std::string line);

/*
 * Method: removeSourceLine
 * Usage: program.removeSourceLine(lineNumber);
 * --------------------------------------------
 * Removes the line with the specified number from the program,
 * freeing the memory associated with any parsed representation.
 * If no such line exists, this method simply returns without
 * performing any action.
 */

    void removeSourceLine(int lineNumber);

/*
 * Method: getSourceLine
 * Usage: string line = program.getSourceLine(lineNumber);
 * -------------------------------------------------------
 * Returns the program line with the specified line number.
 * If no such line exists, this method returns the empty string.
 */

    std::string getSourceLine(int lineNumber);

/*
 * Method: setParsedStatement
 * Usage: program.setParsedStatement(lineNumber, stmt);
 * ----------------------------------------------------
 * Adds the parsed representation of the statement to the statement
 * at the specified line number.  If no such line exists, this
 * method raises an error.  If a previous parsed representation
 * exists, the memory for that statement is reclaimed.
 */

    void setParsedStatement(int lineNumber, Statement *stmt);

/*
 * Method: getParsedStatement
 * Usage: Statement *stmt = program.getParsedStatement(lineNumber);
 * ----------------------------------------------------------------
 * Retrieves the parsed representation of the statement at the
 * specified line number.  If no value has been set, this method
 * returns NULL.
 */

    Statement *getParsedStatement(int lineNumber);

/*
 * Method: getFirstLineNumber
 * Usage: int lineNumber = program.getFirstLineNumber();
 * -----------------------------------------------------
 * Returns the line number of the first line in the program.
 * If the program has no lines, this method returns -1.
 */

    int getFirstLineNumber();

/*
 * Method: getNextLineNumber
 * Usage: int nextLine = program.getNextLineNumber(lineNumber);
 * ------------------------------------------------------------
 * Returns the line number of the first line in the program whose
 * number is larger than the specified one, which must already exist
 * in the program.  If no more lines remain, this method returns -1.
 */

    int getNextLineNumber(int lineNumber);

    //more func to add
    //todo
    void listProgram();
    
    int getVariable(std::string vname);
     
    void runProgram(Program &program);

    bool if_exist(std::string variable_name);

    void setvariable(std::string varname, int value);

    void gotoLine(int x);
    //判断是否END
    bool if_end() {
        return if_end1;
    }

    bool check_line(int check_linenumber);

    void goto_line(int goto_number);

    int get_cur_linenumber();

    bool check_jump() {
        return if_jump;
    }
    
    void jump() {
        if_jump = true;
    }

    void not_jump() {
        if_jump = false;
    }

    bool if_empty() {
        return lineNumbers.empty();
    }
    void endprogram() {
        if_end1 = true;
    }
    void run() {
        if_end1 = false;
    }

private:
    // 存储源代码行
    std::unordered_map<int, std::string> sourceLines;
    
    // 存储每行的解析语句
    std::unordered_map<int, Statement*> parsedStatements;
    
    // 存储程序中所有的行号
    std::set<int> lineNumbers;
    
    // 存储程序的变量
    std::unordered_map<std::string, int> variables;
    
    // 程序的当前执行行号
    int currentLineNumber;

    //判断是否通过GOTO或者IF改变了行号
    bool if_jump = false;
    //todo

    bool if_end1 = false;
};

#endif
