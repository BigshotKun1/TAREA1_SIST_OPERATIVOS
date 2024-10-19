#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <mutex>

using namespace std;
mutex mtx;
vector<int> orden_llegada;

class Auto {
public:
    int id;
    int distancia_total;
    int progreso = 0;

    Auto(int id_, int distancia_total_) : id(id_), distancia_total(distancia_total_) {}

    void correr() {
        while (progreso < distancia_total) {
            // Avance aleatorio entre 1 y 10 metros
            int avance = rand() % 10 + 1;
            progreso += avance;

            // Asegurarse de no superar la distancia total
            if (progreso > distancia_total) progreso = distancia_total;

            // Mostrar el avance en la consola
            {
                lock_guard<mutex> lock(mtx); // Protege la salida concurrente
                cout << "Auto" << id << " avanza " << avance << " metros (total: " << progreso << " metros)" << endl;
            }

            // Pausa aleatoria entre 100 y 500 milisegundos
            this_thread::sleep_for(chrono::milliseconds(rand() % 401 + 100));
        }

        // Registrar el orden de llegada
        {
            lock_guard<mutex> lock(mtx);
            orden_llegada.push_back(id);
            cout << "Auto" << id << " termina la carrera en el lugar " << orden_llegada.size() << "!" << endl;
        }
    }
};

void iniciar_carrera(int N, int M) {
    vector<thread> autos;
    vector<Auto> lista_autos;

    // Crear los autos
    for (int i = 0; i < N; ++i) {
        lista_autos.emplace_back(i, M);
    }

    // Iniciar las hebras
    for (int i = 0; i < N; ++i) {
        autos.emplace_back(&Auto::correr, &lista_autos[i]);
    }

    // Esperar a que todas las hebras terminen
    for (auto& t : autos) {
        t.join();
    }

    // Mostrar el orden final de llegada
    cout << "\nOrden final de llegada:\n";
    for (size_t i = 0; i < orden_llegada.size(); ++i) {
        cout << i + 1 << ": Auto" << orden_llegada[i] << endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Uso: " << argv[0] << " <distancia_total> <numero_autos>" << endl;
        return 1;
    }

    int M = atoi(argv[1]);  // Distancia total
    int N = atoi(argv[2]);  // Número de autos

    // Semilla para generar números aleatorios
    srand(time(0));

    cout << "Distancia total de la carrera: " << M << " metros" << endl;
    cout << "Numero de autos en la carrera: " << N << endl;

    iniciar_carrera(N, M);

    return 0;
}

