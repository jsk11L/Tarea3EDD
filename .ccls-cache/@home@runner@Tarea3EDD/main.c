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
  List *listaPrecedencias;
} Nodo;

typedef enum Accion {
//Esto sirve para la pila de deshacer acciones
  AGREGAR_TAREA,
  ELIMINAR_TAREA,
  PRECEDENCIA
} Accion;

typedef struct Pila {
  char nombre[32];
  Accion accion;
  Nodo* auxNodo;
  List *precedentes;
} Pila;

const char *get_csv_field (char * tmp, int k){
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

int is_equal_string(void *key1, void *key2){
  if (strcmp((char *)key1, (char *)key2) == 0)
    return 1;
  return 0;
}

void leerCadena(char cadena[31]){
  //Función para simplemente leer strings
  scanf("%30[^\n]s", cadena);
  getchar();
}

void agregarTarea(Map *grafo, Stack *stack){
  //Inicializamos variables para guardar datos
  Nodo *nuevaTarea = (Nodo *)malloc(sizeof(Nodo));
  nuevaTarea->listaPrecedencias = createList();
  char tarea[32];
  int prioridad;
  
  //Leemos nombre y prioridad de la tarea, si ya existía una con el mismo nombre, se enviará error
  printf("Ingrese el nombre de la tarea: ");
  leerCadena(tarea);
  while(searchMap(grafo, tarea) != NULL) {
    printf("Ese nombre ya está ocupado. Elige otro: ");
    leerCadena(tarea);
  }
  printf("Ingrese la prioridad de la tarea: ");
  scanf("%d", &prioridad);

  //Ingresamos la tarea al mapa o grafo
  strcpy(nuevaTarea->nombreTarea, strdup(tarea));
  nuevaTarea->prioridad = prioridad;
  insertMap(grafo, nuevaTarea->nombreTarea, nuevaTarea);

  //Añadimos la acción reciente al registro en caso de que el usuario quiera deshacer la acción
  Pila* registro = (Pila *) malloc(sizeof(Pila));
  registro->accion = AGREGAR_TAREA;
  strcpy(registro->nombre, tarea);
  stack_push(stack, registro);
}

void establecerPrecendencia(Map *grafo, Stack *stack){
  //Inicializamos variables para guardar datos
  char tarea1[32];
  char tarea2[32];

  //Pedimos al usuario la tarea 1 que predecerá a la tarea 2, y buscamos que esté efectivamente en el mapa
  printf("Ingrese el nombre de la tarea que precede: ");
  leerCadena(tarea1);
  if (searchMap(grafo, tarea1) == NULL) {
    printf("ERROR: La tarea '%s' no existe dentro del programa. Ingrese otra:", tarea1);
    leerCadena(tarea1);
  }

  //Hacemos el mismo proceso con la tarea 2 y verificamos que se encuentre en el mapa y que no repita la misma tarea anterior para evitar errores
  printf("Ingrese el nombre de la tarea a preceder: ");
  leerCadena(tarea2);
  
  while (1) {
    if(strcmp(tarea1,tarea2)==0) printf("ERROR: No puede elegir la misma tarea. Ingrese otra ");
    if(searchMap(grafo, tarea2) == NULL) printf("ERROR: La tarea '%s' no existe dentro del programa. Ingrese otra: ", tarea2);
    if(strcmp(tarea1,tarea2) != 0 && searchMap(grafo,tarea2) != NULL) break;
    leerCadena(tarea2);
  }

  //Asignamos la precedencia
  Nodo *aux1 = searchMap(grafo, tarea1);
  Nodo *aux2 = searchMap(grafo, tarea2);
  pushBack(aux2->listaPrecedencias, aux1);
  
  //Guardamos un registro en caso de que el usuario quiera deshacer su acción
  Pila* registro = (Pila *) malloc(sizeof(Pila));
  registro->accion = PRECEDENCIA;
  strcpy(registro->nombre , tarea1);
  registro->auxNodo = aux2;
  stack_push(stack, registro);
}

void reestablecerBooleanos(Map *grafo){
  //Función que reestablece los booleanos de mostrarTareas, para que cada vez que se quiera mostrar las tareas, vuelvan al estado normal y todo funcione correctamente
 Nodo *tarea = firstMap(grafo);
    
  while(tarea != NULL) {
    tarea->explorado = false;
    Nodo *nodoAdj = firstList(tarea->listaPrecedencias);
      
    while(nodoAdj != NULL) {
      nodoAdj->visitado = false; 
      nodoAdj = nextList(tarea->listaPrecedencias);
    }
          
    tarea = nextMap(grafo);
  }
}

void nodosCompletados(Map *grafo, char *nombreNodo){
  //Función que marca los nodos completados y asigna el booleano visitado a true
  Nodo *tarea = firstMap(grafo);
  
  while(tarea != NULL) {
  
    Nodo *nodoAdj = firstList(tarea->listaPrecedencias);

    while(nodoAdj != NULL) {
      if(strcmp(nodoAdj->nombreTarea, nombreNodo) == 0) {
        nodoAdj->visitado = true;
      }
      nodoAdj = nextList(tarea->listaPrecedencias);
    }
          
    tarea = nextMap(grafo);
  }
}

void mostrarTareas(Map *grafo){
  //Crearemos el montículo para ordenar y la Lista de Tareas para irlas guardando por orden
  Heap *monticulo = createHeap();
  List *listaTareas = createList();

  //Reestablecemos los booleanos en caso de que se haya usado la función anteriormente
  reestablecerBooleanos(grafo);

  //Primero obtenemos los nodos del grafo/mapa que no tienen precedentes y los ingresamos al montículo
  Nodo *aux = firstMap(grafo);
  while(aux != NULL) {
    if (firstList(aux->listaPrecedencias) == NULL) {
      heap_push(monticulo, aux->nombreTarea, aux->prioridad);
      aux->explorado = true;
    }
    aux = nextMap(grafo);
  }

  //Ahora recorreremos el montículo
  while(heap_top(monticulo) != NULL) {
    //Quitamos el primer nodo del montículo y lo agregamos a la lista de Tareas, va primero porque no tiene precedentes
    Nodo *actualTarea = heap_top(monticulo);
    heap_pop(monticulo);
    pushBack(listaTareas, actualTarea);

    //Llamamos a nodosCompletados para que marque el nodo actual como completado en todas las listas de precedencia de otros nodos
    nodosCompletados(grafo, actualTarea->nombreTarea);

    //Ahora buscamos los nuevos nodos que no tendrán precedencia, o que si tienen pero su precedencia ya fue visitada, y los agregamos al mon´ticulo para continuar el ciclo y recorrer todo el mapa.
    Nodo *aux = firstMap(grafo);
    while(aux != NULL) {
      if (aux->explorado != true) {
        bool tienePrecedencia = false;

        Nodo *aux2 = firstList(aux->listaPrecedencias);
        while(aux2 != NULL) {
          if (aux2->visitado != true) {
            tienePrecedencia = true;
            break;
          }
          aux2 = nextList(aux->listaPrecedencias);
        }

        if (tienePrecedencia != true && aux->explorado != true) {
          heap_push(monticulo, aux, aux->prioridad);
          aux->explorado = true;
        }
      }
      aux = nextMap(grafo);
    }
  }

  //Finalmente se imprimen las tareas ordenadas por prioridad y precedencia
  printf("Tareas por hacer, ordenadas por prioridad y precedencia:\n");
  Nodo *auxTareas = firstList(listaTareas);
  while (auxTareas != NULL) {
    printf("Tarea: %s (Prioridad: %d)", auxTareas->nombreTarea, auxTareas->prioridad);

    //Hacemos un ciclo para imprimir las precedencias
    Nodo *auxAdj = firstList(auxTareas->listaPrecedencias);
    if (auxAdj != NULL) {
      printf(" - Precedente:");
      while (auxAdj != NULL) {
        printf(" %s", auxAdj->nombreTarea);
        auxAdj = nextList(auxTareas->listaPrecedencias);
      }
    }

    printf("\n");
    auxTareas = nextList(listaTareas);
  }

  //Liberamos la memoria del montículo
  free(monticulo);
}

void eliminarTarea(Map* grafo,Nodo* nodoTarea, char* nombreTarea, Stack* stack){
  //Eliminamos la tarea correspondiente del mapa/grafo
  eraseMap(grafo, nombreTarea);

  //Creamos la lista temporal para guardar los nodos que tenian de precedente a la tarea a eliminar
  List *listaTemporal = createList();

  //Ahora revisamos las demás tareas para eliminar las precedencias a la tarea ingresada     previamente
  Nodo* auxTarea = firstMap(grafo);
  while(auxTarea != NULL) {
    Nodo* auxAdj = firstList(auxTarea->listaPrecedencias);
    while(auxAdj != NULL) {
      if (strcmp(auxAdj->nombreTarea, nombreTarea) == 0) {
        pushBack(listaTemporal, auxTarea);
        popCurrent(auxTarea->listaPrecedencias);
      }
      auxAdj = nextList(auxTarea->listaPrecedencias);
    }
   auxTarea = nextMap(grafo); 
  }

  //Se crea un registro por si el usuario decide deshacer su acción
  Pila* registro = (Pila *) malloc(sizeof(Pila));
  registro->accion = ELIMINAR_TAREA;
  strcpy(registro->nombre, nodoTarea->nombreTarea);
  registro->auxNodo = nodoTarea;
  registro->precedentes = listaTemporal;
  stack_push(stack, registro);

  printf("La tarea '%s' ha sido marcada como completada.\n", nombreTarea);
}

void marcarTareaCompletada(Map *grafo, Stack *stack){
  //Le pedimos la tarea al usuario y verificamos que se encuentre en el mapa
  char tarea[32];
  printf("Ingrese el nombre de la tarea completada: ");
  leerCadena(tarea);
  Nodo *nodoTarea = searchMap(grafo, tarea);
  if (nodoTarea == NULL) {
    printf("La tarea '%s' no existe dentro del programa.\n", tarea);
    return;
  }

  //Recorreré el mapa para ver si cualquier nodo tiene de precedente a la tarea
  bool flag = false;
  Nodo* auxTarea = firstMap(grafo);
  while(auxTarea != NULL) {
    Nodo* auxAdj = firstList(auxTarea->listaPrecedencias);
    while(auxAdj != NULL) {
      if (strcmp(auxAdj->nombreTarea, nodoTarea->nombreTarea) == 0){
        flag = true;
        break;
      }
    }
   if (flag == true) break;
   auxTarea = nextMap(grafo); 
  }
  
  if (firstList(nodoTarea->listaPrecedencias) != NULL || flag == true) {
    printf("La tarea '%s' tiene relaciones de precedencia con otras tareas.\n",tarea);
    printf("¿Estás seguro que deseas eliminar la tarea?\n");
    printf("Ingrese 1 para SÍ , 0 para NO: ");
    int respuesta;
    scanf("%d", &respuesta);
    if (respuesta == 0) {
      printf("La tarea no se eliminará\n");
      return;
    }
  }
  eliminarTarea(grafo, nodoTarea, tarea, stack);
}

void deshacerAccion(Map *grafo, Stack *stack){
  //Si la cola está vacía, eviamos el mensaje de que no ha habido ninguna acción
  if(stack_top(stack) == NULL) {
    printf("No hay ninguna acción que deshacer.\n");
    return;
  }

  //Creamos un switch para ver que acción vamos a deshacer
  Pila *aux = stack_pop(stack);
  switch(aux->accion) {
    case AGREGAR_TAREA: 
      //Buscamos la tarea agregada y la eliminamos
      if (searchMap(grafo, aux->nombre) != NULL) {
        eraseMap(grafo, aux->nombre);
        break;
      }
    case ELIMINAR_TAREA:
      //Restauramos la tarea eliminada
      if (searchMap(grafo, aux->nombre) == NULL) {
        insertMap(grafo, aux->nombre, aux->auxNodo);
      }
      //Ahora recorremos la lista de precedentes en caso de que haya y volvemos a añadir la tarea a las listas de precedencia donde estaba previamente
      Nodo* aux2 = firstList(aux->precedentes);
      while(aux2 != NULL){
        Nodo* aux3 = searchMap(grafo,aux2->nombreTarea);
        pushBack(aux3->listaPrecedencias,aux);
        aux2 = nextList(aux->precedentes);
      }
      break;
    case PRECEDENCIA:
      //Volvemos a asignar las precedencias correspondientes
      if (searchMap(grafo, aux->nombre) != NULL && searchMap(grafo, aux->auxNodo->nombreTarea) != NULL) {
        Nodo* aux2 = firstList(aux->auxNodo->listaPrecedencias);
        while (aux2 != NULL) {
          if (strcmp(aux2->nombreTarea, aux->nombre) == 0) {
            popCurrent(aux->auxNodo->listaPrecedencias);
            break;
          }
          nextList(aux->auxNodo->listaPrecedencias);
        }
      }
  }
}

void importarArchivo(Map* grafo){
  //Le pedimos al usuario el nombre del archivo a importar
  FILE* archImportar;
  char nombreArchivo[100];
  printf("Ingrese el nombre del archivo en formato .csv: ");
  scanf("%s", nombreArchivo);

  //Verificamos si el archivo se pudo abrir correctamente
  archImportar = fopen(nombreArchivo, "r");
  printf("Se han importado los datos al archivo %s\n",nombreArchivo);
  if (!archImportar) {
    printf("Error: no se pudo abrir el archivo\n");
    return;
  }

  //delimit sirve para usar strtok() y que pare en todos los espacios blancos, para separar correctamente las precedencias
  char delimit[]=" \t\r\n\v\f"; 
  //Ahora empezamos a importar el archivo
  char linea[1024];
  fgets(linea,1024,archImportar);
  
  while (fgets(linea,1024,archImportar) != NULL){
    //Asignamos memoria a un nuevoNodo que almacenará los datos de cada línea del archivo y a una cadena precedente, que almacenará los precedentes por separado
    Nodo* nuevaTarea = NULL;
    nuevaTarea = (Nodo *)malloc(sizeof(Nodo));
    nuevaTarea->listaPrecedencias = createList();
    char* precedente = "";

    //Entramos al for de la función get_csv_field y a sus casos correspondientes
    for(int i = 0 ; i < 3 ; i++) {
      char* aux = get_csv_field(linea, i);
      switch(i) {
        case 0:
          //Lo primero será ingresar el nombre de la tarea
          strcpy(nuevaTarea->nombreTarea, aux);
          break;

        case 1:
          //Luego la prioridad
          nuevaTarea->prioridad = atoi(aux);
          break;

        case 2:
          //Ahora hacemos un ciclo, mientras precedente no sea nulo, significa que se registró una precedencia y hay que ingresarla, cuando precedente sea nulo, terminará el ciclo y no quedan precedencias a ingresar.
          precedente = strtok(aux, delimit);
          while(precedente != NULL) {
            Nodo *aux2 = searchMap(grafo, precedente);
            if (aux2 != NULL) pushBack(nuevaTarea->listaPrecedencias, aux2);
            precedente = strtok(NULL, delimit);
          }
          break;
        
        default:
          break;
      }
    }
    //Ingresamos al nodo al mapa/grafo y leeremos una nueva linea
    insertMap(grafo, nuevaTarea->nombreTarea, nuevaTarea);
  }
  //Finalmente cerramos el archivo 
  fclose(archImportar);
}

int main(){
  //Creamos el mapa para la tarea y la pila para deshacer acciones
  Map *grafo = createMap(is_equal_string);
  Stack *stack = stack_create();

  //La variable opcion es para elegir en el menú y la de titulo es solamente estética
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
    //Ahora según opción haremos las opciones del menú
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