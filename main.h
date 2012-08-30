#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct pcountera {
  int position;
  int side;
} pcounter;
typedef struct liste {
  pcounter pos;
  char labelname[50];
  struct liste *prox;
} listlabels;
typedef struct ltokens {
  char tokenname[50];
  char tokentype;
  struct ltokens *prox;
} listtokens;
typedef struct labell {
  char *position;
  int side;
} label;
void erro(int err);
listlabels *addlabel(char *name, listlabels *l, pcounter pc);
listtokens *tokenizer(char *codigo, listtokens *l, pcounter pc);
void arithmetics(listtokens *l, pcounter *pc, FILE *hexa);
void trata_diretiva(listtokens *l, pcounter *pc, FILE *hexa);
char *remove_coments(FILE *cod, char *code);
char *remove_double_spaces(char *code);
char *trata_constante(char *temptoken);
char *trata0x(char *in);
char *trata0o(char *in); 
char *trata0b(char *in);
char *trataM(char *in);
label *trataL(char *in);
