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

// Elimina el BOM (EF BB BF) del inicio de una línea si está presente
// El BOM solo aparece en la primera línea del archivo, así que lo sacamos
// una sola vez en lugar de chequearlo en cada llamada al parser
void quitar_bom_si_hay(char* linea) {
    if ((unsigned char)linea[0] == 0xEF &&
        (unsigned char)linea[1] == 0xBB &&
        (unsigned char)linea[2] == 0xBF) {
        // desplazamos todo el contenido 3 posiciones hacia la izquierda
        int i = 0;
        while (linea[i + 3] != '\0') {
            linea[i] = linea[i + 3];
            i++;
        }
        linea[i] = '\0';
    }
}

// ─── CLASE JUGADOR ────────────────────────────────────────────────────────────

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

        GFA = GFA / partidos;
        GCB = GCB / partidos;

        //  Calcular lambda (goles esperados)
        double lambda = mu *
                        pow(GFA / mu, alpha) *
                        pow(GCB / mu, beta);
        // El 0.5 para redondear el numero ej: 2.75 se tomaria 2 pero con esto se tomaria 3
        int goles_esperados = (int)(lambda + 0.5);

        // por si por algun motivo da negativo el lamda
        if (goles_esperados < 0) {
            goles_esperados = 0;
        }

        return goles_esperados;

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

    // void simular_partido(){
    //aqui reparto los goles entre cada uno de los jugadores y hago lo de las faltas y eso

    void mostrar() const {
        cout << equipo1->get_nombre_pais()
        << " vs "
        << equipo2->get_nombre_pais() << "\n";
        cout << "Marcador: " << get_goles_equipo1() << " a "<< get_goles_equipo2() << endl;
    }
    int get_goles_equipo1()            const { return goles1; }
    int get_goles_equipo2()            const { return goles2; }
};
// --- CLASE FASE --------------------------------------------------------------
/*class fase {
    private:
    const unsigned int SELECCIONES_CLASIFICADAS = 32;
    seleccion* primeros[12];
    seleccion* segundos[12];
    seleccion* terceros[8];

//deberia crear aca una lista de la clase partido (16)
    public:

    void organizar_emparejamiento(){

        //Todos los cabezas de grupo, contra los terceros puestos clasificados.
        // El resto de cabezas de grupo, con los 4 peores segundos puestos.
        //El resto de segundos puestos se enfrentan entre sí.
        //para los primeros contra los 3eros
        for(unsigned int i = 0; i < 12; i++){
            if(primeros[i].get_grupo() != terceros[i].get_grupo()){

            }

        }


    }

};
*/
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

            // La primera línea puede traer BOM de Excel, lo quitamos una sola vez
            if (numero_linea == 1) quitar_bom_si_hay(linea);

            // Saltamos las 2 cabeceras: título del archivo y nombres de columnas
            if (numero_linea <= 2) continue;

            // Ignoramos líneas vacías al final del archivo
            if (linea[0] == '\0') continue;

            // Separamos los campos por punto y coma
            char campos[10][200];
            int cantidad_campos = separar_por_punto_y_coma(linea, campos);

            // Si faltan campos la línea está rota, la saltamos con aviso
            if (cantidad_campos < 10) {
                cout << "Linea " << numero_linea << " tiene solo "
                     << cantidad_campos << " campos, se omite\n";
                continue;
            }

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

    /* Muestro Colombia específicamente — índice 12 = fila 15 del CSV (2 cabeceras + 13 datos)
    cout << "\n========================================\n";
    cout << "         DETALLE DE COLOMBIA\n";
    cout << "========================================\n";
    seleccion* col = copa_del_mundo.get_seleccion(12);
    seleccion* fra = copa_del_mundo.get_seleccion(1);
    if (col != nullptr) {
        col->mostrar_info();
        fra ->mostrar_info();
    } else {
        cout << "No se encontro Colombia en el indice 12.\n";
        cout << "Solo hay " << copa_del_mundo.get_cantidad_selecciones() << " selecciones cargadas.\n";
        cout << "Revisa que la ruta del archivo CSV sea correcta.\n";
    }
    */

    seleccion* col = copa_del_mundo.get_seleccion(12);
    seleccion* fra = copa_del_mundo.get_seleccion(1);
    col -> mostrar_info();
    fra -> mostrar_info();
    cout << "--------------------------------------------"<< endl;


    partido p1(col,fra);
    p1.mostrar_titulares();
    return 0;
}
