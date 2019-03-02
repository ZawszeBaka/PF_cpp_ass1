//
// Created by Nguyen Duc Dung on 2019-02-15.
//

#include "core.h"

/// Prototype declaration
// void Initialization();
// void Finalization();
//
// void LoadConfiguration();
// void LoadMenu();
// void DisplayMenu();
// void ProcessUserChoice();

Element * root;

// Implementation of Class
Element::Element(){
    // empty
}
Element::Element(string name){
    this->name = name;
}
Element::Element(string name, int type){
    this->name = name;
    this->type = type;
}
Element::~Element(){};
void Element::setName(string name){
    this->name = name;
}
void Element::setType(int type){
    this->type = type;
}
string Element::getName(){
    return this->name;
}
int Element::getType(){
    return this->type;
}
void Element::addElement(Element *e){
    this->dict[e->name] = e;
}
void Element::addElement(Element *e, string name){
    this->dict[name] = e;
}
void Element::setVal(int intVal){
    this->intVal = intVal;
}
void Element::setVal(string strVal){
    this->strVal = strVal;
}
void Element::setVal(double doubleVal){
    this->doubleVal = doubleVal;
}
Element* Element::getElement(string name){
    return this->dict[name];
}
int Element::getInt(){
    return this->intVal;
}
string Element::getString(){
    return this->strVal;
}
double Element::getDouble(){
    return this->doubleVal;
}
void Element::show(int n){
    printf("\n%s%s:",(this->tab(n).c_str()),(this->name).c_str());
    if (type==1){
        map<string, Element*>::iterator it = (this->dict).begin();
        while(it != (this->dict).end())
        {
            it->second->show(n+1);
            it++;
        }
    }else if(type==2){
        printf("%d",this->intVal);
    }else if(type==3){
        printf("%s", (this->strVal).c_str());
    }else if(type==4){
        printf("%.5f",this->doubleVal);
    }
}
string Element::tab(int n){
    string tmp="";
    for(int i = 0; i < n ; i++){
        tmp = tmp + "\t";
    }
    return tmp;
}

///--------------------------------------------------------------------

bool __coreInitialized = false;  /// an example of global variable
int  __userChoice;               /// a global variable to store user choice
bool __isExiting = false;        /// this variable should be turn on when the program exits

// TODO: add more global variables to complete tasks
string data;
int maxLine=0;
int maxOpt=0;

///--------------------------------------------------------------------
/**
 * Function: Initialization
 * Purpose:  Initialize anything necessary in your program before starting
 * Input:    None
 * Output:   None
 */

void Initialization() {
    // LoadConfiguration();
    // LoadMenu();
    // TODO: write the code to initialize the program
    __coreInitialized = true;
}

/**
 * Function: Finalization
 * Purpose:  Clean up everything before the program exits
 * Input:    None
 * Output:   None
 */
void Finalization() {
    // TODO: write the code to clean up when the program exits
}

void LoadConfiguration() {
    // TODO: write code to load data from the configuration file
    // Read file
    string line;
    ifstream f("conf.json");
    if(f.is_open())
    {
        while(getline(f, line))
        {
            data = data + line;
        }
        f.close();
    }

    // printf("\n[INFO] Original: \n%s", data.c_str());
    data = ignoreSpaces(data);
    // printf("\n[INFO] After ignoring spaces: \n%s", data.c_str());

    // Extract Json to dictionary
    root = new Element("root");
    jsonParser(root, data);
    // root->show(0);

    string mess,subStr;

    mess = "WelcomeText";
    subStr = "line";
    maxLine = getMaxNumName(root->getElement(mess),subStr);

    mess ="Menu";
    subStr = "opt";
    maxOpt = getMaxNumName(root->getElement(mess), subStr);

    // display WelcomeText
    printf("%s",root->getElement("WelcomeText")->getElement(strCatInt("line",1))->getString().c_str());
    for(int i=2; i<=maxLine; i++){
        printf("\n%s",root->getElement("WelcomeText")->getElement(strCatInt("line",i))->getString().c_str());
    }
}

