#include "Map.h"
#include "heap.h"
#include "list.h"
#include "stack.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char nombreTarea[32];
  int prioridad;
  bool visited;
  bool explored;
  List *adj_edges;
} Nodo;

typedef struct nodo {
  void *data;
  int priority;
} heapElem;

typedef struct Heap {
  heapElem *heapArray;
  int size;
  int capac;
} Heap;

/*
  función para comparar claves de tipo string
  retorna 1 si son iguales
*/
int is_equal_string(void *key1, void *key2) {
  if (strcmp((char *)key1, (char *)key2) == 0)
    return 1;
  return 0;
}

void leerCadena(char cadena[31]) {
  scanf("%30[^\n]s", cadena);
  getchar();
}

void agregarTarea(Map *grafo) {
  Nodo *nuevaTarea = (Nodo *)malloc(sizeof(Nodo));
  nuevaTarea->adj_edges = createList();
  char nombre[32];
  int prioridad;
  printf("Ingrese el nombre de la tarea\n");
  getchar();
  leerCadena(nombre);

  printf("Ingrese la prioridad de la tarea\n");
  scanf("%d", &prioridad);

  strcpy(nuevaTarea->nombreTarea, strdup(nombre));
  nuevaTarea->prioridad = prioridad;

  insertMap(grafo, nuevaTarea->nombreTarea, nuevaTarea);
}

void precedenciaTareas(Map *grafo) {
  char tarea1[32];
  char tarea2[32];
  getchar();
  printf("Ingrese el nombre de las tareas a asignar precedencia\n");

  leerCadena(tarea1);
  if (searchMap(grafo, tarea1) == NULL) {
    printf("La tarea '%s' no existe dentro del programa\n", tarea1);
    return;
  }
  
  leerCadena(tarea2);
  if (searchMap(grafo, tarea2) == NULL) {
    printf("La tarea '%s' no existe dentro del programa\n", tarea2);
    return;
  }

  Nodo *aux1 = searchMap(grafo, tarea1);
  Nodo *aux2 = searchMap(grafo, tarea2);

  pushBack(aux1->adj_edges, aux2);
}

void mostrarTareas(Map *grafo) {
  Heap* monticulo = createHeap();
  List* listaTareas = createList();

  Nodo* aux = firstMap(grafo);
  while (aux != NULL) {
    if(firstList(aux->adj_edges) == NULL) {
      heap_push(monticulo, aux->nombreTarea, aux->prioridad);
      aux->explored = true;
    } 
    aux = nextMap(grafo);
  }

  while(monticulo->size > 0) {
    Nodo* actualTarea = heap_top(monticulo);
    heap_pop(monticulo);
    
    pushBack(listaTareas, actualTarea);
    actualTarea->visited = true;
    
    Nodo* aux = firstMap(grafo);
    while(aux != NULL) {
      if(aux->visited != true) {
        bool tienePrecedencia = false;

        Nodo* aux2 = firstList(aux->adj_edges);
        while(aux2 != NULL) {
          if(aux2->visited != true) {
            tienePrecedencia = true;
            break;
          }
          aux2 = nextList(aux->adj_edges);
        }

        if(tienePrecedencia != true && aux->explored != true) {
          heap_push(monticulo, aux, aux->prioridad);
          aux->explored = true;
        }
        
      }
      
      aux = nextMap(grafo);
    }
    
  }
  
  printf("Lista de tareas por hacer:\n");
  Nodo* aux2 = firstList(listaTareas);
  while (aux2 != NULL) {
    printf("Tarea: %s, Prioridad: %d", aux2->nombreTarea, aux2->prioridad);
    
    Nodo* aux3 = firstList(aux2->adj_edges);
    if(aux3 != NULL){
      printf(" - Precedente:");
      while(aux3 != NULL){
        printf(" %s",aux3->nombreTarea);
        aux3 = nextList(aux2->adj_edges);
      }
    }
    
    printf("\n");
    aux2 = nextList(listaTareas);
  }

}

void marcarTareaCompletada(grafo){
  char tarea[32];
  printf("Ingrese el nombre de la tarea completada\n");
  
  leerCadena(tarea1);
  if (searchMap(grafo, tarea1) == NULL) {
    printf("La tarea '%s' no existe dentro del programa\n", tarea1);
    return;
  }
  Nodo *aux = searchMap(grafo, tarea1);
  Nodo *aux2 = searchMap(grafo, aux1->adj_edges);
  pushBack(aux1->adj_edges, aux2);
}

void mostrarTitulo(){
    printf("=========================================\n");
    printf(" === BIENVENIDO AL MENÚ DE TAREAS === \n");
    printf("=========================================\n");
    printf("Escoja una opción: \n\n");
    printf("1. Agregar tarea\n");
    printf("2. Establecer precedencia entre tareas\n");
    printf("3. Mostrar tareas por hacer\n");
    printf("4. Marcar tarea como completada\n");
    printf("5. Deshacer última acción\n");
    printf("6. Cargar datos de tareas desde un archivo de texto\n");
    printf("7. Salir del programa\n");
    printf("=========================================\n");
}

int main() {
  Map *grafo = createMap(is_equal_string);

  int opcion, titulo = 0;

  while (1) {
    
    mostrarTitulo();
    scanf("%d",&opcion);
    while(opcion < 1 || opcion > 7) {
      printf("Ingrese una opción correcta\n");
      scanf("%d",&opcion);
    }
    switch (opcion) {
    case 1:
      agregarTarea(grafo);
      break;
    case 2:
      precedenciaTareas(grafo);
      break;
    case 3:
      mostrarTareas(grafo);
      break;
    case 4:
      marcarTareaCompletada(grafo);
      break;
    case 5:
      printf("Funcion no implementada\n");
      break;
    case 6:
      printf("Funcion no implementada\n");
      break;
    case 7:
      printf("Cerrando el programa...\n");
      return 0;
    }
  }
  return 0;
}