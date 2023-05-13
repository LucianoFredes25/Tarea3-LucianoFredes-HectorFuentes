#include <stdio.h>
#include <stdbool.h>
#include "heap.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"

typedef struct HashMap HashMap;
//int enlarge_called=0;

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

void EstablecerPrecedencia(Heap *elHeap){
    char tarea1[50], tarea2[50];
    int numero1 = 0 , numero2 = 0;
    
    printf("Ingrese el nombre de la tarea precedente:\n");
    scanf("%s", tarea1);
    printf("Ingrese el nombre de la tarea que debe realizarse despues:\n");
    scanf("%s", tarea2);
    if(buscarTarea(elHeap , tarea1 , &numero1) && buscarTarea(elHeap, tarea2 , &numero2)){
      if(elHeap->heapArray[numero1].precedencia == NULL)
        elHeap->heapArray[numero1].precedencia = createMap(100);
      
      insertMap(elHeap->heapArray[numero1].precedencia,elHeap->heapArray[numero2].data , elHeap->heapArray[numero2].priority);

    }
    else
      printf("Error , nombre de tarea(s) invalido!\n");
}


void MostrarTareas(Heap *elHeap){
  int tamaño = elHeap->size;
  for(int i = 0 ; i < tamaño ; i++){
    printf("%s - %d ", elHeap->heapArray[i].data , elHeap->heapArray[i].priority);
    if(elHeap->heapArray[i].precedencia != NULL){
      Pair * pairAux = firstMap(elHeap->heapArray[i].precedencia);
      printf("- ");
      while(pairAux != NULL){
        printf("%s ", pairAux->key);
        pairAux = nextMap(elHeap->heapArray[i].precedencia);
      }
      printf("\n");
    }
    else
      printf("\n");
  }
}

void TareasCompletadas(Heap *elHeap){

}

void deshacerAccion(Heap *elHeap){

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


  
 void cargarDatos(Heap *elHeap){
  char nombreArchivo[50];
  char linea[1024];
  
  printf("Ingrese el archivo para cargar los datos: ");
  scanf("%s", nombreArchivo);

  FILE* archivo = fopen(nombreArchivo, "r");
  if (archivo == NULL){
    printf("Error al abrir el archivo de exportación \n");
    return;
  }

    fgets(linea, 1024, archivo); 
    
    while (fgets(linea, sizeof(linea), archivo)) {
    char* nombre = get_csv_field(linea, 0);
    char* prioridad_str = get_csv_field(linea, 1);

    if (nombre != NULL && prioridad_str != NULL) {
      int prioridad = atoi(prioridad_str);
      heapElem tarea;
      tarea.data = strdup(nombre);
      tarea.priority = prioridad;
      tarea.precedencia = NULL;

      heap_push(elHeap, tarea.data, tarea.priority);

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
