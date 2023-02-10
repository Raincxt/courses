1.Algorithm

<img src="C:\Users\xiaot\Desktop\introduction to computer systems\lab\lab2\总流程图LAB2.jpg" alt="总流程图LAB2" style="zoom: 33%;" />

<img src="C:\Users\xiaot\Desktop\introduction to computer systems\lab\lab2\Search流程图LAB2.jpg" alt="Search流程图LAB2" style="zoom:33%;" />

2.Essencial parts

```
SEARCH  LD      R5,LinAd    ;R5 is the pointer that connects the list
INI     LEA     R1,Name     ;R1<-address of the input name
        LDR     R5,R5,#0    ;pointeri->pointeri+1
        BRz     Final       ;Final means the ending of search
        LDR     R2,R5,#2    ;R2<-address of the first name
Next1   LDR     R3,R1,#0    ;R3 contains the character of the input name
        LDR     R4,R2,#0    ;R4 contains the character of the name from the list
        BRz     CHECK1      ;check whether M[R1] equals M[R2]
FCMP    NOT     R3,R3
        ADD     R3,R3,#1
        ADD     R3,R3,R4
        BRnp    LCMP        ;compare the input name with the last name
        ADD     R1,R1,#1    
        ADD     R2,R2,#1    
        BRnzp   Next1       ;compare the charater one by one
        
CHECK1  ADD     R3,R3,#0
        BRz     Done        ;if M[R1] equals M[R2],jump to the Done

LCMP    LEA     R1,Name     
        LDR     R2,R5,#3    ;R2<-address of the last name
Next2   LDR     R3,R1,#0    
        LDR     R4,R2,#0    
        BRz     CHECK2      ;check whether M[R1] equals M[R2]
        NOT     R3,R3
        ADD     R3,R3,#1
        ADD     R3,R3,R4
        BRnp    Fail        ;fail to match with this name
        ADD     R1,R1,#1
        ADD     R2,R2,#1
        BRnzp   Next2        

CHECK2  ADD     R3,R3,#0
        BRz     Done        ;if M[R1] equals M[R2],jump to the Done
        BRnzp   INI      ;compare the input name with the next person
        Fail    ADD     R6,R6,#0
        BRnzp   INI         ;compare the input name with the next person
        
Final   ADD     R6,R6,#0
        BRz     NotF
        BRnzp   END
        
NotF    LEA     R0,RESULT1
        PUTS
```

3.Q&A

How do you compare the two strings?

It compare the two strings character by character. If it fails to match the first name, compare the last name. If it fails again,trap to the next pointer.