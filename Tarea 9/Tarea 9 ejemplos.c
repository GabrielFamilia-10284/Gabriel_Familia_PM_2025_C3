#include <stdio.h>
#include <stdlib.h>

// =============== 1. LISTA ENLAZADA ===============
typedef struct NodoLista {
    int dato;
    struct NodoLista* sig;
} NodoLista;

NodoLista* lista_cabeza = NULL;  // variable global temporal para el ejemplo

void lista_agregar(int valor) {
    NodoLista* nuevo = (NodoLista*)malloc(sizeof(NodoLista));
    nuevo->dato = valor;
    nuevo->sig = lista_cabeza;
    lista_cabeza = nuevo;
}

void lista_imprimir() {
    NodoLista* actual = lista_cabeza;
    while (actual != NULL) {
        printf("%d", actual->dato);
        if (actual->sig != NULL) printf(" -> ");
        actual = actual->sig;
    }
    printf("\n");
}

void lista_liberar() {
    NodoLista* actual = lista_cabeza;
    while (actual != NULL) {
        NodoLista* temp = actual;
        actual = actual->sig;
        free(temp);
    }
    lista_cabeza = NULL;
}

void ejemploLista() {
    printf("\n--- LISTA ENLAZADA ---\n");
    printf("Agrega 3, 2, 1 al inicio y muestra: 1 -> 2 -> 3\n");

    lista_agregar(3);
    lista_agregar(2);
    lista_agregar(1);

    printf("Lista: ");
    lista_imprimir();

    lista_liberar();
}

// =============== 2. ARBOL BINARIO ===============
typedef struct NodoArbol {
    int dato;
    struct NodoArbol* izq;
    struct NodoArbol* der;
} NodoArbol;

NodoArbol* arbol_crear_nodo(int valor) {
    NodoArbol* n = (NodoArbol*)malloc(sizeof(NodoArbol));
    n->dato = valor;
    n->izq = n->der = NULL;
    return n;
}

NodoArbol* arbol_insertar(NodoArbol* raiz, int valor) {
    if (raiz == NULL) return arbol_crear_nodo(valor);
    if (valor < raiz->dato)
        raiz->izq = arbol_insertar(raiz->izq, valor);
    else
        raiz->der = arbol_insertar(raiz->der, valor);
    return raiz;
}

void arbol_inorden(NodoArbol* raiz) {
    if (raiz != NULL) {
        arbol_inorden(raiz->izq);
        printf("%d ", raiz->dato);
        arbol_inorden(raiz->der);
    }
}

// (Opcional) liberar memoria del arbol — se omite por brevedad en ejemplo

void ejemploArbol() {
    printf("\n--- ARBOL BINARIO (BST) ---\n");
    printf("Inserta: 5, 3, 7, 2, 4 → Recorrido inorden: 2 3 4 5 7\n");

    NodoArbol* raiz = NULL;
    int datos[] = {5, 3, 7, 2, 4};
    for (int i = 0; i < 5; i++) {
        raiz = arbol_insertar(raiz, datos[i]);
    }

    printf("Inorden: ");
    arbol_inorden(raiz);
    printf("\n");

    // Para evitar fugas en ejemplo repetido, se podria liberar, pero se omite por simplicidad
}

// =============== 3. GRAFO (lista de adyacencia simple) ===============
#define MAX_NODOS 5

typedef struct {
    int ady[MAX_NODOS];
    int n;
} ListaAdy;

ListaAdy grafo[MAX_NODOS];

void grafo_reiniciar() {
    for (int i = 0; i < MAX_NODOS; i++) {
        grafo[i].n = 0;
    }
}

void grafo_agregar_arista(int u, int v) {
    if (grafo[u].n < MAX_NODOS) grafo[u].ady[grafo[u].n++] = v;
    if (grafo[v].n < MAX_NODOS) grafo[v].ady[grafo[v].n++] = u;
}

void ejemploGrafo() {
    printf("\n--- GRAFO NO DIRIGIDO ---\n");
    printf("Aristas: 0-1, 0-2, 1-3\n");

    grafo_reiniciar();
    grafo_agregar_arista(0, 1);
    grafo_agregar_arista(0, 2);
    grafo_agregar_arista(1, 3);

    for (int i = 0; i < 4; i++) {
        printf("Nodo %d: ", i);
        for (int j = 0; j < grafo[i].n; j++) {
            printf("%d ", grafo[i].ady[j]);
        }
        printf("\n");
    }
}

