#include "mundial.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

long Mundial::totalIteraciones = 0;

Mundial::Mundial() {
    // totalGrupos debe ser 12 según el requerimiento del mundial de 48 equipos
    grupos = new Grupo*[totalGrupos];
    for(int i = 0; i < totalGrupos; i++) {
        grupos[i] = new Grupo('A' + i);
        registrarIteracion(); // Contamos la creación de cada grupo
    }
    campeon = nullptr;
}

void Mundial::cargarDesdeCSV(string ruta) {
    ifstream archivo(ruta);

    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo CSV de selecciones: " << ruta << endl;
        return;
    }

    string linea;
    // 1. Saltar las DOS líneas de cabecera que tiene tu archivo
    getline(archivo, linea); // Salta "Selecciones clasificadas..."
    getline(archivo, linea); // Salta "Ranking FIFA;País;..."

    unsigned short int contadorID = 0;

    while (getline(archivo, linea)) {
        registrarIteracion(); // Cada línea procesada es una iteración
        if (linea.empty()) continue;

        stringstream ss(linea);
        string r_str, nombre, dt, fed, conf, gf_str, gc_str, p_gan, p_emp, p_per;

        // 2. Extracción de datos con delimitador ';'
        getline(ss, r_str, ';');
        getline(ss, nombre, ';');
        getline(ss, dt, ';');
        getline(ss, fed, ';');
        getline(ss, conf, ';');
        getline(ss, gf_str, ';');
        getline(ss, gc_str, ';');
        getline(ss, p_gan, ';');
        getline(ss, p_emp, ';');
        getline(ss, p_per, ';');

        Equipo* nuevo = nullptr;
        try {
            // 3. Conversión de datos
            int r = stoi(r_str);
            float gf_hist = stof(gf_str);
            float gc_hist = stof(gc_str);

            // Crear el equipo (se inicia con cantidadJugadores = 0 y el arreglo de punteros en null)
            nuevo = new Equipo(r, nombre, dt, fed, conf, gf_hist, gc_hist, contadorID);

            // 4. Agregar el equipo a la lista maestra
            listaMaestra.agregar(nuevo, listaMaestra.tamaño());
            contadorID++;

        } catch (...) {
            // 5. Si algo falló (por ejemplo, stoi o agregar),
            // verificamos si alcanzamos a crear el objeto para borrarlo.
            if (nuevo != nullptr) {
                delete nuevo;
            }
            continue;
        }
    }

    archivo.close();
    cout << "-> Carga de Selecciones: " << listaMaestra.tamaño() << " equipos creados (esperando jugadores)." << endl;
}

void Mundial::cargarJugadoresDesdeCSV(string ruta) {
    ifstream archivo(ruta);

    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo de jugadores: " << ruta << endl;
        return;
    }

    string linea;
    // Saltar la cabecera: pais;nombre;apellido;numero_camiseta;...
    getline(archivo, linea);

    while (getline(archivo, linea)) {
        registrarIteracion(); // Cada línea procesada es una iteración
        if (linea.empty()) continue;

        stringstream ss(linea);
        string pais, nombre, apellido, dorsal_str;

        // Solo extraemos los datos necesarios para el constructor de Jugador
        // El formato es: pais;nombre;apellido;numero_camiseta;...
        getline(ss, pais, ';');
        getline(ss, nombre, ';');
        getline(ss, apellido, ';');
        getline(ss, dorsal_str, ';');

        // 1. Buscar el equipo correspondiente en la listaMaestra
        Equipo* equipoDestino = nullptr;
        for (int i = 0; i < listaMaestra.tamaño(); i++) {
            if (listaMaestra.consultar(i)->getNombre() == pais) {
                equipoDestino = listaMaestra.consultar(i);
                break;
            }
        }

        // 2. Si encontramos el equipo, creamos al jugador y lo agregamos
        if (equipoDestino != nullptr) {
            try {
                unsigned short int dorsal = (unsigned short int)stoi(dorsal_str);

                // Creamos el jugador con tus parámetros de constructor
                Jugador* nuevoJ = new Jugador(nombre.c_str(), apellido.c_str(), dorsal);

                // Usamos tu método de la clase Equipo
                equipoDestino->agregarJugador(nuevoJ);
            } catch (...) {
                continue; // Saltar si hay error de conversión en el dorsal
            }
        }
    }

    archivo.close();
    cout << "-> Carga de jugadores reales finalizada." << endl;
}

