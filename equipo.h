#ifndef EQUIPO_H
#define EQUIPO_H

#include "jugador.h"
#include <string>
#include <iostream>

class Equipo {
private:
    // Datos usando C++ Strings (No char*)
    std::string nombre;
    std::string directorTecnico;
    std::string federacion;
    std::string confederacion;
    unsigned short int rankingFIFA;

    float promGolesFavorHistorico;
    float promGolesContraHistorico;

    // Contenedor de jugadores (Memoria dinámica requerida por el core)
    Jugador** jugadores;
    unsigned short int cantidadJugadores;

    unsigned short int puntos;
    unsigned short int golesFavorTorneo;
    unsigned short int golesContraTorneo;

    unsigned short int partidosJugados;
    unsigned short int partidosGanados;
    unsigned short int partidosEmpatados;
    unsigned short int partidosPerdidos;

public:
    // Constructor con std::string
    Equipo(unsigned short int rank, std::string nom, std::string dt, std::string fed, std::string conf, float pgf, float pgc);
    ~Equipo();

    // Getters necesarios para Mundial y Grupos
    std::string getNombre() const { return nombre; }
    std::string getConfederacion() const { return confederacion; }
    unsigned short int getRanking() const { return rankingFIFA; }
    unsigned short int getPuntos() const { return puntos; }
    unsigned short int getGolesFavorTorneo() const { return golesFavorTorneo; }
    unsigned short int getGolesContraTorneo() const { return golesContraTorneo; }

    // Para el reporte de Máximo Goleador
    unsigned short int getCantidadJugadores() const { return cantidadJugadores; }
    Jugador* getJugador(int indice) const { return jugadores[indice]; }

    Jugador** obtenerTitulares();
    void agregarJugador(Jugador* j);
    void actualizarResultado(unsigned short int gf, unsigned short int gc);
    void mostrarDatos() const;
};

#endif
