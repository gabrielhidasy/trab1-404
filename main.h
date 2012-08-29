#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct pcountera {
  int position;
  int side;
} pcounter;
typedef struct liste {
  pcounter pos;
  char *name;
  struct liste *prox;
} list;
typedef struct ltokens {
  char tokenname[50];
  char tokentype;
  struct ltokens *prox;
} listtokens;
void erro(int err);
listtokens *tokenizer(char *codigo, listtokens *l, pcounter pc);
char *remove_coments(FILE *cod, char *code);
char *remove_double_spaces(char *code);
char *trata_constante(char *temptoken);
char *trata0x(char *in);
char *trata0o(char *in); 
char *trata0b(char *in);
char *trataM(char *in);
