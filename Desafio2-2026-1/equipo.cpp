#include "Equipo.h"
#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

// CAMBIO: Constructor con nombres de parámetros claros y asignación correcta
Equipo::Equipo(unsigned short int rank, const char* nom, const char* dt, const char* fed, const char* conf, float pgf, float pgc) {

    rankingFIFA = rank; // Coincidencia exacta de tipo

    nombre = new char[strlen(nom) + 1];
    strcpy(nombre, nom);

    directorTecnico = new char[strlen(dt) + 1];
    strcpy(directorTecnico, dt);

    federacion = new char[strlen(fed) + 1];
    strcpy(federacion, fed);

    confederacion = new char[strlen(conf) + 1];
    strcpy(confederacion, conf);

    promGolesFavorHistorico = pgf;
    promGolesContraHistorico = pgc;

    // Inicialización explícita
    puntos = 0;
    golesFavorTorneo = 0;
    golesContraTorneo = 0;
    cantidadJugadores = 0;

    jugadores = new Jugador*[26];
    for(int i = 0; i < 26; i++) jugadores[i] = nullptr;
}

// CAMBIO 2: El método de actualización ahora recibe tipos eficientes
void Equipo::actualizarResultado(unsigned short int gf, unsigned short int gc) {
    golesFavorTorneo += gf;
    golesContraTorneo += gc;

    if (gf > gc) {
        puntos += 3;
    } else if (gf == gc) {
        puntos += 1;
    }
}

// CAMBIO 3: La variable 'limite' y los índices deben ser coherentes
Jugador** Equipo::obtenerTitulares() {
    Jugador** titulares = new Jugador*[11];
    bool seleccionados[26] = {false};
    unsigned short int seleccionadosCont = 0; // Uso de tipo eficiente

    unsigned short int limite = (cantidadJugadores < 11) ? cantidadJugadores : 11;

    while (seleccionadosCont < limite) {
        int indiceAleatorio = rand() % cantidadJugadores;
        if (!seleccionados[indiceAleatorio]) {
            titulares[seleccionadosCont] = jugadores[indiceAleatorio];
            seleccionados[indiceAleatorio] = true;
            seleccionadosCont++;
        }
    }
    return titulares;
}
    // NOTA PARA SUSTENTACIÓN: Partido debe hacer delete[] a este retorno.

void Equipo::agregarJugador(Jugador* j) {
    if (cantidadJugadores < 26) {
        jugadores[cantidadJugadores] = j;
        cantidadJugadores++;
    }
}

void Equipo::mostrarDatos() const {
    cout << "Equipo: " << nombre << " | Ranking: " << rankingFIFA << " | Conf: " << confederacion << endl;
}

Equipo::~Equipo() {
    delete[] nombre;
    delete[] directorTecnico;
    delete[] federacion;
    delete[] confederacion;

    for(int i = 0; i < cantidadJugadores; i++) {
        if(jugadores[i] != nullptr) {
            delete jugadores[i];
        }
    }
    delete[] jugadores;
}
