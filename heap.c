#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "heap.h"
#include "hashmap.h"
#include "list.h"

typedef struct HashMap HashMap;

typedef struct Node Node;

struct Node {
    void * data;
    Node * next;
    Node * prev;
};

struct List {
    Node * head;
    Node * tail;
    Node * current;
};

struct HashMap {
    Pair ** buckets;
    long size;
    long capacity;
    long current;
};

typedef struct nodo{
  void * data;
  int priority;
  HashMap * precedencia;    
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
    //pq = realloc(pq , pq->capac);
    pq->heapArray = realloc(pq->heapArray, sizeof(heapElem) * pq->capac);
  }

  //insertar el nuevo nodo
  pq->heapArray[pq->size].data = data;
  pq->heapArray[pq->size].priority = priority;
  pq->size++;
  
  while(true){
    if(posActual == 0)
      break;
    //posicion del padre 
    if(posActual % 2 == 0)
      padre = posActual / 2 - 1;
    else
      padre = posActual / 2;
    
    //correctamente posicionado
    if(pq->heapArray[padre].priority <= pq->heapArray[posActual].priority)
      break;
      
    //cambio de posicion
    else
    { 
      auxData = pq->heapArray[padre].data;
      auxPrio = pq->heapArray[padre].priority;
      
      pq->heapArray[padre].data = pq->heapArray[posActual].data;
      pq->heapArray[padre].priority = pq->heapArray[posActual].priority;
      
      pq->heapArray[posActual].data = auxData;
      pq->heapArray[posActual].priority = auxPrio;

      if(pq->heapArray[padre].precedencia != NULL){
        if(pq->heapArray[posActual].precedencia == NULL){
          pq->heapArray[posActual].precedencia = createMap(100);
          pq->heapArray[posActual].precedencia = pq->heapArray[padre].precedencia;
          pq->heapArray[padre].precedencia = NULL;
        }
        else{
          HashMap * hashAux = pq->heapArray[posActual].precedencia;
          pq->heapArray[posActual].precedencia = pq->heapArray[padre].precedencia;
          pq->heapArray[padre].precedencia = hashAux;
        }
      }
      posActual = padre;
    }
  }
}


void heap_pop(Heap* pq){
  int pos = 0 , auxPrio , mayor , posMayor;
  void * auxData;

  pq->heapArray[pos].data = pq->heapArray[pq->size-1].data;
  pq->heapArray[pos].priority = pq->heapArray[pq->size-1].priority;
  pq->size--;

  while(true){
    int hijoIzq = pq->heapArray[2 * pos + 1].priority;
    int hijoDer = pq->heapArray[2 * pos + 2].priority;
    
    if(hijoDer > hijoIzq ){
      mayor = hijoDer;
      posMayor = pos * 2 + 2;
    }
    else{
      mayor = hijoIzq;
      posMayor = pos * 2 + 1;
    }    

  
    auxData = pq->heapArray[pos].data;
    auxPrio = pq->heapArray[pos].priority;
    
    if(pq->heapArray[pos].priority > mayor || pq->size < posMayor){
      break;
    }
    else{
      pq->heapArray[pos].data = pq->heapArray[posMayor].data;
      pq->heapArray[pos].priority = pq->heapArray[posMayor].priority;
      pq->heapArray[posMayor].data = auxData;
      pq->heapArray[posMayor].priority = auxPrio;
      pos = posMayor;
    }
  }
    
}

Heap* createHeap(){
  Heap * aux = (Heap *) malloc(sizeof(Heap));
  aux->heapArray = malloc(sizeof(heapElem) * 3);
  aux->capac = 3;
  aux->size = 0;
  return aux;
}