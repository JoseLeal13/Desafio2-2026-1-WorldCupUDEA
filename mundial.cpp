#include "mundial.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include "fecha.h"
#include "partido.h"
#include <iostream>

using namespace std;

Mundial::Mundial() {
    // totalGrupos debe ser 12 según el requerimiento del mundial de 48 equipos
    grupos = new Grupo*[totalGrupos];
    for(int i = 0; i < totalGrupos; i++) {
        grupos[i] = new Grupo('A' + i);
    }
    campeon = nullptr;
}

void Mundial::cargarDesdeCSV(string ruta) {
    ifstream archivo(ruta);

    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo CSV: " << ruta << endl;
        return;
    }

    string linea;
    // 1. Saltar las DOS líneas de cabecera que tiene tu archivo
    getline(archivo, linea); // Salta "Selecciones clasificadas..."
    getline(archivo, linea); // Salta "Ranking FIFA;País;Director técnico..."

    unsigned short int contadorID = 0;

    while (getline(archivo, linea)) {
        if (linea.empty()) continue; // Ignorar líneas vacías

        stringstream ss(linea);
        string r_str, nombre, dt, fed, conf, gf_str, gc_str, p_gan, p_emp, p_per;

        // 2. Usar ';' como delimitador
        getline(ss, r_str, ';');
        getline(ss, nombre, ';');
        getline(ss, dt, ';');
        getline(ss, fed, ';');
        getline(ss, conf, ';');
        getline(ss, gf_str, ';');
        getline(ss, gc_str, ';');

        // Extraemos los que sobran aunque no los uses todos ahora para limpiar el stream
        getline(ss, p_gan, ';');
        getline(ss, p_emp, ';');
        getline(ss, p_per, ';');

        try {
            // 3. Conversión de datos
            // stoi puede fallar si hay espacios, por eso usamos un try-catch
            int r = stoi(r_str);
            float gf_hist = stof(gf_str);
            float gc_hist = stof(gc_str);

            // Crear el equipo con el ID que genera el bucle
            Equipo* nuevo = new Equipo(r, nombre, dt, fed, conf, gf_hist, gc_hist, contadorID);

            // Agregar a tu lista genérica
            listaMaestra.agregar(nuevo, listaMaestra.tamaño());

            contadorID++;
        } catch (...) {
            // Esto atrapará errores si una línea está mal formateada
            continue;
        }
    }

    archivo.close();
    cout << "-> Carga completa: " << listaMaestra.tamaño() << " equipos cargados correctamente." << endl;
}

