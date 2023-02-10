1.Algoritm

<img src="C:\Users\xiaot\Desktop\introduction to computer systems\lab\lab3\lab3流程图.jpg" alt="lab3流程图" style="zoom: 33%;" />

2.Essential codes

    ;Interrupt service routine
           .ORIG   x0800
            ST      R3,IN_R3
            LDI     R5,KBDR
            LD      R4,NZERO
            ADD     R3,R5,#-10
            BRz     C_ENTER         ;the input is "Enter"
            ADD     R3,R5,R4
            BRn     C_ECHO          ;the input character is lower than 48(ASCII of 0)
            LD      R4,NNINE
            ADD     R3,R5,R4
            BRp     C_ECHO          ;the input character is larger than 56(ASCII of 9)
            BRnzp   C_NUM           ;the input character is between 48（0） and 57（9）     
    C_ENTER 
            LD      R0,ENTER2
            OUT
            ADD     R1,R1,#0
            BRz     Back
            ADD     R1,R1,#-1
            BRnzp   Back
    C_ECHO  ADD     R0,R5,#0    ;output the input character 40 times
            LD      R4,ECHO_times
            LD      R0,ENTER2
            OUT
    loop2   ADD     R0,R5,#0
            OUT
            ADD     R4,R4,#-1
            BRp     Loop2 
            LD      R0,ENTER2
            OUT
            BRnzp   Back
    C_NUM   LD      R0,ENTER2
            OUT
            LD      R1,NZERO
            ADD     R1,R1,R5
            BRnzp   Back      
    Back    LD      R2,Out_times2
            LD      R0,ZERO
            ADD     R0,R1,R0
            LD      R3,IN_R3
            RTI
    

When the interrupt service routine is over, the contents of R0 ,R1 exactly equal to the contents required in the loop of the user program .



3.Q&A

When the interruption occurs before output the ENTER,the out_times are set back to 40. As a result,there will be 41 times of output. How will you deal with this bug?

I will use Interrupt_R2 to save R2(the number of output times in the user program) during the interrupt service routine,and load back before RTI, thus guarantee the adequate number of output!
