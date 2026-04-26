#ifndef PARTIDO_H
#define PARTIDO_H

#include "equipo.h"
#include <iostream>
#include <cmath>
#include <cstdlib>

class partido {
private:
    Equipo* equipo1;
    Equipo* equipo2;
    Jugador* titulares1[11];
    Jugador* titulares2[11];
    int goles1;
    int goles2;

    // Métodos internos de simulación
    void seleccionar_titulares(Equipo* equipo, Jugador* titulares[11]);
    int simular_goles(Equipo* equipoA, Equipo* equipoB);
    void repartir_goles(Jugador* titulares[11], int goles_a_repartir);
    void repartir_tiempo(Jugador* titulares[11], bool hay_prorroga);

public:
    partido(Equipo* e1, Equipo* e2);

    // Getters para que la clase Grupo pueda leer los resultados
    int get_goles_equipo1() const { return goles1; }
    int get_goles_equipo2() const { return goles2; }
    Equipo* get_equipo1() const { return equipo1; }
    Equipo* get_equipo2() const { return equipo2; }

    void mostrar() const;
    void mostrar_titulares() const;

    long calcularMemoria() const;
};

#endif
