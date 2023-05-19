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
  int posicion;
  char * nombre[50];
  int prioridad;
  HashMap * hashPrecedencias;
  List * listaNombres;
}Accion;

Accion* createAccion(int tipo, int posicion , char * nombre, int prioridad,
HashMap * hashPrecedencias , List * listaNombres){ 
  Accion* newA = calloc(1, sizeof(Accion));
  
  newA->tipo = tipo;
  newA->posicion = posicion;
  strcpy(newA->nombre , nombre);
  newA->prioridad = prioridad;
  newA->hashPrecedencias = hashPrecedencias;
  newA->listaNombres = listaNombres;
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

/* la función solicita al usuario ingresar el nombre y la prioridad de una tarea,
luego crea una estructura heapElem con esos datos y la agrega al montículo (elHeap) utilizando heap_push.*/
void AgregarTareas(Heap * elHeap , List * listaDeshacer){
  char nombre[50];
  int prioridad;
  heapElem tarea;
  
  printf("Ingrese el nombre de la tarea:\n");
  scanf("%s",nombre);
  printf("Ingrese la prioridad de la tarea:\n");
  scanf("%d", &prioridad);
  
  
  tarea.data = strdup(nombre);
  tarea.priority = prioridad;

  
  heap_push(elHeap, tarea.data, tarea.priority);
  Accion * newA = createAccion(1,"", tarea.data, tarea.priority,"" ,"");
  pushFront(listaDeshacer, newA );
  printf("Tarea agregada correctamente\n");
}

/*la función busca una tarea específica en el heap comparando su nombre con los nombres 
de las tareas en el heap. Si encuentra la tarea, retorna 1 y almacena la posición en numero. 
Si no la encuentra, retorna 0*/
int buscarTarea(Heap * elHeap , char * nombre , int * numero){
  for(int i = 0 ; i < elHeap->size ; i++){
    if(strcmp(elHeap->heapArray[i].data , nombre) == 0) return 1;
    *numero+=1;
  }
  return 0;
}

/* la función crea dos listas: listaFinal y listaPrecedencia. Luego, recorre los elementos del heap y los agrega
a la lista listaFinal si no tienen precedencia, o a la lista listaPrecedencia si tienen precedencia. 
Finalmente, reordena los elementos de listaPrecedencia y los agrega a listaFinal. 
La función retorna listaFinal con los elementos reordenados.*/
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
  

/* la función solicita al usuario los nombres de dos tareas y establece la precedencia entre ellas en el heap. 
Si las tareas no se encuentran en el heap o si la tarea precedente es la misma que la tarea siguiente, 
se muestra un mensaje de error. Luego, se crea una estructura precedencia para la tarea precedente si aún no existe, 
y se inserta la tarea siguiente en dicha estructura.  */
void EstablecerPrecedencia(Heap *elHeap , List * listaDeshacer ){
    if(elHeap->size == 0 || elHeap->size == 1){
      printf("No puede realizar precedencias en este momento, agregue mas tareas! \n");
      return;
    }
    
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

    Accion * newA = createAccion(2,numero1, elHeap->heapArray[numero2].data,"","", "");
    pushFront(listaDeshacer, newA );

  printf("Precedencia establecida correctamente!\n");

}

/*la función recorre la lista listaFinal, que contiene el orden de las tareas en el heap,
e imprime en pantalla cada tarea con su prioridad y sus precedencias, si las tienen.*/
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

/* a función verifica si hay tareas en el heap y luego muestra en pantalla las tareas junto con su prioridad 
y sus precedencias, si las tienen. Utiliza la función reordenar para obtener el orden correcto de las tareas 
según las precedencias.*/
void MostrarTareas(Heap *elHeap){
  int tamaño = elHeap->size;
  if (tamaño == 0){
    printf("No hay tareas!\n");
    return;
  }
  
  List * listaOrden = reordenar(elHeap);
  
  imprimirTareas(elHeap , listaOrden, tamaño);
}

/* la función recorre el heap y elimina las precedencias asociadas al nombre de una tarea.
Luego, almacena las tareas afectadas en la lista listaNombres. Esto se realiza mediante 
la eliminación del nombre de las precedencias correspondientes y el almacenamiento 
de las tareas en la lista utilizando la función pushBack.*/
void eliminarPrecedencias(Heap * elHeap , char * nombre , List * listaNombres){
  int tamaño = elHeap->size;
  for(int i = 0 ; i < tamaño ; i++){
    if(elHeap->heapArray[i].precedencia != NULL){
      if(searchMap(elHeap->heapArray[i].precedencia, nombre) != NULL){
        eraseMap(elHeap->heapArray[i].precedencia, nombre);
        pushFront(listaNombres , elHeap->heapArray[i].data);
        if(elHeap->heapArray[i].precedencia->size == 0)
          elHeap->heapArray[i].precedencia = NULL;
      }
    }
  }
}

/*Basicamente esta funcion se encarga de leer el nombre de una tarea y eliminar, para
luego no mostrarla en las tareas por hacer*/
void tareasCompletadas(Heap *elHeap , List * listaDeshacer , char * nombreDeshacer){
  char nombre[50];
  int n = 0;
  List * listaPrecedencias = createList(); 

  //comprobar si se esta realizando una operacion del deshacer
  if(nombreDeshacer != NULL){
    if (buscarTarea(elHeap, nombreDeshacer, &n)){
      for (int i = n; i < elHeap->size - 1; i++) {
          elHeap->heapArray[i] = elHeap->heapArray[i + 1];
        }
        elHeap->size--; 
      return;
    }
  }

  if(elHeap->size == 0){
      printf("No hay tareas para eliminar! \n");
      return;
    }
  
  printf("Ingrese el nombre de la tarea realizada: ");
  scanf("%s", nombre);
  
  //buscamos la tarea para ver si existe
  if (buscarTarea(elHeap, nombre, &n)) {
    
    int prio = elHeap->heapArray[n].priority;

    //esto para ver si la tarea tiene precedencias
    if (elHeap->heapArray[n].precedencia != NULL) {
      HashMap * hashAux = elHeap->heapArray[n].precedencia;
      //confirmacion para eliminar
      printf("¿Estás seguro que deseas eliminar esta tarea? (S/N): ");
      char confirmacion;
      scanf(" %c", &confirmacion);
      if (tolower(confirmacion) == 's') {
        //movemos los datos una posicion a la izquierda hasta el dato a eliminar
        for (int i = n; i < elHeap->size - 1; i++) {
          elHeap->heapArray[i] = elHeap->heapArray[i + 1];
        }
        elHeap->size--; 

        //eliminamos la tarea de las precedencias en que se encuentre
        eliminarPrecedencias(elHeap , nombre, listaPrecedencias);

        //sumamos una accion a la pila
        Accion * newA = createAccion(3,n ,nombre, prio,hashAux, listaPrecedencias);
        pushFront(listaDeshacer, newA );
        printf("Tarea marcada como completada y eliminada correctamente.\n");
      } else {
        printf("Eliminación de tarea cancelada.\n");
      }
      
    } else {
      //eliminacion para datos que no tengan precedencia
      for (int i = n; i < elHeap->size - 1; i++) {
        elHeap->heapArray[i] = elHeap->heapArray[i + 1];
      }
      elHeap->size--;
      eliminarPrecedencias(elHeap , nombre , listaPrecedencias);
      printf("Tarea marcada como completada y eliminada correctamente.\n");
      Accion * newA = createAccion(3,n ,nombre, prio,NULL, listaPrecedencias);
      pushFront(listaDeshacer, newA );
    }
    
    
    
  } else {
    printf("Tarea no encontrada.\n");
  }
}


/* la función se encarga de recorrer la línea de un archivo CSV manejando correctamente 
los casos donde los campos pueden estar entre comillas dobles y separados por comas.
Retorna el campo como una cadena de caracteres.*/
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
/*Esta funcion recibe el heap y una lista de acciones, la funcion leera la primera instancia de accion que se encuentre en la lista ya que esta seria la ultima accion realizada y por ende la que se debera deshacer, la accion tendra guardada un valor int el cual indicara que operaciones se deberan realizar para deshacer la accion*/
void deshacerAccion(Heap *elHeap , List * listaDeshacer)
{
  //ver si hay acciones para deshacer
  if(elHeap->size == 0 || firstList(listaDeshacer)==NULL){
    printf("--- NO hay acciones para deshacer ---\n");
    return;
  }
  //leer la primera accion
  Accion * accion = firstList(listaDeshacer);

  //la accion seria agregar una tarea por lo que solo se debera eliminar del heap
  if(accion->tipo == 1){
    tareasCompletadas(elHeap,listaDeshacer,accion->nombre);
  }

  /*en este caso seria agregar una precedencia, por lo que este debera eliminar dicha precedencia, cabe destacar que no se hara ninguna comprobacion respecto a la precedencia ya que se sabe que esta si existe, solo se dejara su hashmap de precedencias en null en caso de ver que no hay mas precedencias*/
  else if(accion->tipo == 2){
    eraseMap(elHeap->heapArray[accion->posicion].precedencia, accion->nombre);
    if(elHeap->heapArray[accion->posicion].precedencia->size == 0)
      elHeap->heapArray[accion->posicion].precedencia = NULL;
  }

//deshacer una eliminacion, cabe destacar que no es usada la funcion agregar tarea ya que al ser usada esta intercambiara varios datos al dejar la tarea en la posicion que le corresponde pero ademas no la dejara en la posicion que originalmente le tocaba.
  else if(accion->tipo == 3){
    //posicion del ultimo dato que existe
    int ultimo = elHeap->size-1 ;
    elHeap->size++;
    //movemos todos los datos desde la posicion del dato original una posicion a la derecha dentro del heap
    for(int i = ultimo ; i >= accion->posicion ; i--){
      elHeap->heapArray[i] = elHeap->heapArray[i-1];
    }
    //agregamos los datos de la tarea eliminada
    elHeap->heapArray[accion->posicion].data = accion->nombre;
    elHeap->heapArray[accion->posicion].priority = accion->prioridad;
    //comprobar si esta tarea tenia precedencias
    if(accion->hashPrecedencias != NULL)
      elHeap->heapArray[accion->posicion].precedencia = accion->hashPrecedencias;
    else
      //no tenia precedencias pero dejamos esta casilla en NULL por si el dato que habia antes tenia precedencia
      elHeap->heapArray[accion->posicion].precedencia = NULL;

    //entramos a una lista de nombres que indicara si la tarea era precedente de otras tareas
    void * auxNombre = firstList(accion->listaNombres);
    while(auxNombre != NULL){
      int numero1=0;
      //buscamos donde se encuentra la tarea a la que se le agregara la precedencia
      buscarTarea(elHeap, auxNombre, &numero1);
      if(elHeap->heapArray[numero1].precedencia == NULL)
        elHeap->heapArray[numero1].precedencia = createMap(100);
      //insertamos la tarea
      insertMap(elHeap->heapArray[numero1].precedencia, elHeap->heapArray[accion->posicion].data , elHeap->heapArray[accion->posicion].priority);
      auxNombre=nextList(accion->listaNombres);
    }
  }  
  //eliminar el front de la pila 
  popFront(listaDeshacer);
  printf("--- Accion deshecha correctamente ---\n"); 
}
  
/* la función lee un archivo CSV y carga los datos en una estructura de heap, 
donde cada elemento del heap tiene un nombre, una prioridad y un mapa de precedencias.*/  
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
        AgregarTareas(elHeap , listaDeshacer);
        break;
      case 2:
        EstablecerPrecedencia(elHeap , listaDeshacer);
        break;
      case 3:
        MostrarTareas(elHeap);
         break;
      case 4:
        tareasCompletadas(elHeap , listaDeshacer , NULL);
        break;
      case 5:
        deshacerAccion(elHeap ,listaDeshacer);
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
