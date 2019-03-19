#include "handle.h"


using namespace std;


int main(){

	//宣告一個assembler物件
	handle assembler;

	assembler.inputTxtFile();
    assembler.pass1();
    assembler.pass2();
    assembler.opjectProgram();
	return 0;
}


