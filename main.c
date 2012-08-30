#include "main.h"
#include <ctype.h>
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
    if(l->tokentype=='d') {
      printf("Tratarei a diretiva %s\n",l->tokenname);
      trata_diretiva(l,&pc,hexa);
    }
    if(l->tokentype=='b') {
      printf("Tratarei a operação %s\n",l->tokenname);
      arithmetics(l,&pc,hexa);
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
  long long int aux;
  if(!strcmp(l->tokenname,".align")) {
    if(pc->side==1) {
      pc->position++;
      fprintf(hexa,"00000\n");
    }
    pc->side=0;
    l=l->prox;
    aux=pc->position;
    if(pc->position%atoi(l->tokenname)) 
      pc->position=pc->position+atoi(l->tokenname)-pc->position%atoi(l->tokenname);
    printf("aux = %lld, pc->position=%d\n",aux,pc->position);
    if(l->prox!=NULL)
      l=l->prox;
    return;
  }
  if(!strcmp(l->tokenname,".word")) {
    fprintf(hexa,"%03x ",pc->position);
    l=l->prox;
    fprintf(hexa,"%s\n",l->tokenname);
    pc->position++;
    pc->side=0;
    if(l->prox!=NULL)
      l=l->prox;
    return;
  }
  if(!strcmp(l->tokenname,".org")) {
    printf("mudando origem de %X\n",pc->position);
    l=l->prox;
    printf("para %s\n",l->tokenname);
    pc->position=strtoll(l->tokenname,NULL,16);
    if(l->prox!=NULL)
      l=l->prox;
    return;
  }
  if(!strcmp(l->tokenname,".wfill")) {
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
    if(l->prox!=NULL)
      l=l->prox;
    return;
  }
}
void arithmetics(listtokens *l, pcounter *pc, FILE *hexa) {
  char *next;
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
  if(l->tokentype == 'm')
    next = trataM(auxtoken);
  if(l->tokentype == 'l') 
    next = trataL(auxtoken);
  next[3] = '\0';
  printf("O next vale %s\n",next);
  if(!strcmp(token,"add")) 
    fprintf(hexa,"05%s",next); 
  if(!strcmp(token,"addmod")) 
    fprintf(hexa,"07%s",next); 
  if(!strcmp(token,"sub")) 
    fprintf(hexa,"08%s",next); 
  if(!strcmp(token,"mul")) 
    fprintf(hexa,"0B%s",next); 
  if(!strcmp(token,"div")) 
    fprintf(hexa,"0C%s",next); 
  if(!strcmp(token,"load"))
    fprintf(hexa,"0A%s",next); 
  if(!strcmp(token,"loadmqmem")) 
    fprintf(hexa,"09%s",next); 
  if(!strcmp(token,"stor")) 
    fprintf(hexa,"21%s",next); 
  if(pc->side==0) fprintf(hexa,"\n");
  return;
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
    }
    if(aux=='\n') aux = ' ';
    code[i]=aux; i++;
    if(i>=k) {
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
  while(code[i]==' ') i++;
  while(code[i+1]!='\0') {
    if(code[i]==' ' && code[i+1]==' ') {
      i++;
      continue;
    }
    code2[y]=code[i];
    i++;
    y++;
    if(y>=k) {
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
  int i=0,y=0;
  char *temptoken,*auxtoken,type;
  temptoken = malloc(sizeof(char)*50);
  auxtoken = malloc(sizeof(char)*50);
  memset(temptoken,'0',50);
  memset(auxtoken,'0',50);
  //printf("%s",code);
  while(code[i]!='\0') {
    temptoken[y]=tolower(code[i]); i++; y++;
    if(code[i]==' ' || code[i]==',' || code[i]=='\0') {
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
      y=0; i++;
      //printf("Adicionando token --%s-- a lista\n",temptoken);
      //adiciona o token a lista
      while (l->prox!=NULL) l=l->prox;
      l->prox=malloc(sizeof(listtokens));
      l=l->prox;
      l->prox=NULL;
      l->tokentype = type;
      memset(l->tokenname,'0',50);
      strcpy(l->tokenname,temptoken);
      memset(temptoken,'0',50);
    }
  }
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
    char *tmptoken,saida[3];
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
void erro(int err) {
  printf("Erro de sintaxe proximo a linha %d",err);
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
char *trataL(char *in) {
  return "000";
}
