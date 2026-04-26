#ifndef MUNDIAL_H
#define MUNDIAL_H

#include "lista.h"   //Clase lista genérica
#include "grupos.h"
#include "equipo.h"
#include "partido.h"
#include "fecha.h"
#include "fase.h"

class Mundial {
private:

    static long totalIteraciones;
    // 1. LA LISTA MAESTRA: Aquí viven los 48 equipos (Criterio de la guía)
    lista<Equipo*> listaMaestra;

    // 2. ESTRUCTURA DE GRUPOS: 12 grupos (A-L)
    Grupo** grupos;
    const int totalGrupos = 12;

    // 3. ESTADÍSTICAS GLOBALES
    Equipo* campeon;
    Fases* faseFinal;

public:
    Mundial();
    ~Mundial();

    static void registrarIteracion() { totalIteraciones++; }
    static int getTotalIteraciones() { return totalIteraciones; }

    // Lógica principal
    void cargarDesdeCSV(std::string ruta);
    void cargarJugadoresDesdeCSV(std::string ruta);
    void prepararBombosYSorteo();
    void ejecutarFaseGrupos();
    void programarEncuentroEspecifico(int g, int i, int j, Fecha &cal);
    void avanzarAFaseEliminatoria();
    void simularRondasFinales();

    // Reportes finales
    void imprimirReporteFinal();
    void determinarClasificados(Equipo** &primeros, Equipo** &segundos, Equipo** &mejoresTerceros);
    bool compararEstadisticas(Equipo* e1, Equipo* e2);
    Equipo* sorteoPonderado(Equipo* e1, Equipo* e2);

    void imprimirConsumoMemoria();
    Equipo* obtenerMaximoGoleadorTorneo(); // Busca en la listaMaestra

    // Muestra el ranking de los 4 primeros puestos
    void mostrarTop4() const;

    // Máximo goleador del equipo campeón
    Jugador* obtenerMaximoGoleadorEquipo(Equipo* equipo) const;

    // Los 3 mayores goleadores de toda la copa (arreglo de 3 punteros)
    // El llamador debe hacer delete[] al arreglo retornado
    Jugador** obtenerTop3Goleadores() const;

    // El equipo con más goles históricos (promGolesFavor actualizado)
    Equipo* obtenerEquipoMasGoles() const;

    void mostrarReportesFinales() const;
};

#endif
