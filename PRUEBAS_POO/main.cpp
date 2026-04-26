#include <iostream>
#include <fstream>
#include <cmath> //para usar pow que sirve para elevar en decimales
using namespace std;

// ─── UTILIDADES DE TEXTO ──────────────────────────────────────────────────────


int largo_texto(const char* mi_texto) {
    int contador = 0;
    while (mi_texto[contador] != '\0') contador++;
    return contador;
}

void copiar_texto(char* donde_guardo, const char* que_copio) {
    int i = 0;
    while (que_copio[i] != '\0') {
        donde_guardo[i] = que_copio[i];
        i++;
    }
    donde_guardo[i] = '\0';
}

void unir_textos(char* texto_principal, const char* texto_que_agrego) {
    int posicion_final = largo_texto(texto_principal);
    int i = 0;
    while (texto_que_agrego[i] != '\0') {
        texto_principal[posicion_final] = texto_que_agrego[i];
        posicion_final++;
        i++;
    }
    texto_principal[posicion_final] = '\0';
}

void numero_a_texto(int mi_numero, char* donde_guardo_el_texto) {
    if (mi_numero == 0) {
        donde_guardo_el_texto[0] = '0';
        donde_guardo_el_texto[1] = '\0';
        return;
    }
    int numero_temporal = mi_numero;
    int cantidad_digitos = 0;
    while (numero_temporal > 0) { numero_temporal /= 10; cantidad_digitos++; }
    donde_guardo_el_texto[cantidad_digitos] = '\0';
    for (int posicion = cantidad_digitos - 1; posicion >= 0; posicion--) {
        donde_guardo_el_texto[posicion] = '0' + (mi_numero % 10);
        mi_numero /= 10;
    }
}

void crear_nombre_con_numero(const char* mi_prefijo, int mi_numero, char* nombre_generado) {
    copiar_texto(nombre_generado, mi_prefijo);
    char solo_el_numero[12];
    numero_a_texto(mi_numero, solo_el_numero);
    unir_textos(nombre_generado, solo_el_numero);
}

int texto_a_numero(const char* mi_texto) {
    int resultado = 0;
    int i = 0;
    while (mi_texto[i] != '\0') {
        resultado = resultado * 10 + (mi_texto[i] - '0');
        i++;
    }
    return resultado;
}

// Separa una línea CSV por punto y coma y guarda cada campo en el array campos[][]
// Devuelve la cantidad de campos encontrados
// No maneja BOM aquí — se limpia antes de llamar esta función
int separar_por_punto_y_coma(const char* linea, char campos[][200]) {
    int numero_campo = 0;
    int posicion_en_campo = 0;

    for (int i = 0; linea[i] != '\0'; i++) {
        if (linea[i] == ';') {
            // fin de campo: cerramos el string y pasamos al siguiente
            campos[numero_campo][posicion_en_campo] = '\0';
            numero_campo++;
            posicion_en_campo = 0;
        } else if (linea[i] == '\r') {
            // ignoramos el \r que dejan los archivos guardados en Windows
            continue;
        } else {
            campos[numero_campo][posicion_en_campo] = linea[i];
            posicion_en_campo++;
        }
    }
    // cerramos el último campo (no viene seguido de ';')
    campos[numero_campo][posicion_en_campo] = '\0';
    numero_campo++;
    return numero_campo;
}

// --- CLASE JUGADOR ----------------------------------------------

class jugador {
private:
    char* nombre;
    char* apellido;
    int numero_camiseta;
    int cantidad_goles;
    int minutos_jugados;
    int cantidad_asistencias;
    int cantidad_amarillas;
    int cantidad_rojas;
    int faltas_acumuladas;

public:
    jugador(const char* nombre_ingresado, const char* apellido_ingresado, int camiseta) {
        nombre   = new char[largo_texto(nombre_ingresado)   + 1];
        apellido = new char[largo_texto(apellido_ingresado) + 1];
        copiar_texto(nombre,   nombre_ingresado);
        copiar_texto(apellido, apellido_ingresado);
        numero_camiseta      = camiseta;
        cantidad_goles       = 0;
        minutos_jugados      = 0;
        cantidad_asistencias = 0;
        cantidad_amarillas   = 0;
        cantidad_rojas       = 0;
        faltas_acumuladas    = 0;
    }

    ~jugador() {
        delete[] nombre;
        delete[] apellido;
    }

