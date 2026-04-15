#include "Equipo.h"
#include <iostream>
#include <cstring>
#include <cstdlib> // Para rand()

// Constructor
Equipo::Equipo(const char* _nombre, int _ranking, const char* _confederacion, float promGolesFavorHistorico, float promGolesContraHistorico) {
    nombre = new char[strlen(_nombre) + 1];
    strcpy(nombre, _nombre);

    confederacion = new char[strlen(_confederacion) + 1];
    strcpy(confederacion, _confederacion);

    rankingFIFA = _ranking;
    promedioGolesFavor = promGolesFavorHistorico;
    promedioGolesContra = promGolesContraHistorico;

    puntos = 0;
    golesFavor = 0;
    golesContra = 0;

    cantidadJugadores = 0;
    jugadores = new Jugador*[26];
}

// MÉTODO PARA SELECCIONAR 11 JUGADORES AL AZAR
Jugador** Equipo::obtenerTitulares() {
    // Creamos un arreglo dinámico de 11 punteros
    Jugador** titulares = new Jugador*[11];

    // Lógica simple: tomamos los primeros 11 o usamos una semilla aleatoria
    // El desafío pide que se elijan al azar para cada partido
    // Esto también es una idea principal para la selección de jugadores
    bool seleccionados[26] = {false};
    int cont = 0;

    while (cont < 11 && cont < cantidadJugadores) {
        int indiceAleatorio = rand() % cantidadJugadores;
        if (!seleccionados[indiceAleatorio]) {
            titulares[cont] = jugadores[indiceAleatorio];
            seleccionados[indiceAleatorio] = true;
            cont++;
        }
    }
    return titulares;
    // NOTA: El objeto 'Partido' debe liberar este arreglo (delete[]) tras usarlo.
    // Esto no se si deba ser así, como la clase 'Partido' es la que convoca al equipo pienso
    // debe liberar al equipo al finalizar su uso.
}

// Ejemplo de cómo procesarías una línea del CSV: "Colombia,12,CONMEBOL,1.5,0.8"
void Equipo::cargarDatosHistoricos(const char* linea) {
    // Aquí usarías strtok() para separar por comas y asignar a los atributos
    // Este método es vital para que el Mundial cree los equipos correctamente.
    // Esto es una idea, aún no he pensado como implementarla Emmanuel.
}

Equipo::~Equipo() {
    delete[] nombre;
    delete[] confederacion;
    for(int i = 0; i < cantidadJugadores; i++) delete jugadores[i];
    delete[] jugadores;
}
