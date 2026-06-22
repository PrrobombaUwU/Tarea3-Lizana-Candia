#include <iostream>
#include <fstream>
#include <string>

using namespace std;
// Nodo de la lista enlazada, representa un edificio adyacente
struct Nodo {
    int edificio;
    int tiempo;
    Nodo* siguiente;
};
// Estructura que almacena el resultado del calculo de orden y tiempo critico
struct calculoResultado {
    int* orden; 
    int tiempoCritico; 
    int cantidad; 
    bool tieneCiclo; 
};

class tlista
{
private:
    Nodo* head; 
    Nodo* tail; 
    Nodo* curr; 
    unsigned int listSize; 
    unsigned int pos; 

public:
    tlista();
    ~tlista();

    /* ****
     * void tlista
     ******
     * Inicializa una lista enlazada vacia con todos sus punteros en nullptr
     * y contadores en 0.
     ******
     * Input:
     * No recibe nada
     ******
     * Returns:
     * No retorna nada
     **** */
    void insertar(int edificio, int tiempo);

    /* ****
     * Nodo* Obtenercabeza
     ******
     * Retorna el puntero al primer nodo de la lista.
     ******
     * Input:
     * No recibe nada
     ******
     * Returns:
     * Nodo* retorna un puntero de tipo nodo
     **** */
    Nodo*obtenercabeza() const { return head; }

    /* ****
     * void imprimir
     ******
     * Recorre la lista enlazada e imprime cada nodo con su edificio y tiempo
     * en formato [Edificio: X | Tiempo: Y]. 
     ******
     * Input:
     * No recibe nada   
     ******
     * Returns:
     * No retorna nada
     **** */
    void imprimir() {
        Nodo* actual = head;
        while (actual != nullptr) {
            cout << "[Edificio: " << actual->edificio 
                 << " | Tiempo: " << actual->tiempo << "]";
            if (actual->siguiente != nullptr) cout << " -> ";
            actual = actual->siguiente;
        }
        cout << endl;
    }

    void actualizarTiempo(int tiempo);
    bool eliminar(int edificio);

};
/* ****
 * void actualizarTiempo
 ******
 * Actualiza el campo tiempo del nodo (edificio)
 ******
 * Input:
 * int tiempo : nuevo valor de tiempo a asignar al edificio
 * Returns:
 * No retorna nada
 **** */
void tlista::actualizarTiempo(int tiempo) {
    head->tiempo = tiempo;
}

/* ****
 * void insertar
 ******
 * Agrega un nuevo nodo al final de la lista enlazada con el edificio
 * y tiempo indicados.
 ******
 * Input:
 * int edificio : identificador del edificio a insertar
 * int tiempo   : tiempo asociado al nodo
 ******
 * Returns:
 * No retorna nada
 **** */
void tlista::insertar(int edificio, int tiempo) {
    Nodo* nuevo = new Nodo();
    nuevo->edificio = edificio;
    nuevo->tiempo = tiempo;
    nuevo->siguiente = nullptr;

    if (head == nullptr) {  
        head = nuevo;
        tail = nuevo;
    } else {
        tail->siguiente = nuevo;
        tail = nuevo;
    }
    listSize++;
}

/* ****
 * bool eliminar
 ******
 * Busca y elimina el nodo cuyo campo edificio coincide con el valor dado.
 * Actualiza los punteros head y tail segun corresponda.
 ******
 * Input:
 * int edificio : identificador del edificio a eliminar
 ******
 * Returns:
 * bool, true si el nodo fue encontrado y eliminado, false si no existe
 **** */
