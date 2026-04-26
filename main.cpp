#include <iostream>
#include <cstdlib>
#include <ctime>
#include "mundial.h"

using namespace std;

int main() {
    // 0. Configuración inicial
    // Se inicializa la semilla una sola vez para todo el programa
    srand(time(0));

    Mundial udeaCup;

    cout << "=== INICIANDO SIMULACION UdeAWorldCup 2026 ===" << endl;
    cout << "Sede: USA - Mexico - Canada" << endl;
    cout << "---------------------------------------------" << endl;

    // ETAPA 1: Carga de Datos
    // Lee el CSV, asigna IDs correlativos y crea los objetos Equipo/Jugador
    udeaCup.cargarDesdeCSV("selecciones_clasificadas_mundial.csv");
    udeaCup.imprimirConsumoMemoria();

    // ETAPA 2: Sorteo de Grupos
    // Organiza los bombos por ranking y reparte con restricciones geográficas
    cout << "\n> Realizando sorteo de grupos..." << endl;
    udeaCup.prepararBombosYSorteo();
    udeaCup.imprimirConsumoMemoria();

    // ETAPA 3: Fase de Grupos
    // Este método ahora gestiona:
    // 1. Calendario
    // 2. Descanso de 3 días por equipo
    // 3. Máximo 4 partidos por día
    // 4. Simulación de los 72 partidos
    cout << "\n> Iniciando Fase de Grupos (Calendario FIFA)..." << endl;
    udeaCup.ejecutarFaseGrupos();

    // Reporte de memoria tras la simulación masiva de 72 partidos
    udeaCup.imprimirConsumoMemoria();

    // ETAPA 4: Transición
    // Cuando se implemente Fases

    cout << "\n> Clasificando equipos a Fase Final..." << endl;
    udeaCup.avanzarAFaseEliminatoria();
    udeaCup.simularRondasFinales();

    udeaCup.imprimirConsumoMemoria();

    cout << "\n=== SIMULACION DE FASE DE GRUPOS FINALIZADA ===" << endl;
    cout << "Todos los resultados y tablas han sido calculados." << endl;

    return 0;
}
