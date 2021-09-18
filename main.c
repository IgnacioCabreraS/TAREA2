#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "map.h"
#include "list.h"

//Mapa de marca, key = marca del producto, value = Lista con nombres de productos con esa marca.

typedef struct{
    const char * nombre;
    const char * marca;
    const char * sector;
    int stock;
    int precio;
}Producto;

typedef struct{
    const char * nombreMarca;
    List * listaMarcas;
}Marcas;

typedef struct{
    const char * nombreCarro;
    int cantidadProductos;
}carroCompras;

/*/////
typedef struct{
    char* marca;
    char* sector[20];
    int stock;66
    int precio;
}infoProducto;

typedef struct{
    char* nombre;
    infoProducto * info;
}Producto;
*//////

int is_equal_int(void * key1, void * key2) {
    if(*(int*)key1 == *(int*)key2) return 1;
    return 0;
}

int is_equal_string(void * key1, void * key2) {
    if(strcmp((char*)key1, (char*)key2)==0) return 1;
    return 0;
}

char*get_csv_field (char * tmp, int k){
    int open_mark = 0;
    char* ret=(char*) malloc(100*sizeof(char));
    int ini_i=0, i=0;
    int j=0;
    while(tmp[i+1]!='\0'){
        if(tmp[i]== '\"'){
            open_mark = 1-open_mark;
            if(open_mark) ini_i = i+1;
            i++;
            continue;
        }
        if(open_mark || tmp[i]!= ','){
            if(k==j) ret[i-ini_i] = tmp[i];
            i++;
            continue;
        }
        if(tmp[i]== ','){
            if(k==j) {
               ret[i-ini_i] = 0;
               return ret;
            }
            j++; ini_i = i+1;
        }
        i++;
    }
    if(k==j) {
       ret[i-ini_i] = 0;
       return ret;
    }
    return NULL;
}



Map * cargarPorNombre(FILE * file, Map * mapa){
    char lineaArchivo[1024];
    int i;
    int cont = 0;

    while (fgets (lineaArchivo, 1024, file) != NULL) {
        Producto * productos = (Producto*) malloc (sizeof(Producto));
        
        for(i = 0; i <= 4; i++){
            const char * aux = get_csv_field(lineaArchivo, i);

            if(i==0){
                productos->nombre = (char *)aux;
            }
            
            if(i==1){
                productos->marca = (char *)aux;
            }
            
            if(i==2){
                productos->sector = (char *)aux;
            }
            
            if(i==3){
                productos->stock = atoi(aux);
            }

            if(i==4){
                productos->precio = atoi(aux);
            }   
        }
        
        // insert mapas
        insertMap(mapa, (void *) productos->nombre, productos);

        cont++; 
        if(cont == 100) break;
    } 
    
}

Map * cargarPorMarca(FILE * file, Map * mapa){
    char lineaArchivo[1024];
    int i;
    int cont = 0;

    while (fgets (lineaArchivo, 1024, file) != NULL) {
        const char * marcaNueva;
        
        for(i = 0; i <= 4; i++){
            const char * aux = get_csv_field(lineaArchivo, i);
            if(i == 1){
                const char * M;
                marcaNueva = (char*) aux;
                
                M = searchMap(mapa,marcaNueva);
                if(M == NULL){
                    List * lista = createList();
                    const char * nombre = get_csv_field(lineaArchivo, 0);
                    printf("%s \n", marcaNueva);
                    pushFront(lista,nombre);
                    insertMap(mapa,marcaNueva,lista);
                    
                }
                else{
                    const char * nombre = get_csv_field(lineaArchivo, 0);
                    
                }
            }
        }
        

        cont++; 
        if(cont == 100) break;
    } 
    //mostrarVolas(mapa);
}

/*
void mostrarVolas(Map * map){
    const char * marcaNueva;
    marcaNueva = firstMap(map);
    while(marcaNueva != NULL){
        printf("%s \n", marcaNueva);
        marcaNueva = nextMap(map);
    }

}*/