void Mundial::prepararBombosYSorteo() {
    // 1. Crear las 4 listas para los bombos (Poder sembrar los equipos por nivel)
    lista<Equipo*> bombo1, bombo2, bombo3, bombo4;

    for (int i = 0; i < listaMaestra.tamaño(); i++) {
        Equipo* e = listaMaestra.consultar(i);

        // REQUERIMIENTO (a): EE.UU. es anfitrión y va al Grupo A directamente
        if (e->getNombre() == "United States") {
            grupos[0]->agregarEquipo(e); // grupos[0] es el Grupo A
            continue; // No lo agregamos a ningún bombo
        }

        // Llenar bombos (ajustando que ahora hay 47 equipos en los bombos)
        if (bombo1.tamaño() < 11) bombo1.agregar(e, bombo1.tamaño()); // 11 porque USA ya salió
        else if (bombo2.tamaño() < 12) bombo2.agregar(e, bombo2.tamaño());
        else if (bombo3.tamaño() < 12) bombo3.agregar(e, bombo3.tamaño());
        else bombo4.agregar(e, bombo4.tamaño());
    }

    // Array de punteros a nuestras listas de bombos para iterar
    lista<Equipo*>* bombos[] = {&bombo1, &bombo2, &bombo3, &bombo4};

    // 3. Proceso de Sorteo: Por cada Bombo (1 al 4)
    for (int b = 0; b < 4; b++) {
        // Por cada Grupo (A al L)
        for (int g = 0; g < totalGrupos; g++) {
            // Nota: saltar sorteo para el alfitrion
            // Si estamos en el primer bombo y el grupo es el A, saltamos
            // porque United States ya ocupa esa posición.
            if (b == 0 && g == 0) continue;

            bool asignado = false;
            int intentos = 0;

            while (!asignado && !bombos[b]->esVacia()) {

                registrarIteracion();
                int tam = bombos[b]->tamaño();
                int posAleatoria = rand() % tam;
                Equipo* candidato = bombos[b]->consultar(posAleatoria);

                // Verificamos restricciones geográficas (método en grupos.cpp)
                if (grupos[g]->esValidoAgregar(candidato)) {
                    grupos[g]->agregarEquipo(candidato);
                    bombos[b]->eliminar(candidato); // Lo sacamos del bombo para que no repita
                    asignado = true;
                } else {
                    intentos++;
                    // "Seguro" para evitar bucles infinitos en sorteos geográficamente imposibles
                    if (intentos > 100) {
                        grupos[g]->agregarEquipo(candidato);
                        bombos[b]->eliminar(candidato);
                        asignado = true;
                    }
                }
            }
        }
    }
}

void Mundial::ejecutarFaseGrupos() {
    Fecha calendario(60);
    short int partidosTotales = 72;
    short int partidosJugados = 0;

    while (partidosJugados < partidosTotales && calendario.getDiaActual() <= 19) {
        registrarIteracion();
        for (int g = 0; g < totalGrupos; g++) {
            if (calendario.getPartidosHoy() >= 4) break;

            for (int i = 0; i < 4; i++) {
                for (int j = i + 1; j < 4; j++) {
                    if (calendario.getPartidosHoy() >= 4) break;

                    Equipo* e1 = grupos[g]->consultarPorPosicion(i);
                    Equipo* e2 = grupos[g]->consultarPorPosicion(j);

                    if (!e1 || !e2) continue;

                    if (calendario.puedeJugar(e1->getRanking(), e2->getRanking())) {
                        // marcarPartidoComoJugado usa la matriz interna 'enfrentamientos',
                        // esto es perfecto porque no depende del orden.
                        if (grupos[g]->marcarPartidoComoJugado(i, j)) {

                            cout << "["; calendario.mostrarFecha(); cout << "] ";
                            cout << "Grupo " << (char)('A' + g) << ": " << e1->getNombre() << " vs " << e2->getNombre() << endl;

                            partido p(e1, e2);
                            e1->actualizarResultado(p.get_goles_equipo1(), p.get_goles_equipo2());
                            e2->actualizarResultado(p.get_goles_equipo2(), p.get_goles_equipo1());

                            calendario.registrarEncuentro(e1->getRanking(), e2->getRanking());
                            partidosJugados++;
                        }
                    }
                }
            }
        }
        calendario.avanzarDia();
    }

    // RECIEN AQUI ORDENAMOS
    for (int i = 0; i < totalGrupos; i++) {
        registrarIteracion();
        grupos[i]->ordenarPorPuntos(); // Ahora sí, para mostrar la tabla final
        grupos[i]->mostrarTabla();
    }
}

