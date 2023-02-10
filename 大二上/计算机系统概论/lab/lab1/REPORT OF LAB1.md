REPORT OF LAB1

1.algorithm

<img src="C:\Users\xiaot\Desktop\计算机系统概论\lab\lab1流程图.jpg" alt="lab1流程图" style="zoom:50%;" />

2.core codes

r1 is initialized as 0000 0000 0000 0111,then by r1<-r1+r1,r1 is changed into 16-bits codes contained "111",while other bits are all 0.
r2 is used to represent the result 0/1
r3 tis he 16-bit to be compared
r4 is a variable
r5 is used to count the times  of comparing

0000 010 00000 1000 ;（if r5==0,jump to the end of the loop）

0000 111 11111 0111;BRnzp (jump to the head of the loop)

These two instructions above are used to control the loop.

3.questions from TA

The datapath of LEA and will it change the NZP value?

datapath:

<img src="C:\Users\xiaot\Desktop\计算机系统概论\lab\src\lab1 datapath.png" alt="lab1 datapath" style="zoom: 25%;" />

It won't set NZP,since it makes no sense to set the NZP while calculating an address and saving it in the DR.

