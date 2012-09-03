#include "main.h"
int main(int argc, char *argv[]) {
  if(argc!=3) {
    printf("Entrada Invalida\n");
    return -1;
  }
  FILE *codigo;
  FILE *hexa;
  hexa=fopen(argv[2],"w+");
  char *code = NULL;
  listtokens *l = NULL,*lb = NULL;
  listlabels *ll = NULL;
  pcounter pc;
  pc.position=0;
  pc.side=0;
  codigo=fopen(argv[1],"r+");
  if(codigo==NULL) {
    printf("Arquivo de entrada não existe\n");
    return -2;
  }
  code = remove_coments(codigo,code);
  l = tokenizer(code,l,pc);
  //printlist(l);
  if(!strcmp(l->tokenname,"head")) l=l->prox;
  lb = l;
  while(1) {
    //printf("tokenname,%s tokentype %c\n",l->tokenname,l->tokentype);
    if(l->tokentype=='f') {
      if(pc.side==1) fprintf(hexa,"00000\n");
      //printf("Atingido final do codigo\n");
      break;
    }
    if(l->tokentype=='d') {
      //printf("Tratarei a diretiva %s\n",l->tokenname);
      l=trata_diretiva(l,&pc,hexa,ll);
    } 
    if(l->tokentype=='b') {
      //printf("Tratarei a operação %s\n",l->tokenname);
      arithmetics(l,&pc,hexa,ll);
    }
    if(l->tokentype=='l') {
      //printf("Adicionando label\n");
      ll = addlabel(l->tokenname,ll,&pc);
    }
    if(l->prox!=NULL)
      l=l->prox;
    else break;
  }
  /* printf("--------------------------------------\n"); */
  /* printlables(ll); */
  /* printf("--------------------------------------\n"); */
  l = lb; //daqui que voltamos a lista original
  rewind(hexa); //vamos re-escrever
  pc.side=0;
  pc.position=0; //pc reset
  while(1) {
    //printf("%s\n",l->tokenname);
    if(l->tokentype=='f') {
      if(pc.side==1) fprintf(hexa,"00000");
      //printf("Atingido final do codigo\n");
      break;
    } 
    if(l->tokentype=='d') {
      //printf("Tratarei a diretiva %s\n",l->tokenname);
      l=trata_diretiva(l,&pc,hexa,ll);
    }
      
    if(l->tokentype=='b') {
      //printf("Tratarei a operação1 %s\n",l->tokenname);
      arithmetics(l,&pc,hexa,ll);
    } 
    if(l->tokentype=='l') {
      //printf("Adicionando label\n");
      ll = addlabel(l->tokenname,ll,&pc);
    } 
    if(l->prox!=NULL)
      l=l->prox;
    else break;
  }
  fclose(codigo);
  fclose(hexa);
  return 0;
  
}
listtokens *trata_diretiva(listtokens *l, pcounter *pc,
			   FILE *hexa, listlabels *ll) {
  long long int tkname;
  listtokens *ret = l;
  //int aux;
  int err,flag = 0;
  err=l->tokenline;
  if(!strcmp(l->tokenname,".align")) {
    flag = 1;
    if(pc->side==1) {
      pc->position++;
      fprintf(hexa,"00000\n");
    }
    pc->side=0;
    l=l->prox;
    //aux=pc->position;
    tkname = strtol(l->tokenname,NULL,16);
    if(pc->position%tkname) 
      pc->position=pc->position+tkname-pc->position%tkname;
    //printf("aux = %lld, pc->position=%d\n",aux,pc->position);
  }
  if(!strcmp(l->tokenname,".word")) {
    if(pc->side==1) 
      erro(err,".word desalinhado");
    fprintf(hexa,"%03X ",pc->position);
    l=l->prox;
    if(l->tokentype=='b') {
      label *nextl = malloc(sizeof(label));
      nextl = trataL(l->tokenname,pc,ll,nextl);
      if(nextl->side==3) {
	char *invalidop = malloc(sizeof(char)*100);
	sprintf(invalidop,"Erro desconhecido \"%s\"\n",l->tokenname);
	erro(l->tokenline,invalidop);
      }
      fprintf(hexa,"%010llX\n",strtoll(nextl->position,NULL,16));
      l=l->prox;
    }
    else
      fprintf(hexa,"%s\n",l->tokenname);
    pc->position++;
    pc->side=0;
    return l;
  }
  if(!strcmp(l->tokenname,".org")) {
    flag = 1;
    //printf("mudando origem de %X\n",pc->position);
    l=l->prox;
    //printf("para %s\n",l->tokenname);
    if(pc->side==1) {
      fprintf(hexa,"00000\n");
      pc->side = 0;
    }
    pc->position=strtoll(l->tokenname,NULL,16);
  }
  if(!strcmp(l->tokenname,".wfill")) {
    if(pc->side==1) 
      erro(err,".wfill desalinhado");
    l=l->prox;
    int linhas = strtoll(l->tokenname,NULL,16);
    l=l->prox;
    //como funciona isso com label? ele pega o label e poe como palavra ou
    //printa a palavra referenciada pelo label?
    int count = 0;
    label *nextl = malloc(sizeof(label));
    if(l->tokentype=='b') {
      printf("No wfill com LABEL\n");
      nextl = trataL(l->tokenname,pc,ll,nextl);
      if(nextl->side==3) {
	char *invalidop = malloc(sizeof(char)*100);
	sprintf(invalidop,"Erro desconhecido \"%s\"\n",l->tokenname);
	erro(l->tokenline,invalidop);
      }
    }
    else
      nextl->position = l->tokenname;
    for(count=0;count<linhas;count++) {
      fprintf(hexa,"%03X ",pc->position);
      fprintf(hexa,"%010llX\n",strtoll(nextl->position,NULL,16));
      pc->position++;
      pc->side=0;  
    }
    //l=l->prox;
      return l;
  }
  if(!strcmp(l->tokenname,".set")) {
    flag = 1;
    l=l->prox; 
    char setin[50],setin2[52];
    char setout[50],setout2[52];
    int i=0,y=0;
    strcpy(setin,l->tokenname); //um nome
    l=l->prox;
    strcpy(setout,l->tokenname); //e uma constante
    listtokens *aux;
    aux = l;
    sprintf(setin2,"m(%s)",setin);
    sprintf(setout2,"m(%s)",setout);
    while(1) {
      if(!strcmp(aux->tokenname,setin)) {
	strcpy(aux->tokenname,setout);
	aux->tokentype='c';
	i++;
      }
      if(!strcmp(aux->tokenname,setin2)) {
	while(setout2[y]!='\0') y++;
	sprintf(aux->tokenname,"m(%c%c%c)",
		setout2[y-4],setout2[y-3],setout2[y-2]);
	//printf("\n\n%c%c%c\n\n",setout2[y-4],setout2[y-3],setout2[y-2]);
	aux->tokentype='m';
	i++;
      }
      //printf("aux->tokename=%s\n",aux->tokenname);
      if(aux->prox!=NULL) aux=aux->prox;
      else break;
    }
    //printf("Troquei %d coisas\n",i);
    return l;
  }
  if(!flag) erro(err,"Diretiva desconhecida"); 
  if(l->prox!=NULL)
    l=l->prox;
  return ret;
}
void arithmetics(listtokens *l, pcounter *pc, FILE *hexa,listlabels *ll) {
  label *nextl;
  int line = l->tokenline;
  int aux=0;
  nextl = malloc(sizeof(label));
  nextl->side=0;
  nextl->position=malloc(sizeof(char)*4);
  strcpy(nextl->position,"500");
  char token[50],auxtoken[50];
  strcpy(token,l->tokenname);
  if(pc->side==0) {
    pc->side=1;
    fprintf(hexa,"%03X ",pc->position);
  }
  else {
    pc->side=0;
    pc->position++;
  }
  //Aqui as que não tem parametro
  if(!strcmp(token,"loadmq")) { 
    fprintf(hexa,"0A000"); 
    if(pc->side==0) fprintf(hexa,"\n"); //ao contrario porque o pc
    return;                             //muda antes
  }
  if(!strcmp(token,"lsh")) { 
    fprintf(hexa,"14000");
    if(pc->side==0) fprintf(hexa,"\n");
    return;
  }
  if(!strcmp(token,"rsh")) { 
    fprintf(hexa,"15000");
    if(pc->side==0) fprintf(hexa,"\n"); 
    return;  
  }
  l=l->prox;
  strcpy(auxtoken,l->tokenname);
  if(l->tokentype == 'm') {
    nextl = malloc(sizeof(label));
    nextl->side=0;
    nextl->position = trataM(auxtoken);
    nextl->position[3] = '\0';
  }
  if(!strcmp(nextl->position,"500")) {
    nextl=trataL(l->tokenname,pc,ll,nextl);
  }
  if(nextl->side==3) {
    char *invalidop = malloc(sizeof(char)*100);
    sprintf(invalidop,"Operação Invalida %s\n",token);
    erro(line,invalidop);
  }
  //printf("O next vale %s\n",nextl->position);
  //os que tem um parametro
  if(!strcmp(token,"add")) { 
    fprintf(hexa,"05%s",nextl->position); aux=1;  }
  if(!strcmp(token,"sub")) { 
    fprintf(hexa,"06%s",nextl->position); aux=1;  }
  if(!strcmp(token,"addmod")) { 
    fprintf(hexa,"07%s",nextl->position); aux=1;  }
  if(!strcmp(token,"submod")) { 
    fprintf(hexa,"08%s",nextl->position); aux=1;  }
  if(!strcmp(token,"mul")) { 
    fprintf(hexa,"0B%s",nextl->position); aux=1;  }
  if(!strcmp(token,"div")) { 
    fprintf(hexa,"0C%s",nextl->position); aux=1;  }
  if(!strcmp(token,"load")) {
    fprintf(hexa,"01%s",nextl->position); aux=1;  }
  if(!strcmp(token,"loadmqmem")) { 
    fprintf(hexa,"09%s",nextl->position); aux=1;  }
  if(!strcmp(token,"stor")) { 
    fprintf(hexa,"21%s",nextl->position); aux=1;  }
  if(!strcmp(token,"loadneg")) { 
    fprintf(hexa,"02%s",nextl->position); aux=1;  }
  if(!strcmp(token,"loadmod")) { 
    fprintf(hexa,"03%s",nextl->position); aux=1;  }
  //se não for nenhuma dessas, então é das chatas
  if(!strcmp(token,"jump")) {
    aux=1;
    if(nextl->side==0)
      fprintf(hexa,"0D%s",nextl->position);
    else
      fprintf(hexa,"0E%s",nextl->position);
  }
  if(!strcmp(token,"jumppos")) {
    aux=1;
    if(nextl->side==0)
      fprintf(hexa,"0F%s",nextl->position);
    else
      fprintf(hexa,"10%s",nextl->position);
  }
  if(!strcmp(token,"storaddr")) {
    aux=1;
    if(nextl->side==0)
      fprintf(hexa,"12%s",nextl->position);
    else
      fprintf(hexa,"13%s",nextl->position);
  }
  if(pc->side==0) fprintf(hexa,"\n");
  //ou é erro
  if(!aux) erro(line,"Operação Desconhecida"); 
  if(l->prox!=NULL) l=l->prox;
  return;
}
char *trataM(char *in) {
  if(in[0]=='m' && in[1]=='(') {
    int count=0,count2=2;
    char *tmptoken,*auxtoken,saida[4];
    tmptoken = malloc(sizeof(char)*50);
    auxtoken = malloc(sizeof(char)*50);
    while(in[count2]!=')') {
      tmptoken[count]=in[count2];
      count++;
      count2++;
    }
    //em temptoken temos uma constante
    //pode ser binaria, hexa, dec, octa, label
    //temos tratadores para todas
    tmptoken[count]='\0';
    strcpy(auxtoken,tmptoken);
    tmptoken=trata_constante(tmptoken);
    if(!strcmp(auxtoken,tmptoken)) {
      strcpy(tmptoken,"500");
      return tmptoken;
    }
    count=0;
    while(tmptoken[count]!='\0') {
      count++;
    }
    count--;
    sprintf(saida,"%c%c%c",tmptoken[count-2],tmptoken[count-1],tmptoken[count]);
    memset(in,'0',50);
    strcpy(in,saida);
    free(tmptoken);
    return in;
  } 
  //else erro no codigo
  return NULL;
}
void erro(int err, char *desc) {
  if(err<0) 
    printf("Erro de base %d\n%s\n",err,desc);
  else
    printf("Erro de sintaxe proximo a linha %d\n%s\n",err,desc);
  exit(1);
}


