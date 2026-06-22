#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

struct Nodo {
    int edificio;
    int tiempo;
    Nodo* siguiente;
};

struct calculoResultado {
    int* orden; // orden de construccion de edificios
    int tiempoCritico; // tiempo total de construccion
    int cantidad; // cantidad de edificios en el orden
    bool tieneCiclo; // indica si el grafo tiene ciclo
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


    /////
    void insertar(int edificio, int tiempo);

    Nodo*getHead() const { return head; }

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
void tlista::actualizarTiempo(int tiempo) {
    head->tiempo = tiempo;
}

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



tlista::tlista() {
    head = nullptr;
    tail = nullptr;
    curr = nullptr;
    listSize = 0;
    pos = 0;
}

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

    public:
        tGrafo(int vertices) {
            numEdificios = vertices;
            prerequisitos = 0;
            gradoEntrada = new int[vertices](); 
            listaAdyacencia = new tlista[vertices];
        }

        ~tGrafo() {
            delete[] listaAdyacencia;
            delete[] gradoEntrada;
        }

        void CrearNodo(int* tiempos){

            for(int i = 0; i< numEdificios; i++){
                listaAdyacencia[i].insertar(i,tiempos[i]);
            }

        }

        void agregarArco(int a, int b) {
           
            prerequisitos++;
        }
        void quitarArco(int a, int b) {
            if(listaAdyacencia[a].eliminar(b)){
                prerequisitos--;
                gradoEntrada[b]--;
            }
        }
        calculoResultado calcularOrdenYTiempo() {
            calculoResultado resultado;
            resultado.orden = new int[numEdificios];
            resultado.tiempoCritico = 0;
            resultado.cantidad = 0;
            resultado.tieneCiclo = false;
            int* gradoTemp = new int[numEdificios];// grado de entrada temporal
            int* inicio = new int[numEdificios];// inicio[i] = tiempo más temprano en que puede EMPEZAR a construirse el edificio i
            //inicializamos gradoTemp e inicio
            for (int i = 0; i < numEdificios; i++) {
                gradoTemp[i] = gradoEntrada[i];
                inicio[i] = 0;
            }
            //cola manual (arreglo circular) para los nodos con grado 0
            int* cola = new int[numEdificios];
            int colainicio = 0, colafinal = 0;
            // Encolamos todos los edificios sin prerrequisitos
            for (int i = 0; i < numEdificios; i++) {
                if (gradoTemp[i] == 0) {
                    cola[colafinal++] = i;
                }
            }
            int procesados = 0;
            //elemento de menor tiempo de inicio en la cola actual (entre colainicio y colafinal)
            while (colainicio < colafinal) {
                int aux = colainicio, edi, cm;
                for (int i = colainicio; i < colafinal; i++) {
                    edi = cola[i];
                    cm = cola[aux];
                    if (inicio[edi] < inicio[cm]) {
                        aux = i;
                    }
                }
                //se saca ese elemento de la cola (cambia con el frente)
                int u = cola[aux];
                cola[aux] = cola[colainicio];
                colainicio++;
                resultado.orden[resultado.cantidad++] = u;
                //propagamos el tiempo de inicio a los sucesores
                Nodo* vecino = listaAdyacencia[u].getHead();
                while (vecino != nullptr) {
                    int v = vecino->edificio;
                    int posibleInicio = inicio[u] + vecino->tiempo;
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
                // Tiempo crítico = max(inicio[i] + tiempos[i]) para todo i
                for (int i = 0; i < numEdificios; i++) {
                    int fin = inicio[i] + listaAdyacencia[i].getHead()->tiempo;
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
            
        
        void imprimirResultado(const calculoResultado& res) {
            if (res.tieneCiclo) {
                cout << -1 << endl;
                return;
            }
            for (int i = 0; i < res.cantidad; i++) {
                cout << res.orden[i] + 1; // volvemos a 1-indexado para la salida
                if (i < res.cantidad - 1) cout << " ";
            }
            cout << endl;
            cout << res.tiempoCritico << endl;
        }
        void NuevoTiempo(int edificio, int tiempo) {
            listaAdyacencia[edificio].actualizarTiempo(tiempo);
        }
        

//////////////////////////
        void imprimirGrafo() {
    cout << "\n=== GRAFO ===" << endl;
    for (int i = 0; i < numEdificios; i++) {
        cout << "Edificio " << i << ": ";
        listaAdyacencia[i].imprimir();
    }
    cout << "Total prerequisitos: " << prerequisitos << endl;
}

///////////////////////////

};



int main() {
    
    ifstream archivo("archivo.txt");
    int n,m,k,v1,v2,t;
    int* tiempos;
    
    archivo >> n >> m >> k;

    tiempos = new int[n];
    for(int i = 0; i<n; i++){
        archivo >> tiempos[i];
    }

    tGrafo g(n);
    g.CrearNodo(tiempos);

    for(int i = 0; i<m; i++){
        
        archivo >> v1 >> v2;
        g.agregarArco(v1,v2);
        
    }

    for(int i = 0; i < k; i++){
        archivo >> t;

    if(t==1){
        archivo >> v1 >> v2;
        g.NuevoTiempo(v1,v2);
    }
    else if(t==2){
        archivo >> v1 >> v2;
        g.agregarArco(v1,v2);
    }
    else if(t==3){
        archivo >> v1 >> v2;
        g.quitarArco(v1,v2);

    }
    if (i%(int)sqrt(k) == 0) {
        calculoResultado res = g.calcularOrdenYTiempo();
        g.imprimirResultado(res);
        delete[] res.orden;
    }
}
    cout << n <<endl;

    g.CrearNodo(tiempos);
    g.imprimirGrafo(); 
   
    return 0;
}
