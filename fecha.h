#ifndef FECHA_H
#define FECHA_H

#include <iostream>

class Fecha {
private:
    unsigned short int diaActual;      // Rango 1-19 (Días de la fase de grupos)
    unsigned short int partidosHoy;    // Contador 0-4 (Máximo 4 partidos/día)

    // Arreglo dinámico para cumplir con el requerimiento de memoria dinámica
    short* ultimoJuegoEquipo;
    unsigned short int totalEquipos;

    // Arreglo de punteros a caracteres
    const char* meses[2];

public:
    // Constructor que recibe la cantidad total de equipos (48)
    Fecha(unsigned short int numEquipos);

    // Destructor para liberar la memoria dinámica
    ~Fecha();

    // Verifica las reglas del mundial 2026
    bool puedeJugar(unsigned short int idE1, unsigned short int idE2) const;

    // Registra el encuentro y actualiza los contadores
    void registrarEncuentro(unsigned short int idE1, unsigned short int idE2);

    // Muestra la fecha (p.ej. 20 de Junio)
    void mostrarFecha() const;

    // Getters
    unsigned short int getDiaActual() const;
    unsigned short int getPartidosHoy() const;
};

#endif
