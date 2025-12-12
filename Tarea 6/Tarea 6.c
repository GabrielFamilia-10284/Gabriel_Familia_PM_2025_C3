#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOMBRE 50

// atributos producto
typedef struct {
    char nombre[MAX_NOMBRE];
    float precio;
    int cantidad;
} Producto;

// funciones
Producto* cargarInventario(const char* nombreArchivo, int* numProductos);
void mostrarInventario(Producto* inventario, int numProductos);
void agregarProducto(Producto** inventario, int* numProductos);
void guardarInventario(const char* nombreArchivo, Producto* inventario, int numProductos);
void liberarInventario(Producto* inventario);

int main() {
    int opcion;
    int numProductos = 0;
    Producto* inventario = NULL;

    // Cargar inventario desde archivo al inicio
    printf("Cargando inventario desde 'inventario.txt'...\n");
    inventario = cargarInventario("inventario.txt", &numProductos);

    if (inventario == NULL) {
        printf("No se pudo cargar el inventario. Se iniciara vacio.\n");
        numProductos = 0;
    } else {
        printf("Inventario cargado con %d productos.\n", numProductos);
    }

    do {
        printf("\n--- MENU ---\n");
        printf("1. Leer inventario (ya cargado al inicio)\n");
        printf("2. Agregar nuevo producto\n");
        printf("3. Imprimir inventario\n");
        printf("0. Salir y guardar\n");
        printf("Elige una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("\n El inventario ya esta cargado (%d productos).\n", numProductos);
                break;
            case 2:
                agregarProducto(&inventario, &numProductos);
                break;
            case 3:
                if (numProductos == 0) {
                    printf("\n El inventario esta vacio.\n");
                } else {
                    mostrarInventario(inventario, numProductos);
                }
                break;
            case 0:
                printf("\nGuardando inventario y saliendo...\n");
                guardarInventario("inventario.txt", inventario, numProductos);
                break;
            default:
                printf("\n Opcion no valida.\n");
        }
    } while (opcion != 0);

    liberarInventario(inventario);
    return 0;
}

// Función para cargar inventario desde archivo usando memoria 
Producto* cargarInventario(const char* nombreArchivo, int* numProductos) {
    FILE* archivo = fopen(nombreArchivo, "r");
    if (!archivo) {
        return NULL;  // archivo no existe
    }

    // Contar cuántas líneas tiene el archivo (productos)
    int count = 0;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), archivo) != NULL) {
        if (strlen(buffer) > 1) count++;  // ignorar líneas vacías
    }
    rewind(archivo);  // volver al inicio

    Producto* inventario = malloc(count * sizeof(Producto));
    if (!inventario) {
        fclose(archivo);
        return NULL;
    }

    // Leer productos
    int i = 0;
    while (i < count && fscanf(archivo, "%s %f %d", 
                               inventario[i].nombre,
                               &inventario[i].precio,
                               &inventario[i].cantidad) == 3) {
        i++;
    }
    fclose(archivo);

    *numProductos = count;
    return inventario;
}

// Mostrar todos los productos
void mostrarInventario(Producto* inventario, int numProductos) {
    printf("\n--- INVENTARIO ---\n");
    printf("%-15s %-10s %-10s\n", "Producto", "Precio", "Cantidad");
    printf("-----------------------------------\n");
    for (int i = 0; i < numProductos; i++) {
        printf("%-15s %-10.2f %-10d\n", 
               inventario[i].nombre,
               inventario[i].precio,
               inventario[i].cantidad);
    }
}

// Agregar un nuevo producto 
void agregarProducto(Producto** inventario, int* numProductos) {
    char nombre[MAX_NOMBRE];
    float precio;
    int cantidad;

    printf("\nIngrese nombre del producto: ");
    scanf("%s", nombre);
    printf("Ingrese precio: ");
    scanf("%f", &precio);
    printf("Ingrese cantidad: ");
    scanf("%d", &cantidad);

    // Redimensionar memoria
    Producto* temp = realloc(*inventario, (*numProductos + 1) * sizeof(Producto));
    if (!temp) {
        printf("Error: No se pudo asignar memoria.\n");
        return;
    }
    *inventario = temp;

    // Asignar nuevo producto
    strcpy((*inventario)[*numProductos].nombre, nombre);
    (*inventario)[*numProductos].precio = precio;
    (*inventario)[*numProductos].cantidad = cantidad;

    (*numProductos)++;
    printf(" Producto '%s' agregado.\n", nombre);
}

// Guardar inventario de vuelta al archivo
void guardarInventario(const char* nombreArchivo, Producto* inventario, int numProductos) {
    FILE* archivo = fopen(nombreArchivo, "w");
    if (!archivo) {
        printf(" Error al guardar el archivo.\n");
        return;
    }

    for (int i = 0; i < numProductos; i++) {
        fprintf(archivo, "%s %.2f %d\n",
                inventario[i].nombre,
                inventario[i].precio,
                inventario[i].cantidad);
    }
    fclose(archivo);
    printf(" Inventario guardado en '%s'.\n", nombreArchivo);
}

// Liberar memoria
void liberarInventario(Producto* inventario) {
    free(inventario);
}