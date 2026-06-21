#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Nodo {
    int edificio;
    int tiempo;
    Nodo* siguiente;
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
///////////////

};

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

    public:
        tGrafo(int vertices) {
            numEdificios = vertices;
            prerequisitos = 0;
            listaAdyacencia = new tlista[vertices];
        }

        ~tGrafo() {
            delete[] listaAdyacencia;

        }

        void CrearNodo(int* tiempos){

            for(int i = 0; i< numEdificios; i++){
                listaAdyacencia[i].insertar(i,tiempos[i]);
            }

        }

        void agregarArco(int a, int b) {
           
            prerequisitos++;
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

///////////////////////////77
};

void leerPrimeraLinea(const string& nombreArchivo, int& n, int& m, int& k) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo." << endl;
        return;
    }

    archivo >> n >> m >> k;

    cout << "n=" << n << ", m=" << m << ", k=" << k << endl;
    archivo.close();
}


void abrirarchivo() {
    cin.ignore(); // Limpiar el buffer de entrada
    string nombreArchivo;
    cout << "Ingrese el nombre del archivo: ";
    getline(cin, nombreArchivo); // Leer el nombre del archivo con espacios
    string s = nombreArchivo + ".txt"; // Puedes cambiar esto por la ruta de tu archivo
    ifstream archivo(s);
    if (archivo.is_open()) {
        cout << "Archivo '" << s << "' abierto correctamente." << endl;
        string linea;
        while (getline(archivo, linea)) {
            cout << linea << endl; // Imprimir cada línea del archivo
        }
        archivo.close();
    } else {
        cout << "Error al abrir el archivo." << endl;
    }
}

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

    if(t=1){
        
    }



    }

    cout << n <<endl;

    g.CrearNodo(tiempos);
    g.imprimirGrafo(); 
   
    return 0;
}