void Mundial::programarEncuentroEspecifico(int g, int i, int j, Fecha &cal) {
    Equipo* e1 = grupos[g]->consultarPorPosicion(i);
    Equipo* e2 = grupos[g]->consultarPorPosicion(j);

    if (e1 && e2) {
        // El bucle de espera: si no pueden jugar por descanso o cupo, avanza el tiempo
        while (!cal.puedeJugar(e1->getId(), e2->getId())) {
            cal.avanzarDia();
            registrarIteracion();
        }

        if (grupos[g]->marcarPartidoComoJugado(i, j)) {
            cout << "["; cal.mostrarFecha(); cout << "] ";
            cout << "Grupo " << (char)('A' + g) << ": ";

            partido p(e1, e2);
            e1->actualizarResultado(p.get_goles_equipo1(), p.get_goles_equipo2());
            e2->actualizarResultado(p.get_goles_equipo2(), p.get_goles_equipo1());

            cal.registrarEncuentro(e1->getId(), e2->getId());
        }
    }
}

void Mundial::determinarClasificados(Equipo** &primeros, Equipo** &segundos, Equipo** &mejoresTerceros) {
    // 1. Reservar memoria exacta (Eficiencia de memoria)
    primeros = new Equipo*[12];
    segundos = new Equipo*[12];
    Equipo** candidatosTerceros = new Equipo*[12]; // Temporal

    // 2. Extraer de cada grupo (Estructura clara)
    for (int i = 0; i < 12; i++) {
        grupos[i]->ordenarPorPuntos(); // Usa burbuja de grupos.cpp
        registrarIteracion();
        primeros[i] = grupos[i]->consultarPorPosicion(0);
        segundos[i] = grupos[i]->consultarPorPosicion(1);
        candidatosTerceros[i] = grupos[i]->consultarPorPosicion(2);
    }

    // 3. Ordenar la lista de 12 terceros para sacar los 8 mejores
    // Aplicamos burbuja sobre el arreglo dinámico temporal
    for (int i = 0; i < 12 - 1; i++) {
        for (int j = 0; j < 12 - i - 1; j++) {
            // Si el de la derecha es mejor que el de la izquierda, intercambiamos
            // Queremos que los mejores queden al principio (índices 0 a 7)
            registrarIteracion();
            if (compararEstadisticas(candidatosTerceros[j + 1], candidatosTerceros[j])) {
                Equipo* temp = candidatosTerceros[j];
                candidatosTerceros[j] = candidatosTerceros[j + 1];
                candidatosTerceros[j + 1] = temp;
            }
        }
    }

    // 4. Guardar los 8 ganadores finales
    mejoresTerceros = new Equipo*[8];
    for (int i = 0; i < 8; i++) {
        mejoresTerceros[i] = candidatosTerceros[i];
    }

    // 5. Liberar el arreglo temporal de 12 (pero no los equipos)
    delete[] candidatosTerceros;
}

