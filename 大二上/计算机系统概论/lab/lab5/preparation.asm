;draw the new map with boundary
.ORIG x3000
        LD      R0,FIR_A1       ;R0,store data in the first address
        LD      R2,FIR_A2       ;R2,store data in the new address
        LDR     R1,R0,#0        ;R1,free register,used to pass data
        STR     R1,R2,#0
        ADD     R2,R2,#1
        ADD     R0,R0,#1
        LDR     R1,R0,#0
        ADD     R3,R1,#0        
        STR     R1,R2,#0
        ADD     R2,R2,#1
        ADD     R0,R0,#1
        ADD     R3,R3,#2        ;R3:M+2 store N,M in the new address
        LD      R4,N            
        LDR     R4,R4,#0        ;R4:N 
LOOP1   LD      R1,VALUE1       ;store M+2 200 in the new address
        STR     R1,R2,#0
        ADD     R2,R2,#1
        ADD     R3,R3,#-1
        BRz     LOOP2
        BR      LOOP1           
LOOP2   LD      R3,M            ;store M+2 data 
        LDR     R3,R3,#0        ;R3 times
        STR     R1,R2,#0        ;store 200 in the first row
        ADD     R2,R2,#1
LOOPI   LDR     R1,R0,#0        ;store M data from R0 to R2 per inner loop
        STR     R1,R2,#0
        ADD     R2,R2,#1
        ADD     R0,R0,#1
        ADD     R3,R3,#-1
        BRz     NEXT1
        BR      LOOPI
NEXT1   LD      R1,VALUE1       ;store 200 in the last row
        STR     R1,R2,#0
        ADD     R2,R2,#1
        ADD     R4,R4,#-1
        BRz     LOOP3
        BR      LOOP2
LOOP3   LD      R3,M            ;store M+2 data 
        LDR     R3,R3,#0        ;R3:M+2
        ADD     R3,R3,#2
NEXT2   STR     R1,R2,#0        ;store 200 in the first row
        ADD     R2,R2,#1 
        ADD     R3,R3,#-1
        BRp     NEXT2
 STR     R3,R2,#0
DONE    HALT
N           .FILL x4000
M           .FILL X4001
VALUE1      .FILL #200
FIR_A1      .FILL x4000
FIR_A2      .FILL x4100 
.END