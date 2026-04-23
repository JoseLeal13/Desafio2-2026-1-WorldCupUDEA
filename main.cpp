#include <iostream>
#include <cstdlib>
#include <ctime>
#include "mundial.h"

using namespace std;

int main() {
    // 0. Configuración inicial
    srand(time(0));
    Mundial udeaCup;

    cout << "=== INICIANDO SIMULACION UdeAWorldCup 2026 ===" << endl;

    // ETAPA 1: Carga de Datos
    // Lee el CSV y fabrica las estadísticas iniciales de los jugadores
    udeaCup.cargarDesdeCSV("selecciones_clasificadas_mundial.csv");
    udeaCup.imprimirConsumoMemoria(); // Requisito

    // ETAPA 2: Sorteo de Grupos
    // Organiza los bombos y reparte los 48 equipos en 12 grupos[cite: 68, 70].
    udeaCup.prepararBombosYSorteo();
    udeaCup.imprimirConsumoMemoria();

    // ETAPA 3: Fase de Grupos
    // Aquí el código debe recorrer los 12 grupos, simular los 72 partidos
    // y actualizar automáticamente las estadísticas de cada equipo/jugador

/*
    cout << "\n> Ejecutando Fase de Grupos..." << endl;
    for(int i = 0; i < 12; i++) {
        // La lógica interna de Mundial debe ordenar cada grupo al finalizar sus partidos[cite: 100].
        udeaCup.simularPartidosDeGrupo(i);
    }
    udeaCup.imprimirConsumoMemoria();


Aun no implementado
*/

    // ETAPA 4: Transición y Fases Finales
    // Este es el punto que discutimos: Mundial filtra a los 12-12-8 mejores,
    // crea el objeto 'faseFinal' y le entrega los arreglos dinámicos[cite: 172, 173, 174].
    cout << "\n> Realizando transicion a Dieciseisavos de Final..." << endl;
    udeaCup.avanzarAFaseEliminatoria();

    // ETAPA 5: Ejecución del Fixture (Dieciseisavos a Final)
    // El objeto faseFinal toma el control y ejecuta los 32 partidos restantes
    // hasta la gran final

/*
    if (udeaCup.getFaseFinal() != nullptr) {
        udeaCup.getFaseFinal()->ejecutarTorneoEliminatorio();
    }

Idea de como seria fases
*/


    // ETAPA 6: Resultados Finales y Estadísticas del Torneo
    // Una vez terminada la final, se imprimen los reportes exigidos

/*
    udeaCup.imprimirReporteFinal(); // Campeón, Goleadores, Ranking

Metodo aun no implementado
*/

    // REPORTE FINAL DE RECURSOS
    // Cantidad de iteraciones y memoria total al cierre[cite: 212, 213, 215].
    udeaCup.imprimirConsumoMemoria();

    cout << "\n=== SIMULACION FINALIZADA EXITOSAMENTE ===" << endl;

    return 0;
}
