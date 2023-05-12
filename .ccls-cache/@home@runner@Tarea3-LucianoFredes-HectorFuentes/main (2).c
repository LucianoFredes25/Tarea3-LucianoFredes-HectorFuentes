#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "heap.h"

typedef struct Vaca{
  char nombre[30];
}Vaca;

struct Vaca* CreateVaca(char nombre[30]){

  struct Vaca* newVaca = calloc(1, sizeof(struct Vaca));
  strcpy(newVaca->nombre, nombre);
  return newVaca;
}


int main(void) {
  
  Heap* heapNose = CreateHeap(1024, 0);

  Vaca* vaca1 = CreateVaca("jose");
  Vaca* vaca2 = CreateVaca("juan");
  Vaca* vaca3 = CreateVaca("nose");

  Node nodito1 = CreateNode(24, vaca1);
  Node nodito2 = CreateNode(34, vaca2);
  Node nodito3 = CreateNode(10, vaca3);

  Insert(heapNose, nodito1);

  //Resultado esperado 24
  printf("%d \n", GetMax(heapNose).key);

  Insert(heapNose, nodito2);

  //Resultado esperado 34
  printf("%d \n", GetMax(heapNose).key);

  Insert(heapNose, nodito3);

  //Resultado esperado 34
  printf("%d \n", GetMax(heapNose).key);

  DeleteMax(heapNose);

  //Resultado esperado 24
  printf("%d \n", GetMax(heapNose).key);

  DeleteMax(heapNose);

  //Resultado esperado 10
  printf("%d \n", GetMax(heapNose).key);

  DeleteMax(heapNose);
  
  if(Empty(heapNose)) printf("Esta vacio wachin \n");
  
  printf("Holi \n");
  
  return 0;
}