label *trataL(char *in,pcounter *pc,listlabels *ll,label *nextl) {
  //printf("Recebi o label %s pra tratar\n",in);
  char *labelname;
  labelname=malloc(sizeof(char)*50);
  int i=2,y=0;
  if(in[0]=='m' && in[1]=='(') {
  while(in[i]!=')') {
    if(in[i]=='\0') {
      nextl->side=3;
      return nextl;
    }
    labelname[y]=in[i];
    i++;
    y++;
  }
  labelname[y]='\0';
  //printf("Tirado o m() = %s\n",labelname);
  }
  else strcpy(labelname,in);
  listlabels *aux;
  nextl->position = malloc(sizeof(char)*4);
  aux = ll;
  if(aux==NULL) {
    //printf("a lista de labels era fucking nula\n");
    strcpy(nextl->position,"500");
    nextl->side=pc->side;
    return nextl;
  }
  while(1) {
    if(!strcmp(aux->labelname,labelname)) {
      //printf("Achei o label, posição %d\n",aux->label.position);
      sprintf(nextl->position,"%03X",aux->label.position);
      nextl->side = aux->label.side;
      return nextl;
    }
    //else printf("ainda não aux->labelname=%s, labelname=%s\n",
    //aux->labelname,labelname);
    if(aux->prox!=NULL)
      aux=aux->prox;
    else {
      strcpy(nextl->position,"500");
      nextl->side=1;
      //printf("Não foi possivel resolver label\n");
      break;
    }
  }
  return nextl;
}
void printlist (listtokens *l) {
  listtokens *aux;
  aux = l;
  if(aux==NULL) return;
  while(aux->prox!=NULL) {
    printf("Conteudo da lista ->\n    List->tokenname=%s\n  List->tokentype=%c\n List->tokenline=%d\n",
    aux->tokenname,aux->tokentype,aux->tokenline);
    if(aux->prox!=NULL) aux=aux->prox;
    else break;
  }
}
void printlables (listlabels *l) {
  listlabels *aux;
  aux = l;
  if(aux==NULL) return;
  while(1) {
    printf("List->labelname=%s labelpos=%X labelside=%d\n",
	   aux->labelname,aux->label.position,aux->label.side);
    if(aux->prox!=NULL) aux=aux->prox;
    else break;
  }
}