    void anotar_gol()               { cantidad_goles++; }
    void dar_asistencia()           { cantidad_asistencias++; }
    void recibir_amarilla()         { cantidad_amarillas++; }
    void recibir_roja()             { cantidad_rojas++; }
    void cometer_falta()            { faltas_acumuladas++; }
    void sumar_minutos(int minutos) { minutos_jugados += minutos; }

    const char* get_nombre()   const { return nombre; }
    const char* get_apellido() const { return apellido; }
    int get_numero_camiseta()  const { return numero_camiseta; }
    int get_goles()            const { return cantidad_goles; }

    void mostrar_info() const {
        cout << "    [" << numero_camiseta << "] "
             << nombre << " " << apellido
             << " | Goles: "       << cantidad_goles
             << " | Asistencias: " << cantidad_asistencias
             << " | Amarillas: "   << cantidad_amarillas
             << " | Rojas: "       << cantidad_rojas
             << " | Minutos: "     << minutos_jugados << "\n";
    }
};

// ─── CLASE SELECCION ─────────────────────────────────────────────────────────

class seleccion {
private:
    static const int MAXIMO_JUGADORES = 26;

    jugador** lista_jugadores;
    int cantidad_jugadores;
    char* nombre_pais;
    char* nombre_entrenador;
    char* nombre_federacion;
    char* nombre_confederacion;
    char grupo;
    int puesto_ranking_fifa;

    int goles_a_favor;
    int goles_en_contra;
    int partidos_ganados;
    int partidos_empatados;
    int partidos_perdidos;

public:
    /* Constructor principal: recibe los datos ya separados del CSV en dos buffers.
     datos_texto[0]=pais, [1]=entrenador, [2]=federacion, [3]=confederacion
     datos_num[0]=ranking, [1]=goles_favor, [2]=goles_contra,
               [3]=ganados, [4]=empatados, [5]=perdidos*/
    seleccion(const char* datos_texto[], int datos_num[]) {
        puesto_ranking_fifa = datos_num[0];

        nombre_pais          = new char[largo_texto(datos_texto[0]) + 1];
        nombre_entrenador    = new char[largo_texto(datos_texto[1]) + 1];
        nombre_federacion    = new char[largo_texto(datos_texto[2]) + 1];
        nombre_confederacion = new char[largo_texto(datos_texto[3]) + 1];

        copiar_texto(nombre_pais,          datos_texto[0]);
        copiar_texto(nombre_entrenador,    datos_texto[1]);
        copiar_texto(nombre_federacion,    datos_texto[2]);
        copiar_texto(nombre_confederacion, datos_texto[3]);

        goles_a_favor      = datos_num[1];
        goles_en_contra    = datos_num[2];
        partidos_ganados   = datos_num[3];
        partidos_empatados = datos_num[4];
        partidos_perdidos  = datos_num[5];

        lista_jugadores    = new jugador*[MAXIMO_JUGADORES];
        for (int i = 0; i < MAXIMO_JUGADORES; i++) lista_jugadores[i] = nullptr;
        cantidad_jugadores = 0;

        cargar_jugadores_genericos();
    }

    ~seleccion() {
        for (int i = 0; i < MAXIMO_JUGADORES; i++) delete lista_jugadores[i];
        delete[] lista_jugadores;
        delete[] nombre_pais;
        delete[] nombre_entrenador;
        delete[] nombre_federacion;
        delete[] nombre_confederacion;
    }

    // Crea jugadores con nombres genéricos hasta completar el plantel
    void cargar_jugadores_genericos() {
        char nombre_generado[30];
        char apellido_generado[30];
        for (int i = 0; i < MAXIMO_JUGADORES; i++) {
            crear_nombre_con_numero("jugador",  i, nombre_generado);
            crear_nombre_con_numero("apellido", i, apellido_generado);
            lista_jugadores[i] = new jugador(nombre_generado, apellido_generado, i + 1);
        }
        cantidad_jugadores = MAXIMO_JUGADORES;
    }

    int calcular_puntos()           const { return partidos_ganados * 3 + partidos_empatados; }
    int calcular_diferencia_goles() const { return goles_a_favor - goles_en_contra; }

    const char* get_nombre_pais()   const { return nombre_pais; }
    const char* get_confederacion() const { return nombre_confederacion; }
    int get_ranking()               const { return puesto_ranking_fifa; }
    int get_puntos()                const { return calcular_puntos(); }
    int get_goles_favor()           const { return goles_a_favor; }
    int get_diferencia_goles()      const { return calcular_diferencia_goles(); }
    int get_ganados()               const { return partidos_ganados; }
    int get_empatados()             const { return partidos_empatados; }
    int get_perdidos()              const { return partidos_perdidos; }
    char get_grupo()                const { return grupo;}
    int get_goles_en_contra()       const { return goles_en_contra;}
    jugador* get_jugador(int indice)const {
        if (indice < 0 || indice >= cantidad_jugadores) return nullptr;
        return lista_jugadores[indice];
    }

