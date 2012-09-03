#include "main.h"
char *remove_coments(FILE *cod, char *code) { //OK
  int k=100;
  char aux = 0;
  int i=0;
  code = malloc(sizeof(char)*k);
  memset(code,'0',k);
  while(!feof(cod)) {
    fscanf(cod,"%c",&aux);
    if(aux==',' || aux=='\t') aux=' ';
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
  int i=0,y=0,z=0,line=1;
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
      if(temptoken[0]==' ') {
	while(temptoken[z]!='\0') {
	  temptoken[z]=temptoken[z+1];
	  z++;
	}
	z=0;
      }
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
  free(temptoken);
  return auxlist;
}

char *trata_constante(char *temptoken) {
  int aux;
  char temptoken2[50];
  if(temptoken[0]=='-') aux = 1;
  else aux = 0;
  if(temptoken[aux]=='0' || temptoken[aux]=='1' || temptoken[aux]=='2' ||
     temptoken[aux]=='3' || temptoken[aux]=='4' || temptoken[aux]=='5' ||
     temptoken[aux]=='6' || temptoken[aux]=='7' || temptoken[aux]=='8' ||
     temptoken[aux]=='9') {
    if(temptoken[aux+1]!='b' && temptoken[aux+1]!='x' 
       && temptoken[aux+1]!='o') {
      //esse pequeno if acha os base 10
      sprintf(temptoken2,"%016llX",strtoll(temptoken,NULL,10));
      sprintf(temptoken,"%s",temptoken2+6);
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
    palavra[9-count2]=toupper(in[count-count2]);
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
  int count=0,count2=0,i=0;
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
  while(palavra[i]!='\0') {
    if(palavra[i]!='1' && palavra[i]!='0') erro(-1,"Binary not 0/1");
    i++;
  }
  binary=strtoll(palavra,NULL,2);
  sprintf(palavra,"%010llX",binary);
  //printf("%s palavra, %lld binary\n",palavra,binary);
  strcpy(in,palavra);
  return in;
}
char *trataM(char *in) {
  if(in[0]=='m' && in[1]=='(') { //é um if redundante
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
      strcpy(tmptoken,"500"); //significa que é label
      free(auxtoken);
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
    free(auxtoken);
    return in;
  } 
  return NULL;
}
label *trataL(char *in,pcounter *pc,listlabels *ll,label *nextl) {
  //printf("Recebi o label %s pra tratar\n",in);
  char *labelname;
  labelname=malloc(sizeof(char)*50);
  int i=2,y=0;
  if(in[0]=='m' && in[1]=='(') {
    //tira o m( e )
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
  }
  else strcpy(labelname,in);
  listlabels *aux;
  nextl->position = malloc(sizeof(char)*4);
  aux = ll;
  if(aux==NULL) {
    strcpy(nextl->position,"500");
    nextl->side=pc->side;
    free(labelname);
    return nextl;
  }
  while(1) {
    if(!strcmp(aux->labelname,labelname)) {
      sprintf(nextl->position,"%03X",aux->label.position);
      nextl->side = aux->label.side;
      free(labelname);
      return nextl;
    }
    if(aux->prox!=NULL)
      aux=aux->prox;
    else {
      strcpy(nextl->position,"500");
      nextl->side=1;
      break;
    }
  }
  free(labelname);
  return nextl;
}
