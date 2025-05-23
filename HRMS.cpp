#include <iostream> // Para entrada y salida de datos
#include <vector> // Para el uso de vectores
#include <thread> // Para el uso de hilos
#include <mutex> // Para el uso de mutex

#include <chrono> // Para el uso de temporizadores

using namespace std;
std::mutex m1; // Mutex global

class Request {
    public:
        int empleadoId;
        int diasSolicitados;

        // CONTRUCTOR
        Request(int id, int dias) : empleadoId(id), diasSolicitados(dias) {}
};

class Usuario {
    public:
        int id;
        int diasDisponibles;

        // CONSTRUCTOR
        Usuario(int id, int dias) : id(id), diasDisponibles(dias) {}
};

// Clase que solo aprueba o rechaza solicitudes
class Controlador {
    private:
        int id;
        bool status;
        int solicitudesProcesadas = 0;
        int aprobadas = 0;
        int rechazadas = 0;

    public:
        Controlador(int id) : id(id), status(false) {}

        // SETTERS
        void setStatus(bool nuevoStatus) {status = nuevoStatus;}
        
        // GETTERS
        bool getStatus() {return status;}
        int getId() {return id;}
        int getSolicitudesProcesadas() const { return solicitudesProcesadas; }
        int getAprobadas() const { return aprobadas; }
        int getRechazadas() const { return rechazadas; }
    
        // FUNCIONES
        void procesarRequest(Usuario& usu, Request& req) {
            status = true;

            // Bloquear el mutex para que los mensajes no se sobreecriban
            m1.lock();
            cout << "Controlador " << id << " procesando solicitud de empleado " << req.empleadoId << " por " << req.diasSolicitados << " días." << endl;
            m1.unlock();

            // Comienzo del proceso de aprobación o rechazo
            this_thread::sleep_for(chrono::milliseconds(500));

            if (usu.diasDisponibles >= req.diasSolicitados) {
                usu.diasDisponibles -= req.diasSolicitados;
                m1.lock();
                cout << "Controlador  " << id << " aprobo la solicitud || Dias restantes " << usu.diasDisponibles << endl;
                m1.unlock();
                aprobadas++;
            } else {
                m1.lock();
                cout << "Controlador  " << id << " rechazo la solicitud || Insuficientes días, usted cuenta con -> " << usu.diasDisponibles << endl;
                m1.unlock();
                rechazadas++;
            }

            solicitudesProcesadas++;
            status = false;
        }

    };

    // Clase que maneja la virtualización de los controladores
    class gestorControlador {
        private:
            vector <Controlador> controladores;
            std::mutex mtx;
        public:
            gestorControlador(int cantidad) {
                for (int i = 0; i < cantidad; i++) {
                    controladores.emplace_back(i); // Crear controladores
                }
            }

            Controlador* controladorLibre() {
                mtx.lock(); 

                // Buscar un controlador libre
                for (int i = 0; i < controladores.size(); i++) {
                    if (!controladores[i].getStatus()) {
                        mtx.unlock();
                        return &controladores[i]; // Retornar el primer controlador libre
                    }
                }
                mtx.unlock();
                return nullptr; // Si no hay controladores libres, retornar nullptr
            }

            // METODO PARA ACCEDER A NUMERO DE CONTROLADOR
                Controlador& getControlador(int i) {
                    return controladores[i];
                }

    };

    // VALORES DE PRUEBA
    gestorControlador gestor(5); // Crear 5 controladores

    // Se generan 20 solicitudes de vacaciones
    vector<int> diasSolicitados = {
        3, 12, 2, 10, 11, 5, 4, 13, 6, 2,
        1, 15, 3, 6, 9, 2, 4, 5, 10, 3
    };

    void pruebasUsuario(Usuario& usu) {
        int dias = diasSolicitados[usu.id];

        // Definimos la solicitud
        Request req(usu.id, dias);
        
        // Buscar un controlador libre
        Controlador* controlador = nullptr;

        // Esperar hasta que haya un controlador libre
        while (!controlador) {
            controlador = gestor.controladorLibre();
            if (!controlador) {
                this_thread::sleep_for(chrono::milliseconds(100)); // Esperar y volver a intentar
            }
        }

        controlador->procesarRequest(usu, req); // Procesar la solicitud
    }
    
    int main() {

        cout << "---> Pruebas <---" << endl;
        
        // Crear usuarios
        vector<Usuario> usuarios;
        for (int i = 0; i < 20; i++) {
            usuarios.emplace_back(i, 10); // Cada usuario tiene 10 días disponibles
        }

        // Crear hilos para cada usuario
        vector<thread> hilos;
        for (int i = 0; i < usuarios.size(); i++) {
            hilos.emplace_back(pruebasUsuario, ref(usuarios[i]));
        }

        // Esperar a que todos los hilos terminen
        for (auto& hilo : hilos) {
            hilo.join();
        }

        cout << " ---> Fin de las pruebas <---" << endl;

        cout << "\n ---> Resumen Final de Solicitudes <---" << endl;
        for (int i = 0; i < usuarios.size(); i++) {
            if(usuarios[i].diasDisponibles == 10) {
                cout << "Empleado " << usuarios[i].id << " Solicitud no procesada" << endl;
            } else {
            cout << "Empleado " << usuarios[i].id << " → Dias restantes: " << usuarios[i].diasDisponibles << endl;
            }
        }

        int totalAprobadas = 0;
        int totalRechazadas = 0;

        cout << "\n\n--->Resumen de Controladores <---" << endl;
        for (int i = 0; i < 5; i++) {
            Controlador& c = gestor.getControlador(i);

            cout << "Controlador " << i << " proceso -> " << c.getSolicitudesProcesadas() << " solicitudes" << endl;

            totalAprobadas += c.getAprobadas();
            totalRechazadas += c.getRechazadas();
        }

        cout << "\n ---> Resultado General <---" << endl;
        cout << "Solicitudes aprobadas: " << totalAprobadas << endl;
        cout << "Solicitudes rechazadas: " << totalRechazadas << endl;
        cout << "Solicitudes totales: " << totalAprobadas + totalRechazadas << endl;

        return 0;
        }