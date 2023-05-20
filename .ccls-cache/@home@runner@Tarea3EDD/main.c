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
  bool visitado;
  bool explorado;
  List *adj_edges;

} Nodo;

typedef enum Accion {
  AGREGAR_TAREA,
  ELIMINAR_TAREA,
  PRECEDENCIA
} Accion;

typedef struct Pila {
  char nombre[32];
  Accion accion;
  Nodo* auxNodo;
} Pila;

/*
  función para comparar claves de tipo string
  retorna 1 si son iguales
*/
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

int is_equal_string(void *key1, void *key2) {
  if (strcmp((char *)key1, (char *)key2) == 0)
    return 1;
  return 0;
}

void leerCadena(char cadena[31]) {
  scanf("%30[^\n]s", cadena);
  getchar();
}

void agregarTarea(Map *grafo, Stack *stack) {
  Nodo *nuevaTarea = (Nodo *)malloc(sizeof(Nodo));
  nuevaTarea->adj_edges = createList();
  
  char tarea[32];
  int prioridad;
  
  printf("Ingrese el nombre de la tarea\n");
  leerCadena(tarea);
  while(searchMap(grafo, tarea) != NULL) {
    printf("El nombre ya existe dentro del programa!\n");
    leerCadena(tarea);
  }
  
  printf("Ingrese la prioridad de la tarea\n");
  scanf("%d", &prioridad);

  strcpy(nuevaTarea->nombreTarea, strdup(tarea));
  nuevaTarea->prioridad = prioridad;

  insertMap(grafo, nuevaTarea->nombreTarea, nuevaTarea);

  Pila* registro = (Pila *) malloc(sizeof(Pila));
  registro->accion = AGREGAR_TAREA;
  strcpy(registro->nombre, tarea);

  stack_push(stack, registro);
}

void establecerPrecendencia(Map *grafo, Stack *stack) {
  char tarea1[32];
  char tarea2[32];
  
  printf("Ingrese el nombre de la tarea que precede\n");
  leerCadena(tarea1);
  if (searchMap(grafo, tarea1) == NULL) {
    printf("La tarea '%s' no existe dentro del programa\n", tarea1);
    return;
  }
  
  printf("Ingrese el nombre de la tarea a preceder\n");
  leerCadena(tarea2);
  if (searchMap(grafo, tarea2) == NULL) {
    printf("La tarea '%s' no existe dentro del programa\n", tarea2);
    return;
  }

  Nodo *aux1 = searchMap(grafo, tarea1);
  Nodo *aux2 = searchMap(grafo, tarea2);

  pushBack(aux2->adj_edges, aux1);
  
  Pila* registro = (Pila *) malloc(sizeof(Pila));
  registro->accion = PRECEDENCIA;
  strcpy(registro->nombre , tarea1);
  registro->auxNodo = aux2;
  
  stack_push(stack, registro);
  
}

void reestablecerBooleanos(Map *grafo) {
 Nodo *tarea = firstMap(grafo);
    
  while(tarea != NULL) {
    tarea->explorado = false;
    Nodo *nodoAdj = firstList(tarea->adj_edges);
      
    while(nodoAdj != NULL) {
      nodoAdj->visitado = false; 
      nodoAdj = nextList(tarea->adj_edges);
    }
          
    tarea = nextMap(grafo);
  }
}

void nodosCompletados(Map *grafo, char *nombreNodo) {
  Nodo *tarea = firstMap(grafo);
  
  while(tarea != NULL) {
  
    Nodo *nodoAdj = firstList(tarea->adj_edges);

    while(nodoAdj != NULL) {
      if(strcmp(nodoAdj->nombreTarea, nombreNodo) == 0) {
        nodoAdj->visitado = true;
      }
      nodoAdj = nextList(tarea->adj_edges);
    }
          
    tarea = nextMap(grafo);
  }
}

