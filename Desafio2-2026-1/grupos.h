#ifndef GRUPO_H
#define GRUPO_H

#include "Equipo.h"

class Grupo {
private:
    char idGrupo;                    // 'A', 'B', 'C'...
    Equipo** equipos;                // Arreglo de 4 punteros a Equipo
    unsigned short int cantEquipos;  // Control de llenado (máximo 4)

public:
    Grupo(char _id);
    ~Grupo();

    void agregarEquipo(Equipo* e);

    // Este es el método más importante para avanzar a la siguiente fase
    void ordenarPorPuntos();

    // Retorna los equipos que pasan (1ro, 2do y quizás 3ro según el torneo)
    Equipo** obtenerClasificados(unsigned short int &cuantos);

    char getId() const { return idGrupo; }
    void mostrarTabla() const;
};

#endif

//Nota: Usa la lista<T> para el contenedor global de equipos en la clase Mundial.
//Así cumples con el entrenamiento del PDF y mantienes
//el programa rápido donde realmente importa (los partidos).

