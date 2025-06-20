#include <iostream>
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
          padre(NULL), madre(NULL), hijos(NULL), hermanos(NULL) {}
};

class ArbolGenealogico {
private:
    Persona* raiz;

    // Función auxiliar para insertar en ABB
    Persona* insertarABB(Persona* nodo, Persona* nuevo) {
        if (!nodo) return nuevo;
        
        if (nuevo->id < nodo->id)
            nodo->padre = insertarABB(nodo->padre, nuevo);
        else
            nodo->madre = insertarABB(nodo->madre, nuevo);
        
        return nodo;
    }

    // Función para buscar persona por ID
    Persona* buscar(Persona* nodo, int id) {
        if (!nodo) return NULL;
        if (id == nodo->id) return nodo;
        if (id < nodo->id) return buscar(nodo->padre, id);
        return buscar(nodo->madre, id);
    }

    // Función para agregar un hijo a una persona
    void agregarHijo(Persona* padre, Persona* hijo) {
        hijo->hermanos = padre->hijos;
        padre->hijos = hijo;
    }

public:
    ArbolGenealogico() : raiz(NULL) {}

    // Insertar una nueva persona al árbol
    void insertar(int id, string nombre, string fecha = "", int padre_id = -1, int madre_id = -1) {
        Persona* nueva = new Persona(id, nombre, fecha);

        // Insertar en el ABB
        if (!raiz) {
            raiz = nueva;
        } else {
            raiz = insertarABB(raiz, nueva);
        }

        // Establecer relaciones familiares
        if (padre_id != -1) {
            Persona* padre = buscar(raiz, padre_id);
            if (padre) {
                nueva->padre = padre;
                agregarHijo(padre, nueva);
            }
        }

        if (madre_id != -1) {
            Persona* madre = buscar(raiz, madre_id);
            if (madre) {
                nueva->madre = madre;
                agregarHijo(madre, nueva);
            }
        }
    }

    // Mostrar el árbol en preorden
    void mostrarPreorden() {
        cout << "Recorrido Preorden:\n";
        mostrarPreorden(raiz);
        cout << endl;
    }

    void mostrarPreorden(Persona* nodo) {
        if (nodo) {
            cout << nodo->nombre << " (ID: " << nodo->id << ", Nac: " << nodo->fecha_nacimiento << ")\n";
            mostrarPreorden(nodo->padre);
            mostrarPreorden(nodo->madre);
        }
    }

    // Mostrar familia (padres e hijos)
    void mostrarFamilia(int id) {
        Persona* persona = buscar(raiz, id);
        if (!persona) {
            cout << "Persona no encontrada.\n";
            return;
        }

        cout << "\nFamilia de " << persona->nombre << ":\n";
        
        // Mostrar padres
        if (persona->padre || persona->madre) {
            cout << "Padres: ";
            if (persona->padre) cout << persona->padre->nombre;
            if (persona->madre) {
                if (persona->padre) cout << " y ";
                cout << persona->madre->nombre;
            }
            cout << endl;
        }

        // Mostrar hijos
        if (persona->hijos) {
            cout << "Hijos: ";
            Persona* hijo = persona->hijos;
            while (hijo) {
                cout << hijo->nombre;
                hijo = hijo->hermanos;
                if (hijo) cout << ", ";
            }
            cout << endl;
        }
    }
};

int main() {
    ArbolGenealogico arbol;

    // Insertar datos genealógicos
    arbol.insertar(100, "Abuelo", "01/01/1950");
    arbol.insertar(50, "Padre", "01/01/1980", 100);  // Hijo de Abuelo
    arbol.insertar(150, "Tio", "01/01/1982", 100);   // Hijo de Abuelo
    arbol.insertar(25, "Hijo", "01/01/2010", 50);    // Hijo de Padre
    arbol.insertar(75, "Hija", "01/01/2012", 50);    // Hijo de Padre
    arbol.insertar(125, "Primo", "01/01/2011", 150); // Hijo de Tio

    // Mostrar recorrido
    arbol.mostrarPreorden();

    // Mostrar familias
    arbol.mostrarFamilia(100);  // Abuelo
    arbol.mostrarFamilia(50);   // Padre
    arbol.mostrarFamilia(150);  // Tio
    arbol.mostrarFamilia(25);   // Hijo

    return 0;
}
