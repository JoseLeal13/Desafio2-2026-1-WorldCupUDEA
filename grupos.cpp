#include "grupos.h"
#include "partido.h"
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
    // Nota: quien reciba este puntero (Fases) debe hacer delete[]
}

void Grupo::simularPartidosDelGrupo() {
    // Este ciclo hace que los 4 equipos del grupo jueguen entre sí
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {

            // 1. Se crea el partido (usando los equipos del arreglo 'equipos')
            partido p(equipos[i], equipos[j]);

            // 2. Sacamos los goles que el partido de la clase partido
            unsigned short int goles1 = p.get_goles_equipo1();
            unsigned short int goles2 = p.get_goles_equipo2();

            // 3. Método de equipo.cpp
            // Actualizamos al equipo i con sus goles a favor y en contra
            equipos[i]->actualizarResultado(goles1, goles2);
            // Actualizamos al equipo j con los goles al revés
            equipos[j]->actualizarResultado(goles2, goles1);

            // Opcional: imprimir el resultado para ver que funcione
            cout << equipos[i]->getNombre() << " " << goles1 << " - "
                 << goles2 << " " << equipos[j]->getNombre() << endl;
        }
    }
    // Después de los 6 partidos, ordenamos la tabla con tu función de burbuja
    ordenarPorPuntos();
}

void Grupo::mostrarTabla() const {
    cout << "\n===========================================================" << endl;
    cout << "  TABLA DE POSICIONES - GRUPO " << idGrupo << endl;
    cout << "===========================================================" << endl;

    // Usamos \t (tabulador) para separar las columnas
    cout << "POS\t| EQUIPO\t\t| PTS\t| DG" << endl;
    cout << "-----------------------------------------------------------" << endl;

    for (int i = 0; i < cantEquipos; i++) {
        // Calculamos la diferencia de goles
        int dg = equipos[i]->getGolesFavorTorneo() - equipos[i]->getGolesContraTorneo();

        cout << i + 1 << "\t| "
             << equipos[i]->getNombre();

        // Truco: Si el nombre es muy corto, metemos doble tabulación para que no se mueva la tabla
        if (equipos[i]->getNombre().length() < 8) {
            cout << "\t\t| ";
        } else {
            cout << "\t| ";
        }

        cout << equipos[i]->getPuntos() << "\t| "
             << dg << endl;
    }
    cout << "===========================================================" << endl;
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

Equipo* Grupo::consultarPorPosicion(unsigned short int indice) {
    if (indice >= 0 && indice < cantEquipos) {
        return equipos[indice];
    }
    return nullptr;
}

Grupo::~Grupo() {
    // IMPORTANTE: Solo borramos el arreglo de punteros, NO los equipos.
    // Los equipos "viven" en el Mundial, el grupo solo los referencia.
    delete[] equipos;
}
