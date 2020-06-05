        BSA x1
        BSA CALL
        ADD NUM
        OUT
        HLT
        ORG 1000

BP,     DEC 3999
SP,     DEC 3999
N1,     DEC -1
N2,     DEC -2
N4,     DEC -4
P2,     DEC 2
P1,     DEC 1
P10,    DEC 10
R0,     DEC 0
R1,     DEC 0
NUM,    DEC 48

OOO,    DEC 0
        LDA OOO
        BSA CALL
        LDA P10
        SZA
        BUN J1
        BUN J2
J1,     ADD N1
        STA P10
        ADD NUM
        OUT
        BSA OOO
J2,     BSA RET


PUSH,   DEC 0       //將AC PUSH到mem[SP++]
        STA SP I
        LDA SP
        ADD N1
        STA SP
        BUN PUSH I

POP,    DEC 0       //將mem[--SP]讀到AC
        LDA SP
        INC
        STA SP
        LDA SP I
        BUN POP I

SAVE,   DEC 0       //呼叫AC儲存的function位址
        BSA PUSH
        LDA BP
        BSA PUSH
        LDA SP
        STA BP
        BUN SAVE I
//  argc[n]
//  argc[...]
//  argc[0]                     -3
//  ret STA add <- 先不實做     (-3)
//  return add                  -2
//  'BP                         -1
//  new BP,SP ->  //NULL
RET,    DEC 0       //將return值存到AC，並返回上一函式
        STA R1
        LDA BP
        STA SP
        BSA POP
        STA BP
        BSA POP
        STA R0
        LDA R1
        BUN R0 I

CALL,   DEC 0
        STA R0
        LDA CALL
        BSA PUSH
        LDA BP
        BSA PUSH
        LDA SP
        STA BP
        BUN R0 I

GETF,   DEC 0
        LDA GETF
        ADD P1
        BUN GETF I

x1,     DEC 0
        BSA GETF
        BUN x1 I
        LDA P1
        ADD P2
        BSA RET

func,   DEC 0        
        LDA func     
        BSA SAVE     
        LDA P1        
        BSA PUSH     
        LDA P2        
        BSA PUSH     
        BSA POP      
        STA R1       
        BSA POP      
        ADD R1       
        BSA PUSH     
        BSA POP      
        BSA RET      
        END
