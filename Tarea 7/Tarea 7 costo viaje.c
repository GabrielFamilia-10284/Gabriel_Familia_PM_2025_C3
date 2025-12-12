#include <stdio.h>
#include <string.h>

// Maximo de vehiculos y combustibles que se pueden guardar
#define MAX_VEHICULOS 10
#define MAX_COMBUSTIBLES 5

// Estructura para guardar datos de un vehiculo
typedef struct {
    char marca[30];
    char modelo[30];
    int ano;
    int id;
    char placa[10];
    float cons_ciudad;     // consumo en ciudad (litros por 100 km)
    float cons_carretera;  // consumo en carretera
    float costo_seguro_anual;
    float costo_cambio_km; // costo de mantenimiento por km
    int anos_depreciacion;
    float costo_compra;
    float costo_depreciacion; // se calcula despues
    int km_anual_prom;
    float costo_limpieza;
} Vehiculo;

// Estructura para guardar tipo de combustible
typedef struct {
    int id;
    char nombre[20];
    float precio_litro;
} Combustible;

// Listas para guardar los vehiculos y combustibles
Vehiculo vehiculos[MAX_VEHICULOS];
int n_vehiculos = 0;

Combustible combustibles[MAX_COMBUSTIBLES];
int n_combustibles = 0;

// Prototipos de funciones (declaracion anticipada)
void menu();
void agregarVehiculo();
void listarVehiculos();
void modificarVehiculo();
void agregarCombustible();
void modificarCombustible();
void listarCombustibles();
void calcularViaje();
float calcularCostoDepreciacion(Vehiculo v);
void pausar();

// Pausa el programa para que el usuario lea
void pausar() {
    printf("\nPresione Enter para continuar...");
    while (getchar() != '\n'); // limpia lo que sobra en el teclado
    getchar();
}

// Muestra el menu principal y ejecuta lo que elija el usuario
void menu() {
    int opcion;
    do {
        printf("\n=== SISTEMA COSTO DE VIAJE ===\n");
        printf("1. Agregar Vehiculo\n");
        printf("2. Lista Vehiculo\n");
        printf("3. Modificar Vehiculo\n");
        printf("4. Agregar Combustible\n");
        printf("5. Modificar Combustible\n");
        printf("6. Lista Combustible\n");
        printf("7. Calcular Viaje\n");
        printf("0. Salir\n");
        printf("Opcion: ");
        scanf("%d", &opcion);

        // Ejecuta la funcion segun la opcion
        switch (opcion) {
            case 1: agregarVehiculo(); break;
            case 2: listarVehiculos(); break;
            case 3: modificarVehiculo(); break;
            case 4: agregarCombustible(); break;
            case 5: modificarCombustible(); break;
            case 6: listarCombustibles(); break;
            case 7: calcularViaje(); break;
            case 0: printf("Adios!\n"); break;
            default: printf("Opcion no valida.\n"); pausar();
        }
    } while (opcion != 0);
}

// Agrega un vehiculo nuevo a la lista
void agregarVehiculo() {
    if (n_vehiculos >= MAX_VEHICULOS) {
        printf("Ya no se pueden agregar mas vehiculos.\n");
        pausar();
        return;
    }

    Vehiculo v;

    printf("\n--- Agregar Vehiculo ---\n");
    printf("Marca: "); scanf("%s", v.marca);
    printf("Modelo: "); scanf("%s", v.modelo);
    printf("Ano: "); scanf("%d", &v.ano);
    printf("ID: "); scanf("%d", &v.id);
    printf("Placa: "); scanf("%s", v.placa);
    printf("Consumo ciudad (L/100km): "); scanf("%f", &v.cons_ciudad);
    printf("Consumo carretera (L/100km): "); scanf("%f", &v.cons_carretera);
    printf("Costo seguro anual: "); scanf("%f", &v.costo_seguro_anual);
    printf("Costo cambio por km: "); scanf("%f", &v.costo_cambio_km);
    printf("Anos de depreciacion: "); scanf("%d", &v.anos_depreciacion);
    printf("Costo de compra: "); scanf("%f", &v.costo_compra);
    printf("Km promedio al ano: "); scanf("%d", &v.km_anual_prom);
    printf("Costo limpieza por viaje: "); scanf("%f", &v.costo_limpieza);

    // Calcula cuanto se pierde por depreciacion en cada km
    v.costo_depreciacion = v.costo_compra / (v.anos_depreciacion * v.km_anual_prom);

    vehiculos[n_vehiculos] = v;
    n_vehiculos++;
    printf("Vehiculo guardado.\n");
    pausar();
}

// Muestra todos los vehiculos guardados
void listarVehiculos() {
    if (n_vehiculos == 0) {
        printf("\nNo hay vehiculos.\n");
        pausar();
        return;
    }
    printf("\n--- Vehiculos ---\n");
    printf("ID  Marca      Modelo     Ano  Placa    Ciudad  Carr\n");
    printf("-----------------------------------------------------\n");
    for (int i = 0; i < n_vehiculos; i++) {
        printf("%-3d %-10s %-10s %-4d %-8s %-6.1f %-6.1f\n",
            vehiculos[i].id,
            vehiculos[i].marca,
            vehiculos[i].modelo,
            vehiculos[i].ano,
            vehiculos[i].placa,
            vehiculos[i].cons_ciudad,
            vehiculos[i].cons_carretera);
    }
    pausar();
}