void LoadMenu() {
    // TODO: write code to load menu from the configuration file

}

void DisplayMenu() {
    // TODO: Display the menu loaded from configuration file
    string line = "line";
    for(int i=1; i<=maxOpt; i++){
        printf("\n%d. %s",i,root->getElement("Menu")->getElement(strCatInt("opt",i))->getString().c_str());
    }
}

void ProcessUserChoice() {
    // TODO: Read user input and process
    string userChoice;
    printf("\nPlease select:");
    getline(cin, userChoice);
    // printf("\n[DEBUG] You just typed %s",userChoice.c_str());
    int choice=-1;
    if (parseInt(userChoice,choice)){
      if(choice >=1 && choice < maxOpt){
        printf("You select menu item %d. Processing... Done!", choice);
        __userChoice = choice;
      }else if(choice == maxOpt){
        printf("Exiting...");
        __userChoice = choice;
        __isExiting=true;
      }else{
        printf("Please select a number from 1 to %d.",maxOpt);
      }
    }else{
      printf("Invalid input, please input an integer number.");
    }


}

/*
    HELPER FUNCTIONS
*/
string ignoreSpaces(string &data){
    // Ignore spaces that is not in quotation marks
    int i;
    string rs;
    bool isInQuotes = false;
    int size = data.size();
    // int size = 10;
    for(i=0; i<size; i++){
        if(!isInQuotes){
            // if not inside quotation marks
            if(data[i] == '"') {
                isInQuotes = true;
                rs = rs + data[i];
            }
            else if(data[i] != ' ' && data[i] != '\t') rs = rs + data[i];
        }else{
            // if inside quotation marks
            if(data[i] == '"') {
                isInQuotes = false;
                rs = rs + data[i];
            }
            else rs = rs + data[i];
        }
    }
    return rs;
}

void jsonParser(Element* e, string jsonStr){
    int sz = jsonStr.size();
    /*
        type=1: dict
        type=2: int
        type=3: string
        type=4: double
    */
    int type;

    // dict
    if (jsonStr[0]=='{' && jsonStr[sz-1]=='}'){
        e->setType(1);
        // recursive

        /*
            mode=0 : extracting name
            mode=1 : extracting string or dict
            mode=2 : extracting number
        */
        string name;
        int mode=0;
        int countQuotes = 0;
        int countBrackets = 0;
        int indBeginName;
        for(int i=1; i<sz-1; i++){
            if(mode==0){
                if(jsonStr[i]=='"') {
                    countQuotes++;
                    if(countQuotes == 1) indBeginName = i+1;
                    else if(countQuotes == 2) {
                        name = jsonStr.substr(indBeginName, i-indBeginName);
                        indBeginName=i+2;
                        i=i+1; // skip :
                        countQuotes = 0;
                        countBrackets = 0;
                        if(jsonStr[i+1]=='"' || jsonStr[i+1] =='{') mode = 1;
                        else mode = 2;
                    }
                }
            }else if(mode==1){
                if(jsonStr[i]=='"') {
                    countQuotes = (countQuotes+1)%2;
                }else if(jsonStr[i] == '\\' && jsonStr[i+1]=='"'){
                    i = i+1; // skip escape character \"
                }else if(jsonStr[i] == '{'){
                    countBrackets++;
                }else if(jsonStr[i] == '}'){
                    countBrackets--;
                }

                if(countQuotes ==0 && countBrackets == 0){
                    Element *subElem = new Element(name);
                    e->addElement(subElem);
                    jsonParser(subElem, jsonStr.substr(indBeginName, i-indBeginName+1));
                    mode=0;
                    i=i+1; // skip ','
                }
            }else if(mode==2){
                if(jsonStr[i]==',' || i == sz-1){
                    Element *numSubElem = new Element(name);
                    string str = jsonStr.substr(indBeginName, i-indBeginName);
                    e->addElement(numSubElem, name);
                    mode = 0;
                    jsonParser(numSubElem, str);
                }
            }
        }

    // string
    }else if(jsonStr[0]=='"' && jsonStr[sz-1]=='"'){
        e->setType(3);
        string str;
        for(int i=1; i<sz-1; i++){
            str = str + jsonStr[i];
        }
        e->setVal(str);

    // number : int or double
    }else{
        int indPoint;
        // check if type=int or double
        type=2; // int
        for(int i=0; i<sz; i++){
            if(jsonStr[i]=='.'){
                indPoint = i;
                type=4; break; // double
            }
        }
        e->setType(type);

        int coff;
        int num;
        if(type==2){ // int
            e->setVal(str2int(jsonStr));
        }else{
            e->setVal(str2double(jsonStr));
        }
    }
}