    void mostrar_info() const {
        cout << "\n=== " << nombre_pais
             << " | Entrenador: "   << nombre_entrenador
             << " | Ranking FIFA: " << puesto_ranking_fifa
             << " | "               << nombre_confederacion << " ===\n";
        cout << "  Partidos Ganados: "  << partidos_ganados
             << "  Partidos Empatados: " << partidos_empatados
             << "  Partidos Perdidos: "  << partidos_perdidos
             << "  | Puntos: "      << calcular_puntos()
             << "  | DiferenciaGoles: "          << calcular_diferencia_goles() << "\n";
        cout << "  Jugadores:\n";
        for (int i = 0; i < MAXIMO_JUGADORES; i++) {
            if (lista_jugadores[i] != nullptr)
                lista_jugadores[i]->mostrar_info();
        }
    }

};
// --- CLASE PARTIDO ----------------------------------------------------------
class partido {
private:
    seleccion* equipo1;
    seleccion* equipo2;
    jugador* titulares1[11];
    jugador* titulares2[11];
    int goles1;
    int goles2;
    int faltas1, faltas2;
    int amarillas1, amarillas2;
    int rojas1, rojas2;
    int posesion1, posesion2;

public:
    //equipo 1 = e1, equipo 2 = e2
    partido(seleccion* e1, seleccion* e2) {
        equipo1 = e1;
        equipo2 = e2;

        // Seed solo una vez en todo el programa (ponelo en main antes de crear partidos)
        // srand(time(nullptr));

        // 1. Seleccionar titulares
        seleccionar_titulares(equipo1,titulares1);
        seleccionar_titulares(equipo2, titulares2);

        // 2. Calcular goles
        goles1 = simular_goles(equipo1, equipo2);
        goles2 = simular_goles(equipo2, equipo1);

        // 3. Repartir goles a jugadores
        repartir_goles(titulares1, goles1);
        repartir_goles(titulares2, goles2);

        // 4. Repartir tiempo (prorroga si empate)
        bool prorroga = (goles1 == goles2);
        repartir_tiempo(titulares1, prorroga);
        repartir_tiempo(titulares2, prorroga);
        // 5. Posesión
        calcular_posesion();

        // 6. Faltas
        simular_faltas(titulares1, faltas1);
        simular_faltas(titulares2, faltas2);

        // 7. Tarjetas
        simular_tarjetas(faltas1, titulares1, amarillas1, rojas1);
        simular_tarjetas(faltas2, titulares2, amarillas2, rojas2);

        mostrar();
    }
    int simular_goles(seleccion* equipoA, seleccion* equipoB){

        //  Constantes del modelo
        const double mu = 1.35;
        const double alpha = 0.6;
        const double beta = 0.4;


        // Obtener datos de los equipos
        double GFA = equipoA->get_goles_favor();
        double GCB = equipoB->get_goles_en_contra();
        unsigned int partidos_jugadosA = equipoA -> get_ganados() +  equipoA -> get_empatados() + equipoA -> get_perdidos();
        unsigned int partidos_jugadosB = equipoB -> get_ganados() +  equipoB -> get_empatados() + equipoB -> get_perdidos();
        unsigned int partidos;
        //hago un promedio con la cantidad de partidos menores, puesto que hay equipos con mas partidos que otros
        if( partidos_jugadosA > partidos_jugadosB){
            partidos = partidos_jugadosB;
        }
        else{
            partidos = partidos_jugadosA;
        }

        //GFA = GFA / partidos;
        //GCB = GCB / partidos;

        //  Calcular lambda (goles esperados)
        double lambda = mu *
                        pow(GFA / mu, alpha) *
                        pow(GCB / mu, beta)
            ;
        // El 0.5 para redondear el numero ej: 2.75 se tomaria 2 pero con esto se tomaria 3
        int goles_esperados = (int)(lambda + 0.5);

        // por si por algun motivo da negativo el lamda
        if (goles_esperados < 0) {
            goles_esperados = 0;
        }

        return goles_esperados;

    }
    void calcular_posesion() {
        double peso1 = 1.0 / equipo1->get_ranking();
        double peso2 = 1.0 / equipo2->get_ranking();
        double total = peso1 + peso2;

        double proporcion = peso1 / total;  // entre 0 y 1

        // Comprimimos al rango [35, 65]
        posesion1 = (int)(35 + proporcion * 30 + 0.5);
        posesion2 = 100 - posesion1;
    }