bool Mundial::compararEstadisticas(Equipo* e1, Equipo* e2) {
    // CRITERIO 1: Puntos
    if (e1->getPuntos() != e2->getPuntos()) {
        return e1->getPuntos() > e2->getPuntos();
    }

    // CRITERIO 2: Diferencia de Goles
    int dif1 = e1->getGolesFavorTorneo() - e1->getGolesContraTorneo();
    int dif2 = e2->getGolesFavorTorneo() - e2->getGolesContraTorneo();
    if (dif1 != dif2) {
        return dif1 > dif2;
    }

    // CRITERIO 3: Goles a Favor
    if (e1->getGolesFavorTorneo() != e2->getGolesFavorTorneo()) {
        return e1->getGolesFavorTorneo() > e2->getGolesFavorTorneo();
    }

    // CRITERIO 4: Ranking FIFA
    if (e1->getRanking() != e2->getRanking()) {
        return e1->getRanking() < e2->getRanking();
    }

    // --- EMPATE ABSOLUTO ---
    // Si llegó hasta aquí, no hubo return antes, así que sorteamos
    Equipo* ganador = sorteoPonderado(e1, e2);
    return (ganador == e1);
}

Equipo* Mundial::sorteoPonderado(Equipo* e1, Equipo* e2) {
    // 1. Obtener los rankings
    float r1 = (float)e1->getRanking();
    float r2 = (float)e2->getRanking();

    // 2. Calcular probabilidades invertidas
    // (A menor ranking, mayor probabilidad)
    float probE1 = 1.0f / r1;
    float probE2 = 1.0f / r2;

    // 3. Normalizar para que la suma sea 100%
    float sumaTotal = probE1 + probE2;
    float limiteE1 = (probE1 / sumaTotal) * 100.0f;

    // 4. Generar número aleatorio entre 0 y 100
    float resultadoSorteo = (float)(rand() % 101);

    if (resultadoSorteo <= limiteE1) {
        return e1; // Ganó el de mejor ranking (más probable)
    } else {
        return e2; // Ganó el de menor ranking (menos probable)
    }
}

void Mundial::avanzarAFaseEliminatoria() {
    // 1. Declaramos los punteros para recibir los arreglos dinámicos
    Equipo** primeros = nullptr;
    Equipo** segundos = nullptr;
    Equipo** mejoresTerceros = nullptr;

    // 2. Ejecutamos la recolección y el filtrado (Lógica de los 12, 12, 8)
    determinarClasificados(primeros, segundos, mejoresTerceros);

    // 3. Instanciamos la clase
    // Pasamos los 3 arreglos dinámicos al constructor de Fases
    faseFinal = new Fases(primeros, segundos, mejoresTerceros);

    delete[] primeros;
    delete[] segundos;
    delete[] mejoresTerceros;

    cout << "\n------------------------------------------------" << endl;
    cout << " FASE DE GRUPOS FINALIZADA " << endl;
    cout << " Clasificados enviados a eliminacion directa." << endl;
    cout << "------------------------------------------------\n" << endl;
}

void Mundial::simularRondasFinales() {
    if (faseFinal == nullptr) {
        cout << "Error: No se han determinado los clasificados aún." << endl;
        return;
    }

    // 1. Dieciseisavos
    faseFinal->organizar_dieciseisavos();
    faseFinal->jugar_dieciseisavos();

    // 2. Octavos
    faseFinal->organizar_octavos();
    faseFinal->jugar_octavos();

    // 3. Cuartos
    faseFinal->organizar_cuartos();
    faseFinal->jugar_cuartos();

    // 4. Semifinales
    faseFinal->organizar_semifinales();
    faseFinal->jugar_semifinales();

    // 5. Tercer Puesto
    faseFinal->jugar_tercer_puesto();

    // 6. Gran Final
    faseFinal->jugar_final();
}

