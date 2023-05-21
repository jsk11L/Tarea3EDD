# Tarea3EDD

- Crear perfil de jugador/a (nombre) ✔
- Mostrar perfil de jugador/a (nombre) ✔
- Agregar item a jugador/a (nombre, item) ✔
- Eliminar item de jugador/a (nombre, item) ✔
- Agregar puntos de habilidad a el/la jugador/a (nombre, puntos) ✔
- Mostrar jugador@s con item específico (item) ✔
- Deshacer última acción de jugador/a (nombre) ✔
- Exportar datos de jugadores a archivo de texto (nombre_archivo) ✔
- Cargar datos de jugadores desde un archivo de texto (nombre_archivo) ✔

===== 1 - COMO COMPILAR Y EJECUTAR LA TAREA =====

Lo primero que hay que hacer, es en el repositorio de GitHub, apretar el botón que dice **< > Code** y copiar el link que se encuenta abajo de donde dice HTTPS. Nosotros usamos Replit asi que lo que hicimos despúes fue crear un nuevo replit, y seleccionar la opción **Import from Github**, y pegar el link previamente copiado en la sección GitHub URL, luego seleccionamos el botón azul **Import from Github** y el código se importará automáticamente, también, solamente con apretar **Run** el código ya se compila solo y funciona correctamente.

===== 2 - LO QUE FUNCIONA Y LO QUE NO =====

Todo funciona correctamente, como es esperado, si se necesita un bitácora podría comentar que se necesito un cambio a grandes rasgos de los TDA's a la hora de crear al función deshacer, a pesar de la dificultad, no hubieron inconvenientes :).

PD: Se nos olvido hacer commits pero deberiamos tener minimo 7 cada uno, principalmente porque la implentación a GitHub fue posterior a la creación del replit, así que la versión actual es una copia, cuando 5/9 funciones ya estaban creadas y funcionando. 
  

===== 3 - HERRAMIENTAS UTILIZADAS =====

Las herramientas usadas en esencia fueron:

- Replit [https://replit.com/] : Se logró el trabajo en equipo mediante replit, donde pudimos analizar y cambiar grupalmente el código.
- GitHub [https://github.com] : Creamos el repositorio y lo vinculamos a replit, asi teniendo exitosamente un lugar para ver todos los commits y cambios realizados en todos los aspectos de la tarea, como el "main.c", "list_answer.c", "list.h", "map.c/h" y "stack.h", los ".csv" y el README.

===== 4 - COEVALUACIÓN DEL TRABAJO EN EQUIPO =====

Aspectos a mejorar: Mejorar los horarios de comunicación entre los integrantes, cada uno tubo su aporte esta vez, pero hubieron deficiencias en la comunicación del trabajo a la hora de avanzar.

Grupo = Clippers 1
  Participación activa y equitativa = 20
  Resolución de conflictos = 20
  Colaboración = 20
  Responsabilidad = 20
  Comunicación = 15
  Pje Total = 95
----------------------------------------
  
===== 5 - COEVALUACIÓN DEL APORTE INDIVIDUAL =====

Aspectos positivos: Organización, comunicación, disposición, se preocupa por la estabilidad de sus compañeros.

A mejorar: Manejo de conocimientos, mayor participación activa en los trabajos.

Estudiante: Francisca Abarca
  Participación = 15
  Responsabilidad = 20
  Comunicación = 20
  Calidad del Trabajo = 20
  Trabajo en Equipo = 20
  Pje Total = 95
------------------------------

Aspectos positivos: Buen lider, maneja bien los conocimientos y es atento con las inquitudes del equipo. Se dió a la oportunidad de la colaboración grupal.

A mejorar: Comprensión, comunicación bajo desafíos.

Estudiante: Javier Sepúlveda
  Participación = 20
  Responsabilidad = 20
  Comunicación = 15
  Calidad del Trabajo = 20
  Trabajo en Equipo = 20
  Pje Total = 95
------------------------------

===== 6 - ANALISIS DEL CÓDIGO =====

Usaremos la siguiente estructura

````c
typedef struct {
  char nombreTarea[32];
  int prioridad;
  bool visitado;
  bool explorado;
  List *adj_edges;
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
} Pila;
````

Primordialmente para entender la funciones, es necesario entender el **main**, para ello se crea un mapa para implementar la tarea, que incluirá a cada tarea de manera independiente dentro del mapa con su struct correspondiente, el cuál incluye su nombre, su prioridad, entre otros. Y también se crea una pila especificamente para la función deshacer acción.

Luego de eso, presentamos el menú de bienvenida, para realizar cada opción que desee el usuario, usamos variables como 'opcion' y 'titulo' para saber las opciones a elegir del usuario, además de poseer sus restricciones correspondientes para no generar errores, implementamos cada función con un ciclo switch asignando así a cada caso su función correspondiente, y la ultima opcion termina el programa.

En la primera función **agregarTarea** que recibe un mapa y una pila, se creara una nueva tarea tipo 'nodo', y realizamos la reserva de memoria, además de un char y un entero. Posteriormente se le solicita un nombre para la tarea y utilizamos la función anteriormente creada 'leerCadena', luego utilizamos la función 'searchMap' para asegurarse que no exista la misma tarea y sea posible guardar y se le solicitará al usuario otro nombre, en consecuencia se le solicitará al usuario una prioridad a asignar a la tarea correspondiente de este modo agregarla al mapa. Ahora se le asigna dicho nombre y prioridad directamente al mapa con el nodo anteriormente creado. Añadimos la acción reciente al registro en caso de que el usuario quiera deshacer la acción mediante la pila (en el struct 'Acción') realizando una reserva de memoria.

En la función **establecerPrecendencia** que recibe un mapa y una pila, inicializamos variables para guardar datos tipo 'char', se le solictará al usuario la tarea 1 que en escencia será un prerequisito a la tarea 2, y buscamos que ambos existan efectivamente en el mapa usando la función 'leerCadena' primeramente para la tarea 1 y consecutivamente la tarea 2. Luego se implementará un ciclo 'while' infinito que le hará verificar al usuario que la tareas no sean las mismas y que las tareas existan, de lo contrario se realiza un "break". Asignamos la precedencia, para esto usaremos dos auxiliar tipo 'nodo', donde cada auxiliar corresponde a una busqueda individual de cada tarea correspondiente, para posteriormente con los auxiliares adjuntarlos a la lista 'listaPrecedencias' de cada nodo. Finalmente guardamos un registro en caso de que el usuario quiera deshacer su acción mediante la pila (en el struct 'Acción') realizando una reserva de memoria.

La función **mostrarTareas** que recibe el mapa, empezaremos creando un montículo tipo 'Heap' para ordenar y una lista tipo 'List' de Tareas para irlas guardando por orden, antes de ordenarlos se reestablecen los booleanos en caso de que se haya usado la función anteriormente. Primero obtenemos los nodos del mapa que no poseen precedentes y los ingresamos al montículo, ya que estos se realizan directamente, para esto crearemos un auxiliar tipo 'nodo' es cual se le asignará la primera posición del mapa (grafo), el auxiliar se le irá asignando las tareas por su prioridad, siempre recorriendo desde la primera posición y avanzando. Ahora recorreremos el montículo, quitamos el primer nodo del montículo y lo agregamos a la lista de Tareas, va primero porque no tiene precedentes, llamamos a nodosCompletados para que marque el nodo actual como completado en todas las listas de precedencia de otros nodos. Ahora buscamos los nuevos nodos que no tendrán precedencia, o que si tienen pero su precedencia ya fue visitada, y los agregamos al monticulo para continuar el ciclo y recorrer todo el mapa. Finalmente se imprimen las tareas ordenadas por prioridad y precedencia, y hacemos un ciclo para imprimir las precedencias, por último liberamos la memoria del montículo.

En la funcion **marcarTareaCompletada**, la cual recibe un mapa y una pila, le pediremos al usuario la tarea que desea marcar como completada, y verificamos que se encuentre en el mapa con la función 'leerCadena' y se la asignamos a una variable tipo 'nodo' es cuales buscara en el mapa si la tarea efectivamente existe. Luego de verificar, recorreremos el mapa para verificar si los nodos existentes poseen prrecendentes/prerequisitos, crearemos una variable 'bool', y un auxiliar tipo 'nodo' que se iniciará a la primera posición del 'grafo' para recorrer cada posición, luego en el ciclo "while" crearemos una variable auxiliar de precedentes tipo 'nodo', de este modo excederemos a la lista de precedencias y verificamos si la tarea a marcar como completada posee un prerequisito, para esto nuestra variable 'bool' pasa a ser verdadera si posse un prerequsito, de ser lo contrario no sucederá lo del cambio de dato en el a variable 'bool'. Luego verificamos preguntandole al usuario si está seguro de marcar como completeda está tarea, dependiendo de la respuesta del usuario se eliminará con la función 'eliminarTarea' o simplemente de dejará como está.

En la funcion **deshacerAccion**, la cual recibe un mapa y una pila, primeramente se verifica que existan elementos posicionandonos con la función 'stack_top' en el elemento de más arriba. Luego creamos un 'switch' para ver que acción vamos a deshacer, para esto crearemos una variable auxiliar tipo 'Pila' que nos retornará la primera ocurrencia entonces, en la implementación del switch eligieremos una accion. Si selecciona 'AGREGAR_TAREA' buscamos la tarea agregada y la eliminamos, para esto con la función 'searchMap' buscamos la tarea y la eliminamos con la función 'eraseMap'. Sin embargo, si se selecciona 'ELIMINAR_TAREA' restauramos la tarea eliminada para esto con la función 'searchMap' buscamos la tarea y la eliminamos con la función 'insertMap', finalmnete recorreremos la lisat de precedencias en caso de que la tarea a volver a implementar poseía con prerequisito, para agregarla como lo era previamente, para esto creamos otro auxiliar tipo 'nodo', que recorrera la lista de precedentes de dicha tarea, para luego con otro auxiliar (este siendo el tercero), del mismo tipo buscaremos las tareas a implementar que correspondián a las precedencias de la tarea a volver a implementar para de este modo agregarlas. Ahora si seleccionamos 'PRECEDENCIA' volvemos a asignar las precedencias correspondientes, para esto implementamos un 'if' que verifica que la tarea con dichas precedencias existe en el mapa (grafo) y que el nodoAuxiliar en esta tarea también exista, crearemos un auxiliar tipo 'nodo', que se posicionará en la primera posición de la lista de precedencias y luego poscisionaremos los prerequisitos a como correspondían.

Para la funcion **importarArchivo**, la cual recibe un mapa, iniciamos creamos una variable tipo 'FILE' para importar un archivo  y una 'char' para el nombre de ste, se le solictará al usuario el nombre de este. Verificamos si el archivo se pudo abrir correctamente usando la función 'fopen' y se le hará saber al usuario, si el archivo a importar arroja errores también se le hará saber al usuario. Iniciamos usando la variable 'delimit' de tipo 'char', ya que sirve para usar strtok() y que pare en todos los espacios blancos, para separar correctamente las precedencias, empezamos delimitando la cantidad de caracteres que posee una línea del archivo con una variable 'char' llamada "linea" para de este modo usar la función 'fgets'. Introduciremos un ciclo ciclo que funcionará mientras en la función 'fgets' sea distinto de "NULL". Asignamos memoria a un nuevoNodo que almacenará los datos de cada línea del archivo y a una cadena precedente, que almacenará los precedentes por separado, para este iniciaremos una variables tipo 'Nodo' y realizaremos la reserva de memoria correspondiente, y entramos al for de la función get_csv_field y a sus casos correspondientes y restrigiremos con un 'for', crearemos una variable 'char' para extraer cada dato correspondiente. Ahora con un 'switch' asignaremos cada caso al dato que deberá ingresar al sistema de tareas. El primer caso corresponde al nombre de la tarea como tal, para esto simplemente usamos la función 'strcpy'. El segundo caso corresponde a la prioridad de dicha tarea y usaremos la función 'atoi' para almacenar dicho dato. Y el último caso corresponde a los precedentes (si es que posee) y lo extraeremos con la función 'strtok', hacemos un ciclo, mientras precedente no sea nulo, significa que se registró una precedencia y hay que ingresarla, para esto crearemos una variable auxiliar tipo 'nodo' que buscará en el mapa la tarea a asignarle el precedente, en cambio cuando precedente sea nulo, terminará el ciclo y no quedan precedencias a ingresar. Finalmente insertamos el nodo al mapa (grafo) con el que hemos estado trabajando, y pasará a la siguiente línea del archivo y cerramos el archivo.

Funciones complementarias:
**is_equal_string** corresponde al comparación de strings.
**leerCadena** como dice su nombre función para simplemente leer strings
**reestablecerBooleanos** reestablecerá los booleanos de mostrarTareas, para que cada vez que se quiera mostrar las tareas, vuelvan al estado normal, en caso de ser necesarios utilizarlos y todo funcione correctamente
**nodosCompletados** función que marca los nodos completados y asigna el booleano visitado a true
**eliminarTarea** como dice su nombre se usará para eliminar la tarea correspondiente del mapa/grafo

De este modo el código a lo largo de la construcción e implementación de TDA's fue un proceso y ánalisis de cada ejecución que procesaba el Replit, además de la construcción de funciones tanto principales como complementarias.