void Mundial::prepararBombosYSorteo() {
    // 1. Crear las 4 listas para los bombos (Poder sembrar los equipos por nivel)
    lista<Equipo*> bombo1, bombo2, bombo3, bombo4;

    for (int i = 0; i < listaMaestra.tamaño(); i++) {
        Equipo* e = listaMaestra.consultar(i);

        // REQUERIMIENTO (a): EE.UU. es anfitrión y va al Grupo A directamente
        if (e->getNombre() == "Estados Unidos" || e->getNombre() == "USA") {
            grupos[0]->agregarEquipo(e); // grupos[0] es el Grupo A
            continue; // No lo agregamos a ningún bombo
        }

        // Llenar bombos (ajustando que ahora hay 47 equipos en los bombos)
        if (bombo1.tamaño() < 11) bombo1.agregar(e, bombo1.tamaño()); // 11 porque USA ya salió
        else if (bombo2.tamaño() < 12) bombo2.agregar(e, bombo2.tamaño());
        else if (bombo3.tamaño() < 12) bombo3.agregar(e, bombo3.tamaño());
        else bombo4.agregar(e, bombo4.tamaño());

    // Array de punteros a nuestras listas de bombos para iterar
        lista<Equipo*>* bombos[] = {&bombo1, &bombo2, &bombo3, &bombo4};

        //   srand(time(0));

        // 3. Proceso de Sorteo: Por cada Bombo (1 al 4)
        for (int b = 0; b < 4; b++) {
            // Por cada Grupo (A al L)
            for (int g = 0; g < totalGrupos; g++) {
                bool asignado = false;
                int intentos = 0;

                while (!asignado && !bombos[b]->esVacia()) {
                    int tam = bombos[b]->tamaño();
                    int posAleatoria = rand() % tam;
                    Equipo* candidato = bombos[b]->consultar(posAleatoria);

                    // Verificamos restricciones geográficas (Tu método en grupos.cpp)
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
}

void Mundial::ejecutarFaseGrupos() {
    Fecha calendario(48);
    short int partidosTotales = 72; // 12 grupos * 6 partidos cada uno
    short int partidosJugados = 0;

    while (partidosJugados < partidosTotales) {
        bool seProgramoAlgoHoy = false;

        // Intentamos buscar partidos para el día actual recorriendo los grupos
        for (int g = 0; g < totalGrupos; g++) {

            // Cada grupo tiene 4 equipos, revisamos todas las combinaciones (i vs j)
            for(int i = 0; i < 4; i++) {
                for(int j = i + 1; j < 4; j++) {

                    Equipo* e1 = grupos[g]->consultarPorPosicion(i);
                    Equipo* e2 = grupos[g]->consultarPorPosicion(j);

                    // 1. Verificamos si los equipos tienen menos de 3 partidos (fase grupos)
                    // 2. Verificamos si la clase FECHA permite que jueguen (descanso y tope de 4/día)
                    if (e1->getPartidosJugados() < 3 && e2->getPartidosJugados() < 3) {

                        if (calendario.puedeJugar(e1->getId(), e2->getId())) {

                            // Intentamos marcar el partido en el grupo para no repetirlo
                            if (grupos[g]->marcarPartidoComoJugado(i, j)) {

                                cout << "["; calendario.mostrarFecha(); cout << "] ";
                                cout << "Grupo " << (char)('A' + g) << ": ";

                                // Creamos el objeto partido (tu constructor ya simula y muestra)
                                partido p(e1, e2);

                                // Usamos los nuevos métodos get de partido.h
                                unsigned short int g1 = p.get_goles_equipo1();
                                unsigned short int g2 = p.get_goles_equipo2();

                                // Actualizamos los puntos y goles en la tabla
                                e1->actualizarResultado(g1, g2);
                                e2->actualizarResultado(g2, g1);

                                // IMPORTANTE: Registrar en el calendario para el descanso de 3 días
                                calendario.registrarEncuentro(e1->getId(), e2->getId());

                                partidosJugados++;
                                seProgramoAlgoHoy = true;
                            }
                        }
                    }
                }
            }
        }

        // SI NADIE PUDO JUGAR HOY:
        // Si recorrimos todos los grupos y 'seProgramoAlgoHoy' sigue en false,
        // significa que los equipos disponibles están descansando o ya se jugaron 4 partidos.
        // Debemos avanzar el día en el calendario.
        if (!seProgramoAlgoHoy && partidosJugados < partidosTotales) {
            // Registramos un encuentro "fantasma" para forzar que partidosHoy llegue a 4
            // y la clase Fecha pase al día siguiente automáticamente.
            while(calendario.getPartidosHoy() != 0) {
                calendario.registrarEncuentro(99, 99); // 99 es un ID que no existe
            }
        }
    }

    // Al finalizar todos los partidos, mostramos las tablas de posiciones
    for (int i = 0; i < totalGrupos; i++) {
        grupos[i]->mostrarTabla();
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
/*
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
*/
void Mundial::imprimirConsumoMemoria() {
    cout << "========================================" << endl;
    cout << " REPORTE DE MEMORIA (ESTRUCTURAS DINAMICAS)" << endl;

    // Nota: Asegúrate que memoriaUsada() o memoryUsage() sea el nombre correcto en lista.h
    cout << "Memoria en Listas Enlazadas: "
         << lista<Equipo*>::memoriaUsada() << " bytes." << endl;

    long memoriaGrupos = sizeof(Grupo*) * totalGrupos + (sizeof(Grupo) * totalGrupos);
    cout << "Memoria en Estructura de Grupos: "
         << memoriaGrupos << " bytes." << endl;

    cout << "========================================" << endl;
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
