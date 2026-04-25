#include "fecha.h"

using namespace std;

Fecha::Fecha(unsigned short int numEquipos) {
    this->diaActual = 1;
    this->partidosHoy = 0;
    this->totalEquipos = 60; // Seguridad para Rankings hasta 52
    this->ultimoJuegoEquipo = new short[60];

    for (int i = 0; i < 60; i++) {
        ultimoJuegoEquipo[i] = -5;
    }
    meses[0] = "Junio";
    meses[1] = "Julio";
}

Fecha::~Fecha() {
    delete[] ultimoJuegoEquipo;
}

bool Fecha::puedeJugar(unsigned short int idE1, unsigned short int idE2) const {
    // REGLA: Máximo 4 partidos por día
    if (partidosHoy >= 4) return false;
    if (diaActual > 19) return false;

    // REGLA: Ningún equipo puede jugar más de un partido en el lapso de 3 días
    // (Diferencia entre día actual y último día de juego debe ser >= 3)
    if ((diaActual - ultimoJuegoEquipo[idE1]) < 3) return false;
    if ((diaActual - ultimoJuegoEquipo[idE2]) < 3) return false;

    return true;
}

void Fecha::registrarEncuentro(unsigned short int idE1, unsigned short int idE2) {
    // Actualizamos el registro de los equipos
    ultimoJuegoEquipo[idE1] = (short)diaActual;
    ultimoJuegoEquipo[idE2] = (short)diaActual;

    partidosHoy++;
}

void Fecha::avanzarDia() {
    this->diaActual++;
    this->partidosHoy = 0;
}

void Fecha::mostrarFecha() const {
    unsigned short int diaCalendario = 19 + diaActual;
    const char* mes = meses[0];

    // Ajuste de mes: Junio tiene 30 días
    if (diaCalendario > 30) {
        diaCalendario -= 30;
        mes = meses[1];
    }

    cout << diaCalendario << " de " << mes << " de 2026";
}

unsigned short int Fecha::getDiaActual() const {
    return diaActual;
}

unsigned short int Fecha::getPartidosHoy() const {
    return partidosHoy;
}
