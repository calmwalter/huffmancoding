#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef linux
#include<unistd.h>
#endif
#ifdef _WIN32
#include<windows.h>
#endif
typedef struct node
{
  char c;
  double freq;
  int left;
  int right;
  int parent;
  int pos;
} node;

int huffman(node *nodes, int size, node *tr, int tr_size)
{
  if (tr_size < 2)
    return size;
  int x, y;
  if (tr[0].freq < tr[1].freq){
    x = tr[0].pos;y = tr[1].pos;
  }else{
    y = tr[0].pos;x = tr[1].pos;
  }

  for (int i = 2; i < tr_size; i++)
    {
      if (tr[i].freq < nodes[x].freq)
        {
          y = x;
          x = tr[i].pos;
        }
      else if (tr[i].freq < nodes[y].freq)
        y = tr[i].pos;

    }
  nodes[size].left = x;
  nodes[size].right = y;
  nodes[size].parent=-1;
  nodes[x].parent=size;
  nodes[y].parent=size;
  nodes[size].pos = size;

  nodes[size].freq = nodes[x].freq + nodes[y].freq;
  nodes[size].c=' ';
  node new_tr[tr_size];
  int cnt = 0;
  for (int i = 0; i < tr_size; i++)
    if (tr[i].pos != x && tr[i].pos != y)
      new_tr[cnt++] = tr[i];
  new_tr[tr_size - 2] = nodes[size];
  int new_size=huffman(nodes, size + 1, new_tr, tr_size - 1);
  return new_size;
}

int findpos(node *nodes,int size,char c){
  for(int i=0;i<size;i++){
    if(nodes[i].c=='\0'){
      break;
    }
    if(nodes[i].c==c){
      return i;
    }
  }
  return -1;
}

void encoding_char(node *nodes,int size,char c,char *code){
  int pos=findpos(nodes,size,c);
  if(pos>=size)
    return ;
  strcpy(code,"");
  int cnt=0;
  while(pos!=-1){
    if(pos==nodes[nodes[pos].parent].left){
      *(code+cnt)='0';
      cnt++;
    }
    else if(pos==nodes[nodes[pos].parent].right){
      *(code+cnt)='1';
      cnt++;
    }
    pos=nodes[pos].parent;
  }
  *(code+cnt)='\0';
  int l=strlen(code);
  //reverse the code
  for(int i=0;i<l/2;i++){
    char tmp=*(code+i);
    *(code+i)=*(code+l-1-i);
    *(code+l-1-i)=tmp;
  }
}

void encoding_string(node *nodes,int size,char *str,char *code){
  char tmp[256];
  strcpy(code,"");
  int l=strlen(str);
  for(int i=0;i<l;i++){
    encoding_char (nodes, size, str[i], tmp);
    strcat(code,tmp);
  }
}

void decoding(node *nodes,int size,char *code,char *str){
  int l=strlen(code)+1;
  int pos=size-1;
  int cnt=0;
  strcpy(str,"");
  for(int i=0;i<l;i++){
    if(nodes[pos].left==-1 && nodes[pos].right==-1){
      str[cnt]=nodes[pos].c;
      pos=size-1;
      cnt++;
    }
    if(code[i]=='0'){
      pos=nodes[pos].left;
    }
    else if(code[i]=='1'){
      pos=nodes[pos].right;
    }
  }
  str[cnt]='\0';
}

int count_char(char *str,node *nodes){
  int l=strlen(str);
  int cnt=0;
  for(int i=0;i<l;i++){
    int flag=0;
    for(int k=0;k<=cnt;k++){
      if(nodes[k].c==str[i]){
        flag=1;
        nodes[k].freq+=1.0;
        break;
      }
    }
    //if the char is not in nodes, then create the node
    if(flag==0){
      nodes[cnt].c=str[i];
      nodes[cnt].freq=1.0;
      nodes[cnt].left=-1;
      nodes[cnt].right=-1;
      nodes[cnt].parent=-1;
      nodes[cnt].pos=cnt;
      cnt++;
    }
  }
  return cnt;
}

void print_nodes(node nodes[256],int size){
  printf("%-8s%-8s%-8s%-8s%-8s%-8s\n","node","freq","left","right","parent","char");
  for (int i = 0; i < size; i++){
    printf("node%-4d%-8.2lf", i, nodes[i].freq);
    if (nodes[i].left != -1)
      printf("%-8d", nodes[i].left);
    else
      printf("%-8s", "NULL");
    if (nodes[i].right != -1)
      printf("%-8d", nodes[i].right);
    else
      printf("%-8s", "NULL");
    if (nodes[i].parent != -1)
      printf("%-8d", nodes[i].parent);
    else
      printf("%-8s", "NULL");
    printf("%c",nodes[i].c);
    printf("\n");
  }
}

void write_huffman_to_file(node nodes[256],int size){
  FILE *wf=fopen("hfmTree.txt","w");
  for (int i = 0; i < size; i++){
    fprintf(wf,"%lf ", nodes[i].freq);
    fprintf(wf,"%d ", nodes[i].left);
    fprintf(wf,"%d ", nodes[i].right);
    fprintf(wf,"%d ", nodes[i].parent);
    if(nodes[i].left==-1 && nodes[i].right==-1){
      fprintf(wf,"%c",nodes[i].c);
    }
    fprintf(wf,"\n");
  }
  fclose (wf);
}
int read_huffman_from_file(node nodes[256]){
  FILE *wf=fopen("hfmTree.txt","r");
  int i=0;
  while(fscanf(wf,"%lf", &nodes[i].freq)!=EOF)
    {
      fscanf(wf,"%d", &nodes[i].left);
      fscanf(wf,"%d", &nodes[i].right);
      fscanf(wf,"%d", &nodes[i].parent);
      if(nodes[i].left==-1 && nodes[i].right==-1){
        fgetc (wf);
        nodes[i].c=fgetc (wf);
      }
      else{
        nodes[i].c=' ';
      }
      i++;
    }
  fclose(wf);
  return i;
}
void read_content_from_file(char *str,char *filename){
  FILE *fr=fopen(filename,"r");
  int i=0;
  while((str[i]=fgetc(fr))!=EOF){
    i++;
  }
  str[i]='\0';
  fclose(fr);
}
void write_content_to_file(char *str,char* filename){
  int l=strlen(str);
  FILE *fw=fopen(filename,"w");
  for(int i=0;i<l;i++){
    fputc(str[i],fw);
  }
  fclose(fw);
}