void agregar(Map * map){

    Producto * P = (Producto*)malloc(sizeof(Producto));
    // entrada de datos;
    char * nombre = (char*) malloc(30*sizeof(char));
    printf("Ingrese el nombre del producto : ");
    scanf(" %[^\n]s]", nombre);
    
    P = searchMap(map,nombre);
    if(P){
        printf("Este producto ya existe, aumentando stock.\n"); 
        (P->stock)++;

    }
    else{
        
        Producto *nuevoP = (Producto *) malloc (sizeof (Producto));

        printf("Este producto no existe, agregue datos.\n"); 

        char * marca = (char*) malloc(30*sizeof(char));
        printf("Marca: ");
        scanf(" %[^\n]s]", marca);
        

        char * sector = (char*) malloc(30*sizeof(char));
        printf("Sector: ");
        scanf(" %[^\n]s]", sector);
        
        
        int precio;
        printf("Precio: ");
        scanf("%d", &precio);
        
        nuevoP->nombre = nombre;
        nuevoP->marca = marca;
        nuevoP->sector = sector;
        nuevoP->precio = precio;
        nuevoP->stock = 1;

        insertMap (map, (void *) nuevoP -> nombre, nuevoP);

        printf("Producto agregado. \n");
        
    }

}

void buscarNombre(Map * map){

    char * nombre = (char*) malloc(70*sizeof(char));
    printf("Ingrese el nombre del producto : ");
    scanf(" %[^\n]s]", nombre);
    Producto * P;
    P = searchMap(map,nombre);
    if(P){
        printf("%s \n", P->nombre);
        
    }else{
        printf("No existe este producto.\n");
    }
    
}

void mostrarTodo(Map* map){

    Producto *P = firstMap(map);

    while(P){
        printf("%s ,", P->nombre);
        printf("%s ,", P->marca);
        printf("%s ,", P->sector);
        printf("%d ,", P->stock);
        printf("%d \n", P->precio);
        P = nextMap(map);
    }
}

Map * importarPorNombre(){
    char archivo[101];
    FILE *file;

    do{
      printf("Ingresar nombre archivo: ");
      scanf("%s", &archivo);
      strcat(archivo, ".csv"); 
      file  = fopen(archivo, "r");
    }while(!file);
    Map* mapa = createMap(is_equal_string);
    cargarPorNombre(file,mapa);
    fclose(file);
    return mapa;
}

Map * importarPorMarca(){
    char archivo[101];
    FILE *file;

    do{
      printf("Ingresar nombre archivo: ");
      scanf("%s", &archivo);
      strcat(archivo, ".csv"); 
      file  = fopen(archivo, "r");
    }while(!file);
    Map* mapa = createMap(is_equal_string);
    cargarPorMarca(file,mapa);
    fclose(file);
    return mapa;
}

int main(){
    Map * map;
    Map * map2;

    map = importarPorNombre();
    map2 = importarPorMarca();

    int opcion=1;
    printf("============== SUPERMERCADO MAYORISTA 11 =============\n");

    while(opcion!=0){
        printf("1. Agregar producto.\n");
        printf("2. Buscar producto/s por tipo.\n");
        printf("3. Buscar producto/s por marca.\n");
        printf("4. Buscar producto/s por nombre.\n");
        printf("5. Mostrar todos los productos.\n");
        printf("6. Agregar producto a carrito.\n");
        printf("7. Concretar compra.\n");
        printf("0. Salir del acuenta.\n");
        scanf("%d",&opcion);

        switch(opcion){
            case 1:agregar(map);break;
            case 2:printf("NO HECHA.\n");break;
            case 3:printf("NO HECHA.\n");break;
            case 4:buscarNombre(map);break;
            case 5:mostrarTodo(map);break;
            case 6:printf("NO HECHA.\n");break;
            case 7:printf("NO HECHA.\n");break;
        }
    }
    return 0;
}
