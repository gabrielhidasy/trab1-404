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
    printlist(l);
    //printf("%s\n",l->tokenname);
    if(l->tokentype=='f') {
      if(pc.side==1) fprintf(hexa,"00000");
      //printf("Atingido final do codigo\n");
      break;
    }
    if(l->tokentype=='d') {
      //printf("Tratarei a diretiva %s\n",l->tokenname);
      l=trata_diretiva(l,&pc,hexa);
    } 
    if(l->tokentype=='b') {
      printf("Tratarei a operação %s\n",l->tokenname);
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
  printf("--------------------------------------\n");
  printlables(ll);
  printf("--------------------------------------\n");
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
      l=trata_diretiva(l,&pc,hexa);
    }
      
    if(l->tokentype=='b') {
      printf("Tratarei a operação1 %s\n",l->tokenname);
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
listtokens *trata_diretiva(listtokens *l, pcounter *pc, FILE *hexa) {
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
    flag = 1;
    if(pc->side==1) 
      erro(err,".word desalinhado");
    fprintf(hexa,"%03x ",pc->position);
    l=l->prox;
    fprintf(hexa,"%s\n",l->tokenname);
    pc->position++;
    pc->side=0;
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
    flag = 1;
    if(pc->side==1) 
      erro(err,".wfill desalinhado");
    l=l->prox;
    int linhas = strtoll(l->tokenname,NULL,16);
    l=l->prox;
    //como funciona isso com label? ele pega o label e poe como palavra ou
    //printa a palavra referenciada pelo label?
    int count = 0;
    for(count=0;count<linhas;count++) {
      fprintf(hexa,"%03x ",pc->position);
      fprintf(hexa,"%s\n",l->tokenname);
      pc->position++;
      pc->side=0;
    }
  }
  if(!strcmp(l->tokenname,".set")) {
    flag = 1;
    l=l->prox; 
    char setin[50];
    char setout[50];
    int i=0;
    strcpy(setin,l->tokenname); //um nome
    l=l->prox;
    strcpy(setout,l->tokenname); //é uma constante
    listtokens *aux;
    printf("aqui na .set vamos trocar todas as %s por %s\n",setin,setout);
    aux = l;
    while(1) {
      if(!strcmp(aux->tokenname,setin)) {
	strcpy(aux->tokenname,setout);
	aux->tokentype='c';
	i++;
      }
      if(aux->prox!=NULL) aux=aux->prox;
      else break;
    }
    printf("Troquei %d coisas\n",i);
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
  strcpy(nextl->position,"666");
  char token[50],auxtoken[50];
  strcpy(token,l->tokenname);
  if(pc->side==0) {
    pc->side=1;
    fprintf(hexa,"%03x ",pc->position);
  }
  else {
    pc->side=0;
    pc->position++;
  }
  l=l->prox;
  strcpy(auxtoken,l->tokenname);
  if(l->tokentype == 'm') {
    nextl = malloc(sizeof(label));
    nextl->side=0;
    nextl->position = trataM(auxtoken);
    nextl->position[3] = '\0';
  }
  if(!strcmp(nextl->position,"666")) {
    nextl=trataL(l->tokenname,pc,ll,nextl);
  }
  if(nextl->side==3) {
    char *invalidop = malloc(sizeof(char)*100);
    sprintf(invalidop,"Operação Invalida %s\n",token);
    erro(line,invalidop);
  }
  //printf("O next vale %s\n",nextl->position);
  if(!strcmp(token,"add")) { 
    fprintf(hexa,"05%s",nextl->position); aux=1;  }
  if(!strcmp(token,"addmod")) { 
    fprintf(hexa,"07%s",nextl->position); aux=1;  }
  if(!strcmp(token,"sub")) { 
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
  if(!strcmp(token,"loadmq")) { 
    fprintf(hexa,"0A%s",nextl->position); aux=1;  }
  if(!strcmp(token,"lsh")) { 
    fprintf(hexa,"14%s",nextl->position); aux=1;  }
  if(!strcmp(token,"rsh")) { 
    fprintf(hexa,"15%s",nextl->position); aux=1;  }
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
char *remove_coments(FILE *cod, char *code) { //OK
  int k=100;
  char aux = 0;
  int i=0;
  code = malloc(sizeof(char)*k);
  memset(code,'0',k);
  while(!feof(cod)) {
    fscanf(cod,"%c",&aux);
    if(aux=='@') {
      while(aux!='\n') {
	fscanf(cod,"%c",&aux);
      } 
      code[i]='!'; i++;
    } 
    if(aux=='\n') aux = '#';
    code[i]=aux; i++;
    if(i>=k-10) {
      k = k*2;
      code = realloc(code,sizeof(char)*k);
    }
  }
  code[i]='\0';
  code = remove_double_spaces(code);
  return code;
}       
char *remove_double_spaces(char *code) {
  //Remove os espaços duplos e espaços a mais no inicio do arquivo
  int i=0,k=100,y=0;
  char *code2;
  code2 = malloc(sizeof(char)*k);
  memset(code2,'0',k);
  while(code[i]==' ' || code[i]=='#') i++; 
  while(code[i+1]!='\0') {
    if((code[i]==' ' && code[i+1]==' ') || (code[i]=='#' && code[i+1]=='#') ||
       (code[i]==' ' && code[i+1]=='\0') || (code[i]=='#' && code[i+1]=='\0') ||
       (code[i]==' ' && code[i+1]=='#') || (code[i]=='#' && code[i+1]==' ')) {
      i++;
      continue;
    }
    code2[y]=code[i];
    i++;
    y++;
    if(y>=k-10) {
      k=2*k;
      code2 = realloc(code2,sizeof(char)*k);
    }
  }
  code2[y] = ' '; y++;
  code2[y] = '$'; y++;
  code2[y]='\0'; //marca o final
  memset(code,'0',i);
  strcpy(code,code2);
  free(code2);
  return code;
}
listtokens *tokenizer(char *code, listtokens *l, pcounter pc) {
  listtokens *auxlist;
  l = malloc(sizeof(listtokens));
  strcpy(l->tokenname,"head");
  l->prox=NULL;
  auxlist = l;
  int i=0,y=0,line=1;
  char *temptoken,*auxtoken,type;
  temptoken = malloc(sizeof(char)*50);
  auxtoken = malloc(sizeof(char)*50);
  memset(temptoken,'0',50);
  memset(auxtoken,'0',50);
  printf("\n%s\n",code);
  while(code[i]!='\0') {
    temptoken[y]=tolower(code[i]); i++; y++;
    if(code[i]==' ' || code[i]==',' || code[i]=='\0' || code[i]=='#') {
      if(code[i]=='#') {
	line++;
      }
      temptoken[y] = '\0';
      strcpy(auxtoken,temptoken);
      temptoken = trata_constante(temptoken); //constantes em hexa
      type = 'b'; //tipo aleatorio
      if(strcmp(temptoken,auxtoken))
	type='c'; //constante
      if(temptoken[y-1]==':')
	type = 'l'; //label
      if(temptoken[0]=='m' && temptoken[1]=='(')
	type = 'm';
      if(temptoken[0]=='.')
	type = 'd'; //diretiva
      if(!strcmp(temptoken,"!")) {
	type = 'z'; //comentario
      }
      if(!strcmp(temptoken,"$")) {
	type = 'f'; //final do arquivo
	break;
      }
      y=0; i++;
      //printf("Adicionando token --%s-- a lista\n",temptoken);
      //adiciona o token a lista
      while (l->prox!=NULL) l=l->prox;
      l->prox=malloc(sizeof(listtokens));
      l=l->prox;
      l->prox=NULL;
      l->tokentype = type;
      l->tokenline = line;
      memset(l->tokenname,'0',50);
      strcpy(l->tokenname,temptoken);
      memset(temptoken,'0',50);
    }
  }
  //inserir token de finalização
  while (l->prox!=NULL) l=l->prox;
  l->prox=malloc(sizeof(listtokens));
  l=l->prox;
  l->prox=NULL;
  l->tokentype = 'f';
  l->tokenline = line+1;
  memset(l->tokenname,'0',50);
  strcpy(l->tokenname,"FINAL");
  memset(temptoken,'0',50);
  free(auxtoken);
  return auxlist;
}

char *trata_constante(char *temptoken) {
  int aux;
  if(temptoken[0]=='-') aux = 1;
  else aux = 0;
  if(temptoken[aux]=='0' || temptoken[aux]=='1' || temptoken[aux]=='2' ||
     temptoken[aux]=='3' || temptoken[aux]=='4' || temptoken[aux]=='5' ||
     temptoken[aux]=='6' || temptoken[aux]=='7' || temptoken[aux]=='8' ||
     temptoken[aux]=='9') {
    if(temptoken[aux+1]!='b' && temptoken[aux+1]!='x' 
       && temptoken[aux+1]!='o') {
      //esse pequeno if acha os decimais
      //printf("entre nesse megaif com temptoken = %s\n",temptoken);
      sprintf(temptoken,"%010X",atoi(temptoken));
      //printf("Saindo com temptoken = %s\n",temptoken);
    }
  }
  if(temptoken[aux]=='0' && temptoken[aux+1]=='o') { 
    //trata octal pra hexa
    //binary e dec pra string
    temptoken=trata0o(temptoken); //hexa sem 0x
    //printf("out %s\n",temptoken);
  }
  if(temptoken[aux]=='0' && temptoken[aux+1]=='x') {
    temptoken=trata0x(temptoken);
    //printf("out %s\n",temptoken);
  }
  if(temptoken[aux]=='0' && temptoken[aux+1]=='b') {
    temptoken=trata0b(temptoken);
    //printf("out %s\n",temptoken);
  }
  return temptoken;
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
      strcpy(tmptoken,"666");
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
  printf("Erro de sintaxe proximo a linha %d\n%s\n",err,desc);
  exit(1);
}

char *trata0x(char *in) {
  int count=0,count2=0;
  char palavra[11];
  memset(palavra,'0',10);
  while(in[count]!='\0') {
    count++;
  } 
  count--;
  //printf("o magnifico count=%d\n",count);
  for(count2=0;count2<count-1;count2++)
    palavra[9-count2]=in[count-count2];
  palavra[10]='\0';
  strcpy(in,palavra);
  return in;
}
char *trata0o(char *in) {
  int count=0,count2=0;
  char palavra[15];
  long long int octal;
  memset(palavra,'0',15);
  while(in[count]!='\0') {
    count++;
  } 
  count--;
  for(count2=0;count2<count-1;count2++) //o -1 mata o 'o'
    palavra[13-count2]=in[count-count2];
  palavra[14]='\0';
  octal=strtoll(palavra,NULL,8);
  sprintf(palavra,"%010llX",octal);
  strcpy(in,palavra);
  //printf("Recebi %s, fiz o octal %lld, saiu o hexa %s\n",in,octal,palavra);
  return in;
}
char *trata0b(char *in) {
  int count=0,count2=0;
  char palavra[50];
  long long int binary;
  memset(palavra,'0',50);
  while(in[count]!='\0') {
    count++;
  } 
  count--;
  for(count2=0;count2<count-1;count2++) //o -1 mata o 'o'
    palavra[48-count2]=in[count-count2];
  palavra[49]='\0';
  binary=strtoll(palavra,NULL,2);
  sprintf(palavra,"%010llX",binary);
  //printf("%s palavra, %lld binary\n",palavra,binary);
  strcpy(in,palavra);
  return in;
}
label *trataL(char *in,pcounter *pc,listlabels *ll,label *nextl) {
  printf("Recebi o label %s pra tratar\n",in);
  char *labelname;
  labelname=malloc(sizeof(char)*50);
  int i=2,y=0;
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
  printf("Tirado o m() = %s\n",labelname);
  listlabels *aux;
  nextl->position = malloc(sizeof(char)*4);
  aux = ll;
  if(aux==NULL) {
    printf("a lista de labels era fucking nula\n");
    strcpy(nextl->position,"666");
    nextl->side=pc->side;
    return nextl;
  }
  while(1) {
    if(!strcmp(aux->labelname,labelname)) {
      printf("Achei o label, posição %d\n",aux->label.position);
      sprintf(nextl->position,"%03X",aux->label.position);
      nextl->side = aux->label.side;
      return nextl;
    }
    else printf("ainda não aux->labelname=%s, labelname=%s\n",
		aux->labelname,labelname);
    if(aux->prox!=NULL)
      aux=aux->prox;
    else {
      strcpy(nextl->position,"666");
      nextl->side=1;
      printf("Não foi possivel resolver label\n");
      break;
    }
  }
  return nextl;
}
void printlist (listtokens *l) {
  listtokens *aux;
  aux = l;
  while(aux->prox!=NULL) {
    printf("Conteudo da lista ->\n    List->tokenname=%s\n    List->tokentype=%c\n    List->tokenline=%d\n",aux->tokenname,aux->tokentype,aux->tokenline);
    if(aux->prox!=NULL) aux=aux->prox;
    else break;
  }
}
void printlables (listlabels *l) {
  listlabels *aux;
  aux = l;
  while(1) {
    printf("List->labelname=%s\n",aux->labelname);
    if(aux->prox!=NULL) aux=aux->prox;
    else break;
  }
}
