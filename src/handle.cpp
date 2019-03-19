#include "handle.h"
#include <sstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
//#include "jni.h"

using namespace std;

handle::handle()
{


}
void handle::pass1(){



    int top=0;
    string judge="";
    string Loc;



    for(unsigned int i=0;i<D_register.size();i++){
        //建立Littab
        if(D_register[i].label[0]=='='){
            if(judge!=D_register[i].label){
              Littab.push_back(D_register[i].label);
              judge=D_register[i].label;
            }
        }
        //判斷assembler directive
        if(D_register[i].opcode=="START"){

                Loc="0000";
                D_register[i].loc=Loc;

        }else if(D_register[i].opcode=="EXTDEF"){

                string reEXTDEF;
                vector<string> eEXTDEF;
                istringstream delim(D_register[i].label);
                while(getline(delim,reEXTDEF,',')){
                    eEXTDEF.push_back(reEXTDEF);
                }
                EXTDEF.push_back(eEXTDEF);

        }else if(D_register[i].opcode=="EXTREF"){
                string reEXTREF;
                vector<string> eEXTREF;

                istringstream delim(D_register[i].label);
                while(getline(delim,reEXTREF,',')){
                    eEXTREF.push_back(reEXTREF);
                }
                EXTREF.push_back(eEXTREF);

        }else if(D_register[i].opcode=="RESW"){
                D_register[i].loc=Loc;
                int re = StringToInt(D_register[i].label);
                Loc=hexAddOperation(Loc,IntToString(re*3));

        }else if(D_register[i].opcode=="LTORG"){
            //取出字元池
            for(unsigned int j=top;j<Littab.size();i++,j++){
                DataStore re;
                re.line=IntToString(i);
                re.mark="*";
                re.opcode=Littab[top++];
                re.loc=Loc;
                if(re.opcode[1]=='C'){
                     Loc=hexAddOperation(Loc,"3");
                }else if(re.opcode[1]=='X'){
                    Loc=hexAddOperation(Loc,"1");
                }
                D_register.insert(D_register.begin()+i+1,re);
                Symtab[re.opcode]=D_register[i+1].loc;
            }


        }else if(D_register[i].opcode=="END"){
           for(unsigned int j=top;j<Littab.size();i++,j++){
                DataStore re;
                re.line=IntToString(i);
                re.mark="*";
                re.opcode=Littab[top++];
                re.loc=Loc;
                if(re.opcode[1]=='C'){
                     Loc=hexAddOperation(Loc,"3");
                }else if(re.opcode[1]=='X'){
                    Loc=hexAddOperation(Loc,"1");
                }
                D_register.insert(D_register.begin()+i+1,re);
                Symtab[re.opcode]=D_register[i+1].loc;
            }
        }else if(D_register[i].opcode=="."||D_register[i].opcode==""){

        }else if(D_register[i].opcode=="EQU"){
               if(D_register[i].label=="*") {
                 D_register[i].loc=Loc;
               }
        }else if(D_register[i].opcode=="RESB"){
                D_register[i].loc=Loc;
                if(D_register[i].label=="4096"){
                    Loc=hexAddOperation(Loc,"1000");
                }else {
                    int re = StringToInt(D_register[i].label);
                    Loc=hexAddOperation(Loc,IntToString(re));
                }
        }else if(D_register[i].opcode=="CSECT"){
                Loc="0000";
                D_register[i].loc=Loc;

        }else if(D_register[i].opcode=="BYTE"){
                D_register[i].loc=Loc;
                Loc=hexAddOperation(Loc,"1");

        }else if(D_register[i].opcode[D_register[i].opcode.size()-1]=='R'){
             D_register[i].two='1';
             D_register[i].loc=Loc;
            Loc=hexAddOperation(Loc,"2");

        }else if(D_register[i].e=='1'){
            D_register[i].loc=Loc;
            Loc=hexAddOperation(Loc,"4");


        }else{
            D_register[i].loc=Loc;
            Loc=hexAddOperation(Loc,"3");
        }
        //建立SYMTAB
        if((D_register[i].mark!=" "&&D_register[i].mark!="*")||D_register[i].opcode=="EQU"){
            string re=D_register[i].mark;
            Symtab[re]=D_register[i].loc;
        }
        //建立OPCODE
        if(judgeop(D_register[i].opcode)){
            for(unsigned int j=0;j<op_code.size();j++){
                if( REAL(D_register[i].opcode) ==op_code[j].opcode){
                    string re=REAL(D_register[i].opcode);
                    Optab[re]=op_code[j].binarycode;

                }
            }
            if(D_register[i].opcode=="EQU"){
                if(D_register[i].label=="*"){
                    string re=D_register[i].mark;
                    Symtab[re]=D_register[i].loc;
                }else{
                    istringstream ss(D_register[i].label);
                    string s1,s2;
                    getline(ss,s1,'-');
                    ss>>s2;
                    s1=hexSub(Symtab[s1],Symtab[s2]);

                    string re=D_register[i].mark;



                }

            }
        }
    }


}
void handle::pass2(){

    unsigned int i;
    int EXTD=0,EXTR=0;

    for(i=0;i<D_register.size();i++){
        if(D_register[i].mark=="FIRST"){

            break;
        }
    }

    for(;i<D_register.size();i++){
        string reString = "";

        if(D_register[i].opcode=="EXTDEF"||D_register[i].opcode=="EXTREF"
           ||D_register[i].opcode=="RESW"||D_register[i].opcode=="LTORG"
           ||D_register[i].opcode=="RESB"||D_register[i].opcode=="EQU"
           ||D_register[i].opcode==""||D_register[i].opcode=="."
           ||D_register[i].opcode=="END"){
                continue;
        }else if(D_register[i].opcode=="CSECT"){
            EXTR++;
            continue;
        }else if(D_register[i].opcode == "=C\'EOF\'"){
            reString= "454F46";
            D_register[i].objectCode= reString;
        }else if(D_register[i].opcode == "=X\'05\'"){
            reString= "05";
            D_register[i].objectCode= reString;
        }else if(D_register[i].opcode=="WORD"){
            reString="000000";
            D_register[i].objectCode=reString;
        }else if(D_register[i].opcode=="BYTE"){
            reString="F1";
            D_register[i].objectCode=reString;
        }else{
            reString+=Optab[REAL(D_register[i].opcode)];
            //判斷是不是FORMAT2

            if(D_register[i].two == '1'){
                if(D_register[i].opcode=="CLEAR"){
                    if(D_register[i].label=="X"){
                        reString+="00010000";
                    }else if(D_register[i].label=="A"){
                        reString+="00000000";
                    }else if(D_register[i].label=="S"){
                        reString+="01000000";
                    }
                }else if(D_register[i].opcode=="COMPR"){
                      reString+="00000100";
                }else if(D_register[i].opcode=="TIXR"){
                      reString+="01010000";
                }
                reString=binaryTohex(reString);
                D_register[i].objectCode=reString;

            }else if(D_register[i].opcode=="RSUB"){
                reString[6]=D_register[i].n;
                reString[7]=D_register[i].i;
                reString+="0000000000000000";
                reString=binaryTohex(reString);
                D_register[i].objectCode=reString;
            }else {
                reString[6]=D_register[i].n;
                reString[7]=D_register[i].i;
                reString.push_back(D_register[i].x);
                reString.push_back(D_register[i].b);
                reString.push_back(D_register[i].p);
                reString.push_back(D_register[i].e);

                //判斷是不是EXTDEF、EXTREF

                if(judgeEX(EXTREF,EXTR,D_register[i])){
                     reString[8]=D_register[i].x;
                     reString[9]='0';
                     reString[10]='0';
                     if(D_register[i].e=='1'){
                         reString+="00000000000000000000";
                    }else{
                        reString+="000000000000";
                    }

                    reString=binaryTohex(reString);
                    D_register[i].objectCode=reString;

                }else if(D_register[i].label[0] == '#'){//immediate addressing mode
                    reString[6]='0';
                    reString[7]='1';
                    reString[9]='0';
                    reString[10]='0';
                    if(D_register[i].label.substr(1)=="0"){
                        reString+="000000000000";
                    }else if(D_register[i].label.substr(1)=="3"){
                        reString+="000000000011";
                    }

                    reString=binaryTohex(reString);
                    D_register[i].objectCode=reString;

                }else{//正常使用
                    string re;
                    if(D_register[i].label[0]=='@'){
                        reString[6]='1';
                        reString[7]='0';
                        re=Symtab[D_register[i].label.substr(1)];

                    }else{
                        re=REAL_label(D_register[i].label,D_register[i]);
                        reString[8]=D_register[i].x;
                        re=Symtab[re];
                    }
                    re = hexSub(re,D_register[i+1].loc);
                    re = re.substr(4);
                    reString+=re;
                    reString=binaryTohex(reString);
                    D_register[i].objectCode=reString;

                }

            }
        }


    }

    PrintVector(D_register);
        cout<<"Symbol table"<<endl;
        PrintSymtab(Symtab);
        cout<<"OP Table"<<endl;
        PrintOptab(Optab);


}
void handle::opjectProgram(){

    vector<string>p1;
    vector<string>p2;
    vector<string>p3;
    //first

    string re;
    re+="H";
    re+="^";
    re+=D_register[0].mark;
    re+="  ";
    re+=hance(D_register[0].loc);
    re+=hance(D_register[21].loc);
    p1.push_back(re);
    re.clear();
    re+="D";
    for(int i=0;i<EXTDEF[0].size();i++){
        re+="^";
        re+=EXTDEF[0][i];
        re+=hance(Symtab[EXTDEF[0][i]] );
    }
    p1.push_back(re);
    re.clear();
    re+="R";
    for(int i=0;i<EXTREF[0].size();i++){
        re+=hanceSTR(EXTREF[0][i]);
    }
    p1.push_back(re);
    re.clear();
    re+="T";
    re+=hance(D_register[3].loc);
    re+="^";
    re+=binaryTohex(hexSub(D_register[12].loc,D_register[3].loc)).substr(2);
    for(int i=3;i<12;++i){
        re+=hance(D_register[i].objectCode);
    }
    p1.push_back(re);
    re.clear();
    re+="T";
    re+=hance(D_register[12].loc);
    re+="^";
    re+=binaryTohex(hexSub(D_register[16].loc,D_register[12].loc)).substr(2);
    for(int i=12;i<16;++i){
        re+=hance(D_register[i].objectCode);
    }
    p1.push_back(re);
    re.clear();
    re+="T";
    re+=hance(D_register[19].loc);
    re+="^";
    re+="03";
    re+=hance(D_register[19].objectCode);
    p1.push_back(re);
    re.clear();
    //修正
    re+="M^000004^05^+RDREC";
    p1.push_back(re);
    re.clear();
    re+="M000011^05^+WRREC";
    p1.push_back(re);
    re.clear();
    re+="M000024^05^+WRREC";
    p1.push_back(re);
    re.clear();
    re+="E^000000";
    p1.push_back(re);
    re.clear();

    //second

    re+="H";
    re+="^";
    re+=D_register[24].mark;
    re+=" ";
    re+=hance(D_register[24].loc);
    re+=hance(hexAddOperation(hance(D_register[44].loc),"0003"));
    p2.push_back(re);
    re.clear();
    re+="R";
    for(int i=0;i<EXTREF[1].size();i++){
        re+=hanceSTR(EXTREF[1][i]);
    }
    p2.push_back(re);
    re.clear();
    re+="T";
    re+=hance(D_register[29].loc);
    re+="^";
    re+=binaryTohex(hexSub(D_register[40].loc,D_register[29].loc)).substr(2);
    for(int i=29;i<40;++i){
        re+="^";
        re+=D_register[i].objectCode;
    }
    p2.push_back(re);
    re.clear();
    re+="T";
    re+=hance(D_register[40].loc);
    re+="^";
    re+=hexAddOperation(binaryTohex(hexSub(D_register[44].loc,D_register[40].loc)).substr(2),"02");
    for(int i=40;i<45;++i){
        re+="^";
        re+=D_register[i].objectCode;
    }
    p2.push_back(re);
    re.clear();
    //修正
    re+="M^000018^05^+BUFFER";
    p2.push_back(re);
    re.clear();
    re+="M^000021^05^+LENGTH";
    p2.push_back(re);
    re.clear();
    re+="M^000028^06^+BUFEND";
    p2.push_back(re);
    re.clear();
    re+="M^000028^06^-BUFEER";
    p2.push_back(re);
    re.clear();
    re+="E";
    p2.push_back(re);
    re.clear();

    //third

    re+="H";
    re+="^";
    re+=D_register[46].mark;
    re+=" ";
    re+=hance(D_register[51].loc);
    re+=hance(hexAddOperation(hance(D_register[61].loc),"0001"));
    p3.push_back(re);
    re.clear();
    re+="R";
    for(int i=0;i<EXTREF[2].size();i++){
        re+=hanceSTR(EXTREF[2][i]);
    }
    p3.push_back(re);
    re.clear();
    re+="T";
    re+=hance(D_register[51].loc);
    re+="^";
    string z=hexAddOperation(hance(D_register[61].loc),"01");
    re+=z.substr(4);
    for(int i=51;i<62;i++){
            if(i==60){
                continue;
            }
        re+="^";
        re+=D_register[i].objectCode;
    }
    p3.push_back(re);
    re.clear();
    //修正
    re+="M^000003^05^+LENGTH";
    p3.push_back(re);
    re.clear();
    re+="M^00000D^05^+BUFFER";
    p3.push_back(re);
    re.clear();
    re+="E";
    p3.push_back(re);
    re.clear();


    for(unsigned int i =0; i<p1.size();i++){
        cout<<p1[i]<<endl;
    }
    cout<<endl;
    for(unsigned int i =0; i<p2.size();i++){
        cout<<p2[i]<<endl;
    }
    cout<<endl;
    for(unsigned int i =0; i<p3.size();i++){
        cout<<p3[i]<<endl;
    }
    cout<<endl;
    SetColor(7);

}
//加空白
string handle::hanceSTR(string r1){
    int longX = r1.size();
        if(longX>6){
                r1.insert(0,"^");
            return r1;
        }else {
            string re="";
            int x = 6 -longX;
            for(int i = 0;i<x;i++){
                re+=" ";
            }
            r1.insert(r1.size(),re);
            r1.insert(0,"^");
            return r1;
        }
        return r1;
}
//加零
string handle::hance(string r1){
    int longX = r1.size();
    if(longX>6){
            r1.insert(0,"^");
        return r1;
    }else {
        int x = 6 -longX;
        for(int i = 0;i<x;i++){
            r1.insert(0,"0");
        }
        r1.insert(0,"^");
        return r1;
    }
    return r1;
}
//判斷是不是EXTDEF
bool handle::judgeEX(vector< vector<string> >EXTREF,int&EXTR,struct DataStore& aa){
    for(unsigned int j=0;j<EXTREF[EXTR].size();j++){
        if(REAL_label(aa.label,aa)==EXTREF[EXTR][j]){
            return true;
        }
    }
    return false;
}
//輸出真正的label
string handle::REAL_label(string str,struct DataStore &aa){
    istringstream ss(str);
    string s1,s2;
    getline(ss,s1,',');
    ss>>s2;
    if(s2=="X"){
        aa.x='1';
    }
    return s1;

}
//輸出真正的OPCODE
string handle::REAL(string str){

   if(str.size()>0){
    if(str[0]=='+'){

        str = str.substr(1);
    }
   }
    for(unsigned int i =0; i<op_code.size();i++){
        if(op_code[i].opcode==str){
                return str;
        }
        if(op_code[i].opcode=="assembler_directive"){
            return "NO";
        }
    }
    return "NO";
}
//多載十六進位加法(string加int)
string handle::hexAddOperation(string LOC,int num){


    vector<int>count1(0,0);
    vector<int>count2(0,0);
    string returnString="";
    int sizeV=0;


    count1.resize(6);
    count2.resize(6);

    for(int i = LOC.size()-1, j=0;i>=0;--i,j++){
            count1[j]=hextoint(LOC[i]);

    }

    while(num>=16){
       count2.push_back(num%16);
       num/=16;
    }

  sizeV =count1.size()>count2.size()?count1.size():count2.size();

    for(int i=0;i<sizeV;i++){

            count1[i]+=count2[i];


        if(count1[i]>=16){
            count1[i+1]+=count1[i]/16;
            count1[i]%=16;
        }
        switch(count1[i]){
            case 0:{
                returnString+="0";
                break;
            }
            case 1 :{
                returnString+="1";
                break;
            }
            case 2 :{
                returnString+="2";
                break;
            }
            case 3 :{
                returnString+="3";
                break;
            }
            case 4 :{
                returnString+="4";
                break;
            }
            case 5 :{
                returnString+="5";
                break;
            }case 6 :{
                returnString+="6";
                break;
            }
            case 7 :{
                returnString+="7";
                break;
            }
            case 8 :{
                returnString+="8";
                break;
            }
            case 9 :{
                returnString+="9";
                break;
            }
            case 10 :{
                returnString+="A";
                break;
            }
            case 11 :{
                returnString+="B";
                break;
            }
            case 12 :{
                returnString+="C";
                break;
            }
            case 13 :{
                returnString+="D";
                break;
            }
            case 14 :{
                returnString+="E";
                break;
            }
            case 15 :{
                returnString+="F";
                break;
            }
        }
    }
    reverse(returnString.begin(),returnString.end());


    return returnString;


}
//多載十六進位加法(string加string)
string handle::hexAddOperation(string LOC,string num){

    vector<int>count1(0,0);
    vector<int>count2(0,0);
    string returnString="";
    int sizeV=0;


    count1.resize(6);
    count2.resize(6);

    for(int i = LOC.size()-1, j=0;i>=0;--i,j++){
            count1[j]=hextoint(LOC[i]);

    }

    for(int i = num.size()-1, j=0;i>=0;--i,j++){
            count2[j]=hextoint(num[i]);
    }

  sizeV =LOC.size()>num.size()?LOC.size():num.size();

    for(int i=0;i<sizeV;i++){

            count1[i]+=count2[i];


        if(count1[i]>=16){
            count1[i+1]+=count1[i]/16;
            count1[i]%=16;
        }
        switch(count1[i]){
            case 0:{
                returnString+="0";
                break;
            }
            case 1 :{
                returnString+="1";
                break;
            }
            case 2 :{
                returnString+="2";
                break;
            }
            case 3 :{
                returnString+="3";
                break;
            }
            case 4 :{
                returnString+="4";
                break;
            }
            case 5 :{
                returnString+="5";
                break;
            }case 6 :{
                returnString+="6";
                break;
            }
            case 7 :{
                returnString+="7";
                break;
            }
            case 8 :{
                returnString+="8";
                break;
            }
            case 9 :{
                returnString+="9";
                break;
            }
            case 10 :{
                returnString+="A";
                break;
            }
            case 11 :{
                returnString+="B";
                break;
            }
            case 12 :{
                returnString+="C";
                break;
            }
            case 13 :{
                returnString+="D";
                break;
            }
            case 14 :{
                returnString+="E";
                break;
            }
            case 15 :{
                returnString+="F";
                break;
            }
        }
    }
    reverse(returnString.begin(),returnString.end());


    return returnString;


}
//十六進位轉整數
int handle::hextoint(char a){

    switch(a){
        case '0':return 0;break;
        case '1':return 1;break;
        case '2':return 2;break;
        case '3':return 3;break;
        case '4':return 4;break;
        case '5':return 5;break;
        case '6':return 6;break;
        case '7':return 7;break;
        case '8':return 8;break;
        case '9':return 9;break;
        case 'A':return 10;break;
        case 'B':return 11;break;
        case 'C':return 12;break;
        case 'D':return 13;break;
        case 'E':return 14;break;
        case 'F':return 15;break;

    }

    return 0;
}
//十六進位減法(回傳二進位算出的值)
string handle::hexSub(string s1,string s2){
    string ss;
    s1=HexToBinary(s1);
    s2=HexToBinary(s2);
    ss=binarySub(s1,s2);
    return  ss;


}
//二進位減法
string handle::binarySub(string s1,string s2){

        unsigned int i;//j;
        for(i=0;i<s2.size();i++){
            if(s2[i]=='0'){
                s2[i]='1';
            }else if(s2[i]=='1'){
                s2[i]='0';
            }
        }

        if(s2[s2.size()-1]=='0'){

            s2[s2.size()-1]='1';
        }else if(s2[s2.size()-1]=='1'){
            s2[s2.size()-1]='2';
        }


       for(int i=s2.size()-1;i >=0; --i){

            if(i!=0){
                if(s2[i]=='2'){
                    s2[i]='0';
                    if(s2[i-1]=='0'){
                        s2[i-1]='1';
                    }else if(s2[i-1]=='1'){
                        s2[i-1]='2';
                    }
                }
            }else{
                if(s2[0]=='2'){
                    s2[0]='0';
                }
            }
        }

        vector<int>count1(0,0);
        vector<int>count2(0,0);
        string returnString="";
        int sizeV=0;


        count1.resize(100);
        count2.resize(100);

        for(int i = s1.size()-1, j=0;i >= 0 ;--i,j++){
                count1[j]=s1[i]-'0';


        }

        for(int i = s2.size()-1, j=0;i >=0;--i,j++){
                count2[j]=s2[i]-'0';

        }

      sizeV =s1.size()>s2.size()?s1.size():s2.size();

        for(int i=0;i<sizeV;i++){

            count1[i]+=count2[i];

            if(i==sizeV-1){
                if(count1[i]>=2){
                    count1[i]=0;
                }
            }else {
                if(count1[i]>=2){
                    count1[i+1]+=count1[i]/2;
                    count1[i]%=2;
                }
            }
            switch(count1[i]){
                case 0:{
                    returnString+="0";
                    break;
                }
                case 1 :{
                    returnString+="1";
                    break;
                }
            }

        }


        reverse(returnString.begin(),returnString.end());



        return returnString;

}
//二進位轉十六進位
string handle::binaryTohex(string s1){
    unsigned int i;
    unsigned int sizeX =s1.size()/4;
    unsigned int mod=s1.size()%4;
    unsigned int xx = 4-mod;
    string str="";
    if(mod!=0){
        for(i=0;i<xx;i++){
            s1.insert(0,"0");
        }
        sizeX++;
    }

    int j=0;
    for(i =0;i<sizeX;i++){
        string re="";
        int p=4;
        while(p--){
            re.push_back(s1[j++]);
        }
        if(re=="0000"){
            str+="0";
        }else if(re=="0001"){
            str+="1";
        }else if(re=="0010"){
            str+="2";
        }else if(re=="0011"){
            str+="3";
        }else if(re=="0100"){
            str+="4";
        }else if(re=="0101"){
            str+="5";
        }else if(re=="0110"){
            str+="6";
        }else if(re=="0111"){
            str+="7";
        }else if(re=="1000"){
            str+="8";
        }else if(re=="1001"){
            str+="9";
        }else if(re=="1010"){
            str+="A";
        }else if(re=="1011"){
            str+="B";
        }else if(re=="1100"){
            str+="C";
        }else if(re=="1101"){
            str+="D";
        }else if(re=="1110"){
            str+="E";
        }else if(re=="1111"){
            str+="F";
        }
    }
    return str;

}






