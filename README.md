# E4-Demonstración de Paradigma de Programación

Ángel David Candelario Rolon
A01712387

## Introducción
El proyecto que realice esta basado en un proyecto que fue hecho durante el bloque de Construcción de Software y toma de Desiciones. Nuestro Socio Formador era Nuclea Solutions, quien necesitaba de un sistema para poder gestionar su departamento de Recursos Humanos, una de las necesidades a cubrir era la gestión de las solicitudes de vacaciones realizadas por su colaboradores. Entonces se me ocurrió crear la siguiente situación:

Una empresa requiere de un sistema para poder optimizar el departamento de Recursos Humanos. El departamento ya cuenta con un control para diferentes tareas y reportes administrativos a realizar, sin embargo, se requiere de un sistema para las solicitudes de días de vacaciones enviadas por su equipo de trabajo. Estas tienen que ser aprobadas o rechazadas acorde a sus días libres disponibles. El sistema debe garantizar:

 - Correcto proceso de aceptación/rechazo 
 - Verificación de los días libres disponibles
 - Manejo multiple de solicitudes

## Paradigma de Concurrencia
Para el desarrollo de este proyecto se utilizo el paradigma de **programación concurrente**. La concurrencia mejora el rendimiento en la ejecución de multiples tareas que se generan simultaneamente. Las tareas no se ejecutan una por una o de manera secuencial y no necesariamente lleva un orden, lo que sucede es que se ejecutan varias tareas a la vez.

Bajo el contexto de esta situación, podemos ver cada usuario representado mediante un thread. Estas solicitudes son enviadas a los controladores, los cuales se encargan de gestionar los recursos virtualizados.

Este enfoque me permitio simular multiples procesos que actuan al mismo tiempo, simulando el flujo natural de un equipo de Recursos Humanos procesando las solicitudes de su organización.

Para poder implementar esta situación y el paradigma se uso el lenguaje de programación C++ y con ello las bibliotecas de:
 - Thread (```std::thread```) - Permite crear multiples hilos de ejecución para procesar cada una de las solicitudes 
 - Mutex (```std::mutex```) - Permite proteger recursos compartidos para que no se usen simultaneamente y que unicamente un solo thread trabaje sobre la funcion.

Con estas bases se pudo implementar un sistema capaz de manejar de manera concurrente y segura cada una de las solicitudes, sin tener errores de sincronización e incosistencias.

## Modelo - Estructura del sistema

