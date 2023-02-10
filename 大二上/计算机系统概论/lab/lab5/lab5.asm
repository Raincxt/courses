;skiing
;input: A N*M map provided as a two-dimensional
;array and stored from memory location x4000.
;output:R2.the longest distance 
;
;draw the new map with boundary
;
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
        AND     R3,R3,#0
        ADD     R3,R3,#-2
        STR     R3,R2,#0
;
;the real processing part
;
        AND     R2,R2,#0        ;initialization
        AND     R4,R4,#0
        LD      R6, USER_SP
        ADD     R5,R6,#0
        LD      R1,FIR_A2
        LDR     R3,R1,#1
        ADD     R3,R3,#4
        ADD     R3,R3,R1
        ADD     R1,R3,#0
loop    ADD     R0,R1,#1        ;check every point to get the longest distance
        ADD     R1,R1,#1        ;R1:address of the current starting point
        ADD     R6,R6,#-1       ;push R1,R2 
        STR     R1,R6,#0
        ADD     R6,R6,#-1       
        STR     R2,R6,#0
        LDR     R2,R0,#0        ;initialize R2,R3（DFS）（cell data and address of the caller）
        ADD     R3,R0,#0
        JSR     DFS
        LDR     R2,R6,#0        ;pop R2,R1
        ADD     R6,R6,#1
        LDR     R1,R6,#0
        ADD     R6,R6,#1
        LDR     R7,R1,#0 
        ADD     R7,R7,#2
        BRz     END1
        BR      loop
END1    TRAP    x25
; 
;R4,the longest distance
;R7,PC of the caller
;R3,cell address of the caller
;R2,cell data of the caller
;
DFS     ADD     R4,R4,#1        
        ADD     R6,R6,#-1       ;push R7,R3,R2
        STR     R2,R6,#0
        ADD     R6,R6,#-1
        STR     R3,R6,#0
        ADD     R6,R6,#-1
        STR     R7,R6,#0
        ADD     R3,R0,#0
        LDR     R2,R3,#0        ;if data of the current address is 200,return DONE
        LD      R1,VALUE1
        NOT     R1,R1
        ADD     R1,R1,#1
        ADD     R1,R1,R2
        BRz     DONE
        AND     R2,R2,#0        ;put breadcrumb in the current cell
        ADD     R2,R2,#-1
        
NORTH   LDI     R7,M            ;tmp variable:R7,R0,R1
        ADD     R7,R7,#2
        NOT     R7,R7
        ADD     R7,R7,#1
        ADD     R7,R7,R3        ;R7:address of the north cell
        LDR     R0,R7,#0        ;R0:data of the north cell
        BRn     EAST            ;if a breadcrumb is in the north cell,check the EAST
        NOT     R0,R0
        ADD     R0,R0,#1
        LDR     R1,R3,#0        ;R1: data of the current cell
        ADD     R0,R0,R1        
        BRnz    EAST
        ADD     R0,R7,#0        ;put the address of the north cell in R0
        JSR     DFS
        
EAST    ADD     R7,R3,#1        ;R7:address of the east cell
        LDR     R0,R7,#0        ;R0:data of the east cell
        BRn     SOUTH           ;if a breadcrumb is in the east cell,check the SOUTH
        NOT     R0,R0
        ADD     R0,R0,#1
        LDR     R1,R3,#0        ;R1: data of the current cell
        ADD     R0,R0,R1        
        BRnz    SOUTH
        ADD     R0,R7,#0        ;put the address of the east cell in R0
        JSR     DFS

SOUTH   LDI     R7,M            ;tmp variable:R7,R0,R1
        ADD     R7,R7,#2
        ADD     R7,R7,R3        ;R7:address of the SOUTH cell
        LDR     R0,R7,#0        ;R0:data of the SOUTH cell
        BRn     WEST            ;if a breadcrumb is in the SOUTH cell,check the WEST
        NOT     R0,R0
        ADD     R0,R0,#1
        LDR     R1,R3,#0        ;R1: data of the current cell
        ADD     R0,R0,R1        
        BRnz    WEST
        ADD     R0,R7,#0        ;put the address of the SOUTH cell in R0
        JSR     DFS

WEST    ADD     R7,R3,#-1        ;R7:address of the west cell
        LDR     R0,R7,#0        ;R0:data of the west cell
        BRn     SOUTH           ;if a breadcrumb is in the west cell,return DONE
        NOT     R0,R0
        ADD     R0,R0,#1
        LDR     R1,R3,#0        ;R1: data of the current cell
        ADD     R0,R0,R1        
        BRnz    DONE
        ADD     R0,R7,#0        ;put the address of the east cell in R0
        JSR     DFS
        
DONE    ADD     R0,R3,#0        ;return the address stored in R3 back to R0
        LD      R5,USER_SP
        ADD     R5,R5,#-2
        LDR     R2,R5,#0
        NOT     R2,R2
        ADD     R2,R2,#1
        ADD     R2,R2,R4
        BRp     CHANGE
        BR      RESTORE
        
CHANGE  STR     R4,R5,#0        
RESTORE ADD     R4,R4,#-1 
        LDR     R7,R6,#0        ;pop R7,R3,R1 
        ADD     R6,R6,#1
        LDR     R3,R6,#0
        ADD     R6,R6,#1
        LDR     R2,R6,#0
        ADD     R6,R6,#1
        RET
        
        N           .FILL x4000
        M           .FILL X4001
        VALUE1      .FILL #200
        FIR_A1      .FILL x4000
        FIR_A2      .FILL x4100
        USER_SP     .FILL xFE00
       
.END


