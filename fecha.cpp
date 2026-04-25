#include "fecha.h"

using namespace std;

Fecha::Fecha(unsigned short int numEquipos) {
    this->diaActual = 1;
    this->partidosHoy = 0;
    this->totalEquipos = numEquipos;

    this->ultimoJuegoEquipo = new short[numEquipos];

    // Inicializamos con -5 para asegurar que todos puedan jugar el día 1
    // (Día 1 - (-5) = 6, que es mayor a los 3 días de descanso requeridos)
    for (unsigned short int i = 0; i < numEquipos; i++) {
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

    // REGLA: Ningún equipo puede jugar más de un partido en el lapso de 3 días
    // (Diferencia entre día actual y último día de juego debe ser >= 3)
    if ((diaActual - ultimoJuegoEquipo[idE1]) < 3) return false;
    if ((diaActual - ultimoJuegoEquipo[idE2]) < 3) return false;

    // REGLA: La fase de grupos dura máximo 19 días
    if (diaActual > 19) return false;

    return true;
}

void Fecha::registrarEncuentro(unsigned short int idE1, unsigned short int idE2) {
    // Actualizamos el registro de los equipos
    ultimoJuegoEquipo[idE1] = (short)diaActual;
    ultimoJuegoEquipo[idE2] = (short)diaActual;

    partidosHoy++;

    // Si se alcanza el límite diario, avanzamos el calendario
    if (partidosHoy == 4) {
        diaActual++;
        partidosHoy = 0;
    }
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
