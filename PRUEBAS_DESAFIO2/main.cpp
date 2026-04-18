#include <iostream>
#include <fstream>
using namespace std;

int tamaño_arreglos(const char* cadena){
    int contador = 0;
    while(cadena[contador] != '\0') {
        contador++;
    }
    return contador;

}

class jugador{

private:

    char* nombre;
    char* apellido;
    int numero_camiseta;
    int cantidad_goles;
    int minutos_jugados;//totales
    int cantidad_asistencias;
    int cantidad_amarillas;
    int cantidad_rojas;
    int faltas_acomuladas;
public:
    //CONSTRUCTOR
    jugador(const char* nombreEntrada, const char* apellidoEntrada,int num_camisaEntrada){

        //calculamos tamaño del arreglo de caracteres
        int tamaño_nombre = tamaño_arreglos(nombreEntrada);
        int tamaño_apellido = tamaño_arreglos(apellidoEntrada);

        //inicializacion de los atributos de la clase
        nombre = new char[tamaño_nombre + 1]; //para el ultimo caracter del arreglo '\0'
        for(unsigned int i = 0; i < tamaño_nombre; i++){
            nombre[i] = nombreEntrada[i];
        }
        nombre[tamaño_nombre] = '\0';

        apellido = new char[tamaño_apellido + 1];//para el ultimo caracter del arreglo '\0'
        for(unsigned int i = 0; i < tamaño_apellido; i++){
            apellido[i] = apellidoEntrada[i];
        }
        apellido[tamaño_apellido] = '\0';

        numero_camiseta = num_camisaEntrada;
        cantidad_goles = 0;
        minutos_jugados = 0;
        cantidad_asistencias = 0;
        cantidad_amarillas = 0;
        cantidad_rojas = 0;
        faltas_acomuladas = 0;


    }
    //DESTRUCTOR
    ~jugador() {
        delete[] nombre;
        delete[] apellido;
    }

};
/*
class seleccion{

private:
    jugador** jugadores;
    char* director_tecnico;
    int ranking;
    int cantidad_goles_afavor;
    int cantidad_goles_contra;
    int partidos_ganados;
    int partidos_empatados;
    int partidos_perdidos;
    int cantidad_tarjetas_amarillas;
    int cantidad_tarjetas_rojas;
    int cantidad_faltas;

public:

    void ingresar_jugador(const char** jugadores){
        //limpio las direcciones de memoria a usar
        jugadores = new jugadores*[26];
        for(unsigned int i = 0; i < 26; i++){
            jugadores[i] = nullptr;
        }

        for(unsigned int jugadores = 0; jugadores < 26; i++){
            char nombre_entrada[] = "jugador" +
            jugador()
        }


    }

    //CONSTRUCTOR
    seleccion()


}; */

int main() {
    ifstream archivo("C:\\Users\\Emmanuel\\Documents\\PRUEBAS_DESAFIO2\\selecciones_clasificadas_mundial.csv");

    if (!archivo.is_open()) {
        cout << "Error al abrir archivo\n";
        return 1;
    }

    char linea[200];

    while (archivo.getline(linea, 200)) {
        cout << linea << endl;  // por ahora solo imprime
    }

    archivo.close();
}

