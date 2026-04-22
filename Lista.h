#ifndef LISTA_H
#define LISTA_H

#include <iostream>

// 3.1 Estructura de datos: Clase Nodo genérica
class Nodo {
public:
    T data;            // Campo de datos genérico [cite: 680, 684]
    Nodo<T>* ptrNext;  // Apuntador al siguiente nodo [cite: 682, 685]

    Nodo(T value) : data(value), ptrNext(nullptr) {}
};

// 3.2. Operaciones de la clase lista [cite: 693, 694]
template <typename T>
class lista {
private:
    Nodo<T>* head;      // Apuntador al primer elemento [cite: 698]
    Nodo<T>* tail;      // Eficiencia: Apuntador al último elemento para O(1) [cite: 699, 708]
    int _tamaño;        // Atributo para almacenar el tamaño actual [cite: 696]

    // Atributo estático para rastrear memoria ocupada
    static long memoriaTotal;

public:
    // Construir una lista vacía [cite: 695]
    lista() : head(nullptr), tail(nullptr), _tamaño(0) {}

    // Destruir lista: Libera la memoria de todos los nodos
    ~lista() {
        while (!esVacia()) {
            // Se podría implementar una lógica interna de borrado aquí
            Nodo<T>* temp = head;
            head = head->ptrNext;
            memoriaTotal -= sizeof(Nodo<T>); // Restar memoria al liberar
            delete temp;
            _tamaño--;
        }
    }

    // tamaño(): Devuelve el tamaño de la lista
    int tamaño() const { return _tamaño; }

    // esVacia(): Verifica si la lista está vacía
    bool esVacia() const { return head == nullptr; }

    // primero(): Devuelve el primer elemento (Dato)
    T primero() const {
        if (esVacia()) throw std::runtime_error("Lista vacia");
        return head->data;
    }

    // ultimo(): Devuelve el último elemento (Dato)
    // Eficiencia: O(1) gracias al puntero tail
    T ultimo() const {
        if (esVacia()) throw std::runtime_error("Lista vacia");
        return tail->data;
    }

    // agregar(e, i): Agrega un elemento e en la posición i
    void agregar(T e, int i) {
        if (i < 0 || i > _tamaño) return;

        Nodo<T>* nuevo = new Nodo<T>(e);
        totalMemory += sizeof(Nodo<T>); // Rastrear memoria del nuevo nodo

        if (i == 0) { // Insertar al inicio
            nuevo->ptrNext = head;
            head = nuevo;
            if (_tamaño == 0) tail = nuevo;
        } else if (i == _tamaño) { // Insertar al final (Eficiencia O(1))
            tail->ptrNext = nuevo;
            tail = nuevo;
        } else { // Insertar en medio
            Nodo<T>* ant = head;
            for (int j = 0; j < i - 1; j++) ant = ant->ptrNext;
            nuevo->ptrNext = ant->ptrNext;
            ant->ptrNext = nuevo;
        }
        _tamano++;
    }

    // eliminar(e): Elimina la primera aparición del elemento e
    void eliminar(T e) {
        if (esVacia()) return;

        Nodo<T>* actual = head;
        Nodo<T>* anterior = nullptr;

        while (actual != nullptr && actual->data != e) {
            anterior = actual;
            actual = actual->ptrNext;
        }

        if (actual != nullptr) { // Se encontró el elemento
            if (actual == head) head = head->ptrNext;
            else anterior->ptrNext = actual->ptrNext;

            if (actual == tail) tail = anterior;

            memoriaTotal -= sizeof(Nodo<T>); // Liberar rastreo de memoria
            delete actual;
            _tamaño--;
        }
    }

    // ■ consultar(i): Devuelve el dato en la posición i
    T consultar(int i) const {
        if (i < 0 || i >= _tamaño) throw std::out_of_range("Indice invalido");
        Nodo<T>* actual = head;
        for (int j = 0; j < i; j++) actual = actual->ptrNext;
        return actual->data;
    }

    // reemplazar(e, i): Reemplaza el elemento en la posición i [cite: 704]
    void reemplazar(T e, int i) {
        if (i < 0 || i >= _tamaño) return;
        Nodo<T>* actual = head;
        for (int j = 0; j < i; j++) actual = actual->ptrNext;
        actual->data = e;
    }

    // memoriaUsada(): Devuelve la memoria ocupada (estático)
    static long memoriaUsada() {
        return memoriaTotal;
    }

    // ■ concatenar(B): Concatena la lista B al final
    void concatenar(const lista<T>& B) {
        for (int i = 0; i < B.tamano(); i++) {
            this->agregar(B.consultar(i), _tamaño);
        }
    }
};

// Inicialización del atributo estático
template <typename T>
long lista<T>::memoriaTotal = 0;

#endif
