#include <iostream>
#include <string>
using namespace std;

struct Persona {
    int id;
    string nombre;
    string fecha_nacimiento;
    Persona* padre;
    Persona* madre;
    Persona* hijos;
    Persona* hermanos;

    Persona(int id, string nombre, string fecha = "") 
        : id(id), nombre(nombre), fecha_nacimiento(fecha), 
          padre(NULL), madre(NULL), hijos(NULL), hermanos(NULL) {}
};

class ArbolGenealogico {
private:
    Persona* raiz;


    Persona* insertarABB(Persona* nodo, Persona* nuevo) {
        if (!nodo) return nuevo;
        
        if (nuevo->id < nodo->id)
            nodo->padre = insertarABB(nodo->padre, nuevo);
        else
            nodo->madre = insertarABB(nodo->madre, nuevo);
        
        return nodo;
    }

    Persona* buscar(Persona* nodo, int id) {
        if (!nodo) return NULL;
        if (id == nodo->id) return nodo;
        if (id < nodo->id) return buscar(nodo->padre, id);
        return buscar(nodo->madre, id);
    }

    void agregarHijo(Persona* padre, Persona* hijo) {
        hijo->hermanos = padre->hijos;
        padre->hijos = hijo;
    }

    void eliminarHijo(Persona* padre, int id_hijo) {
        if (!padre || !padre->hijos) return;
        
        Persona* actual = padre->hijos;
        Persona* anterior = NULL;
        
        while (actual && actual->id != id_hijo) {
            anterior = actual;
            actual = actual->hermanos;
        }
        
        if (!actual) return;
        
        if (!anterior)
            padre->hijos = actual->hermanos;
        else
            anterior->hermanos = actual->hermanos;
    }

    Persona* encontrarMinimo(Persona* nodo) {
        while (nodo && nodo->padre)
            nodo = nodo->padre;
        return nodo;
    }

    Persona* eliminarNodoABB(Persona* nodo, int id) {
        if (!nodo) return NULL;
        
        if (id < nodo->id) {
            nodo->padre = eliminarNodoABB(nodo->padre, id);
        } else if (id > nodo->id) {
            nodo->madre = eliminarNodoABB(nodo->madre, id);
        } else {
            if (!nodo->padre) {
                Persona* temp = nodo->madre;
                delete nodo;
                return temp;
            } else if (!nodo->madre) {
                Persona* temp = nodo->padre;
                delete nodo;
                return temp;
            }
            
            Persona* temp = encontrarMinimo(nodo->madre);
            nodo->id = temp->id;
            nodo->nombre = temp->nombre;
            nodo->fecha_nacimiento = temp->fecha_nacimiento;
            nodo->madre = eliminarNodoABB(nodo->madre, temp->id);
        }
        return nodo;
    }

public:
    ArbolGenealogico() : raiz(NULL) {}

    void insertar(int id, string nombre, string fecha = "", int padre_id = -1, int madre_id = -1) {
        if (buscar(raiz, id)) {
            cout << "Error: ID ya existe\n";
            return;
        }

        Persona* nueva = new Persona(id, nombre, fecha);

        if (!raiz) {
            raiz = nueva;
        } else {
            raiz = insertarABB(raiz, nueva);
        }

        if (padre_id != -1) {
            Persona* padre = buscar(raiz, padre_id);
            if (padre) {
                nueva->padre = padre;
                agregarHijo(padre, nueva);
            } else {
                cout << "Advertencia: Padre ID " << padre_id << " no encontrado\n";
            }
        }

        if (madre_id != -1) {
            Persona* madre = buscar(raiz, madre_id);
            if (madre) {
                nueva->madre = madre;
                agregarHijo(madre, nueva);
            } else {
                cout << "Advertencia: Madre ID " << madre_id << " no encontrado\n";
            }
        }
    }

    bool editar(int id, string nuevo_nombre = "", string nueva_fecha = "", int nuevo_padre_id = -1, int nueva_madre_id = -1) {
        Persona* persona = buscar(raiz, id);
        if (!persona) {
            cout << "Error: Persona no encontrada\n";
            return false;
        }

        if (!nuevo_nombre.empty()) persona->nombre = nuevo_nombre;
        if (!nueva_fecha.empty()) persona->fecha_nacimiento = nueva_fecha;

        if (nuevo_padre_id != -1) {
            Persona* nuevo_padre = buscar(raiz, nuevo_padre_id);
            if (nuevo_padre) {
                if (persona->padre) eliminarHijo(persona->padre, id);
                persona->padre = nuevo_padre;
                agregarHijo(nuevo_padre, persona);
            } else {
                cout << "Advertencia: Nuevo padre no encontrado\n";
            }
        }

        if (nueva_madre_id != -1) {
            Persona* nueva_madre = buscar(raiz, nueva_madre_id);
            if (nueva_madre) {
                if (persona->madre) eliminarHijo(persona->madre, id);
                persona->madre = nueva_madre;
                agregarHijo(nueva_madre, persona);
            } else {
                cout << "Advertencia: Nueva madre no encontrada\n";
            }
        }

        return true;
    }

