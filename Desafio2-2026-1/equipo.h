#ifndef EQUIPO_H
#define EQUIPO_H

//#include "Jugador.h"

class Equipo {
private:
    // Datos de identidad (leídos del archivo)
    char* nombre;
    char* confederacion;
    int rankingFIFA;
    float promGolesFavorHistorico; // Necesario para la fórmula del partido
    float promGolesContraHistorico;

    // Contenedor de jugadores
    Jugador** jugadores;
    int cantidadJugadores; // llegará a 26

    // Estadísticas del torneo actual (empiezan en 0)
    int puntos;
    int golesFavorTorneo;
    int golesContraTorneo;

public:
    // Constructor que usarás al leer el archivo
    Equipo(const char* nom, int rank, const char* conf, float pgf, float pgc);
    ~Equipo();

    // Getters para que la clase Partido haga sus cálculos
    float getPromGolesFavor() const {
        return promGolesFavorHistorico;
    }
    const char* getNombre() const {
        return nombre;
    }

    // LA FUNCIÓN QUE TE PIDIERON: Selecciona 11 al azar
    Jugador** llamarTitulares();

    // Métodos de actualización
    void agregarJugador(Jugador* j);
    void actualizarResultado(int gf, int gc);
};

#endif
