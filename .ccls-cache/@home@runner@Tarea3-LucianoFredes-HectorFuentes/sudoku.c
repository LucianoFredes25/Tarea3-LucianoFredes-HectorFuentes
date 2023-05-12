#include <stdio.h>
#include <stdlib.h>
#include "list.h"


typedef struct{
   int sudo[9][9];
}Node;

Node* createNode(){
  Node* n=(Node*) malloc(sizeof(Node));
  return n;
}

Node* copy(Node* n){
    Node* new=(Node*) malloc(sizeof(Node));
    *new = *n;
    return new;
}

Node* read_file (char* file_name){
  Node* n = createNode();
  FILE* file = fopen (file_name, "r");
  int i,j;
  for(i=0;i<9;i++){
       for(j=0;j<9;j++){
          if(!fscanf (file, "%d", &n->sudo[i][j]))
            printf("failed to read data!");
       }
  }

  fclose (file);
  return n;
}

void print_node(Node* n){
    int i,j;
    for(i=0;i<9;i++){
       for(j=0;j<9;j++)
          printf("%d ", n->sudo[i][j]);
       printf("\n");
    }
    printf("\n");
}

int is_valid(Node* n){
  int numero ;
  int sub , contX = 0;
    
    for(int i = 0 ; i < 9 ; i++)
    {
      sub = 0;
      if(i > 2) sub += 3;
      if(i > 5) sub += 3;

      for(int j = 0 ; j < 9 ; j++)
      {
        contX++;
        
        if(n->sudo[i][j] != 0)
        {
          numero = n->sudo[i][j];
          for(int k = 0 ; k < 9 ; k++)
          {

            int auxX = 3*(sub/3) + (k/3);
            int auxY = 3*(sub%3) + (k%3);
            
            if(n->sudo[auxX][auxY] == numero && auxX != i && auxY != j) return 0;
            
            if(n->sudo[i][k] == numero && k != j) return 0;
            if(n->sudo[k][j] == numero && k != i) return 0;
          }
        }
        if(contX > 2)
        {
          contX = 0;
          sub++;
        }
      }
    }
  return 1;
}


List* get_adj_nodes(Node* n){
    List* list=createList();
    Node * aux = createNode();
    for(int i = 0 ; i < 9 ; i++)
    {
      for(int j = 0 ; j < 9 ; j++)
      {
        if(n->sudo[i][j] == 0)
        {
          for(int cont = 1 ; cont < 10 ; cont++){
            n->sudo[i][j] = cont;
            aux = copy(n);

            if(is_valid(aux))
              pushBack(list, aux);
          }
          return list;
        }
      }
    }
  return list;
}

int is_final(Node* n){
  for(int i = 0 ; i < 9 ; i++)
    for(int j = 0 ; j < 9 ; j++)
      if(n->sudo[i][j] == 0) return 0;
  return 1;
}

Node* DFS(Node* initial, int* cont){
  Stack * snack = createStack();
  push(snack , initial);
  Node * nodoAux = createNode();
  *cont = 0;
  while(is_empty(snack) != 1)
    {
      *cont += 1;
      nodoAux = copy(first(snack));
      popFront(snack);
      
      if(is_final(nodoAux)){
        return nodoAux;
      }
        
      else
      {
        List * listaAdj = get_adj_nodes(nodoAux);
        int size = get_size(listaAdj);
        for(int i = 0 ; i < size ; i++)
          {
            push(snack, first(listaAdj));
            popFront(listaAdj);
          }
        free(nodoAux);
        
      }
    }  
  return NULL;
}



/*
int main( int argc, char *argv[] ){

  Node* initial= read_file("s12a.txt");;

  int cont=0;
  Node* final = DFS(initial, &cont);
  printf("iterations:%d\n",cont);
  print_node(final);

  return 0;
}*/