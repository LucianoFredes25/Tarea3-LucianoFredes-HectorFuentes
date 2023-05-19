#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

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

typedef List List;

Node * createNode(void * data) {
    Node * new = (Node *)malloc(sizeof(Node));
    assert(new != NULL);
    new->data = data;
    new->prev = NULL;
    new->next = NULL;
    return new;
}

List * createList() {
  List* list = (List *) malloc(sizeof(List));
  list->head = NULL;
  list->tail = NULL;
     return list;
}

void * firstList(List * list) {
  if(list->head == NULL||list == NULL)
    return NULL;
  list->current = list->head;
  return list->head->data; 
}

void * nextList(List * list) {
  Node * n = list->current;
  if(list == NULL || n == NULL || n->next == NULL)
    return NULL;
  list->current = n->next;
  n = list->current;
  return n->data;
}

void * lastList(List * list) {
  if(list->tail == NULL||list == NULL)
    return NULL;
  list->current = list->tail;
  return list->tail->data; 
}

void * prevList(List * list) {
  Node * n = list->current;
  if(list == NULL || n == NULL || n->prev == NULL)
    return NULL;
  list->current = n->prev;
  n = list->current;
  return n->data;
}

void pushFront(List * list, void * data) {
  if(list->head == NULL){
    Node * Aux = createNode(data);
    list->head = Aux;
    list->tail = Aux;
  }
  else{
    Node * Aux = createNode(data);
    Node * AuxHead = list->head;
    list->head = Aux;
    Aux->next = AuxHead;
  }
}

void pushBack(List * list, void * data) {
  list->current = list->tail;
  pushCurrent(list,data);
}

void pushCurrent(List * list, void * data) {
  Node * Aux = createNode(data);
  Node * AuxNext = list->current->next;
  list->current->next = Aux;
  Aux->next = AuxNext;
  Aux->prev = list->current;
  if(list->current == list->tail)
    list->tail = Aux;
}

void * popFront(List * list) {
    list->current = list->head;
    return popCurrent(list);
}

void * popBack(List * list) {
    list->current = list->tail;
    return popCurrent(list);
}

void * popCurrent(List * list) {
    Node * current = list->current;
    void * data = current->data;
    if (current->prev != NULL) {
        current->prev->next = current->next;
    } else {
        list->head = current->next;
    }

    if (current->next != NULL) {
        current->next->prev = current->prev;
    } else {
        list->tail = current->prev;
    }

    list->current = current->next;

  return data;
}

void cleanList(List * list) {
    while (list->head != NULL) {
        popFront(list);
    }
}