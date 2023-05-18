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

typedef struct{
  int tipo;
  char * nombre;
}Accion;

Accion* createAccion(int tipo , char * nombre ){
  Accion* newA = calloc(1, sizeof(Accion));
  newA->tipo = 

  return newA;
}

void mostrarMenu() {
  printf("\n---  Tareas  ---\n");
  printf("[1] Agregar Tarea.\n");
  printf("[2] Establecer precedencia.\n");
  printf("[3] Mostrar tareas por hacer.\n");
  printf("[4] Marcar tareas completadas.\n");
  printf("[5] Deshacer última acción\n");
  printf("[6] Cargar datos de tareas desde un archivo de texto\n");
  printf("[7] Salir.\n");
  printf("Seleccione una opción: ");
}


void AgregarTareas(Heap * elHeap){
  char nombre[50];
  int prioridad;

  printf("Ingrese el nombre de la tarea:\n");
  scanf("%s",nombre);
  printf("Ingrese la prioridad de la tarea:\n");
  scanf("%d", &prioridad);
  
  heapElem tarea;
  tarea.data = strdup(nombre);
  tarea.priority = prioridad;

  
  heap_push(elHeap, tarea.data, tarea.priority);

  printf("Tarea agregada correctamente\n");
}

int buscarTarea(Heap * elHeap , char * nombre , int * numero){
  for(int i = 0 ; i < elHeap->size ; i++){
    if(strcmp(elHeap->heapArray[i].data , nombre) == 0) return 1;
    *numero+=1;
  }
  return 0;
}

List * reordenar(Heap * elHeap){
  List * listaFinal = createList();
  List * listaPrecedencia = createList();
  
  int tamaño = elHeap->size , cont = 0;

  for(int i = 0 ; i < tamaño ; i++){
    
    if(elHeap->heapArray[i].precedencia == NULL){
      if(listaFinal->head == NULL)
        pushFront(listaFinal, i);
      else
        pushBack(listaFinal, i);
    }
    else{
      cont++;
      if(listaPrecedencia->head == NULL){
        pushFront(listaPrecedencia, i);
      }
      else{
        pushBack(listaPrecedencia, i);
      }
    }
  }
    
  if(cont != 0){
    void * aux = firstList(listaPrecedencia);
    for(int j = 0 ; j < cont ; j++){
      if(listaFinal->head == NULL)
        pushFront(listaFinal, aux);
      else
        pushBack(listaFinal, aux);
      aux = nextList(listaPrecedencia);
    }
  }
  return listaFinal;
}
  


void EstablecerPrecedencia(Heap *elHeap){
    char tarea1[50], tarea2[50];
    int numero1 = 0 , numero2 = 0;
    
    printf("Ingrese el nombre de la tarea precedente:\n");
    scanf("%s", tarea1);
  
    if(buscarTarea(elHeap , tarea1 , &numero1) == 0){
      printf("Tarea no encontrada!\n");
      return;
    }
    
    printf("Ingrese el nombre de la tarea que debe realizarse despues:\n");
    scanf("%s", tarea2);
    
    if(buscarTarea(elHeap, tarea2 , &numero2) == 0){
      printf("Tarea no encontrada!\n");
      return;
    }
      
    if(strcmp(tarea1,tarea2)==0){
      printf("Una tarea no puede ser precedente de si misma! \n");
      return;
    }
    
    if(elHeap->heapArray[numero1].precedencia == NULL)
      elHeap->heapArray[numero1].precedencia = createMap(100);
      
    insertMap(elHeap->heapArray[numero1].precedencia, elHeap->heapArray[numero2].data , elHeap->heapArray[numero2].priority);


  printf("Precedencia establecida correctamente!\n");

}
void imprimirTareas(Heap * elHeap , List * listaFinal , int tamaño){

  void * aux = firstList(listaFinal);
  
  for(int i = 0 ; i < tamaño ; i++){
    int nose = aux;
    printf("%-2d) %2s - %1d ", (i+1),elHeap->heapArray[nose].data , elHeap->heapArray[nose].priority);
    if(elHeap->heapArray[nose].precedencia != NULL){
      printf("Precedencias: ");
      Pair * pairAux = firstMap(elHeap->heapArray[nose].precedencia);
    
      while(pairAux != NULL){
        printf("%s ",   pairAux->key);
        pairAux = nextMap(elHeap->heapArray[nose].precedencia);
      }
      printf("\n");
    }
    else
      printf("\n");
    aux=nextList(listaFinal);
  }
}

void MostrarTareas(Heap *elHeap){
  int tamaño = elHeap->size;
  if (tamaño == 0){
    printf("No hay tareas!\n");
    return;
  }
  
  List * listaOrden = reordenar(elHeap);

  imprimirTareas(elHeap , listaOrden, tamaño);
}


void eliminarPrecedencias(Heap * elHeap , char * nombre){
  int tamaño = elHeap->size;
  for(int i = 0 ; i < tamaño ; i++){
    if(elHeap->heapArray[i].precedencia != NULL){
      eraseMap(elHeap->heapArray[i].precedencia, nombre);
      if(elHeap->heapArray[i].precedencia->size == 0)
        elHeap->heapArray[i].precedencia = NULL;
    }
  }
}

