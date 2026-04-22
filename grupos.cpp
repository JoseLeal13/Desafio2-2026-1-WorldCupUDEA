#include "grupos.h"
#include <iostream>

using namespace std;

Grupo::Grupo(char _id) {
    idGrupo = _id;
    cantEquipos = 0;
    // Reservamos espacio para 4 punteros a Equipo
    equipos = new Equipo*[4];
    for (int i = 0; i < 4; i++) {
        equipos[i] = nullptr;
    }
}

void Grupo::agregarEquipo(Equipo* e) {
    if (cantEquipos < 4) {
        equipos[cantEquipos] = e;
        cantEquipos++;
    }
}

// LOGICA DE ORDENAMIENTO (Burbuja optimizado para múltiples criterios)
void Grupo::ordenarPorPuntos() {
    for (int i = 0; i < cantEquipos - 1; i++) {
        for (int j = 0; j < cantEquipos - i - 1; j++) {

            bool intercambiar = false;
            Equipo* e1 = equipos[j];
            Equipo* e2 = equipos[j + 1];

            // 1. Comparar Puntos
            if (e1->getPuntos() < e2->getPuntos()) {
                intercambiar = true;
            }
            // 2. Empate en puntos -> Diferencia de Goles (GolesTorneo - GolesContraTorneo)
            else if (e1->getPuntos() == e2->getPuntos()) {

                int dif1 = e1->getGolesFavorTorneo() - e1->getGolesContraTorneo();
                int dif2 = e2->getGolesFavorTorneo() - e2->getGolesContraTorneo();

                if (dif1 < dif2) {
                    intercambiar = true;
                }
                // 3. Empate en diferencia -> Goles a Favor
                else if (dif1 == dif2) {
                    if (e1->getGolesFavorTorneo() < e2->getGolesFavorTorneo()) {
                        intercambiar = true;
                    }
                    // 4. Empate extremo -> Ranking FIFA (el menor número es mejor ranking)
                    else if (e1->getGolesFavorTorneo() == e2->getGolesFavorTorneo()) {
                        if (e1->getRanking() > e2->getRanking()) {
                            intercambiar = true;
                        }
                    }
                }
            }

            if (intercambiar) {
                Equipo* temp = equipos[j];
                equipos[j] = equipos[j + 1];
                equipos[j + 1] = temp;
            }
        }
    }
}

// Retorna los 2 mejores (o 3 según necesites)
Equipo** Grupo::obtenerClasificados(unsigned short int &cuantos) {
    ordenarPorPuntos(); // Aseguramos que esté ordenada

    cuantos = 2; // En un mundial estándar pasan 2 por grupo
    Equipo** clasificados = new Equipo*[cuantos];

    for(int i = 0; i < cuantos; i++) {
        clasificados[i] = equipos[i];
    }

    return clasificados;
    // Recuerda: quien reciba este puntero (Fase) debe hacer delete[]
}

void Grupo::mostrarTabla() const {
    cout << "\n--- TABLA GRUPO " << idGrupo << " ---" << endl;
    cout << "POS | EQUIPO       | PTS | DG" << endl;
    for (int i = 0; i < cantEquipos; i++) {
        int dg = equipos[i]->getGolesFavorTorneo() - equipos[i]->getGolesContraTorneo();
        cout << i + 1 << "   | " << equipos[i]->getNombre() << "\t | "
             << equipos[i]->getPuntos() << "   | " << dg << endl;
    }
}

bool Grupo::esValidoAgregar(Equipo* nuevo) {
    int mismoContinente = 0;
    int conteoUEFA = 0;

    for (int i = 0; i < cantEquipos; i++) {
        if (equipos[i]->getConfederacion() == nuevo->getConfederacion()) {
            mismoContinente++;
        }
        if (equipos[i]->getConfederacion() == "UEFA") {
            conteoUEFA++;
        }
    }

    // Regla 1: No puede haber 2 de la misma confederación (excepto UEFA)
    if (nuevo->getConfederacion() != "UEFA" && mismoContinente > 0) {
        return false;
    }

    // Regla 2: Máximo 2 de la UEFA por grupo
    if (nuevo->getConfederacion() == "UEFA" && conteoUEFA >= 2) {
        return false;
    }

    return true;
}

Grupo::~Grupo() {
    // IMPORTANTE: Solo borramos el arreglo de punteros, NO los equipos.
    // Los equipos "viven" en el Mundial, el grupo solo los referencia.
    delete[] equipos;
}
