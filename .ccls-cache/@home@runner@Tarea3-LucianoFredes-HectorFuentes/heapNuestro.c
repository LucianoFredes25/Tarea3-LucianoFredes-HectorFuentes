#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "heapNuestro.h"
#include <stdbool.h>

typedef struct nodo{
   void* data;
   int priority;
}heapElem;

typedef struct Heap{
  heapElem* heapArray;
  int size;
  int capac;
} Heap;


void* heap_top(Heap* pq){ 
  if(pq->size == 0)
    return NULL;
  
  void * first = pq->heapArray[0].data;
  
  return first;
}



void heap_push(Heap* pq, void* data, int priority){
  void * auxData;
  int auxPrio , padre , posActual = pq->size;

  //aumento capacidad
  if(pq->size == pq->capac){
    pq->capac *= 2;
    pq->capac += 1;
    pq = realloc(pq , pq->capac);
  }

  //insertar el nuevo nodo
  pq->heapArray[pq->size].data = data;
  pq->heapArray[pq->size].priority = priority;
  pq->size++;

  //hasta aqui recibe bien los datos
  
  while(true){
    printf("%d\n",pq->heapArray[posActual].priority );
    if(posActual == 0)
      break;
    //posicion del padre 
    if(posActual % 2 == 0)
      padre = posActual / 2 - 1;
    else
      padre = posActual / 2;
    
    //correctamente posicionado
    if(pq->heapArray[padre].priority < pq->heapArray[posActual].priority){
      printf("%d\n",pq->heapArray[posActual].priority );
      break;
    }
      
      
    //cambio de posicion
    else
    {
      auxData = pq->heapArray[padre].data;
      auxPrio = pq->heapArray[padre].priority;
      
      pq->heapArray[padre].data = pq->heapArray[posActual].data;
      pq->heapArray[padre].priority = pq->heapArray[posActual].priority;
      
      pq->heapArray[posActual].data = auxData;
      pq->heapArray[posActual].priority = auxPrio;
      
      posActual = padre;
    }
  }
}


void heap_pop(Heap* pq){

}

Heap* createHeap(){
  Heap * aux = (Heap *) malloc(sizeof(Heap));
  aux->heapArray = malloc(sizeof(heapElem) * 3);
  aux->capac = 3;
  aux->size = 0;
  return aux;
}
