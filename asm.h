#ifndef __ASM_H_
#define __ASM_H_

#define MEM 4096
#define STR 30
struct symbol 
{
    char label[STR];
    int lc;
};

struct line
{
    char label[STR];
    char instr[STR];
    char param[STR];
    int i;
    int lc;
};

int first_pass(FILE *in, struct line *table, struct symbol *symbol_table);
//處理資料和symbol_table

uint16_t *second_pass(struct line *table, struct symbol *symbol_table);
//依靠處理過的資料和symobl_table產生機器碼
#endif
