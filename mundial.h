#ifndef MUNDIAL_H
#define MUNDIAL_H

#include "lista.h"   //Clase lista genérica
#include "grupos.h"
#include "equipo.h"
#include <string>

class Mundial {
private:
    // 1. LA LISTA MAESTRA: Aquí viven los 48 equipos (Criterio de la guía)
    lista<Equipo*> listaMaestra;

    // 2. ESTRUCTURA DE GRUPOS: 12 grupos (A-L)
    Grupo** grupos;
    const int totalGrupos = 12;

    // 3. ESTADÍSTICAS GLOBALES
    Equipo* campeon;

public:
    Mundial();
    ~Mundial();

    // Lógica principal
    void cargarDesdeCSV(std::string ruta);
    void prepararBombosYSorteo();
    void ejecutarFaseGrupos();

    // Reportes finales (Lo que pediste)
    void mostrarReporteFinal();
    void imprimirConsumoMemoria();
    Equipo* obtenerMaximoGoleadorTorneo(); // Busca en la listaMaestra
};

#endif
