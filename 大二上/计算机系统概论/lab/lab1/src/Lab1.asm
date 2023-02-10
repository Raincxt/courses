0011 0000 0000 0000 ;not an instruction,it tells the simulator to put the program at x3000

0101 001 001 1 00000;clear r1 r1("111")
0101 010 010 1 00000;clear r2 result 0/1
0101 011 011 1 00000;clear r3 the 16-bit to be compared
0101 100 100 1 00000;clear r4 variable
0101 101 101 1 00000;clear r5 times   

0001 001 001 1 00111;r1<-7
0010 011 01111 1001;load r3 (x3100) 
0001 101 101 1 01111;r5<-r5+15

0001 101 101 1 11111;r5<-r5-1
0000 010 00000 1000 ;（if r5==0,jump to the end of the loop）

0101 100 001 000 011 ;r4<-r1 AND r3
1001 100 100 111111;
0001 100 100 100001;
0001 100 100 000 001;判断R4=R1 
0000 010 00000 0010;BRz x3??? (if r4==r1, jump to the end&&r2=1)

0001 001 001 0 00 001 ;r1<-r1+r1
0000 111 11111 0111;BRnzp (jump to the head of the loop)

0001 010 010 1 00001;r2<-r2+1 ("111"exsit)
1111 0000 0010 0101 ;;halt(trap)