#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ash.h"
#define MEM 4096

int first_pass(asmcode *code, FILE *input)
{
    int i;
    int lc = 0;
    int n = 0;
    char str[MAX];

    for(i = 0;n >= 0 && fgets(str, MAX-10, input) != NULL; i++)
    {

        clean(str); //處理註解
        if(n = func(str))
        {
            lc = n - 1;
            i--;
        }
        else
        {
            code[i].lc = lc;
            tabset(&code[i], str);
            lc++;
        }
    }
    code[i].lc = -1;
    fclose(input);

    return i + 1;
}

uint16_t *second_pass(asmcode *code)
{
    int i, j;
    char *str;
    uint16_t *out = malloc(MEM * sizeof(uint16_t));

    memset(out, 0, sizeof(uint16_t) * MEM);
    for(i = 0; code[i].lc != -1; i++)
    {
        int head, para;
        
        head = decode(code[i].code);
        para = depara(code, code[i].para);
        out[code[i].lc] = hex(head, para);
        printf("memory[%X] = 0x%hx;\n",code[i].lc, hex(head, para));
    }

    return out;
}

void tabset(asmcode *code, char *str)
{
    char reg[4][10] = {};
    int i;
   
    sscanf(str,"%s%s%s%s", reg[0], reg[1], reg[2], reg[3]);

    if(islabel(reg[0]))
    {
        strcpy(code->label, reg[0]);
        if(iscode(reg[1]))
        {
            strcpy(code->code, reg[1]);
            if(ispara(reg[2]))
            {
                strcpy(code->para, reg[2]);
                if(isi(reg[3]))
                    code->i = 1;
                else if(isblank_(reg[3]))
                    ;
                else
                    err();
            }
            else if(isblank_(reg[3]))
                ;
            else
                err();
        }
        else if(ispara(reg[1]))
        {
            if(isblank_(reg[2]))
                ;
            else
                err();
        }
    }       
    else if(iscode(reg[0]))
    {
        strcpy(code->code, reg[0]);
        if(ispara(reg[1]))
        {
            strcpy(code->para, reg[1]);
            if(isi(reg[2]))
                code->i = 1;
            else if(isblank_(reg[2]))
                ;
            else 
                err();
        }
        else if(isblank_(reg[1]))
            ;
        else
            err();
    }
    else if(ispara(reg[0]))
    {
        if(isblank_(reg[1]))
            ;
        else
            err();
    }
    else
        err();
}


void clean(char *str)
{
    int i;

    for(i = 0; str[i] != '\0'; i++)
        if(str[i] == '/' && str[i+1] == '/' || str[i] == '\n')
            str[i] = str[i+1] = '\0';
}

int func(char *str)
{
    char *ptr;
    int reg;

    if((ptr = strstr(str, "END")) != NULL)
        return -10;
    if((ptr = strstr(str, "ORG")) != NULL)
    {
        sscanf(ptr, "%*s%x", &reg);
        return reg + 1;
    }

    return 0;
}

void print_code(asmcode *code, int count)
{
    int i;

    for(i = 0; i < count; i++)
    {
        printf("%d:\n", i+1);
        printf("label:%s\n", code[i].label);
        printf("code :%s\n", code[i].code);
        printf("para :%s\n", code[i].para);
        printf("lc   :%X\n\n", code[i].lc);
    }
}

optab *init(void)
{
    optab *reg = malloc(sizeof(optab) * 26);

    strcpy(reg[0].op, "AND");
    reg[0].num = 0x0;
    strcpy(reg[1].op, "ADD");
    reg[1].num = 0x1;
    strcpy(reg[2].op , "LDA");
    reg[2].num = 0x2;
    strcpy(reg[3].op , "STA");
    reg[3].num = 0x3;
    strcpy(reg[4].op, "BUN");
    reg[4].num = 0x4;
    strcpy(reg[5].op, "ISZ");
    reg[5].num = 0x6;
    
    strcpy(reg[6].op, "CLA");
    reg[6].num  = 0x7800;
    strcpy(reg[7].op, "CLE");
    reg[7].num  = 0x7400;
    strcpy(reg[8].op, "CMA");
    reg[8].num  = 0x7200;
    strcpy(reg[9].op, "CME");
    reg[9].num  = 0x7100;
    strcpy(reg[10].op, "CIR");
    reg[10].num = 0x7080;
    strcpy(reg[11].op, "CIL");
    reg[11].num = 0x7040;
    strcpy(reg[12].op, "INC");
    reg[12].num = 0x7020;
    strcpy(reg[13].op, "SPA");
    reg[13].num = 0x7010;
    strcpy(reg[14].op, "SNA");
    reg[14].num = 0x7008;
    strcpy(reg[15].op, "SZA");
    reg[15].num = 0x7004;
    strcpy(reg[16].op, "HLT");
    reg[16].num = 0x7001;

    strcpy(reg[17].op, "INP");
    reg[17].num = 0xF800;
    strcpy(reg[18].op, "OUT");
    reg[18].num = 0xF400;
    strcpy(reg[19].op, "SKI");
    reg[19].num = 0xF200;
    strcpy(reg[20].op, "SKO");
    reg[20].num = 0xF100;
    strcpy(reg[21].op, "ION");
    reg[21].num = 0xF080;
    strcpy(reg[22].op, "IOF");
    reg[22].num = 0xF040;
    
    strcpy(reg[23].op, "DEC");
    reg[23].num = 0x0;
    strcpy(reg[24].op, "HEX");
    reg[24].num = 0x0;
    strcpy(reg[25].op, "BSA");
    reg[25].num = 0x5;
        
    return reg;
}

int islabel(char *str)
{
    int i;

    for(i = 0; str[i] != '\0'; i++);

    if(str[i - 1] == ':')
        return 1;
    return 0;
}

int iscode(char *str)
{
    int i;
    optab *code = init();

    for(i = 0;i < 25 && strstr(str, code[i].op) == NULL; i++);
    if(i < 25)
        return code[i].num + 1;
    return 0;
}

int ispara(char *str)
{
    if(isblank_(str))
        return 0;
    if(str[0] >= '0' && str[0] <= '9')
        return 1;
    if(!iscode(str))
        return 1;
    return 0;
}

int isi(char *str)
{
    if(str[0] == 'I')
        return 1;
    return 0;
}

int isblank_(char *str)
{
    return !(str[0] > 32);
}

int decode(char *str)
{
    int n;

    if(!(n = iscode(str)))
        return -1;
    return n - 1;
}

int depara(asmcode *code, char *str)
{
    int i;
    int n = strlen(str);

    if(atoi(str) < 0)
        return atoi(str);
    if(str[0] == '\0')
        return 0;
    if((str[0] >= '0' && str[0] <= '9'))
        return atoi(str);
    for(i = 0; code[i].lc != -1; i++)
    {
        if(strstr(code[i].label, str) != NULL)
            if(code[i].label[n] == ':')
                return code[i].lc;
    }
    return -1;
}

int16_t hex(int head, int para)
{
    if(head > 15)
        return head;
    return ((head << 12) + para);
}
void err(void)
{

}