// =============== 4. TABLA HASH (sondeo lineal) ===============
#define TAM_HASH 7

int tabla_hash[TAM_HASH];
int ocupado_hash[TAM_HASH];

int hash_func(int clave) {
    return clave % TAM_HASH;
}

void hash_reiniciar() {
    for (int i = 0; i < TAM_HASH; i++) {
        tabla_hash[i] = -1;
        ocupado_hash[i] = 0;
    }
}

void hash_insertar(int clave) {
    int i = 0;
    int pos = hash_func(clave);
    while (ocupado_hash[pos] && i < TAM_HASH) {
        pos = (hash_func(clave) + i) % TAM_HASH;
        i++;
    }
    if (i < TAM_HASH) {
        tabla_hash[pos] = clave;
        ocupado_hash[pos] = 1;
    }
}

void ejemploTablaHash() {
    printf("\n--- TABLA HASH (sondeo lineal) ---\n");
    printf("Claves: 10, 20, 30 → hash = clave %% 7\n");

    hash_reiniciar();
    hash_insertar(10);
    hash_insertar(20);
    hash_insertar(30);

    for (int i = 0; i < TAM_HASH; i++) {
        if (ocupado_hash[i])
            printf("[%d]: %d\n", i, tabla_hash[i]);
        else
            printf("[%d]: vacio\n", i);
    }
}

// =============== 5. PILA (arreglo) ===============
#define MAX_PILA 5

int pila_datos[MAX_PILA];
int pila_tope = -1;

void pila_reiniciar() {
    pila_tope = -1;
}

void pila_push(int x) {
    if (pila_tope < MAX_PILA - 1)
        pila_datos[++pila_tope] = x;
}

int pila_pop() {
    if (pila_tope >= 0)
        return pila_datos[pila_tope--];
    return -1;
}

void ejemploPila() {
    printf("\n--- PILA (LIFO) ---\n");
    printf("Push: 10, 20, 30 → Pop: 30, 20, 10\n");

    pila_reiniciar();
    pila_push(10);
    pila_push(20);
    pila_push(30);

    printf("Pop: ");
    while (pila_tope >= 0) {
        printf("%d ", pila_pop());
    }
    printf("\n");
}

// =============== 6. COLA (circular) ===============
#define MAX_COLA 5

int cola_datos[MAX_COLA];
int cola_frente = 0, cola_final = -1, cola_n = 0;

void cola_reiniciar() {
    cola_frente = 0;
    cola_final = -1;
    cola_n = 0;
}

void cola_enqueue(int x) {
    if (cola_n < MAX_COLA) {
        cola_final = (cola_final + 1) % MAX_COLA;
        cola_datos[cola_final] = x;
        cola_n++;
    }
}

int cola_dequeue() {
    if (cola_n > 0) {
        int x = cola_datos[cola_frente];
        cola_frente = (cola_frente + 1) % MAX_COLA;
        cola_n--;
        return x;
    }
    return -1;
}

void ejemploCola() {
    printf("\n--- COLA (FIFO) ---\n");
    printf("Enqueue: 100, 200, 300 → Dequeue: 100, 200, 300\n");

    cola_reiniciar();
    cola_enqueue(100);
    cola_enqueue(200);
    cola_enqueue(300);

    printf("Dequeue: ");
    while (cola_n > 0) {
        printf("%d ", cola_dequeue());
    }
    printf("\n");
}

// =============== MENU ===============
void mostrar_menu() {
    printf("\n=== EJEMPLOS DE ESTRUCTURAS DE DATOS ===\n");
    printf("1. Lista Enlazada\n");
    printf("2. Arbol Binario\n");
    printf("3. Grafo\n");
    printf("4. Tabla Hash\n");
    printf("5. Pila\n");
    printf("6. Cola\n");
    printf("0. Salir\n");
    printf("Opcion: ");
}

int main() {
    int opc;
    do {
        mostrar_menu();
        scanf("%d", &opc);

        switch (opc) {
            case 1: ejemploLista(); break;
            case 2: ejemploArbol(); break;
            case 3: ejemploGrafo(); break;
            case 4: ejemploTablaHash(); break;
            case 5: ejemploPila(); break;
            case 6: ejemploCola(); break;
            case 0: printf("Adios.\n"); break;
            default: printf("Opcion no valida.\n");
        }
    } while (opc != 0);

    return 0;
}