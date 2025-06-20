class ArbolGenealogico {
private:
    Persona* raiz;

    // M�todos auxiliares privados
    Persona* insertarABB(Persona* nodo, Persona* nuevo);
    Persona* buscar(Persona* nodo, int id);
    void agregarHijo(Persona* padre, Persona* hijo);

public:
    // Interfaz p�blica
    ArbolGenealogico();
    void insertar(int id, string nombre, string fecha = "", int padre_id = -1, int madre_id = -1);
    void mostrarPreorden();
    void mostrarPreorden(Persona* nodo);
    void mostrarFamilia(int id);
};