void Mundial::imprimirConsumoMemoria() {
    cout << "========================================" << endl;
    cout << " REPORTE DE MEMORIA (ESTRUCTURAS DINAMICAS)" << endl;

    // 1. Memoria de Datos Maestros (Equipos y Jugadores)
    // Se calcula a través de la listaMaestra que recorre los objetos en el Heap.
    long memObjetos = lista<Equipo*>::memoriaUsada();
    cout << "1. Equipos y Jugadores (Lista Maestra): " << memObjetos << " bytes." << endl;

    // 2. Memoria de la Estructura de Grupos
    // Sumamos el arreglo de punteros y los 12 objetos Grupo.
    long memGrupos = (sizeof(Grupo*) * totalGrupos) + (sizeof(Grupo) * totalGrupos);
    cout << "2. Estructura de Grupos (Punteros/Objetos): " << memGrupos << " bytes." << endl;

    // 3. Memoria de Gestión de Fechas (Calendario)
    // Instanciamos una fecha temporal para obtener su cálculo interno o lo calculamos directo
    // Si tienes el objeto 'calendario' como atributo, usa: calendario->calcularMemoria()
    long memCalendario = sizeof(short) * 60 + 8; // Arreglo dinámico + puntero/base
    cout << "3. Gestor de Calendario y Descansos:    " << memCalendario << " bytes." << endl;

    // 4. Memoria de la Fase Final (Solo si ya se creó)
    long memFaseFinal = 0;
    if (faseFinal != nullptr) {
        memFaseFinal = faseFinal->calcularMemoriaDinamica();
        cout << "4. Fase Final (Objetos Partido nuevos): " << memFaseFinal << " bytes." << endl;
    } else {
        cout << "4. Fase Final:                          0 bytes (No iniciada)." << endl;
    }

    // CÁLCULO DEL TOTAL
    long totalTotal = memObjetos + memGrupos + memCalendario + memFaseFinal;

    cout << "----------------------------------------------------" << endl;
    cout << " TOTAL ESTIMADO EN MEMORIA HEAP:        " << totalTotal << " bytes." << endl;
    cout << " TOTAL EN KILOBYTES (KB):               " << (totalTotal / 1024.0) << " KB." << endl;
    cout << "====================================================\n" << endl;
}

void Mundial::imprimirReporteFinal() {

    imprimirConsumoMemoria();

    cout << "========================================" << endl;
    cout << "   ESTADISTICAS DE EJECUCION (CPU)" << endl;
    cout << "========================================" << endl;
    cout << " Total de iteraciones procesadas en la Clase Mundial: " << totalIteraciones << endl;

    cout << " Complejidad gestionada: ALTA (O(n^2) en sorteos y ordenamiento)" << endl;
    cout << "========================================\n" << endl;
}

// ─── REPORTES FINALES DEL TORNEO ─────────────────────────────────────────────

void Mundial::mostrarTop4() const {
    if (faseFinal == nullptr) {
        cout << "Error: La fase final aún no se ha jugado." << endl;
        return;
    }
    Equipo** top4 = faseFinal->getTop4();

    cout << "\n========================================\n";
    cout << "   RANKING FINAL - TOP 4 DEL MUNDIAL\n";
    cout << "========================================\n";

    const char* medallas[] = {"🥇 1er Lugar (CAMPEON)   ",
                              "🥈 2do Lugar (SUBCAMPEON)",
                              "🥉 3er Lugar             ",
                              "   4to Lugar             "};
    for (int i = 0; i < 4; i++) {
        if (top4[i] != nullptr) {
            cout << medallas[i] << ": " << top4[i]->getNombre()
            << " (Ranking FIFA #" << top4[i]->getRanking() << ")\n";
        }
    }
    cout << "========================================\n";
    delete[] top4;
}

Jugador* Mundial::obtenerMaximoGoleadorEquipo(Equipo* equipo) const {
    if (equipo == nullptr || equipo->getCantidadJugadores() == 0) return nullptr;

    Jugador* maximo = equipo->getJugador(0);
    for (int i = 1; i < equipo->getCantidadJugadores(); i++) {
        Jugador* j = equipo->getJugador(i);
        if (j != nullptr && j->get_goles() > maximo->get_goles()) {
            maximo = j;
        }
    }
    return maximo;
}

Jugador** Mundial::obtenerTop3Goleadores() const {
    // Arreglo de resultado: 3 punteros a los máximos goleadores globales
    Jugador** top3 = new Jugador*[3];
    top3[0] = top3[1] = top3[2] = nullptr;

    // Recorremos todos los equipos y todos sus jugadores
    for (int e = 0; e < listaMaestra.tamaño(); e++) {
        Equipo* equipo = listaMaestra.consultar(e);
        for (int j = 0; j < equipo->getCantidadJugadores(); j++) {
            Jugador* jugador = equipo->getJugador(j);
            if (jugador == nullptr) continue;

            // Inserción en el top 3 (burbuja de inserción simple)
            if (top3[0] == nullptr || jugador->get_goles() > top3[0]->get_goles()) {
                top3[2] = top3[1];
                top3[1] = top3[0];
                top3[0] = jugador;
            } else if (top3[1] == nullptr || jugador->get_goles() > top3[1]->get_goles()) {
                top3[2] = top3[1];
                top3[1] = jugador;
            } else if (top3[2] == nullptr || jugador->get_goles() > top3[2]->get_goles()) {
                top3[2] = jugador;
            }
        }
    }
    return top3;
    // NOTA: El llamador debe hacer delete[] al arreglo (no a los jugadores)
}

