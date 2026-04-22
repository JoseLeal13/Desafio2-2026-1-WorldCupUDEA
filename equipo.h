#ifndef EQUIPO_H
#define EQUIPO_H

//#include "Jugador.h"
#include <fstream> //CAMBIO: Necesario para leer archivos

class Equipo {
private:
    // Datos del CSV
    char* nombre;
    char* directorTecnico; // CAMBIO: Añadido según el CSV
    char* federacion;      // CAMBIO: Añadido según el CSV
    char* confederacion;
    unsigned short int rankingFIFA;

    // Estadísticas Históricas para la fórmula Lambda
    float promGolesFavorHistorico;
    float promGolesContraHistorico;

    // Contenedor de jugadores
    Jugador** jugadores;
    unsigned short int cantidadJugadores;

    // Estadísticas del torneo actual
    unsigned short int puntos;
    unsigned short int golesFavorTorneo;
    unsigned short int golesContraTorneo;

public:
    // CAMBIO: Constructor ajustado a los datos del archivo
    Equipo(int rank, const char* nom, const char* dt, const char* fed, const char* conf, float pgf, float pgc);
    ~Equipo();

    // Getters
    float getPromGolesFavor() const { return promGolesFavorHistorico; }
    const char* getNombre() const { return nombre; }

    // Función de selección
    Jugador** obtenerTitulares(); // CAMBIO: Nombre unificado

    // Métodos de actualización
    void agregarJugador(Jugador* j);
    void actualizarResultado(int gf, int gc);
    void mostrarDatos() const; // Para verificar la carga
};

#endif
