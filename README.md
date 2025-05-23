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
Para el desarrollo de este proyecto se utilizo el paradigma de **programación concurrente**. La concurrencia mejora el rendimiento en la ejecución de multiples tareas que se generan simultaneamente. Las tareas no se ejecutan una por una o de manera secuencias y no necesariamente lleva un orden, lo que sucede es que se ejecutan varias tareas a la vez.

Bajo el contexto de esta situación, cada usuario es representado mediante un thread. Estas solicitudes son enviadas a los controladores, los cuales se encargan de gestionar los recursos virtualizados.

Este enfoque me permitio simular multiples procesos que actuan al mismo tiempo, simulando el flujo natural de un equipo de Recursos Humanos procesando las solicitudes de su organización.

Para poder implementar esta situación y el paradigma se uso el lenguaje de programación C++ y con ellos las bibliotecas de:
 - Thread (```std::thread```) - Permite crear multiples hilos de ejecución para procesar cada una de las solicitudes 
 - Mutex (```std::mutex```) - Permite proteger recursos compartidos para que no se usen simultaneamente y que unicamente un solo thread trabaje sobre la funcion.

Con estas bases se pudo implementar un sistema capaz de manejar de manera concurrente y segura cada una de las solicitudes, sin tener errores de sincronización ni incosistencias.

## Modelo - Estructura del sistema

### Componentes principales
 - **Usuarios** (Empleados) -> Cada usuario o empleado cuenta con una cantidad inicial de 10 días disponibles, se representa con los threads.
 - **Solicitudes** (Request) -> Cada solicitud tiene un **ID del empleado**, **Número de días que desea solicitar** y las solicitudes se generan con valores de prueba predefinidos.
- **Controladores**  
  Procesan las solicitudes de vacaciones. Se crean **5 controladores** que están disponibles para atender solicitudes de forma concurrente. Cada controlador:
  - Verifica si hay días disponibles
  - Aprueba o rechaza la solicitud
  - Actualiza el estado del usuario
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
   - Los días restantes por usuario.
   - Cantidad de solicitudes procesadas, aprobadas y rechazadas por cada controlador.
   - Estadísticas globales del sistema (totales).

# Pruebas
El objetivo de las pruebas es poder visualizar la distribución individual de cada una de las solicitudes generadas en los controladores, ademas de ver cuantas solicitudes fueron aprobadas o rechazadas de acuerdo a la disponibilidad de días.
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
 - Hay 4 solicitudes que serán rechazadas por exceder el número de días solicitados: 12, 11, 13 y 15
 - El resto de solicitudes serán aprobadas es decir 16

### Resultados Obtenidos
 - **Solicitudes totales procesadas:** 20
 - **Solicitudes aprobadas:** 16
 - **Solicitudes rechazadas:** 4
 - **Distribución del trabajo:** cada controlador procesa 4 solicitudes

## Complejidad temporal y espacial
La complejidad temporal del sistema es **O(n)**, donde *n* es el número de solicitudes generadas. Esto se debe a que cada solicitud es procesada una sola vez por cada controlador, provocanto que el tiempo total de ejecución crece linealmente acorde al número de solicitudes. Sin embargo en el peor de los casos se considera que las solicitudes sean demasiadas y el número de controladores no sean suficientes, se estaría mandando a un cierto tiempo de espera y a repetir el proceso de busqueda, esto provoca que la complejidad sea de **O(N x C**. 

La complejidad espacial es de **O(n)** donde *n* es el número de días disponibles, numero de controladores, numero de hilos. Es decir el sistema escala de manera lineal acorde a la cantidad de sus componentes principales.

## Paradigma funcional
Podemos resolver la misma problema haciendo uso del **paradigma funcional**, ya que solamente estamos considerando una evaluación de una lista conforme a ciertas condiciones. Todo esto lo podemos seccionar en funciones puras cada una encargada de una tarea especifica sin la necesidad de manejar estados cambiantes o ejecución de multiples tareas.

Primero deberiamos de definir nuestros empleados y sus respectivas solicitudes junto con sus valores de prueba:
```Racket
(define empleados (list 10 10 10))
(define solicitudes (list 5 12 10))
```

Luego tenemos que ver si cada solicitud es aprobada o rechazada en funcion de los días disponibles de cada empleado correspondiente. Hacemos uso de la funcion ```map``` y una función lambda:

```Racket
(map (lambda (e s) (>= e s)) empleados solicitudes)
```
Finalmente obtenemos una lista de los resultados de cada evaluación
```Racket
(#t #f #t) ; aprobado, rechazado, aprobado
```

Este enfoque es predecible y de facil mantenimiento, sus resultados dependen unicamente de las entradas establecidas.