Equipo* Mundial::obtenerEquipoMasGoles() const {
    if (listaMaestra.esVacia()) return nullptr;

    Equipo* mejor = listaMaestra.consultar(0);
    for (int i = 1; i < listaMaestra.tamaño(); i++) {
        Equipo* e = listaMaestra.consultar(i);
        // Comparamos el promedio histórico actualizado con los resultados del torneo
        if (e->getPromGolesFavor() > mejor->getPromGolesFavor()) {
            mejor = e;
        }
    }
    return mejor;
}

void Mundial::mostrarReportesFinales() const {
    // ── 1. TOP 4 ──────────────────────────────────────────────────────────────
    mostrarTop4();

    // ── 2. MÁXIMO GOLEADOR DEL CAMPEÓN ───────────────────────────────────────
    cout << "\n========================================\n";
    cout << "   MAXIMO GOLEADOR DEL EQUIPO CAMPEON\n";
    cout << "========================================\n";
    if (faseFinal != nullptr) {
        Equipo* campeon = faseFinal->getCampeon();
        if (campeon != nullptr) {
            Jugador* goleador = obtenerMaximoGoleadorEquipo(campeon);
            if (goleador != nullptr) {
                cout << "  Equipo: " << campeon->getNombre() << "\n";
                cout << "  Jugador: " << goleador->get_nombre() << " "
                     << goleador->get_apellido()
                     << " | Goles en el torneo: " << goleador->get_goles() << "\n";
            } else {
                cout << "  Sin datos de goleadores.\n";
            }
        }
    }
    cout << "========================================\n";


    // ── 3. TOP 3 GOLEADORES GLOBALES ─────────────────────────────────────────
    cout << "\n========================================\n";
    cout << "   TOP 3 GOLEADORES DE LA COPA MUNDIAL\n";
    cout << "========================================\n";
    Jugador** top3 = obtenerTop3Goleadores();
    const char* posiciones[] = {"1ro", "2do", "3ro"};
    for (int i = 0; i < 3; i++) {
        if (top3[i] != nullptr) {
            cout << "  " << posiciones[i] << ": " << top3[i]->get_nombre()
            << " " << top3[i]->get_apellido()
            << " | Goles: " << top3[i]->get_goles() << "\n";
        }
    }
    delete[] top3;
    cout << "========================================\n";

    // ── 4. EQUIPO CON MÁS GOLES HISTÓRICOS ───────────────────────────────────
    cout << "\n========================================\n";
    cout << "   EQUIPO CON MAS GOLES HISTORICOS\n";
    cout << "   (Promedios actualizados con el torneo)\n";
    cout << "========================================\n";
    Equipo* masGoles = obtenerEquipoMasGoles();
    if (masGoles != nullptr) {
        cout << "  Equipo: " << masGoles->getNombre() << "\n";
        cout << "  Goles a Favor (historico+torneo): "
             << masGoles->getGolesFavorTorneo() << "\n";
    }
    cout << "========================================\n";

    // ── 5. CONFEDERACIÓN DOMINANTE POR RONDA ─────────────────────────────────
    if (faseFinal != nullptr) {
        faseFinal->reporteConfederaciones();
    }
}

Mundial::~Mundial() {
    // Primero liberamos los grupos
    for(int i = 0; i < totalGrupos; i++) {
        delete grupos[i];
    }
    delete[] grupos;

    // Luego liberamos los equipos de la lista maestra
    // Esto limpia la memoria principal de las entidades
    for(int i = 0; i < listaMaestra.tamaño(); i++) {
        delete listaMaestra.consultar(i);
    }
}
