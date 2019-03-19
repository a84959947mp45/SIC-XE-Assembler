#include "screen.h"
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <map>

using namespace std;
//輸出Symtab
void screen::PrintSymtab(map<string,string>a){
    SetColor(11);
    for(map<string, string>::iterator it = a.begin(); it != a.end(); ++it) {
            cout << "a[" << (it->first) << "] = " << (it->second) << endl;
    }
    SetColor(7);
    cout<<endl;
}
//輸出Opmtab
void screen::PrintOptab(map<string,string>a){

    SetColor(13);
    for(map<string, string>::iterator it = a.begin(); it != a.end(); ++it) {
            cout << "a[" << (it->first) << "] = " << (it->second) << endl;
    }
    SetColor(7);
    cout<<endl;
}

//輸出DataStore資料
void screen::PrintVector(vector<struct DataStore> rrr){
        SetColor(11);
        cout<<left<<setw(6)<<"line";
        SetColor(13);
        cout<<setw(10)<<"loc";
        SetColor(9);
        cout<<setw(10)<<"label";
        SetColor(12);
        cout<<"//"<<left<<setw(10)<<"opcode";
        SetColor(14);
        cout<<right<<setw(20)<<"operand"<<"  ";
        SetColor(7);
        cout<<left<<setw(15)<<"object_code"<<endl;
        cout<<endl;
    for(unsigned int i = 0; i<rrr.size();i++){
        SetColor(11);
        cout<<left<<setw(6)<<rrr[i].line;
        SetColor(13);
        cout<<setw(10)<<rrr[i].loc;
        SetColor(9);
        cout<<setw(10)<<rrr[i].mark;
        SetColor(12);
        cout<<"//"<<left<<setw(10)<<rrr[i].opcode;
        SetColor(14);
        cout<<right<<setw(20)<<rrr[i].label<<"  ";
        SetColor(7);
        cout<<left<<setw(15)<<rrr[i].objectCode<<endl;

    }
    cout<<endl;

}
//輸出OP_CODE資料
void screen::PrintOpCode(vector<struct op> opPrint){


    for(unsigned int i = 0; i<opPrint.size();i++){
        SetColor(11);
        cout<<left<<setw(10)<<opPrint[i].opcode;
        SetColor(9);
        cout<<setw(10)<<opPrint[i].format;
        SetColor(12);
        cout<<setw(10)<<opPrint[i].num;
        SetColor(14);
        cout<<setw(10)<<opPrint[i].binarycode<<endl;
        SetColor(7);

    }

}
//設定Console畫面字體顏色
void screen::SetColor(int color)
{
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//改變顏色
	SetConsoleTextAttribute(hConsole,color);
}
