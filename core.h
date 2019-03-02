//
// Created by Nguyen Duc Dung on 2019-02-15.
//

#ifndef A01_CORE_H
#define A01_CORE_H

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iomanip>
#include <string>
#include <vector>
#include <regex>
#include <map>
#include <math.h>

using namespace std;


/*TODO: If you are using some global variables and you intent to let other
 * users access it, you better declare it here.
 * Hint: use extern as shown in the below
 */
extern bool __coreInitialized;
extern int  __userChoice;
extern bool __isExiting;

/**
 * Function: Initialization
 * Purpose:  Initialize anything necessary in your program before starting
 * Input:    None
 * Output:   None
 */
void Initialization();
/**
 * Function: Finalization
 * Purpose:  Clean up everything before the program exits
 * Input:    None
 * Output:   None
 */
void Finalization();

void LoadConfiguration();
void LoadMenu();
void DisplayMenu();
void ProcessUserChoice();

// JSON
class Element{
public:
  string name;
  /*
      type=1: dict
      type=2: int
      type=3: string
      type=4: double
  */
  int type;

  map<string, Element*> dict;
  int intVal;
  string strVal;
  double doubleVal;

  Element();
  Element(string name);
  Element(string name, int type);
  ~Element();

  void setName(string name);
  void setType(int type);
  string getName();
  int getType();

  void addElement(Element *e);
  void addElement(Element *e, string name);
  void setVal(int intVal);
  void setVal(string strVal);
  void setVal(double doubleVal);

  Element* getElement(string name);
  int getInt();
  string getString();
  double getDouble();

  void show(int n);

private:
  string tab(int n);
};

extern Element *root;
extern string data;
extern int maxLine;
extern int maxOpt;

/******************** Helper functions *************************/
/*
    Args:
      Raw Json String
    Returns:
      Json String without spaces , except spaces in quotations
    Status: Done! Tested!
    Version: 1.0.0
    Things can go wrong: None!
*/
string ignoreSpaces(string &data);

/*
    Description: This is recursive method!
      Ignore spaces, tabs
    Args:
      Root element
      Json String
    Returns:
      Root has elements
    Status: Done! Not-yet-tested!
    Things can go wrong:
      1. Wrong format JSON !!
*/
void jsonParser(Element* e, string jsonStr);

/*
    Args:
      "WelcomeText":{
          "line1": "a...",
          "line2": "b...",
      }
      e is Element "WelcomeText"
      subStr is "line"
    Returns:
      max number followed by subStr
      for ex: return 2 if there are only 2 lines
    Status:
    Version:
    Things can go wrong:
*/
int getMaxNumName(Element *e, string subStr);

/*
    Args:
      fullStr : "line%d"
      subStr: "line"
    Returns:
      1 or 2 or 10   (int)
      -1 if not in the case of format "line%d"
    Status: Done! Tested!
    Version: 1.0.1
    Things can go wrong:
*/
int extractNum(string fullStr, string subStr);

/*
    Args:
      string: str .Make sure str is actually int
    Returns:
      int: corresponds to the string number
    Status: Done! Tested!
    Version: 1.0.0
    Things can go wrong:
*/
int str2int(string str);
double str2double(string str);  // Make sure str is actually double

bool isInt(string str);
bool isDouble(string str);

string strCatInt(string str, int i);

/*
    Args:
      string:
        "  0 0122" : 122
        "+5 2": 52
        "- 22": 22
        "+aqw2.1": false
        " .214" : false
    Returns:
      true: if can parse from string; otherwise false
      int parse from string
    Status:
    Version 1.0.0
    Things can go wrong:
*/
bool parseInt(string str, int &rs);

bool isDecimal(char t);

#endif //A01_CORE_H
