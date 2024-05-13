#include<stdio.h>
#include<stdlib.h>



typedef struct emp_{
char emp_name[30];
unsigned int emp_id;
unsigned int age;
struct emp_ *mgr;
float salary;
}emp_t;

int main()
{

emp_t *x=(emp_t*)malloc(sizeof(emp_t));

printf("\n");
printf("%ul\n",&x->emp_name);
printf("%ul\n",&x->emp_id);




}