void mostrarTareas(Map *grafo) {
  Heap *monticulo = createHeap();
  List *listaTareas = createList();

  reestablecerBooleanos(grafo);

  Nodo *aux = firstMap(grafo);
  while(aux != NULL) {
    if (firstList(aux->adj_edges) == NULL) {
      heap_push(monticulo, aux->nombreTarea, aux->prioridad);
      aux->explorado = true;
    }
    aux = nextMap(grafo);
  }

  while(heap_top(monticulo) != NULL) {
    Nodo *actualTarea = heap_top(monticulo);
    heap_pop(monticulo);

    pushBack(listaTareas, actualTarea);
    nodosCompletados(grafo, actualTarea->nombreTarea);

    Nodo *aux = firstMap(grafo);
    while(aux != NULL) {
      if (aux->explorado != true) {
        bool tienePrecedencia = false;

        Nodo *aux2 = firstList(aux->adj_edges);
        while(aux2 != NULL) {
          if (aux2->visitado != true) {
            tienePrecedencia = true;
            break;
          }
          aux2 = nextList(aux->adj_edges);
        }

        if (tienePrecedencia != true && aux->explorado != true) {
          heap_push(monticulo, aux, aux->prioridad);
          aux->explorado = true;
        }
      }
      aux = nextMap(grafo);
    }
  }

  printf("Tareas por hacer, ordenadas por prioridad y precedencia:\n");
  Nodo *auxTareas = firstList(listaTareas);
  while (auxTareas != NULL) {
    printf("Tarea: %s, Prioridad: %d", auxTareas->nombreTarea, auxTareas->prioridad);

    Nodo *auxAdj = firstList(auxTareas->adj_edges);
    if (auxAdj != NULL) {
      printf(" - Precedente:");
      while (auxAdj != NULL) {
        printf(" %s", auxAdj->nombreTarea);
        auxAdj = nextList(auxTareas->adj_edges);
      }
    }

    printf("\n");
    auxTareas = nextList(listaTareas);
  }

  free(monticulo);
}

void eliminarTarea(Map* grafo,Nodo* nodoTarea, char* nombreTarea, Stack* stack) {

  eraseMap(grafo, nombreTarea);

  Nodo* auxTarea = firstMap(grafo);
  while(auxTarea != NULL) {
    Nodo* auxAdj = firstList(auxTarea->adj_edges);
    while(auxAdj != NULL) {
      if (strcmp(auxAdj->nombreTarea, nombreTarea) == 0) {
        popCurrent(auxTarea->adj_edges);
      }
      auxAdj = nextList(auxTarea->adj_edges);
    }
   auxTarea = nextMap(grafo); 
  }


  Pila* registro = (Pila *) malloc(sizeof(Pila));
  registro->accion = ELIMINAR_TAREA;
  
  strcpy(registro->nombre, nodoTarea->nombreTarea);
  registro->auxNodo = nodoTarea;
  stack_push(stack, registro);

  printf("La tarea '%s' ha sido marcada como completada.\n", nombreTarea);

}

void marcarTareaCompletada(Map *grafo, Stack *stack) {
  
  char tarea[32];
  printf("Ingrese el nombre de la tarea completada: ");
  leerCadena(tarea);

  Nodo *nodoTarea = searchMap(grafo, tarea);
  if (nodoTarea == NULL) {
    printf("La tarea '%s' no existe dentro del programa.\n", tarea);
    return;
  }

  if (firstList(nodoTarea->adj_edges) != NULL) {
    printf("La tarea '%s' tiene relaciones de precedencia con otras tareas.\n",tarea);
    printf("¿Estás seguro que deseas eliminar la tarea?\n");
    printf("  === Ingrese 1 para SÍ , 0 para NO ===\n");
    int respuesta;
    scanf("%d", &respuesta);
    if (respuesta == 0) {
      printf("Operación cancelada. La tarea no ha sido eliminada.\n");
      return;
    }
    if(respuesta == 1) {
      eliminarTarea(grafo, nodoTarea, tarea, stack);
    }
    
  } else eliminarTarea(grafo, nodoTarea, tarea, stack);
}

