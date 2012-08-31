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
  lb = l;
  if(!strcmp(l->tokenname,"head")) l=l->prox;
  while(1) {
    //printf("%s\n",l->tokenname);
    if(l->tokentype=='f') {
      printf("Atingido final do codigo\n");
      break;
    }
    if(l->tokentype=='d') {
      //printf("Tratarei a diretiva %s\n",l->tokenname);
      trata_diretiva(l,&pc,hexa);
    }
    if(l->tokentype=='b') {
      printf("Tratarei a operação %s\n",l->tokenname);
      arithmetics(l,&pc,hexa,ll);
    }
    if(l->tokentype=='l') {
      printf("Adicionando label\n");
      addlabel(l->tokenname,ll,&pc);
    }
    if(l->prox!=NULL)
      l=l->prox;
    else break;
  }
  l = lb; //daqui que voltamos a lista original
  fclose(codigo);
  fclose(hexa);
  return 0;
  
}
void trata_diretiva(listtokens *l, pcounter *pc, FILE *hexa) {
  long long int aux, tkname;
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
    aux=pc->position;
    tkname = strtol(l->tokenname,NULL,16);
    if(pc->position%tkname) 
      pc->position=pc->position+tkname-pc->position%tkname;
    printf("aux = %lld, pc->position=%d\n",aux,pc->position);
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
    printf("mudando origem de %X\n",pc->position);
    l=l->prox;
    printf("para %s\n",l->tokenname);
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
    //preciso estudar melhor a .set
  }
   if(!flag) erro(err,"Diretiva desconhecida"); 
   if(l->prox!=NULL)
      l=l->prox;
    return;
}
void arithmetics(listtokens *l, pcounter *pc, FILE *hexa,listlabels *ll) {
  label *nextl;
  int line = l->tokenline;
  int aux=0;
  nextl = malloc(sizeof(label));
  nextl->side=0;
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
    //o pau é o label dentro do m, resolve ai
    nextl = malloc(sizeof(label));
    nextl->side=0;
    nextl->position = trataM(auxtoken);
    nextl->position[3] = '\0';
  }
  if(l->tokentype == 'l')  {
    nextl = malloc(sizeof(label));
    nextl = trataL(auxtoken,pc,ll,nextl);
    printf("%s",nextl->position);
    printf("Entrei aqui, recebi nextl = %s\n",nextl->position);
    system("sleep 1");
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
  if(!aux) erro(line,token); 
  if(l->prox!=NULL) l=l->prox;
  return;
}
listlabels *addlabel(char *name, listlabels *l, pcounter *pc) {
  listlabels *aux;
  aux = l;
  printf("Recebi o token %s pra inserir com pc %d (%X)\n",
	name,pc->position,pc->position); 
  if(l==NULL) {
    l = malloc(sizeof(listlabels));
    aux = l;
    l->label.position=pc->position;
    l->label.side=pc->side;
    l->labelname=malloc(sizeof(char)*50);
    strcpy(l->labelname,name);
    return aux;
  }
  while(l->prox!=NULL) l=l->prox;
  l->prox = malloc(sizeof(listlabels));
  l=l->prox;
  l->label.position=pc->position;
  l->label.side=pc->side;
  l->labelname=malloc(sizeof(char)*50);
  strcpy(l->labelname,name);
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
       (code[i]==' ' && code[i+1]=='#') || (code[i]=='#' && code[i+1]=='0')){
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
  code2[y] = '\0';
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
  //printf("\n%s\n",code);
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
      //type = 'b';
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
    char *tmptoken,saida[4];
    tmptoken = malloc(sizeof(char)*50);
    while(in[count2]!=')') {
      tmptoken[count]=in[count2];
      count++;
      count2++;
    }
    //em temptoken temos uma constante
    //pode ser binaria, hexa, dec, octa
    //temos tratadores para todas
    tmptoken[count]='\0';
    tmptoken=trata_constante(tmptoken);
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
  printf("Erro de sintaxe proximo a linha %d\ntoken %s\n",err,desc);
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
label *trataL(char *in,pcounter *pc,listlabels *l, label *nextl) {
  printf("Recebi o label %s pra tratar\n",in);
  listlabels *aux;
  nextl->position = malloc(sizeof(char)*4);
  aux = l;
  if(aux==NULL) {
    printf("a lista de labels era fucking nula\n");
    strcpy(nextl->position,"666");
    nextl->side=1;
    return nextl;
  }
  while(1) {
    if(!strcmp(aux->labelname,in)) {
      printf("Achei o bendito, vou retornar esperto, posição %d\n",
	     aux->label.position);
      sprintf(nextl->position,"%X",aux->label.position);
      nextl->side = aux->label.side;
      return nextl;
    }
    if(aux->prox!=NULL)
      aux=aux->prox;
    else {
      strcpy(nextl->position,"666");
      nextl->side=1;
      break;
    }
  }
  return nextl;
}
