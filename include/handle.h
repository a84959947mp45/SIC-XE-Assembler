#ifndef HANDLE_H
#define HANDLE_H

#include "InputFile.h"


class handle: public InputFile
{
    public:
        handle();
        void pass1();
        void pass2();
        void opjectProgram();

    protected:

    private:
        string REAl(string);
        string hexAddOperation(string ,string);
        string hexAddOperation(string ,int);
        int hextoint(char );
        string REAL(string);
        string REAL_label(string,struct DataStore &);
        string hexSub(string,string);
        string binarySub(string,string);
        string binaryTohex(string);
        map<string,string>Symtab;
        map<string,string>Optab;
        vector<string>Littab;
        vector< vector<string> >EXTDEF;
        vector< vector<string> >EXTREF;
        bool judgeEX(vector< vector<string> >,int&,struct DataStore&);
        string hance(string);
        string hanceSTR(string );

};

#endif // HANDLE_H