### Diagrama de flujo
![image](https://github.com/user-attachments/assets/520fc84f-6478-499c-b419-21cf0f047956)


### Componentes principales
 - **Usuarios** (Empleados) -> Cada usuario o empleado cuenta con una cantidad inicial de 10 días disponibles, se representa con los threads.
 - **Solicitudes** (Request) -> Cada solicitud tiene un **ID del empleado**, **Número de días que desea solicitar** los cuales se generan con valores de prueba predefinidos.
- **Controladores**  
  Procesan las solicitudes de vacaciones. El caso base cuenta con **5 controladores** que están disponibles para atender solicitudes de forma concurrente. Cada controlador:
  - Verifica si hay días disponibles
  - Aprueba o rechaza la solicitud
  - Actualizar su status (si esta ocupado o no)
  - Actualizar los días disponibles restantes de cada usuario
 - **Gestor de Controladores**  
  Administra el acceso a los controladores disponibles. Se asegura de:
   - Asignar solicitudes de manera sincronizada
   - Evitar conflictos y condiciones de carrera usando **`std::mutex`**
   - Simular el uso concurrente de recursos limitados de forma segura y eficiente.

### Flujo de operación
1. **Inicialización del sistema**  
   - Se crean **20 usuarios**, cada uno con **10 días disponibles**.
   - A cada usuario se le asigna una **solicitud predeterminada** de vacaciones (cantidad variable de días).

2. **Generación concurrente de solicitudes**  
   - Se lanza un **hilo por cada usuario**, simulando el envío simultáneo de solicitudes.

3. **Asignación de controlador**  
   - Cada hilo busca un **controlador libre** a través del gestor.
   - Si todos los controladores están ocupados, el hilo **espera** antes de volver a intentarlo.

4. **Procesamiento de la solicitud**  
   - El controlador verifica si el usuario tiene suficientes días disponibles:
     - Si **sí los tiene**, aprueba la solicitud y descuenta los días.
     - Si **no los tiene**, la solicitud es rechazada.

5. **Finalización de la solicitud**  
   - El controlador vuelve a quedar libre.
   - El hilo del usuario finaliza su ejecución.

6. **Resumen de resultados**  
   Al terminar todos los hilos, el sistema muestra:
   - **Resumen Final de Solicitudes**: Los días restantes por usuario.
   - **Resumen de Controladores**: Cantidad de solicitudes procesadas por cada controlador.
   - **Resultado General**: Cantidad de solicitudes aprobadas / rechazadas / en total.

# Pruebas
El objetivo de las pruebas es poder visualizar la **distribución individual de cada una de las solicitudes generadas en los controladores**, ademas de ver cuantas **solicitudes fueron aprobadas o rechazadas** de acuerdo a la disponibilidad de días.
``` python
 for (int i = 0; i < usuarios.size(); i++) {
            if(usuarios[i].diasDisponibles == 10) {
                cout << "Empleado " << usuarios[i].id << " Solicitud no procesada" << endl;
            } else {
            cout << "Empleado " << usuarios[i].id << " → Dias restantes: " << usuarios[i].diasDisponibles << endl;
            }
        }

...

cout << "\n ---> Resultado General <---" << endl;
        cout << "Solicitudes aprobadas: " << totalAprobadas << endl;
        cout << "Solicitudes rechazadas: " << totalRechazadas << endl;
        cout << "Solicitudes totales: " << totalAprobadas + totalRechazadas << endl;
```

Se diseño el siguiente escenario:
 - **20 empleados** con **10 días disponibles cada uno**
 - **5 controladores disponibles** para procesar las solicitudes de manera concurrente
 - Cada solicitud es procesada por un controlador libre
 - Hay 4 solicitudes que serán rechazadas por exceder el número de días solicitados -> **12 / 11 / 13 / 15**
 - El resto de solicitudes serán aprobadas -> **16_**

### Resultados Obtenidos
Con este caso base se puede ver una distribución equivalente de las solicitudes en cada uno de los controladores, podemos verlo en los siguientes resultado:

 - Controlador 0 proceso -> 4 solicitudes
 - Controlador 1 proceso -> 4 solicitudes
 - Controlador 2 proceso -> 4 solicitudes
 - Controlador 3 proceso -> 4 solicitudes
 - Controlador 4 proceso -> 4 solicitudes

 - **Solicitudes totales procesadas:** 20
 - **Solicitudes aprobadas:** 16
 - **Solicitudes rechazadas:** 4
 - **Distribución del trabajo:** cada controlador procesa 4 solicitudes

## Complejidad temporal y espacial
La complejidad temporal del sistema es **O(n)**, donde *n* es el número de solicitudes generadas. Esto se debe a que cada solicitud es procesada una sola vez por cada controlador, provocanto que el tiempo total de ejecución crece linealmente acorde al número de solicitudes. Sin embargo en el peor de los casos se considera que las solicitudes sean demasiadas y el número de controladores no sean suficientes, se estaría mandando a un cierto tiempo de espera y a repetir el proceso de busqueda de controladores libres, esto provoca que la complejidad sea de **O(N x C)** donde *C* es el número de ciclos o repeticiones que hizo cada solicitud. 

La complejidad espacial es de **O(n)** donde *n* es el número de días disponibles, numero de controladores, numero de hilos, etc; es decir el sistema escala de manera lineal acorde a la cantidad establecida de sus componentes principales.

## Paradigma funcional
Podemos resolver la misma problema haciendo uso del **paradigma funcional**, ya que solamente estamos considerando una evaluación de una *lista* conforme a ciertas *condiciones*. Todo esto lo podemos seccionar en *funciones puras*, donde cada funcion estaría encargada de una tarea especifica sin la necesidad de manejar estados cambiantes o ejecución de multiples tareas.

Primero deberiamos de definir nuestros usuarios y sus respectivas solicitudes, podemos hacer uso de las listas en las que los empleados guardan sus días de vacaciones disponibles, y un lista de solicitudes en la que se guarde el numéro de días solicitados, cada una de las listas estaría ordenada de manera que cada solicitud corresponda con su usuario
```Racket
(define usuarios '(10 10 10))
(define solicitudes '(4 23 10))
```

Luego tenemos que ver si cada solicitud es aprobada o rechazada en funcion de una comparación de los días disponibles de cada usuarios **(u)** con los días de las solicitudes correspondientes **(s)**. Hacemos uso de la funcion ```map``` para recorrer las listas de ```usuarios``` y ```solicitudes```; ademas una función anonima *(lambda)* para la comparación los valores y devolver si la solicitud es aprobada *(true)* o si es rechazada *(false)* 

```Racket
(define resultados
  (map (lambda (e s) (>= e s))
       empleados
       solicitudes))
```
Finalmente obtenemos una lista de los resultados de cada evaluación
```Racket
(displayln resultados)
```

### Complejidad temporal y espacial
La complejidad del paradigma funcional en base al contexto de las solicitudes de vacaciones es de O(n) en ambos. En el temporal *n* representa el numero de empleados y solicitudes, es decir que su tiempo de ejecución será lineal respecto al numero de usuarios o solicitudes establecidas. Mientras que *n* en la complejidad espacial representa el resultado de ```map``` ya que devuelve una lista booleana el cual el espacio ira incrementando de manera lineal conforme a las listas ingresadas en la función. 

### Comparacion de paradigmas
Ambos paradigmas **concurrente y funcional** tienen su forma de abarcar los problemas y cada uno tiene sus ventajas y desventajas bajo distintos contextos.

**Paradigma Concurrente**
Su principal ventaja es la ejecución de multiples tareas al mismo tiempo, utiliza los threads para el proceso y sincronización con diferentes recursos. Puede ayudar con el rendimiento de las tareas y en mi caso lo utlice para poder ejecutar multiples solicitudes generadas al mismo tiempo. Sin embargo esto le agrega complejidad a su programación y es propenso a errores de condiciones de carrera o bloqueos debido al uso compartido de recursos.

**Paradigma Funcional**
Es la base de las funciones puras, ya que solo se enfocan en una sola "operación", su código es más facil de razonar e implementar, es ideal para procesar datos, recursión y funciones con listas. Sin embargo puede llegar a ser menos eficiente al ser secuencial y ademas no puede funcionar en base a cambios de variables en tiempo real.

Basicamente el **Paradigma funcional** se destaca por su funcionalidad pura y simplisidad, mientras que el **Paradigma concurrente** se destaca más por la ejecución de multiples tareas a la vez. Ambos son útiles y su efectividad depende más del contexto o problema a resolver en el que se desea utilizar.

# Referencias

Programación concurrente UNAL. (s. f.). https://ferestrepoca.github.io/paradigmas-de-programacion/progconcurrente/concurrente_teoria/index.html

Ismael Garcia, E. (2019, 10 abril). ¿Qué es la Programación Funcional? Codigofacilito. https://codigofacilito.com/articulos/programacion-funcional

GeeksforGeeks. (2023, 20 noviembre). Mutex in C++. GeeksforGeeks. https://www.geeksforgeeks.org/std-mutex-in-cpp/

GeeksforGeeks. (2025, 21 febrero). Thread in Operating System. GeeksforGeeks. https://www.geeksforgeeks.org/thread-in-operating-system/