    bool eliminar(int id) {
        Persona* persona = buscar(raiz, id);
        if (!persona) {
            cout << "Error: Persona no encontrada\n";
            return false;
        }

        if (persona->padre) eliminarHijo(persona->padre, id);
        if (persona->madre) eliminarHijo(persona->madre, id);

        raiz = eliminarNodoABB(raiz, id);

        return true;
    }

    void mostrarFamilia(int id) {
        Persona* persona = buscar(raiz, id);
        if (!persona) {
            cout << "Persona no encontrada.\n";
            return;
        }

        cout << "\nInformacion de " << persona->nombre << " (ID: " << persona->id << "):\n";
        cout << "Fecha de nacimiento: " << (persona->fecha_nacimiento.empty() ? "Desconocida" : persona->fecha_nacimiento) << "\n";
        
        if (persona->padre || persona->madre) {
            cout << "Padres: ";
            if (persona->padre) cout << persona->padre->nombre << " (ID: " << persona->padre->id << ")";
            if (persona->madre) {
                if (persona->padre) cout << " y ";
                cout << persona->madre->nombre << " (ID: " << persona->madre->id << ")";
            }
            cout << endl;
        }

        if (persona->hijos) {
            cout << "Hijos: ";
            Persona* hijo = persona->hijos;
            while (hijo) {
                cout << hijo->nombre << " (ID: " << hijo->id << ")";
                hijo = hijo->hermanos;
                if (hijo) cout << ", ";
            }
            cout << endl;
        } else {
            cout << "No tiene hijos registrados\n";
        }
    }

    void mostrarMenu() {
        int opcion;
        do {
            cout << "\nMENU ARBOL GENEALOGICO\n";
            cout << "1. Agregar persona\n";
            cout << "2. Mostrar familia\n";
            cout << "3. Editar persona\n";
            cout << "4. Eliminar persona\n";
            cout << "5. Salir\n";
            cout << "Seleccione una opcion: ";
            cin >> opcion;

            switch(opcion) {
                case 1: {
                    int id, padre_id, madre_id;
                    string nombre, fecha;
                    
                    cout << "Ingrese ID: ";
                    cin >> id;
                    cin.ignore();
                    
                    cout << "Ingrese nombre: ";
                    getline(cin, nombre);
                    
                    cout << "Ingrese fecha de nacimiento (opcional): ";
                    getline(cin, fecha);
                    
                    cout << "Ingrese ID del padre (-1 si no aplica): ";
                    cin >> padre_id;
                    
                    cout << "Ingrese ID de la madre (-1 si no aplica): ";
                    cin >> madre_id;
                    
                    insertar(id, nombre, fecha, padre_id, madre_id);
                    break;
                }
                case 2: {
                    int id;
                    cout << "Ingrese ID de la persona a buscar: ";
                    cin >> id;
                    mostrarFamilia(id);
                    break;
                }
                case 3: {
                    int id, nuevo_padre_id, nueva_madre_id;
                    string nuevo_nombre, nueva_fecha;
                    
                    cout << "Ingrese ID de la persona a editar: ";
                    cin >> id;
                    cin.ignore();
                    
                    cout << "Ingrese nuevo nombre (dejar vacío para no cambiar): ";
                    getline(cin, nuevo_nombre);
                    
                    cout << "Ingrese nueva fecha de nacimiento (dejar vacío para no cambiar): ";
                    getline(cin, nueva_fecha);
                    
                    cout << "Ingrese nuevo ID del padre (-1 para no cambiar): ";
                    cin >> nuevo_padre_id;
                    
                    cout << "Ingrese nuevo ID de la madre (-1 para no cambiar): ";
                    cin >> nueva_madre_id;
                    
                    editar(id, nuevo_nombre, nueva_fecha, nuevo_padre_id, nueva_madre_id);
                    break;
                }
                case 4: {
                    int id;
                    cout << "Ingrese ID de la persona a eliminar: ";
                    cin >> id;
                    eliminar(id);
                    break;
                }
                case 5:
                    cout << "Saliendo...\n";
                    break;
                default:
                    cout << "Opcion no valida\n";
            }
        } while (opcion != 5);
    }
};

int main() {
    ArbolGenealogico arbol;
    arbol.mostrarMenu();
    return 0;
}
