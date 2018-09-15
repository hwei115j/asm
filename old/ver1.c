#include <stdio.h>
#include <string.h>
#define MAX 110

void clean(char *str)
{
    int i;

    for(i = 0; str[i] != '\0'; i++)
        if(str[i] == '/' && str[i+1] == '/' || str[i] == '\n')
            str[i] = str[i+1] = '\0';
}

int func(char *str, char *code)
{
    char *ptr;
    int reg;

    if((ptr = strstr(str, code)) == NULL)
        return 0;
    sscanf(ptr, "%*s%d", &reg);

    return reg;
}

int first_pass(void)
{
    int i;
    int pc = 0;
    char str[MAX];
    FILE *input, *output;

    input = fopen("input", "r");
    output = fopen("output", "w");
    while(fgets(str, MAX-10, input) != NULL)
    {
        clean(str); //處理註解
        fprintf(output, "%03d  ", pc);
        fprintf(output, "%s\n", str);
        pc += func(str, "ORG");
        pc++;
    }
    fclose(input);
    fclose(output);

    return 0;
}

int second_pass()
{
}
int main()
{
    first_pass();
    return 0;

}