    void simular_faltas(jugador* titulares[11], int& faltas_equipo) {
        faltas_equipo = 0;
        for (int i = 0; i < 11; i++) {
            int dado = rand() % 10000;  // usamos 10000 para manejar decimales

            // 1ra falta: 13%    → dado < 1300
            // 2da falta: 2.75%  → dado < 275
            // 3ra falta: 0.7%   → dado < 70
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
    void simular_tarjetas(int faltas_equipo, jugador* titulares[11],
                          int& amarillas_equipo, int& rojas_equipo) {
        amarillas_equipo = 0;
        rojas_equipo     = 0;

        for (int f = 0; f < faltas_equipo; f++) {
            int dado = rand() % 10000;

            // Roja (doble amarilla): 1.6% → dado < 160
            if (dado < 160) {
                rojas_equipo++;
                amarillas_equipo++;  // la doble amarilla cuenta como amarilla también
            }
            // Amarilla simple: 6% → dado < 600
            else if (dado < 600) {
                amarillas_equipo++;
            }
        }

        // Asignamos las amarillas a jugadores titulares aleatoriamente
        for (int a = 0; a < amarillas_equipo; a++) {
            int jugador_idx = rand() % 11;
            titulares[jugador_idx]->recibir_amarilla();
        }
        // Las rojas no se asignan a nadie específico (solo simulación)
        for(int m = 0; m < rojas_equipo; m++){
            int jugador_idx = rand() % 11;
            titulares[jugador_idx] -> recibir_roja();
            titulares[jugador_idx] -> recibir_amarilla();
        }
    }
    void seleccionar_titulares(seleccion* equipo, jugador* titulares[11]) {
        // Armo un arreglo con los 26 índices y hago Fisher-Yates shuffle
        int indices[26];
        for (int i = 0; i < 26; i++) indices[i] = i;

        // Mezcla parcial: solo necesito los primeros 11
        for (int i = 0; i < 11; i++) {
            int j = i + rand() % (26 - i);   // j entre i y 25
            // swap
            int temp  = indices[i];
            indices[i] = indices[j];
            indices[j] = temp;

            titulares[i] = equipo->get_jugador(indices[i]);
        }
    }

    void repartir_goles(jugador* titulares[11], int goles_a_repartir) {
        // Cada jugador tiene 4% de chance por gol
        // Para cada gol, tiramos "ruleta" hasta que alguien lo anote
        for (int g = 0; g < goles_a_repartir; g++) {
            bool anotado = false;
            while (!anotado) {
                for (int i = 0; i < 11 && !anotado; i++) {
                    // rand() % 100 < 4  =>  4% de probabilidad
                    if (rand() % 100 < 4) {
                        titulares[i]->anotar_gol();
                        anotado = true;
                    }
                }
                // Si nadie anotó en esta pasada, volvemos a intentar
                // (evita el caso de que el gol quede sin dueño)
            }
        }
    }


    void repartir_tiempo(jugador* titulares[11], bool hay_prorroga) {
        int minutos = hay_prorroga ? 120 : 90;
        for (int i = 0; i < 11; i++) {
            titulares[i]->sumar_minutos(minutos);
        }
    }
    void mostrar_titulares() const {

        cout << "\n=== " << equipo1 -> get_nombre_pais()
        << " | "               << equipo1 -> get_confederacion() << " ===\n";
        cout << "  Partidos Ganados: "  << equipo1 -> get_ganados()
             << "  Partidos Empatados: " << equipo1 -> get_empatados()
             << "  Partidos Perdidos: "  << equipo1 -> get_perdidos()
             << "  | Puntos: "      << equipo1 -> get_puntos();
        cout << "  Jugadores:\n";
        for (int i = 0; i < 11; i++) {
            if (titulares1[i] != nullptr)
                titulares1[i]->mostrar_info();
        }
        cout << "\n=== " << equipo2 -> get_nombre_pais()
             << " | "               << equipo2 -> get_confederacion() << " ===\n";
        cout << "  Partidos Ganados: "  << equipo2 -> get_ganados()
             << "  Partidos Empatados: " << equipo2 -> get_empatados()
             << "  Partidos Perdidos: "  << equipo2 -> get_perdidos()
             << "  | Puntos: "      << equipo2 -> get_puntos();
        cout << "  Jugadores:\n";
        for (int i = 0; i < 11; i++) {
            if (titulares2[i] != nullptr)
                titulares2[i]->mostrar_info();
        }
    }

    void mostrar() const {
        cout << equipo1->get_nombre_pais()
        << " vs "
        << equipo2->get_nombre_pais() << "\n";
        cout << "Marcador:  " << goles1 << " - " << goles2 << "\n";
        cout << "Posesion:  " << posesion1 << "% - " << posesion2 << "%\n";
        cout << "Faltas:    " << faltas1   << " - " << faltas2   << "\n";
        cout << "Amarillas: " << amarillas1 << " - " << amarillas2 << "\n";
        cout << "Rojas:     " << rojas1    << " - " << rojas2    << "\n";
    }
    int get_goles_equipo1()            const { return goles1; }
    int get_goles_equipo2()            const { return goles2; }
    seleccion* get_equipo1()           const { return equipo1;}
    seleccion* get_equipo2()           const { return equipo2;}
};

// --- CLASE FASE --------------------------------------------------------------
class fase {
private:
    seleccion* primeros[12];
    seleccion* segundos[12];
    seleccion* terceros[8];

    partido* dieciseisavos[16];
    partido* octavos[8];
    partido* cuartos[4];
    partido* semifinales[2];
    partido* tercer_puesto;
    partido* final_mundial;

    seleccion* ganadores_dieciseisavos[16];
    seleccion* ganadores_octavos[8];
    seleccion* ganadores_cuartos[4];
    seleccion* ganadores_semis[2];
    seleccion* perdedores_semis[2];

    seleccion* emparejamiento_dieciseisavos[16][2];
    int cantidad_emparejamientos;


    seleccion* obtener_ganador(partido* p) {
        if (p->get_goles_equipo1() > p->get_goles_equipo2())
            return p->get_equipo1();
        if (p->get_goles_equipo2() > p->get_goles_equipo1())
            return p->get_equipo2();

        // EMPATE → penales (50/50 aleatorio)
        cout << "  EMPATE! Se van a penales...\n";
        if (rand() % 2 == 0) {
            cout << "  " << p->get_equipo1()->get_nombre_pais() << " gana en penales!\n";
            return p->get_equipo1();
        } else {
            cout << "  " << p->get_equipo2()->get_nombre_pais() << " gana en penales!\n";
            return p->get_equipo2();
        }
    }

    seleccion* obtener_perdedor(partido* p) {
        if (p->get_goles_equipo1() < p->get_goles_equipo2())
            return p->get_equipo1();
        return p->get_equipo2();
    }

    bool diferente_grupo(seleccion* a, seleccion* b) {
        return a->get_grupo() != b->get_grupo();
    }

    void mostrar_clasificados(seleccion* ganadores[], int cantidad, const char* titulo) {
        cout << "\n===== CLASIFICADOS A " << titulo << " =====\n";
        for (int i = 0; i < cantidad; i++)
            cout << "  " << (i+1) << ". " << ganadores[i]->get_nombre_pais() << "\n";
    }

public:
    fase(seleccion* p[12], seleccion* s[12], seleccion* t[8]) {
        for (int i = 0; i < 12; i++) primeros[i] = p[i];
        for (int i = 0; i < 12; i++) segundos[i] = s[i];
        for (int i = 0;  i < 8; i++) terceros[i] = t[i];

        for (int i = 0; i < 16; i++) { dieciseisavos[i] = nullptr; ganadores_dieciseisavos[i] = nullptr; }
        for (int i = 0; i <  8; i++) { octavos[i]       = nullptr; ganadores_octavos[i]       = nullptr; }
        for (int i = 0; i <  4; i++) { cuartos[i]       = nullptr; ganadores_cuartos[i]       = nullptr; }
        for (int i = 0; i <  2; i++) { semifinales[i]   = nullptr; ganadores_semis[i]         = nullptr; perdedores_semis[i] = nullptr; }
        tercer_puesto = nullptr;
        final_mundial = nullptr;
        cantidad_emparejamientos = 0;
    }

    ~fase() {
        for (int i = 0; i < 16; i++) delete dieciseisavos[i];
        for (int i = 0; i <  8; i++) delete octavos[i];
        for (int i = 0; i <  4; i++) delete cuartos[i];
        for (int i = 0; i <  2; i++) delete semifinales[i];
        delete tercer_puesto;
        delete final_mundial;
    }

    // ── 16AVOS ───────────────────────────────────────────────────────────────
    void organizar_dieciseisavos() {
        cantidad_emparejamientos = 0;

        // BLOQUE A: primeros[0..7] vs terceros[0..7]
        bool tercero_usado[8] = {false};
        for (int i = 0; i < 8; i++) {
            bool emparejado = false;
            for (int j = 0; j < 8 && !emparejado; j++) {
                if (!tercero_usado[j] && diferente_grupo(primeros[i], terceros[j])) {
                    emparejamiento_dieciseisavos[cantidad_emparejamientos][0] = primeros[i];
                    emparejamiento_dieciseisavos[cantidad_emparejamientos][1] = terceros[j];
                    cantidad_emparejamientos++;
                    tercero_usado[j] = true;
                    emparejado = true;
                }
            }
            // fallback sin restriccion de grupo
            for (int j = 0; j < 8 && !emparejado; j++) {
                if (!tercero_usado[j]) {
                    emparejamiento_dieciseisavos[cantidad_emparejamientos][0] = primeros[i];
                    emparejamiento_dieciseisavos[cantidad_emparejamientos][1] = terceros[j];
                    cantidad_emparejamientos++;
                    tercero_usado[j] = true;
                    emparejado = true;
                }
            }
        }

        // BLOQUE B: primeros[8..11] vs segundos[8..11]
        bool segundo_peor_usado[4] = {false};
        for (int i = 8; i < 12; i++) {
            bool emparejado = false;
            for (int j = 0; j < 4 && !emparejado; j++) {
                if (!segundo_peor_usado[j] && diferente_grupo(primeros[i], segundos[8+j])) {
                    emparejamiento_dieciseisavos[cantidad_emparejamientos][0] = primeros[i];
                    emparejamiento_dieciseisavos[cantidad_emparejamientos][1] = segundos[8+j];
                    cantidad_emparejamientos++;
                    segundo_peor_usado[j] = true;
                    emparejado = true;
                }
            }
            for (int j = 0; j < 4 && !emparejado; j++) {
                if (!segundo_peor_usado[j]) {
                    emparejamiento_dieciseisavos[cantidad_emparejamientos][0] = primeros[i];
                    emparejamiento_dieciseisavos[cantidad_emparejamientos][1] = segundos[8+j];
                    cantidad_emparejamientos++;
                    segundo_peor_usado[j] = true;
                    emparejado = true;
                }
            }
        }

        // BLOQUE C: segundos[0..7] entre si
        bool segundo_top_usado[8] = {false};
        for (int i = 0; i < 8; i++) {
            if (segundo_top_usado[i]) continue;
            bool emparejado = false;
            for (int j = i+1; j < 8 && !emparejado; j++) {
                if (!segundo_top_usado[j] && diferente_grupo(segundos[i], segundos[j])) {
                    emparejamiento_dieciseisavos[cantidad_emparejamientos][0] = segundos[i];
                    emparejamiento_dieciseisavos[cantidad_emparejamientos][1] = segundos[j];
                    cantidad_emparejamientos++;
                    segundo_top_usado[i] = true;
                    segundo_top_usado[j] = true;
                    emparejado = true;
                }
            }
            for (int j = i+1; j < 8 && !emparejado; j++) {
                if (!segundo_top_usado[j]) {
                    emparejamiento_dieciseisavos[cantidad_emparejamientos][0] = segundos[i];
                    emparejamiento_dieciseisavos[cantidad_emparejamientos][1] = segundos[j];
                    cantidad_emparejamientos++;
                    segundo_top_usado[i] = true;
                    segundo_top_usado[j] = true;
                    emparejado = true;
                }
            }
        }

        // Mostrar enfrentamientos
        cout << "\n========== 16AVOS DE FINAL ==========\n";
        cout << "Enfrentamientos:\n";
        for (int i = 0; i < cantidad_emparejamientos; i++)
            cout << "  Partido " << (i+1) << ": "
                 << emparejamiento_dieciseisavos[i][0]->get_nombre_pais()
                 << " vs "
                 << emparejamiento_dieciseisavos[i][1]->get_nombre_pais() << "\n";
    }

    void jugar_dieciseisavos() {
        cout << "\n--- Jugando 16avos ---\n";
        for (int i = 0; i < cantidad_emparejamientos; i++) {
            dieciseisavos[i] = new partido(emparejamiento_dieciseisavos[i][0],
                                           emparejamiento_dieciseisavos[i][1]);
            ganadores_dieciseisavos[i] = obtener_ganador(dieciseisavos[i]);
        }
        mostrar_clasificados(ganadores_dieciseisavos, 16, "OCTAVOS");
    }

    // ── OCTAVOS ──────────────────────────────────────────────────────────────
    void organizar_octavos() {
        cout << "\n========== OCTAVOS DE FINAL ==========\n";
        cout << "Enfrentamientos:\n";
        for (int i = 0; i < 16; i += 2)
            cout << "  Partido " << (i/2+1) << ": "
                 << ganadores_dieciseisavos[i]->get_nombre_pais()
                 << " vs "
                 << ganadores_dieciseisavos[i+1]->get_nombre_pais() << "\n";
    }

    void jugar_octavos() {
        cout << "\n--- Jugando Octavos ---\n";
        for (int i = 0; i < 8; i++) {
            octavos[i] = new partido(ganadores_dieciseisavos[i*2],
                                     ganadores_dieciseisavos[i*2+1]);
            ganadores_octavos[i] = obtener_ganador(octavos[i]);
        }
        mostrar_clasificados(ganadores_octavos, 8, "CUARTOS");
    }

    // ── CUARTOS ──────────────────────────────────────────────────────────────
    void organizar_cuartos() {
        cout << "\n========== CUARTOS DE FINAL ==========\n";
        cout << "Enfrentamientos:\n";
        for (int i = 0; i < 8; i += 2)
            cout << "  Partido " << (i/2+1) << ": "
                 << ganadores_octavos[i]->get_nombre_pais()
                 << " vs "
                 << ganadores_octavos[i+1]->get_nombre_pais() << "\n";
    }

    void jugar_cuartos() {
        cout << "\n--- Jugando Cuartos ---\n";
        for (int i = 0; i < 4; i++) {
            cuartos[i] = new partido(ganadores_octavos[i*2],
                                     ganadores_octavos[i*2+1]);
            ganadores_cuartos[i] = obtener_ganador(cuartos[i]);
        }
        mostrar_clasificados(ganadores_cuartos, 4, "SEMIFINALES");
    }

    // ── SEMIFINALES ──────────────────────────────────────────────────────────
    void organizar_semifinales() {
        cout << "\n========== SEMIFINALES ==========\n";
        cout << "Enfrentamientos:\n";
        for (int i = 0; i < 4; i += 2)
            cout << "  Partido " << (i/2+1) << ": "
                 << ganadores_cuartos[i]->get_nombre_pais()
                 << " vs "
                 << ganadores_cuartos[i+1]->get_nombre_pais() << "\n";
    }

    void jugar_semifinales() {
        cout << "\n--- Jugando Semifinales ---\n";
        for (int i = 0; i < 2; i++) {
            semifinales[i] = new partido(ganadores_cuartos[i*2],
                                         ganadores_cuartos[i*2+1]);
            ganadores_semis[i]  = obtener_ganador(semifinales[i]);
            perdedores_semis[i] = obtener_perdedor(semifinales[i]);
        }
        cout << "\nFinales:\n";
        cout << "  3er y 4to puesto: "
             << perdedores_semis[0]->get_nombre_pais()
             << " vs "
             << perdedores_semis[1]->get_nombre_pais() << "\n";
        cout << "  Final: "
             << ganadores_semis[0]->get_nombre_pais()
             << " vs "
             << ganadores_semis[1]->get_nombre_pais() << "\n";
    }

    // ── 3ER Y 4TO PUESTO ─────────────────────────────────────────────────────
    void jugar_tercer_puesto() {
        cout << "\n========== 3ER Y 4TO PUESTO ==========\n";
        tercer_puesto = new partido(perdedores_semis[0], perdedores_semis[1]);
        seleccion* tercero = obtener_ganador(tercer_puesto);
        seleccion* cuarto  = obtener_perdedor(tercer_puesto);
        cout << "  3er lugar: " << tercero->get_nombre_pais() << "\n";
        cout << "  4to lugar: " << cuarto->get_nombre_pais()  << "\n";
    }

    // ── FINAL ────────────────────────────────────────────────────────────────
    void jugar_final() {
        cout << "\n========== GRAN FINAL ==========\n";
        cout << ganadores_semis[0]->get_nombre_pais()
             << " vs "
             << ganadores_semis[1]->get_nombre_pais() << "\n";
        final_mundial = new partido(ganadores_semis[0], ganadores_semis[1]);
        seleccion* campeon = obtener_ganador(final_mundial);
        cout << "\n🏆 CAMPEON DEL MUNDO: " << campeon->get_nombre_pais() << "\n";

        campeon -> mostrar_info();
    }
};

// ─── CLASE MUNDIAL ───────────────────────────────────────────────────────────

class mundial {
private:
    static const int MAXIMO_SELECCIONES = 48;

    seleccion** lista_selecciones;
    int cantidad_selecciones;



public:
    mundial() {
        lista_selecciones = new seleccion*[MAXIMO_SELECCIONES];
        for (int i = 0; i < MAXIMO_SELECCIONES; i++) lista_selecciones[i] = nullptr;
        cantidad_selecciones = 0;
    }

    ~mundial() {
        for (int i = 0; i < MAXIMO_SELECCIONES; i++) delete lista_selecciones[i];
        delete[] lista_selecciones;
    }

    void cargar_desde_archivo(const char* ruta_archivo) {
        ifstream archivo(ruta_archivo);
        if (!archivo.is_open()) {
            cout << "Error: no se pudo abrir el archivo '" << ruta_archivo << "'\n";
            return;
        }

        char linea[500];
        int numero_linea = 0;

        while (archivo.getline(linea, 500)) {
            numero_linea++;

            // Saltamos las 2 cabeceras: título del archivo y nombres de columnas
            if (numero_linea <= 2) continue;

            // Ignoramos líneas vacías al final del archivo
            if (linea[0] == '\0') continue;

            // Separamos los campos por punto y coma
            char campos[10][200];
            int cantidad_campos = separar_por_punto_y_coma(linea, campos);

            // Buffer de texto: pais, entrenador, federacion, confederacion
            const char* datos_texto[4] = {
                campos[1],
                campos[2],
                campos[3],
                campos[4]
            };

            // Buffer numérico: ranking, goles_favor, goles_contra, G, E, P
            int datos_num[6] = {
                texto_a_numero(campos[0]),
                texto_a_numero(campos[5]),
                texto_a_numero(campos[6]),
                texto_a_numero(campos[7]),
                texto_a_numero(campos[8]),
                texto_a_numero(campos[9])
            };

            lista_selecciones[cantidad_selecciones] = new seleccion(datos_texto, datos_num);
            cantidad_selecciones++;
        }

        archivo.close();
        cout << "Se cargaron " << cantidad_selecciones << " selecciones\n";
    }

    seleccion* get_seleccion(int indice) const {
        if (indice < 0 || indice >= cantidad_selecciones) return nullptr;
        return lista_selecciones[indice];
    }

    int get_cantidad_selecciones() const { return cantidad_selecciones; }

    void mostrar_todas() const {
        for (int i = 0; i < cantidad_selecciones; i++)
            lista_selecciones[i]->mostrar_info();
    }
};

// ─── MAIN ─────────────────────────────────────────────────────────────────────

int main() {
    mundial copa_del_mundo;
    srand(time(0));
    // IMPORTANTE: cambia esta ruta por donde tengas el CSV en tu PC
    copa_del_mundo.cargar_desde_archivo("C:\\Users\\Emmanuel\\Documents\\PRUEBAS_POO\\selecciones_clasificadas_mundial.csv");
    //copa_del_mundo.cargar_desde_archivo("C:\\Users\\Emmanuel\\Documents\\PRUEBAS_POO\\archivo.csv");
    // Si da 0 aquí es porque la ruta está mal o el archivo tiene otro formato
    cout << "Selecciones cargadas: " << copa_del_mundo.get_cantidad_selecciones() << "\n";

    // Muestro todas las selecciones
    //copa_del_mundo.mostrar_todas();

    seleccion* primeros[12];
    seleccion* segundos[12];
    seleccion* terceros[8];

    // 12 primeros (índices 0 al 11)
    for (int i = 0; i < 12; i++)
        primeros[i] = copa_del_mundo.get_seleccion(i);

    // 12 segundos (índices 12 al 23)
    for (int i = 0; i < 12; i++)
        segundos[i] = copa_del_mundo.get_seleccion(12 + i);

    // 8 terceros (índices 24 al 31)
    for (int i = 0; i < 8; i++)
        terceros[i] = copa_del_mundo.get_seleccion(24 + i);
    fase eliminatorias(primeros, segundos, terceros);

    eliminatorias.organizar_dieciseisavos();
    eliminatorias.jugar_dieciseisavos();

    eliminatorias.organizar_octavos();
    eliminatorias.jugar_octavos();

    eliminatorias.organizar_cuartos();
    eliminatorias.jugar_cuartos();

    eliminatorias.organizar_semifinales();
    eliminatorias.jugar_semifinales();

    eliminatorias.jugar_tercer_puesto();
    eliminatorias.jugar_final();

    return 0;
}

