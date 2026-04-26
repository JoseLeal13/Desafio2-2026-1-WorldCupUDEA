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

    // 5. NUEVA LOGICA: Posesión, Faltas y Tarjetas
    calcular_posesion();
    simular_faltas(titulares1, faltas1);
    simular_faltas(titulares2, faltas2);
    simular_tarjetas(faltas1, titulares1, amarillas1, rojas1);
    simular_tarjetas(faltas2, titulares2, amarillas2, rojas2);

    mostrar();
}

int partido::simular_goles(Equipo* equipoA, Equipo* equipoB) {
    const double mu = 1.35;
    const double alpha = 0.6;
    const double beta = 0.4;

    double GFA = equipoA->getPromGolesFavor();
    double GCB = equipoB->getPromGolesContra();

    double lambda = mu * pow(GFA / mu, alpha) * pow(GCB / mu, beta);

    int goles_esperados = (int)(lambda + 0.5);
    return (goles_esperados < 0) ? 0 : goles_esperados;
}

void partido::calcular_posesion() {
    double peso1 = 1.0 / equipo1->getRanking();
    double peso2 = 1.0 / equipo2->getRanking();
    double total = peso1 + peso2;
    double proporcion = peso1 / total;

    posesion1 = (int)(35 + proporcion * 30 + 0.5);
    posesion2 = 100 - posesion1;
}

void partido::simular_faltas(Jugador* titulares[11], int& faltas_equipo) {
    faltas_equipo = 0;
    for (int i = 0; i < 11; i++) {
        int dado = rand() % 10000;
        if (dado < 70) {
            titulares[i]->cometer_falta();
            titulares[i]->cometer_falta();
            titulares[i]->cometer_falta();
            faltas_equipo += 3;
        } else if (dado < 275) {
            titulares[i]->cometer_falta();
            titulares[i]->cometer_falta();
            faltas_equipo += 2;
        } else if (dado < 1300) {
            titulares[i]->cometer_falta();
            faltas_equipo += 1;
        }
    }
}

void partido::simular_tarjetas(int faltas_equipo, Jugador* titulares[11], int& amarillas_equipo, int& rojas_equipo) {
    amarillas_equipo = 0;
    rojas_equipo = 0;
    for (int f = 0; f < faltas_equipo; f++) {
        int dado = rand() % 10000;
        if (dado < 160) {
            rojas_equipo++;
            amarillas_equipo++;
        } else if (dado < 600) {
            amarillas_equipo++;
        }
    }
    for (int a = 0; a < amarillas_equipo; a++) {
        titulares[rand() % 11]->recibir_amarilla();
    }
    for(int m = 0; m < rojas_equipo; m++){
        Jugador* j = titulares[rand() % 11];
        j->recibir_roja();
        j->recibir_amarilla();
    }
}

void partido::seleccionar_titulares(Equipo* equipo, Jugador* titulares[11]) {
    Jugador** temporal = equipo->obtenerTitulares();
    if (temporal != nullptr) {
        for(int i = 0; i < 11; i++) {
            titulares[i] = temporal[i];
        }
        delete[] temporal;
    }
}

void partido::repartir_goles(Jugador* titulares[11], int goles_a_repartir) {
    for (int g = 0; g < goles_a_repartir; g++) {
        bool anotado = false;
        while (!anotado) {
            for (int i = 0; i < 11 && !anotado; i++) {
                if (rand() % 100 < 4) {
                    titulares[i]->anotar_gol();
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
    cout << "Posesion:  " << posesion1 << "% - " << posesion2 << "%" << endl;
    cout << "Faltas:    " << faltas1 << " - " << faltas2 << endl;
    cout << "Amarillas: " << amarillas1 << " - " << amarillas2 << endl;
    cout << "Rojas:     " << rojas1 << " - " << rojas2 << endl;
}

long partido::calcularMemoria() const {
    long memoria = sizeof(partido);
    return memoria;
}
