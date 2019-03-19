#ifndef INPUTFILE_H
#define INPUTFILE_H

#include <string>
#include <vector>
#include "screen.h"

using namespace std;

//繼承SCREEN
class InputFile : public screen
{
    public:

    InputFile();
    void inputTxtFile();

    protected:
        string IntToString(int);
        int StringToInt(string);
        string HexToBinary(string ss);
        vector<struct op> op_code;
        //宣告一個struct vector準備存取檔案資料
        vector<struct DataStore>D_register;
        bool judgeop(string str);
    private:

        struct DataStore grabLine(string,string ,string);
        struct DataStore grabLine(string,string , string,string);
        void judgeformat(string str,vector<struct DataStore>&,int);


};

#endif // INPUTFILE_H
