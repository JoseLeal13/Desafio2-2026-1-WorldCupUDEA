#include "mundial.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

using namespace std;

Mundial::Mundial() {
    grupos = new Grupo*[totalGrupos];
    for(int i = 0; i < totalGrupos; i++) {
        grupos[i] = new Grupo('A' + i);
    }
    campeon = nullptr;
}

void Mundial::cargarDesdeCSV(string ruta) {
    ifstream archivo(ruta);

    //CORRECCIÓN: validar si el archivo abrió
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo CSV\n";
        return;
    }

    string linea, dato;

    getline(archivo, linea); // Saltar cabecera

    while (getline(archivo, linea)) {
        stringstream ss(linea);

        // CORRECCIÓN: declarar variables
        int r, gf, gc;
        string nombre, dt, fed, conf;

        // CORRECCIÓN: EXTRAER datos del CSV
        getline(ss, dato, ',');
        r = stoi(dato);

        getline(ss, nombre, ',');

        getline(ss, dt, ',');

        getline(ss, fed, ',');

        getline(ss, conf, ',');

        getline(ss, dato, ',');
        gf = stoi(dato);

        getline(ss, dato, ',');
        gc = stoi(dato);

        Equipo* nuevo = new Equipo(r, nombre, dt, fed, conf, gf, gc);

        listaMaestra.agregar(nuevo, listaMaestra.tamaño());
    }

    archivo.close();
}

void Mundial::prepararBombosYSorteo() {
    lista<Equipo*> bombo1, bombo2, bombo3, bombo4;

    for (int i = 0; i < listaMaestra.tamaño(); i++) {
        Equipo* e = listaMaestra.consultar(i);
        if (i < 12) bombo1.agregar(e, bombo1.tamaño());
        else if (i < 24) bombo2.agregar(e, bombo2.tamaño());
        else if (i < 36) bombo3.agregar(e, bombo3.tamaño());
        else bombo4.agregar(e, bombo4.tamaño());
    }

    lista<Equipo*>* bombos[] = {&bombo1, &bombo2, &bombo3, &bombo4};

    srand(time(0)); // Nota: solo debería llamarse una vez en main()

    for (int b = 0; b < 4; b++) {

        for (int g = 0; g < totalGrupos; g++) {
            bool asignado = false;
            int intentos = 0;

            while (!asignado && !bombos[b]->esVacia()) {

                // Nota: guardar tamaño para evitar inconsistencias
                int tam = bombos[b]->tamaño();
                int posAleatoria = rand() % tam;

                Equipo* candidato = bombos[b]->consultar(posAleatoria);

                if (grupos[g]->esValidoAgregar(candidato)) {
                    grupos[g]->agregarEquipo(candidato);
                    bombos[b]->eliminar(candidato);
                    asignado = true;
                } else {
                    intentos++;

                    if (intentos > 100) {
                        // Nota: avisar que rompiste la restricción
                        cout << "Advertencia: asignacion forzada\n";

                        grupos[g]->agregarEquipo(candidato);
                        bombos[b]->eliminar(candidato);
                        asignado = true;
                    }
                }
            }
        }
    }
}

void Mundial::imprimirConsumoMemoria() {
    cout << "========================================" << endl;
    cout << " REPORTE DE MEMORIA (ESTRUCTURAS DINAMICAS)" << endl;

    cout << "Memoria en Listas Enlazadas: "
         << lista<Equipo*>::memoryUsage() << " bytes." << endl;

    long memoriaGrupos = sizeof(Grupo*) * totalGrupos + (sizeof(Grupo) * totalGrupos);
    cout << "Memoria en Estructura de Grupos: "
         << memoriaGrupos << " bytes." << endl;

    cout << "========================================" << endl;
}

Mundial::~Mundial() {

    for(int i = 0; i < totalGrupos; i++) {
        delete grupos[i];
    }
    delete[] grupos;

    // Nota: cuidado con double delete
    for(int i = 0; i < listaMaestra.tamaño(); i++) {
        delete listaMaestra.consultar(i);
    }
}
