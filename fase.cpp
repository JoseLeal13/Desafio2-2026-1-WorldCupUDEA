#include "fase.h"
#include <iostream>
#include <cstdlib>   // Para rand()
    using namespace std;

// ─── CONSTRUCTOR / DESTRUCTOR ────────────────────────────────────────────────

Fases::Fases(Equipo* p[12], Equipo* s[12], Equipo* t[8]) {
    // Copiamos los punteros de los clasificados
    for (int i = 0; i < 12; i++) primeros[i] = p[i];
    for (int i = 0; i < 12; i++) segundos[i] = s[i];
    for (int i = 0; i < 8; i++) terceros[i] = t[i];

    // Inicializamos arreglos de partidos y ganadores en nullptr
    for (int i = 0; i < 16; i++) {
        dieciseisavos[i] = nullptr;
        ganadores_dieciseisavos[i] = nullptr;
    }
    for (int i = 0; i < 8; i++) {
        octavos[i] = nullptr;
        ganadores_octavos[i] = nullptr;
    }
    for (int i = 0; i < 4; i++) {
        cuartos[i] = nullptr;
        ganadores_cuartos[i] = nullptr;
    }
    for (int i = 0; i < 2; i++) {
        semifinales[i] = nullptr;
        ganadores_semis[i] = nullptr;
        perdedores_semis[i] = nullptr;
    }
    tercer_puesto = nullptr;
    final_mundial = nullptr;
}

Fases::~Fases() {
    // Liberamos la memoria de los objetos partido creados con 'new'
    for (int i = 0; i < 16; i++) if (dieciseisavos[i]) delete dieciseisavos[i];
    for (int i = 0; i < 8; i++) if (octavos[i]) delete octavos[i];
    for (int i = 0; i < 4; i++) if (cuartos[i]) delete cuartos[i];
    for (int i = 0; i < 2; i++) if (semifinales[i]) delete semifinales[i];
    if (tercer_puesto) delete tercer_puesto;
    if (final_mundial) delete final_mundial;
}

// ─── HELPERS PRIVADOS ────────────────────────────────────────────────────────

Equipo* Fases::obtener_ganador(partido* p) {
    if (p->get_goles_equipo1() > p->get_goles_equipo2())
        return p->get_equipo1();
    if (p->get_goles_equipo2() > p->get_goles_equipo1())
        return p->get_equipo2();

    // Lógica de empate (Penales 50/50) según el código de tu compañero
    cout << "  EMPATE! Se van a penales...\n";
    if (rand() % 2 == 0) {
        cout << "  " << p->get_equipo1()->getNombre() << " gana en penales!\n";
        return p->get_equipo1();
    } else {
        cout << "  " << p->get_equipo2()->getNombre() << " gana en penales!\n";
        return p->get_equipo2();
    }
}

Equipo* Fases::obtener_perdedor(partido* p) {
    // Si el equipo 1 tiene menos goles, perdió. Si no, perdió el 2.
    // (Incluso en penales, esta lógica funciona porque el partido registra el score)
    if (p->get_goles_equipo1() < p->get_goles_equipo2())
        return p->get_equipo1();
    return p->get_equipo2();
}

void Fases::mostrar_clasificados(Equipo* ganadores[], int cantidad, const char* titulo) {
    cout << "\n===== CLASIFICADOS A " << titulo << " =====\n";
    for (int i = 0; i < cantidad; i++)
        cout << "  " << (i + 1) << ". " << ganadores[i]->getNombre() << "\n";
}

// ─── 16AVOS ──────────────────────────────────────────────────────────────────

void Fases::organizar_dieciseisavos() {
    int contador = 0;

    // BLOQUE A: primeros[0..7] vs terceros[0..7]
    for (int i = 0; i < 8; i++) {
        emparejamiento_dieciseisavos[contador][0] = primeros[i];
        emparejamiento_dieciseisavos[contador][1] = terceros[i];
        contador++;
    }

    // BLOQUE B: primeros[8..11] vs segundos[8..11]
    for (int i = 8; i < 12; i++) {
        emparejamiento_dieciseisavos[contador][0] = primeros[i];
        emparejamiento_dieciseisavos[contador][1] = segundos[i];
        contador++;
    }

    // BLOQUE C: segundos[0..7] entre sí (0v1, 2v3, 4v5, 6v7)
    for (int i = 0; i < 8; i += 2) {
        emparejamiento_dieciseisavos[contador][0] = segundos[i];
        emparejamiento_dieciseisavos[contador][1] = segundos[i+1];
        contador++;
    }

    cout << "\n========== 16AVOS DE FINAL ==========\n";
    cout << "Enfrentamientos definidos.\n";
}

