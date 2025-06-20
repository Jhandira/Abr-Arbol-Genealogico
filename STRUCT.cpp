#include <iostream>
#include <string>
using namespace std;
struct Persona {
    int id;
    string nombre;
    string fecha_nacimiento;
    Persona* padre;    // Subárbol izquierdo (ABB)
    Persona* madre;     // Subárbol derecho (ABB)
    Persona* hijos;     // Lista de hijos (solo primer hijo)
    Persona* hermanos;  // Hermanos (lista enlazada)

    Persona(int id, string nombre, string fecha = "") 
        : id(id), nombre(nombre), fecha_nacimiento(fecha), 
          padre(nullptr), madre(nullptr), hijos(nullptr), hermanos(nullptr) {}
};