// Cambia algunos datos de un vehiculo (por ID)
void modificarVehiculo() {
    int id, pos = -1;
    printf("\nID del vehiculo a cambiar: ");
    scanf("%d", &id);

    // Busca el vehiculo con ese ID
    for (int i = 0; i < n_vehiculos; i++) {
        if (vehiculos[i].id == id) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("No se encontro ese vehiculo.\n");
        pausar();
        return;
    }

    // Pide nuevos datos 
    printf("Nuevo consumo ciudad (actual %.1f): ", vehiculos[pos].cons_ciudad);
    scanf("%f", &vehiculos[pos].cons_ciudad);
    printf("Nuevo consumo carretera (actual %.1f): ", vehiculos[pos].cons_carretera);
    scanf("%f", &vehiculos[pos].cons_carretera);
    printf("Nuevo costo seguro anual (actual %.2f): ", vehiculos[pos].costo_seguro_anual);
    scanf("%f", &vehiculos[pos].costo_seguro_anual);

    // Actualiza la depreciacion
    vehiculos[pos].costo_depreciacion = vehiculos[pos].costo_compra / 
        (vehiculos[pos].anos_depreciacion * vehiculos[pos].km_anual_prom);

    printf("Datos actualizados.\n");
    pausar();
}

// Agrega un tipo de combustible
void agregarCombustible() {
    if (n_combustibles >= MAX_COMBUSTIBLES) {
        printf("Ya no se pueden agregar mas combustibles.\n");
        pausar();
        return;
    }

    Combustible c;
    printf("\n--- Agregar Combustible ---\n");
    printf("ID: "); scanf("%d", &c.id);
    printf("Nombre (ej: Gasolina): "); scanf("%s", c.nombre);
    printf("Precio por litro: "); scanf("%f", &c.precio_litro);

    combustibles[n_combustibles] = c;
    n_combustibles++;
    printf("Combustible guardado.\n");
    pausar();
}

// Cambia el precio de un combustible
void modificarCombustible() {
    int id, pos = -1;
    printf("\nID del combustible a cambiar: ");
    scanf("%d", &id);

    for (int i = 0; i < n_combustibles; i++) {
        if (combustibles[i].id == id) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("No se encontro ese combustible.\n");
        pausar();
        return;
    }

    printf("Nuevo precio (actual %.2f): ", combustibles[pos].precio_litro);
    scanf("%f", &combustibles[pos].precio_litro);
    printf("Precio actualizado.\n");
    pausar();
}

// Muestra todos los combustibles
void listarCombustibles() {
    if (n_combustibles == 0) {
        printf("\nNo hay combustibles.\n");
        pausar();
        return;
    }
    printf("\n--- Combustibles ---\n");
    printf("ID  Nombre       Precio/L\n");
    printf("-------------------------\n");
    for (int i = 0; i < n_combustibles; i++) {
        printf("%-3d %-12s $%.2f\n",
            combustibles[i].id,
            combustibles[i].nombre,
            combustibles[i].precio_litro);
    }
    pausar();
}

// Calcula el costo de un viaje
void calcularViaje() {
    if (n_vehiculos == 0 || n_combustibles == 0) {
        printf("\nPrimero agregue al menos 1 vehiculo y 1 combustible.\n");
        pausar();
        return;
    }

    int id_veh, id_comb;
    float km, porc_ciudad;
    Vehiculo *v = NULL;
    Combustible *c = NULL;

    // Elige vehiculo
    listarVehiculos();
    printf("Elija ID de vehiculo: ");
    scanf("%d", &id_veh);
    for (int i = 0; i < n_vehiculos; i++) {
        if (vehiculos[i].id == id_veh) {
            v = &vehiculos[i];
            break;
        }
    }
    if (!v) { printf("Vehiculo no valido.\n"); pausar(); return; }

    // Elige combustible
    listarCombustibles();
    printf("Elija ID de combustible: ");
    scanf("%d", &id_comb);
    for (int i = 0; i < n_combustibles; i++) {
        if (combustibles[i].id == id_comb) {
            c = &combustibles[i];
            break;
        }
    }
    if (!c) { printf("Combustible no valido.\n"); pausar(); return; }

    // Datos del viaje
    printf("Cantidad de km: ");
    scanf("%f", &km);
    printf("Porcentaje de km en ciudad (ej: 60): ");
    scanf("%f", &porc_ciudad);

    // Calcula km en ciudad y carretera
    float km_ciudad = km * porc_ciudad / 100.0;
    float km_carretera = km - km_ciudad;

    // Calcula litros usados
    float litros_ciudad = (km_ciudad / 100.0) * v->cons_ciudad;
    float litros_carretera = (km_carretera / 100.0) * v->cons_carretera;
    float litros_totales = litros_ciudad + litros_carretera;

    // Calcula costos
    float costo_comb = litros_totales * c->precio_litro;
    float costo_dep = km * v->costo_depreciacion;
    float costo_seg = (km / v->km_anual_prom) * v->costo_seguro_anual;
    float costo_man = km * v->costo_cambio_km;
    float costo_lim = v->costo_limpieza;

    float total = costo_comb + costo_dep + costo_seg + costo_man + costo_lim;
    float por_km = (km > 0) ? total / km : 0;

    // Muestra resultados
    printf("\n=== RESULTADO ===\n");
    printf("Vehiculo: %s %s (%s)\n", v->marca, v->modelo, v->placa);
    printf("Distancia: %.1f km\n", km);
    printf("Costo total: $%.2f\n", total);
    printf("Costo por km: $%.3f\n", por_km);

    pausar();
}


int main() {
    menu();
    return 0;
}