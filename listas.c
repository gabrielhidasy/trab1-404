#include "main.h"
/* Adiciona Labels na lista */
listlabels *addlabel(char *name, listlabels *l, pcounter *pc) {
  listlabels *aux;
  char *auxname = malloc(sizeof(char)*50);
  int i=0;
  aux = l;
  if(l==NULL) { //não precisa checar repetição, era null
    l = malloc(sizeof(listlabels));
    aux = l;
    l->prox = NULL;
    l->label.position=pc->position;
    l->label.side=pc->side;
    l->labelname=malloc(sizeof(char)*50);
    strcpy(l->labelname,name);
    i=0;
    while(l->labelname[i]!=':') i++;
    l->labelname[i]='\0';
    free(auxname);
    return aux;
  }
  sprintf(auxname,"%s",name);
  i=0;
  while(auxname[i]!=':') i++;
  auxname[i]='\0';
  while(l->prox!=NULL) {
    if(!strcmp(l->labelname,auxname)) { //checa repetição
      free(auxname);
      return aux;
    }
    l=l->prox;
  }
  l->prox = malloc(sizeof(listlabels));
  l=l->prox;
  l->prox=NULL;
  l->label.position=pc->position;
  l->label.side=pc->side;
  l->labelname=malloc(sizeof(char)*50);
  strcpy(l->labelname,name);
  i=0;
  while(l->labelname[i]!=':') i++;
  l->labelname[i]='\0';
  free(auxname);
  return aux;
}
