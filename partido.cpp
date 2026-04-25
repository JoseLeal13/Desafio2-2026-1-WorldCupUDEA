#include "partido.h"

using namespace std;

partido::partido(Equipo* e1, Equipo* e2) {
    equipo1 = e1;
    equipo2 = e2;

    // 1. Seleccionar titulares usando método de Equipo
    seleccionar_titulares(equipo1, titulares1);
    seleccionar_titulares(equipo2, titulares2);

    // 2. Calcular goles usando promedios históricos
    goles1 = simular_goles(equipo1, equipo2);
    goles2 = simular_goles(equipo2, equipo1);

    // 3. Repartir estadísticas a los jugadores
    repartir_goles(titulares1, goles1);
    repartir_goles(titulares2, goles2);

    // 4. Repartir tiempo
    bool prorroga = (goles1 == goles2);
    repartir_tiempo(titulares1, prorroga);
    repartir_tiempo(titulares2, prorroga);

    mostrar();
}

int partido::simular_goles(Equipo* equipoA, Equipo* equipoB) {
    const double mu = 1.35;
    const double alpha = 0.6;
    const double beta = 0.4;

    //Métodos de la clase Equipo
    double GFA = equipoA->getPromGolesFavor();
    double GCB = equipoB->getPromGolesContra();

    // Calcular lambda
    double lambda = mu * pow(GFA / mu, alpha) * pow(GCB / mu, beta);

    int goles_esperados = (int)(lambda + 0.5);
    return (goles_esperados < 0) ? 0 : goles_esperados;
}

void partido::seleccionar_titulares(Equipo* equipo, Jugador* titulares[11]) {
    Jugador** temporal = equipo->obtenerTitulares();
    if (temporal != nullptr) {
        for(int i = 0; i < 11; i++) {
            titulares[i] = temporal[i];
        }
        delete[] temporal; // Borra el contenedor de 11, NO a los jugadores.
    }
}

void partido::repartir_goles(Jugador* titulares[11], int goles_a_repartir) {
    for (int g = 0; g < goles_a_repartir; g++) {
        bool anotado = false;
        while (!anotado) {
            for (int i = 0; i < 11 && !anotado; i++) {
                if (rand() % 100 < 4) { // 4% de probabilidad por jugador
                    titulares[i]->anotar_gol(); // Asegúrate que en Jugador.h se llame así
                    anotado = true;
                }
            }
        }
    }
}

void partido::repartir_tiempo(Jugador* titulares[11], bool hay_prorroga) {
    int minutos = hay_prorroga ? 120 : 90;
    for (int i = 0; i < 11; i++) {
        titulares[i]->sumar_minutos(minutos);
    }
}

void partido::mostrar() const {
    cout << equipo1->getNombre() << " [" << goles1 << "] vs ["
         << goles2 << "] " << equipo2->getNombre() << endl;
}