int extractNum(string fullStr, string subStr){
    // Check if the first 4 characters is subStr
    //string line = "line";
    int fullSize = fullStr.size();
    int subSize = subStr.size();

    string first4chars = fullStr.substr(0,subSize);
    bool isMatch = true;
    for(int i=0; i<subSize; i++){
      if(first4chars[i] != subStr[i]){
            isMatch = false;
            break;
        }
    }
    if(isMatch){
        string numStr = fullStr.substr(subSize,fullSize-subSize);
        return str2int(numStr);
    }else{
        return -1;
    }
}

bool isInt(string str){
    return false;
}

bool isDouble(string str){
    return true;
}

int type(string str){
    return 0;
}

int str2int(string str){
    int coff = 0;
    int val =0;
    for(int i=str.size()-1; i>=0; i--){
        val += ((int)(str[i] - '0'))*pow(10,coff);
        coff++;
    }
    return val;
}

double str2double(string str){
    int sz = str.size();
    int indPoint; // find index of point (.)
    for(int i=0; i<sz; i++){
        if(str[i]=='.'){
            indPoint = i;
            break;
        }
    }
    double coff = 0;
    double val =0;
    for(int i = indPoint-1; i>=0; i--){
        val += ((double)(int)(str[i]-'0'))*pow(10.0,coff);
        coff++;
    }
    coff = -1;
    for(int i = indPoint+1; i<sz; i++){
        val += ((double)(int)(str[i]-'0'))*pow(10.0,coff);
        coff--;
    }
    return val;
}

int getMaxNumName(Element *e, string subStr){
    map<string, Element*>::iterator it = (e->dict).begin();
    vector<int> lstNums;
    int max=0;
    while(it != (e->dict).end())
    {
        string name = (it->first);
        lstNums.push_back(extractNum(name, subStr));
        it++;
    }
    int sz = lstNums.size();
    if(sz == 0) return -1;

    // printf("\n[DEBUG] Size : %d", sz);

    // check if continuous , ignore the index 0
    // for instances: if exists number 1, mask[1] = 1, otherwise
    vector<int> mask(sz+1,0);
    for(int i=0; i<sz; i++){
        mask[lstNums[i]]=1;
        int tmp = lstNums[i];
        if(tmp>max) max = tmp;
    }
    for(int i=1; i<sz+1; i++){
        if(mask[i]==0) return -1;
    }
    return max;
}

string strCatInt(string str, int i){
    return str + to_string(i);
}

bool parseInt(string str, int &rs){
    // check if contains only number and sign + or -
    str = ignoreSpaces(str);
    int sz = str.size();
    // case  1: + -
    if(str[0]=='+'||str[0]=='-'){
        for(int i=1; i<sz; i++){
            int t = (int)(str[i] - '0');
            if(t<=9 && t>=0){}else{return false;}
        }
        if(str[0]=='+') rs = str2int(str.substr(1,sz-1));
        else rs = -str2int(str);
        return true;
    }else{
        for(int i=0; i<sz; i++){
            int t = (int)(str[i] - '0');
            if(t<=9 && t>=0){}else{return false;}
        }
        rs = str2int(str);
        return true;
    }
}
