#ifndef JUGADOR_H
#define JUGADOR_H

class Jugador {
private:
    char* nombre;
    char* apellido;
    unsigned short int numero_camiseta;
    unsigned short int cantidad_goles;
    unsigned short int minutos_jugados;
    unsigned short int cantidad_asistencias;
    unsigned short int cantidad_amarillas;
    unsigned short int cantidad_rojas;
    unsigned short int faltas_acumuladas;

    // Funciones de utilidad manuales (Para no usar <cstring>)
    int largo_texto(const char* texto);
    void copiar_texto(char* destino, const char* origen);

public:
    Jugador(const char* nombre_ingresado, const char* apellido_ingresado, unsigned short camiseta);
    ~Jugador();

    // Métodos de acción
    void anotar_gol();
    void dar_asistencia();
    void recibir_amarilla();
    void recibir_roja();
    void cometer_falta();
    void sumar_minutos(int minutos);

    // Getters
    const char* get_nombre() const;
    const char* get_apellido() const;
    unsigned short int get_numero_camiseta() const;
    unsigned short int get_goles() const;

    void mostrar_info() const;
};

#endif
