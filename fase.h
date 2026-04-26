#ifndef FASE_H
#define FASE_H

#include "equipo.h"
#include "partido.h"

class Fases {
private:
    // Clasificados recibidos del Mundial
    Equipo* primeros[12];
    Equipo* segundos[12];
    Equipo* terceros[8];

    // Arreglos de punteros para cada etapa de eliminación
    partido* dieciseisavos[16];
    partido* octavos[8];
    partido* cuartos[4];
    partido* semifinales[2];
    partido* tercer_puesto;
    partido* final_mundial;

    // Almacenamiento de resultados para avanzar de ronda
    Equipo* ganadores_dieciseisavos[16];
    Equipo* ganadores_octavos[8];
    Equipo* ganadores_cuartos[4];
    Equipo* ganadores_semis[2];
    Equipo* perdedores_semis[2]; // Para jugar el 3er puesto

    // Lógica interna para organizar quién va contra quién
    // Nota: El emparejamiento usa [16][2] porque son 16 partidos de 2 equipos cada uno
    Equipo* emparejamiento_dieciseisavos[16][2];

    // ── Almacenamiento de equipos por ronda (para reporte de confederaciones) ─
    // R16: los 32 equipos que jugaron dieciseisavos
    Equipo* equiposR16[32];
    // R8: los 16 equipos que jugaron octavos
    Equipo* equiposR8[16];
    // R4: los 8 equipos que jugaron cuartos
    Equipo* equiposR4[8];

    // Podio final
    Equipo* podio[4]; // [0]=campeon, [1]=subcampeon, [2]=tercero, [3]=cuarto

    // ── Helpers privados (Utilidades internas) ────────────────────────────────
    Equipo* obtener_ganador(partido* p);
    Equipo* obtener_perdedor(partido* p);

    // Método para imprimir resultados de forma estética
    void mostrar_clasificados(Equipo* ganadores[], int cantidad, const char* titulo);

public:
    // El constructor recibe los 32 equipos clasificados
    Fases(Equipo* p[12], Equipo* s[12], Equipo* t[8]);
    ~Fases();

    // ── Dieciseisavos (Ronda de 32) ──────────────────────────────────────────
    void organizar_dieciseisavos();
    void jugar_dieciseisavos();

    // ── Octavos de Final ─────────────────────────────────────────────────────
    void organizar_octavos();
    void jugar_octavos();

    // ── Cuartos de Final ─────────────────────────────────────────────────────
    void organizar_cuartos();
    void jugar_cuartos();

    // ── Semifinales ──────────────────────────────────────────────────────────
    void organizar_semifinales();
    void jugar_semifinales();

    // ── Definición del Podio (Final y 3er Puesto) ───────────────────────────
    void jugar_tercer_puesto();
    void jugar_final();

    long calcularMemoriaDinamica() const;

    // ── Getters para reportes finales ────────────────────────────────────────
    // Retorna el campeón (ganador de la final)
    Equipo* getCampeon() const;

    // Retorna el podio completo: [0]=campeon, [1]=subcampeon, [2]=tercero, [3]=cuarto
    // El llamador debe hacer delete[] al arreglo retornado
    Equipo** getTop4() const;

    // Confederación dominante en R16 (32 equipos), R8 (16) y R4 (8)
    // Imprime el reporte directamente
    void reporteConfederaciones() const;


};

#endif
