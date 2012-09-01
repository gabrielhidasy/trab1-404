#include "main.h"
listlabels *addlabel(char *name, listlabels *l, pcounter *pc) {
  listlabels *aux;
  char *auxname = malloc(sizeof(char)*50);
  int i=0;
  aux = l;
  //printf("Recebi o token %s pra inserir com pc %d (%X)\n",
  //name,pc->position,pc->position); 
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
  //printf("l->labelname foi inserido como %s\n",l->labelname);
  return aux;
 }
sprintf(auxname,"%s",name);
i=0;
while(auxname[i]!=':') i++;
auxname[i]='\0';
while(l->prox!=NULL) {
  //printf("l->labelname=%s,name=%s\n",l->labelname,auxname);
  if(!strcmp(l->labelname,auxname)) { //checa repetição
    //printf("Oia nego tentando inserir repetido soo\n\n");
    return aux;
  }
  l=l->prox;
 }
//printf("Vou ter que inserir um label novo mesmo\n");
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
return aux;
}
