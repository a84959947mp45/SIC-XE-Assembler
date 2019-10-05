以C++語言撰寫的Assembler，將組合語言以16進位的機器語言表示
會將結果以彩色的模式印製到CMD視窗上

使用code::block編譯器使用  點擊.cbp檔案開啟

說明:

input:

Figure2.15.txt 為範例組合語言格式

OP_code.txt 為SIC/XE所使用的指令集格式(包含指令種類、指令16進位的機器語言、assembler_directive)

結果:

產出檔案:

Figure2.16.txt 將Figure2.15.txt每行進行標號

op_code2.txt   將OP_code.txt整理並將十六進位的編碼改為二進位 新增一行至檔案

螢幕顯示:

將輸入的組合語言新增loc行跟object_code行，以彩色部份顯示

![](https://github.com/a84959947mp45/SIC-XE-Assembler/blob/master/img/answerview1.PNG)


所使用的symbol_table及OP_code也會顯示

![](https://github.com/a84959947mp45/SIC-XE-Assembler/blob/master/img/answerview2.PNG)


最後組合語言組譯，以十六進位碼表示所有數值

![](https://github.com/a84959947mp45/SIC-XE-Assembler/blob/master/img/answerview3.PNG)