bool tlista::eliminar(int edificio) {
    Nodo* actual = head;
    Nodo* anterior = nullptr;
    while (actual != nullptr) {
        if (actual->edificio == edificio) {
            if (anterior == nullptr) {
                head = actual->siguiente;
            } else {
                anterior->siguiente = actual->siguiente;
            }
            if (actual == tail) {
                tail = anterior;
            }
            delete actual;
            listSize--;
            return true;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    return false;
}

/* ****
 * Constructor tlista
 ******
 * Inicializa todos los punteros en nullptr y los contadores en 0.
 ******
 * Input:
 * No recibe nada
 ******
 * Returns:
 * No retorna nada
 **** */
tlista::tlista() {
    head = nullptr;
    tail = nullptr;
    curr = nullptr;
    listSize = 0;
    pos = 0;
}

/* ****
 * Destructor tlista
 ******
 * Libera la memoria de todos los nodos de la lista enlazada.
 ******
 * Input:
 * No recibe nada
 ******
 * Returns:
 * No retorna nada
 **** */
tlista::~tlista() {
    Nodo* actual = head;
    while (actual != nullptr) {
        Nodo* temp = actual->siguiente;
        delete actual;
        actual = temp;
    }
}

class tGrafo {
    private:
        int numEdificios;
        int prerequisitos;
        tlista* listaAdyacencia;
        int* gradoEntrada;
        int* duraciones;

    public:
        /* ****
         * Constructor tGrafo
         ******
         * Inicializa el grafo con el numero de vertices dado. Reserva memoria
         * para la lista de adyacencia, grados de entrada y duraciones.
         ******
         * Input:
         * int vertices : numero de edificios (nodos) del grafo
         ******
         * Returns:
         * No retorna nada
         **** */
        tGrafo(int vertices) {
            numEdificios = vertices;
            prerequisitos = 0;
            gradoEntrada = new int[vertices]();
            duraciones = new int[vertices]();
            listaAdyacencia = new tlista[vertices];
        }
         /* ****
         * Destructor tGrafo
         ******
         * Libera la memoria de la lista de adyacencia, grados de entrada
         * y duraciones.
         ******
         * Input:
         * No recibe nada
         ******
         * Returns:
         * No retorna nada
         **** */
        ~tGrafo() {
            delete[] listaAdyacencia;
            delete[] gradoEntrada;
            delete[] duraciones;
        }

        /* ****
         * Void CrearNodo
         ******
         * Copia el arreglo de tiempos de construccion al arreglo interno
         * de duraciones del grafo.
         ******
         * Input:
         * int* tiempos : arreglo con los tiempos de construccion de cada edificio
         ******
         * Returns:
         * No retorna nada
         **** */
        void CrearNodo(int* tiempos) {
            for (int i = 0; i < numEdificios; i++) {
                duraciones[i] = tiempos[i];
            }
        }

        /* ****
         * void agregarArco
         ******
         * Agrega un arco dirigido de a hacia b en la lista de adyacencia
         * e incrementa el grado de entrada de b.
         ******
         * Input:
         * int a : indice del edificio origen (0-indexado)
         * int b : indice del edificio destino (0-indexado)
         ******
         * Returns:
         * No retorna nada
         **** */
        void agregarArco(int a, int b) {
            if (a < 0 || a >= numEdificios || b < 0 || b >= numEdificios) return;
            listaAdyacencia[a].insertar(b, 0);
            gradoEntrada[b]++;
            prerequisitos++;
        }

        /* ****
         * void quitarArco
         ******
         * Elimina el arco dirigido de a hacia b de la lista de adyacencia
         * y decrementa el grado de entrada de b si el arco existia.
         ******
         * Input:
         * int a : indice del edificio origen (0-indexado)
         * int b : indice del edificio destino (0-indexado)
         ******
         * Returns:
         * No retorna nada
         **** */
        void quitarArco(int a, int b) {
            if (a < 0 || a >= numEdificios || b < 0 || b >= numEdificios) return;
            if (listaAdyacencia[a].eliminar(b)) {
                prerequisitos--;
                gradoEntrada[b]--;
            }
        }

        /* ****
         * calculoResultado calcularOrdenYTiempo
         ******
         * Aplica el algoritmo de Kahn modificado para obtener el orden
         * topologico de construccion, priorizando el edificio con menor
         * tiempo de inicio disponible. Calcula ademas el tiempo critico
         * total. Si detecta un ciclo, marca el resultado con tieneCiclo=true.
         ******
         * Input:
         * No recibe nada
         ******
         * Returns:
         * calculoResultado, estructura con el orden de construccion,
         * tiempo critico, cantidad de edificios procesados e indicador de ciclo
         **** */
        calculoResultado calcularOrdenYTiempo() {
            calculoResultado resultado;
            resultado.orden = new int[numEdificios];
            resultado.tiempoCritico = 0;
            resultado.cantidad = 0;
            resultado.tieneCiclo = false;

            int* gradoTemp = new int[numEdificios];
            int* inicio = new int[numEdificios];
            for (int i = 0; i < numEdificios; i++) {
                gradoTemp[i] = gradoEntrada[i];
                inicio[i] = 0;
            }

            int* cola = new int[numEdificios];
            int colainicio = 0, colafinal = 0;
            for (int i = 0; i < numEdificios; i++) {
                if (gradoTemp[i] == 0) {
                    cola[colafinal++] = i;
                }
            }

            int procesados = 0;
            while (colainicio < colafinal) {
                int aux = colainicio;
                for (int i = colainicio; i < colafinal; i++) {
                    int edi = cola[i];
                    int cm = cola[aux];
                    if (inicio[edi] < inicio[cm]) {
                        aux = i;
                    }
                }
                int u = cola[aux];
                cola[aux] = cola[colainicio];
                colainicio++;
                resultado.orden[resultado.cantidad++] = u;
                procesados++;

                Nodo* vecino = listaAdyacencia[u].obtenercabeza();
                while (vecino != nullptr) {
                    int v = vecino->edificio;
                    int posibleInicio = inicio[u] + duraciones[u];
                    if (posibleInicio > inicio[v]) {
                        inicio[v] = posibleInicio;
                    }
                    gradoTemp[v]--;
                    if (gradoTemp[v] == 0) {
                        cola[colafinal++] = v;
                    }
                    vecino = vecino->siguiente;
                }
            }

            if (procesados < numEdificios) {
                resultado.tieneCiclo = true;
                resultado.tiempoCritico = -1;
            } else {
                resultado.cantidad = procesados;
                for (int i = 0; i < numEdificios; i++) {
                    int fin = inicio[i] + duraciones[i];
                    if (fin > resultado.tiempoCritico) {
                        resultado.tiempoCritico = fin;
                    }
                }
            }

            delete[] gradoTemp;
            delete[] inicio;
            delete[] cola;
            return resultado;
        }

        /* ****
         * void imprimirResultado
         ******
         * Imprime el orden de construccion y el tiempo critico. Si el grafo
         * tiene ciclo, imprime -1 unicamente.
         ******
         * Input:
         * const calculoResultado& res : resultado obtenido de calcularOrdenYTiempo
         ******
         * Returns:
         * No retorna nada
         **** */
        void imprimirResultado(const calculoResultado& res) {
            if (res.tieneCiclo) {
                cout << -1 << endl;
                return;
            }
            for (int i = 0; i < res.cantidad; i++) {
                cout << res.orden[i] + 1;
                if (i < res.cantidad - 1) cout << " ";
            }
            cout << endl;
            cout << res.tiempoCritico << endl;
        }

        /* ****
         * void NuevoTiempo
         ******
         * Actualiza la duracion de construccion del edificio indicado.
         ******
         * Input:
         * int edificio : indice del edificio a modificar (0-indexado)
         * int tiempo   : nueva duracion de construccion
         ******
         * Returns:
         * No retorna nada
         **** */
        void NuevoTiempo(int edificio, int tiempo) {
            if (edificio < 0 || edificio >= numEdificios) return;
            duraciones[edificio] = tiempo;
        }

};



int main() {
    ifstream archivo("archivo.txt");
    int n, m, k, v1, v2, t;
    int* tiempos;

    if (!archivo.is_open()) {
        cerr << "No se pudo abrir archivo.txt" << endl;
        return 1;
    }

    archivo >> n >> m >> k;
    if (n <= 0) {
        cerr << "Numero de edificios invalido" << endl;
        return 1;
    }

    tiempos = new int[n];
    for (int i = 0; i < n; i++) {
        archivo >> tiempos[i];
    }

    // Calcula la raiz cuadrada entera de k para determinar cada cuantas consultas imprimir
    int sqrtK = 1;
    while ((sqrtK + 1) * (sqrtK + 1) <= k) sqrtK++; //calcula la raiz cuadrada de k para determinar cada cuantas consultas imprimir resultado
    tGrafo g(n);
    g.CrearNodo(tiempos);
    
    // Cargar prerequisitos iniciales
    for (int i = 0; i < m; i++) {
        archivo >> v1 >> v2;
        v1--; v2--;
        g.agregarArco(v1, v2);
    }
    // Imprimir estado inicial del grafo
    calculoResultado resInicial = g.calcularOrdenYTiempo();
    g.imprimirResultado(resInicial);
    delete[] resInicial.orden;

    // Procesar consultas e imprimir resultado cada sqrtK consultas
    for (int i = 0; i < k; i++) {
        archivo >> t;
        if (t == 1) {
            archivo >> v1 >> v2;
            v1--;
            g.NuevoTiempo(v1, v2);
        } else if (t == 2) {
            archivo >> v1 >> v2;
            v1--; v2--;
            g.agregarArco(v1, v2);
        } else if (t == 3) {
            archivo >> v1 >> v2;
            v1--; v2--;
            g.quitarArco(v1, v2);
        }
        if ((i+1) % sqrtK == 0) {
            calculoResultado res = g.calcularOrdenYTiempo();
            g.imprimirResultado(res);
            delete[] res.orden;
        }
    }

    delete[] tiempos;
    return 0;
}