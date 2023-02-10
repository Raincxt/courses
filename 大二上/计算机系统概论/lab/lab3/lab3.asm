;
;user program:The user program prints
;the number of tasks continuously and infinitely
;
        .ORIG   x3000     
;initialization
        AND     R0,R0,#0
        AND     R1,R1,#0
        AND     R2,R2,#0
        AND     R3,R3,#0
        AND     R4,R4,#0
        AND     R5,R5,#0
;
;output the character continualy
;
        LD      R2,Out_times
        ADD     R1,R1,#7
loop1   LD      R0,ZERO1
        ADD     R0,R1,R0
        OUT
        JSR     Delay
        ADD     R2,R2,#-1
        BRz     NewLine
        BRnzp   Loop1
NewLine LD      R0,Enter        ;next line
        OUT
        LD      R2,Out_times
        BRnzp   Loop1
        
;
;slow down the scroll
;
Delay   ST      R3, Delay_R
        LD      R3, Delay_COUNT
LoopD   ADD     R3, R3, #-1
        BRnp    LoopD
        LD      R3, Delay_R
        RET
        
        ZERO1           .FILL   #48
        Enter           .FILL   #10
        Lucky_n         .FILL   #55
        Out_times       .FILL   #40
        Delay_COUNT     .FILL   #1000
        Delay_R         .BLKW   #1
      
        .END
;
;Interrupt service routine
;
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

        KBDR        .FILL   xFE02
        ENTER2      .FILL   #10
        ECHO_times  .FILL   #40
        NZERO       .FILL   #-48
        ZERO        .FILL   #48
        NNINE       .FILL   #-57
        Out_times2  .FILL   #41
        IN_R3       .BLKW   #1
        .END
;
;system booting code
        .ORIG   x0200
        
        LD      R1,INEN         
        STI     R1,KBSR         ; KBSR[14] <- 1
        LD      R1,ISR
        STI     R1,INTV         ; INTV[0180]<- x0800
        
        LD      R6, OS_SP
        LD      R0, USER_PSR
        ADD     R6, R6, #-1
        STR     R0, R6, #0
        LD      R0, USER_PC
        ADD     R6, R6, #-1
        STR     R0, R6, #0
        RTI
        
        INEN        .FILL x4000
        KBSR        .FILL xFE00
        ISR         .FILL x0800
        INTV        .FILL x0180
        OS_SP       .FILL x3000
        USER_PSR    .FILL x8002
        USER_PC     .FILL x3000
        .END