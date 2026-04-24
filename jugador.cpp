#include "Jugador.h"
#include <iostream>

using namespace std;

// Implementación de utilidades manuales
int Jugador::largo_texto(const char* texto) {
    int c = 0;
    while (texto[c] != '\0') c++;
    return c;
}

void Jugador::copiar_texto(char* destino, const char* origen) {
    int i = 0;
    while (origen[i] != '\0') {
        destino[i] = origen[i];
        i++;
    }
    destino[i] = '\0';
}

// Constructor con memoria dinámica manual
Jugador::Jugador(const char* nom, const char* ape, unsigned short int camiseta) {
    nombre = new char[largo_texto(nom) + 1];
    apellido = new char[largo_texto(ape) + 1];
    copiar_texto(nombre, nom);
    copiar_texto(apellido, ape);

    numero_camiseta = camiseta;
    cantidad_goles = 0;
    minutos_jugados = 0;
    cantidad_asistencias = 0;
    cantidad_amarillas = 0;
    cantidad_rojas = 0;
    faltas_acumuladas = 0;
}

Jugador::~Jugador() {
    delete[] nombre;
    delete[] apellido;
}

void Jugador::anotar_gol() {
    cantidad_goles++;
}

void Jugador::dar_asistencia() {
    cantidad_asistencias++;
}

void Jugador::recibir_amarilla() {
    cantidad_amarillas++;
}

void Jugador::recibir_roja() {
    cantidad_rojas++;
}

void Jugador::cometer_falta() {
    faltas_acumuladas++;
}

void Jugador::sumar_minutos(int m) {
    minutos_jugados += m;
}

const char* Jugador::get_nombre() const {
    return nombre;
}

const char* Jugador::get_apellido() const {
    return apellido;
}

unsigned short int Jugador::get_numero_camiseta() const {
    return numero_camiseta;
}

unsigned short int Jugador::get_goles() const {
    return cantidad_goles;
}

void Jugador::mostrar_info() const {
    cout << "    [" << numero_camiseta << "] " << nombre << " " << apellido
         << " | Goles: " << cantidad_goles << " | Minutos: " << minutos_jugados << endl;
}