void deshacerAccion(Heap *elHeap)
{
 

  if(){
    printf("--- NO hay acciones para deshacer ---\n");
    return;
  }

  
  

  printf("--- Accion deshecha correctamente ---\n"); 
}


void tareasCompletadas(Heap *elHeap){
  char nombre[50];
  int n = 0;
  
  printf("Ingrese el nombre de la tarea realizada: ");
  scanf("%s", nombre);

  if (buscarTarea(elHeap, nombre, &n)) {
    if (elHeap->heapArray[n].precedencia != NULL) {
      printf("¿Estás seguro que deseas eliminar esta tarea? (s/n): ");
      char confirmacion;
      scanf(" %c", &confirmacion);
      if (tolower(confirmacion) == 's') {
        
        for (int i = n; i < elHeap->size - 1; i++) {
          elHeap->heapArray[i] = elHeap->heapArray[i + 1];
        }
        elHeap->size--;
       eliminarPrecedencias(elHeap , nombre);
      printf("sali al precedencia\n");
        
        printf("Tarea marcada como completada y eliminada correctamente.\n");
      } else {
        printf("Eliminación de tarea cancelada.\n");
      }
    } else {
      //free(elHeap->heapArray[n].data);
      
      for (int i = n; i < elHeap->size - 1; i++) {
        elHeap->heapArray[i] = elHeap->heapArray[i + 1];
      }
      elHeap->size--;

      eliminarPrecedencias(elHeap , nombre);
      
      printf("Tarea marcada como completada y eliminada correctamente.\n");
    }
  } else {
    printf("Tarea no encontrada.\n");
  }
}



const char *get_csv_field (char * tmp, int k) {
    int open_mark = 0;
    char* ret=(char*) malloc (100*sizeof(char));
    int ini_i=0, i=0;
    int j=0;
    while(tmp[i+1]!='\0'){

        if(tmp[i]== '\"'){
            open_mark = 1-open_mark;
            if(open_mark) ini_i = i+1;
            i++;
            continue;
        }

        if(open_mark || tmp[i]!= ','){
            if(k==j) ret[i-ini_i] = tmp[i];
            i++;
            continue;
        }

        if(tmp[i]== ','){
            if(k==j) {
               ret[i-ini_i] = 0;
               return ret;
            }
            j++; ini_i = i+1;
        }

        i++;
    }

    if(k==j) {
       ret[i-ini_i] = 0;
       return ret;
    }

    return NULL;
}


  
void cargarDatos(Heap *elHeap) {
  char nombreArchivo[50];
  char linea[1024];
  
  printf("Ingrese el archivo para cargar los datos: ");
  scanf("%s", nombreArchivo);

  FILE* archivo = fopen(nombreArchivo, "r");
  if (archivo == NULL) {
    printf("Error al abrir el archivo de exportación \n");
    return;
  }

  fgets(linea, 1024, archivo);

  while (fgets(linea, sizeof(linea), archivo)) {
    int numero1 = 0 , numero2 = 0;
    char* nombre = get_csv_field(linea, 0);
    char* prioridad_str = get_csv_field(linea, 1);
    char* precedencias = get_csv_field(linea, 2);

     
    
    if (nombre != NULL && prioridad_str != NULL && precedencias != NULL) {
      int prioridad = atoi(prioridad_str);

      heapElem tarea;
      tarea.data = strdup(nombre);
      tarea.priority = prioridad;
      
     
      heap_push(elHeap, tarea.data, tarea.priority);
      
      if(strlen(precedencias) > 1){
        
        buscarTarea(elHeap , tarea.data , &numero1);
        
        char* precedencia = strtok(precedencias, " ");

        buscarTarea(elHeap , precedencia , &numero2);
        
        while (precedencia != NULL) {
          if(elHeap->heapArray[numero1].precedencia == NULL)
            elHeap->heapArray[numero1].precedencia = createMap(100);
          
          insertMap(elHeap->heapArray[numero1].precedencia, elHeap->heapArray[numero2].data , elHeap->heapArray[numero2].priority );
          
          precedencia = strtok(NULL, " ");
        }
      }
      free(precedencias);
      free(nombre);
      free(prioridad_str);
     
    }
  }

  printf("Datos cargados correctamente\n");
  fclose(archivo);
}



int main(void) {
  int opcion = 7;
  Heap * elHeap = createHeap();
  
  List * listaDeshacer = createList();
  
  while (true) {

    mostrarMenu();
    
    scanf("%d", &opcion);
    printf("\n");
    
    switch(opcion){
      case 1:
        AgregarTareas(elHeap);
        break;
      case 2:
        EstablecerPrecedencia(elHeap);
        break;
      case 3:
        MostrarTareas(elHeap);
         break;
      case 4:
        tareasCompletadas(elHeap);
        break;
      case 5:
        break;
      case 6:
        cargarDatos(elHeap);
        break;
      case 7:
        break;
      default:
        printf("Opcion Invalida, ingrese nueva opcion\n");
    }
    if(opcion == 7)
      break;
  }

  return 0;
}