int main(int argc, char *argv[])
{
  //////////////////////test the first requirement////////////////////////////////
  /* //the input data */
  /* double freq[] = {0.05, 0.29, 0.07, 0.08, 0.14, 0.23, 0.03, 0.11}; */
  /* node nodes[256]; */
  /* for (int i = 0; i < 8; i++){ */
  /*     nodes[i].pos = i;nodes[i].freq = freq[i]; */
  /*     nodes[i].left = -1;nodes[i].right = -1; */
  /*     nodes[i].parent=-1; */
  /*     nodes[i].c='a'+i; */
  /* } */
  /* //build huffman tree */
  /* int size=huffman(nodes, 8, nodes, 8); */
  /* //test the huffman tree */
  /* printf("%-8s%-8s%-8s%-8s%-8s%-8s\n","node","freq","left","right","parent","char"); */
  /* for (int i = 0; i < size; i++){ */
  /*     printf("node%-4d%-8.2lf", i, nodes[i].freq); */
  /*     if (nodes[i].left != -1) */
  /*         printf("%-8d", nodes[i].left); */
  /*     else */
  /*         printf("%-8s", "NULL"); */
  /*     if (nodes[i].right != -1) */
  /*         printf("%-8d", nodes[i].right); */
  /*     else */
  /*         printf("%-8s", "NULL"); */
  /*     if (nodes[i].parent != -1) */
  /*         printf("%-8d", nodes[i].parent); */
  /*     else */
  /*         printf("%-8s", "NULL"); */
  /*     printf("%c",nodes[i].c); */
  /*     printf("\n"); */
  /* } */

  /* //encoding the infomation */
  /* // single char encoding */
  /* printf("\nsingle char encoding\n"); */
  /* for(int i=0;i<8;i++){ */
  /*     char code[256]=""; */
  /*     encoding_char (nodes, size, i,code); */
  /*     printf ("%s\n",code); */
  /* } */

  /* //string encoding */
  /* printf("\nstring encoding\n"); */
  /* char code[256]; */
  /* encoding_string (nodes, size,8,code); */
  /* printf("%s\n",code); */

  /* //decoding the infomation */
  /* // string decoding */
  /* printf("\nstring decoding\n"); */
  /* char str[256]; */
  /* decoding (nodes, size, code, str); */
  /* printf("%s\n",str); */
  ////////////////////////////////////////////////////////////////////////////////

  //////////////////////test the second requirement///////////////////////////////
  //test the second requirement
  /* char content[1024]="THIS PROGRAME IS MY FAVORITE"; */
  /* node nodes[1024]; */
  /* int size=count_char(content, nodes); */
  /* size=huffman (nodes, size, nodes, size); */
  /* char code[1024]; */
  /* encoding_string (nodes, size, content, code); */
  /* printf("%s\n",code); */
  /* char str[1024]; */
  /* decoding (nodes, size, code, str); */
  /* printf("%s\n",str); */
  ////////////////////////////////////////////////////////////////////////////////


  ///////////////////////////////UI IN COMMAND LINE///////////////////////////////
  int size;
  printf("input the size of the characters(n):");
  scanf("%d",&size);
  int cnt=0;
  node nodes[256];
  // ABCDEFGHIJKLMNOPQRSTUVWXYZ
  printf("please input n characters:");
  getchar();
  while(cnt<size){
    nodes[cnt].c=getchar();
    nodes[cnt].left=-1;
    nodes[cnt].right=-1;
    nodes[cnt].parent=-1;
    nodes[cnt].pos=cnt;
    cnt++;
  }
  cnt=0;

  //186 64 13 22 32 103 21 15 47 57 1 5 32 20 57 63 15 1 48 51 80 23 8 18 1 16 1
  printf("please input n weight of the characters:");
  while(cnt<size){
    scanf("%lf",&nodes[cnt].freq);
    cnt++;
  }
  //build huffman tree
  size=huffman(nodes,size,nodes,size);
  //write huffman tree to file
  write_huffman_to_file(nodes,size);
  char code[1024];
  char content[1024]="THIS PROGRAME IS MY FAVORITE";
  encoding_string (nodes, size, content, code);
  printf("%s\n",code);
  char str[1024];
  decoding (nodes, size, code, str);
  printf("%s\n",str);


  print_nodes(nodes,size);


  node new_nodes[256];
  int x=read_huffman_from_file (new_nodes);
  //printf("\nx=%d\n",x);
  //print_nodes (new_nodes, x);

  ///get string
  char new_str[256];
  read_content_from_file(new_str,"ToBeTra.txt");

  //encoding
  char new_code[1024];
  encoding_string(new_nodes, x, new_str, new_code);
  write_content_to_file(new_code, "CodeFile.txt");

  //read code from file
  char codes[1024];
  read_content_from_file(codes,"CodeFile.txt");

  //put decoded content to file
  char strs[256];
  decoding(new_nodes, x, codes, strs);
  write_content_to_file(strs, "Textfile.txt");
  system("python3 tree.py");
  return 0;
}
