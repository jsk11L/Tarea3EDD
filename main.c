#include "Map.h"
#include "list.h"
#include "stack.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct Nodo {
    char tarea[20];
    int prioridad;
    struct Nodo* next;
};

struct Grafo {
    struct Nodo* inicio;
};

struct Nodo* crearNodo(char tarea[], int prioridad) {
    struct Nodo* nodo = (struct Nodo*)malloc(sizeof(struct Nodo));
    strcpy(nodo->tarea, tarea);
    nodo->prioridad = prioridad;
    nodo->next = NULL;
    return nodo;
}

void agregarTarea(grafo){
  
}

void establecerPrecedencia(grafo){
  
}

void mostrarTareas(grafo){
  //FRAN
}

void marcarTareaCompletada(grafo){
  
}

void deshacerAccion(grafo){
  
}

void cargarArchivo(grafo){
  //FRAN
}

int main() {
  //Se inicializa el grafo de las tareas.
  Map* grafo = createMap(is_equal_string);
  int opcion, titulo = 0;

  while(1) {
    if (titulo == 0) {
      printf("=========================================\n");
      printf(" === BIENVENIDO AL MENÚ DE TAREAS === \n");
      titulo++;
    }

    printf("=========================================\n");
    printf("Escoja una opción: \n\n");
    printf("1. Agregar tarea\n");
    printf("2. Establecer precedencia entre tareas\n");
    printf("3. Mostrar tareas por hacer\n");
    printf("4. Marcar tarea como completada\n");
    printf("5. Deshacer última acción\n");
    printf("6. Cargar datos de tareas desde un archivo de texto\n");
    printf("7. Salir del programa\n");
    
    scanf("%d",&opcion);
    while(opcion < 1 || opcion > 10) {
      scanf("%d",&opcion);
    }

    switch(opcion) {
      case 1:
        agregarTarea(grafo);
        break;
      case 2:
        establecerPrecedencia(grafo);
        break;
      case 3:
        mostrarTareas(grafo);
        break;
      case 4:
        marcarTareaCompletada(grafo);
        break;
      case 5:
        deshacerAccion(grafo);
        break;
      case 6:
        cargarArchivo(grafo);
        break;
      case 7:
        printf("Cerrando el programa...\n");
      return 0;
    }
  }
  return 0;
}