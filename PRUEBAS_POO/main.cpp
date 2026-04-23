#include <iostream>
#include <fstream>
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

    void mostrar_info() const {
        cout << "\n=== " << nombre_pais
             << " | Entrenador: "   << nombre_entrenador
             << " | Ranking FIFA: " << puesto_ranking_fifa
             << " | "               << nombre_confederacion << " ===\n";
        cout << "  Partidos — G: "  << partidos_ganados
             << "  E: "             << partidos_empatados
             << "  P: "             << partidos_perdidos
             << "  | Puntos: "      << calcular_puntos()
             << "  | DG: "          << calcular_diferencia_goles() << "\n";
        cout << "  Jugadores:\n";
        for (int i = 0; i < MAXIMO_JUGADORES; i++) {
            if (lista_jugadores[i] != nullptr)
                lista_jugadores[i]->mostrar_info();
        }
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

    // IMPORTANTE: cambia esta ruta por donde tengas el CSV en tu PC
    copa_del_mundo.cargar_desde_archivo("C:\\Users\\Emmanuel\\Documents\\PRUEBAS_POO\\selecciones_clasificadas_mundial.csv");
//copa_del_mundo.cargar_desde_archivo("C:\\Users\\Emmanuel\\Documents\\PRUEBAS_POO\\archivo.csv");
    // Si da 0 aquí es porque la ruta está mal o el archivo tiene otro formato
    cout << "Selecciones cargadas: " << copa_del_mundo.get_cantidad_selecciones() << "\n";

    // Muestro todas las selecciones
    copa_del_mundo.mostrar_todas();

    // Muestro Colombia específicamente — índice 12 = fila 15 del CSV (2 cabeceras + 13 datos)
    cout << "\n========================================\n";
    cout << "         DETALLE DE COLOMBIA\n";
    cout << "========================================\n";
    seleccion* col = copa_del_mundo.get_seleccion(12);
    if (col != nullptr) {
        col->mostrar_info();
    } else {
        cout << "No se encontro Colombia en el indice 12.\n";
        cout << "Solo hay " << copa_del_mundo.get_cantidad_selecciones() << " selecciones cargadas.\n";
        cout << "Revisa que la ruta del archivo CSV sea correcta.\n";
    }

    return 0;
}