void deshacerAccion(Map *grafo, Stack *stack) {
  if(stack_top(stack) == NULL) {
    printf("No se ha registrado ninguna acción reciente!\n");
    return;
  }

  Pila *aux = stack_pop(stack);
  
  switch(aux->accion) {
    case AGREGAR_TAREA: 
      if (searchMap(grafo, aux->nombre) != NULL) {
        eraseMap(grafo, aux->nombre);
        break;
      }
    case ELIMINAR_TAREA:
      if (searchMap(grafo, aux->nombre) == NULL) {
        insertMap(grafo, aux->nombre, aux->auxNodo);
        break;
      }
    case PRECEDENCIA:
      if (searchMap(grafo, aux->nombre) != NULL && searchMap(grafo, aux->auxNodo->nombreTarea) != NULL) {
        Nodo* aux2 = firstList(aux->auxNodo->adj_edges);
        while (aux2 != NULL) {
          if (strcmp(aux2->nombreTarea, aux->nombre) == 0) {
            popCurrent(aux->auxNodo->adj_edges);
            break;
          }
          nextList(aux->auxNodo->adj_edges);
        }
      }
  }
}

void importarArchivo(Map* grafo) {
  FILE* archImportar;
  char nombreArchivo[100];
  printf("Ingrese el nombre del archivo del cual desea importar tareas en formato .csv\n");
  scanf("%s", nombreArchivo);

  archImportar = fopen(nombreArchivo, "r");
  printf("Se han importado los datos al archivo %s\n",nombreArchivo);
  if (!archImportar) {
    printf("Error: no se pudo abrir el archivo\n");
    return;
  }
  
  char delimit[]=" \t\r\n\v\f"; 
  char linea[1024];
  fgets(linea,1024,archImportar);
  
  while (fgets(linea,1024,archImportar) != NULL){
    Nodo* nuevaTarea = NULL;
    nuevaTarea = (Nodo *)malloc(sizeof(Nodo));
    nuevaTarea->adj_edges = createList();
    char* precedente = "";
    int contador = 1;
    for(int i = 0 ; i < 3 ; i++) {
      char* aux = get_csv_field(linea, i);
      switch(i) {
        case 0:
          strcpy(nuevaTarea->nombreTarea, aux);
          break;

        case 1:
          nuevaTarea->prioridad = atoi(aux);
          break;

        case 2:
          precedente = strtok(aux, delimit);
          while(precedente != NULL) {
            Nodo *aux2 = searchMap(grafo, precedente);
            if (aux2 != NULL) pushBack(nuevaTarea->adj_edges, aux2);
            precedente = strtok(NULL, delimit);
          }
          break;
        
        default:
          break;
      }
    }
    insertMap(grafo, nuevaTarea->nombreTarea, nuevaTarea);
  }
  fclose(archImportar);
}

int main() {
  Map *grafo = createMap(is_equal_string);
  Stack *stack = stack_create();

  int opcion, titulo = 0;

  while (1) {
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
    printf("6. Importar un archivo .csv de tareas\n");
    printf("7. Salir del programa\n");
    printf("=========================================\n");

    scanf("%d", &opcion);
    while (opcion < 1 || opcion > 7) {
      scanf("%d", &opcion);
    }
    getchar();
    switch (opcion) {
    case 1:
      agregarTarea(grafo, stack);
      break;
    case 2:
      establecerPrecendencia(grafo, stack);
      break;
    case 3:
      mostrarTareas(grafo);
      break;
    case 4:
      marcarTareaCompletada(grafo, stack);
      break;
    case 5:
      deshacerAccion(grafo, stack);
      break;
    case 6:
      importarArchivo(grafo);
      break;
    case 7:
      printf("Cerrando el programa...\n");
      return 0;
    }
  }
  return 0;
}