void Fases::jugar_dieciseisavos() {
    cout << "\n--- Jugando 16avos ---\n";
    for (int i = 0; i < 16; i++) {
        dieciseisavos[i] = new partido(emparejamiento_dieciseisavos[i][0],
                                       emparejamiento_dieciseisavos[i][1]);
        ganadores_dieciseisavos[i] = obtener_ganador(dieciseisavos[i]);
    }
    mostrar_clasificados(ganadores_dieciseisavos, 16, "OCTAVOS");
}

// ─── OCTAVOS ─────────────────────────────────────────────────────────────────

void Fases::organizar_octavos() {
    cout << "\n========== OCTAVOS DE FINAL ==========\n";
    for (int i = 0; i < 16; i += 2) {
        cout << "  Partido " << (i / 2 + 1) << ": "
             << ganadores_dieciseisavos[i]->getNombre() << " vs "
             << ganadores_dieciseisavos[i+1]->getNombre() << "\n";
    }
}

void Fases::jugar_octavos() {
    cout << "\n--- Jugando Octavos ---\n";
    for (int i = 0; i < 8; i++) {
        octavos[i] = new partido(ganadores_dieciseisavos[i * 2],
                                 ganadores_dieciseisavos[i * 2 + 1]);
        ganadores_octavos[i] = obtener_ganador(octavos[i]);
    }
    mostrar_clasificados(ganadores_octavos, 8, "CUARTOS");
}

// ─── CUARTOS ─────────────────────────────────────────────────────────────────

void Fases::organizar_cuartos() {
    cout << "\n========== CUARTOS DE FINAL ==========\n";
    for (int i = 0; i < 8; i += 2) {
        cout << "  Partido " << (i / 2 + 1) << ": "
             << ganadores_octavos[i]->getNombre() << " vs "
             << ganadores_octavos[i+1]->getNombre() << "\n";
    }
}

void Fases::jugar_cuartos() {
    cout << "\n--- Jugando Cuartos ---\n";
    for (int i = 0; i < 4; i++) {
        cuartos[i] = new partido(ganadores_octavos[i * 2],
                                 ganadores_octavos[i * 2 + 1]);
        ganadores_cuartos[i] = obtener_ganador(cuartos[i]);
    }
    mostrar_clasificados(ganadores_cuartos, 4, "SEMIFINALES");
}

// ─── SEMIFINALES ─────────────────────────────────────────────────────────────

void Fases::organizar_semifinales() {
    cout << "\n========== SEMIFINALES ==========\n";
    for (int i = 0; i < 4; i += 2) {
        cout << "  Partido " << (i / 2 + 1) << ": "
             << ganadores_cuartos[i]->getNombre() << " vs "
             << ganadores_cuartos[i+1]->getNombre() << "\n";
    }
}

void Fases::jugar_semifinales() {
    cout << "\n--- Jugando Semifinales ---\n";
    for (int i = 0; i < 2; i++) {
        semifinales[i] = new partido(ganadores_cuartos[i * 2],
                                     ganadores_cuartos[i * 2 + 1]);
        ganadores_semis[i] = obtener_ganador(semifinales[i]);
        perdedores_semis[i] = obtener_perdedor(semifinales[i]);
    }
}

// ─── 3ER PUESTO Y FINAL ───────────────────────────────────────────────────

void Fases::jugar_tercer_puesto() {
    cout << "\n========== 3ER Y 4TO PUESTO ==========\n";
    tercer_puesto = new partido(perdedores_semis[0], perdedores_semis[1]);
    Equipo* tercero = obtener_ganador(tercer_puesto);
    Equipo* cuarto  = obtener_perdedor(tercer_puesto);
    cout << "  3er lugar: " << tercero->getNombre() << "\n";
    cout << "  4to lugar: " << cuarto->getNombre() << "\n";
}

void Fases::jugar_final() {
    cout << "\n========== GRAN FINAL ==========\n";
    final_mundial = new partido(ganadores_semis[0], ganadores_semis[1]);
    Equipo* campeon = obtener_ganador(final_mundial);

    cout << "\n🏆 ¡CAMPEON DEL MUNDO: " << campeon->getNombre() << "! 🏆\n";
    // Nota: El método mostrar_info() debe existir en Equipo o Selección.
}

long Fases::calcularMemoriaDinamica() const {
    long memoriaPartidos = 0;

    // Sumamos el tamaño de cada objeto partido creado
    for (int i = 0; i < 16; i++) if (dieciseisavos[i]) memoriaPartidos += sizeof(partido);
    for (int i = 0; i < 8; i++)  if (octavos[i])       memoriaPartidos += sizeof(partido);
    for (int i = 0; i < 4; i++)  if (cuartos[i])       memoriaPartidos += sizeof(partido);
    for (int i = 0; i < 2; i++)  if (semifinales[i])   memoriaPartidos += sizeof(partido);
    if (tercer_puesto) memoriaPartidos += sizeof(partido);
    if (final_mundial) memoriaPartidos += sizeof(partido);

    return memoriaPartidos;
}
