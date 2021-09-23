#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "map.h"
#include "list.h"

typedef struct{
    char * nombre;
    const char * marca;
    const char * sector;
    int stock;
    int precio;
}Producto;

typedef struct{
    const char * nombreCarro;
    int cantidadProductos;
}carroCompras;

int is_equal_int(void * key1, void * key2) {
    if(*(int*)key1 == *(int*)key2) return 1;
    return 0;
}

int isEqualString(void * key1, void * key2) {
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

    //printf("MAPA GENERAL CARGADO\n");
    
}

Map * cargarPorMarca(FILE * file, Map * mapa){
    char lineaArchivo[1024];
    int i;
    int cont = 0;

    while (fgets (lineaArchivo, 1024, file) != NULL) {
        Producto * producto = (Producto*)malloc(sizeof(Producto));
        
        for(i = 0; i <= 4; i++){
            const char * aux = get_csv_field(lineaArchivo, i);
            if(i == 1){
                
                const char * M;
                char * marcaEntrante = (char*) aux;
                M = searchMap(mapa,marcaEntrante);
                
                if(M == NULL){
                    List * listaMarcas = createList();
                    producto->nombre = get_csv_field(lineaArchivo, 0);
                    //printf("%s \n", marcaEntrante);
                    pushFront(listaMarcas,producto);
                    insertMap(mapa,marcaEntrante,listaMarcas);
                }
                else{
                    //printf("SE REPITE MARCA \n");
                    List * L = (List*)searchMap(mapa,marcaEntrante);
                    producto->nombre = get_csv_field(lineaArchivo, 0);
                    pushFront(L,producto);
                }
            }
        }
        
        cont++; 
        if(cont == 100) break;
    } 

    //printf("MAPA MARCAS CARGADO\n");
    
}

Map * cargarPorTipo(FILE * file, Map * map){
    char lineaArchivo[1024];
    int i;
    int cont = 0;

    while (fgets (lineaArchivo, 1024, file) != NULL) {
        Producto * producto = (Producto*)malloc(sizeof(Producto));
        
        for(i = 0; i <= 4; i++){
            const char * aux = get_csv_field(lineaArchivo, i);
            if(i == 2){
                
                const char * M;
                char * sectorEntrante = (char*) aux;
                M = searchMap(map,sectorEntrante);
                
                if(M == NULL){
                    List * listaSector = createList();
                    producto->nombre = get_csv_field(lineaArchivo, 0);
                    //printf("%s \n", sectorEntrante);
                    pushFront(listaSector,producto);
                    insertMap(map,sectorEntrante,listaSector);
                }
                else{
                    //printf("SE REPITE TIPO \n");
                    List * L = (List*)searchMap(map,sectorEntrante);
                    producto->nombre = get_csv_field(lineaArchivo, 0);
                    pushFront(L,producto);
                }
            }
        }
        cont++; 
        if(cont == 100) break;
    } 

    //printf("MAPA SECTOR CARGADO\n");
    
}

void annadirEnMapas(Map * mapa, char * nombreProducto, char * nombreMoS, Producto * nuevoP){
    const char * M;
    char * MoSEntrante = nombreMoS;
    M = searchMap(mapa,MoSEntrante);
                
    if(M == NULL){
        List * lista = createList();
        nuevoP->nombre = nombreProducto;
        //printf("%s \n", marcaEntrante);
        pushFront(lista,nuevoP);
        insertMap(mapa,MoSEntrante,lista);
    }
    else{
        //printf("SE REPITE MARCA \n");
        List * L = (List*)searchMap(mapa,MoSEntrante);
        nuevoP->nombre = nombreProducto;
        pushFront(L,nuevoP);
    }

}

void buscarPorTipo(Map * mapTipo, Map * mapaGeneral){

    char * nombre = (char*) malloc(70*sizeof(char));
    printf("Ingrese el tipo de producto deseado : ");
    scanf(" %[^\n]s]", nombre);
    List * L;
    L = searchMap(mapTipo,nombre);

    if(L){
        Producto * P = firstList(L);
        while(P != NULL){
            
            printf("%s ,", P->nombre);
            Producto * datos = (Producto*)searchMap(mapaGeneral,P->nombre);
            printf("%s ,", datos->marca);
            printf("%s ,", datos->sector);
            printf("%d ,", datos->stock);
            printf("%d \n", datos->precio);
            P = nextList(L);
            if(!L)break;
        }
    }
    else{
        printf("No existe este producto.\n");
    }
    
    
}

void buscarPorMarca(Map * mapMarcas, Map * mapaGeneral){

    char * nombre = (char*) malloc(70*sizeof(char));
    printf("Ingrese la marca del producto: ");
    scanf(" %[^\n]s]", nombre);
    List * L;
    L = searchMap(mapMarcas,nombre);
    
    if(L){
        Producto * P = firstList(L);
        while(P != NULL){
            
            printf("%s ,", P->nombre);
            Producto * datos = (Producto*)searchMap(mapaGeneral,P->nombre);
            printf("%s ,", datos->marca);
            printf("%s ,", datos->sector);
            printf("%d ,", datos->stock);
            printf("%d \n", datos->precio);
            P = nextList(L);
            if(!L)break;
        }
    }
    else{
        printf("No existe este producto.\n");
    }
    
    
}

void buscarNombre(Map * mapaGeneral){

    char * nombre = (char*) malloc(70*sizeof(char));
    printf("Ingrese el nombre del producto : ");
    scanf(" %[^\n]s]", nombre);
    Producto * P;
    P = searchMap(mapaGeneral,nombre);
    if(P){
        printf("%s ,", P->nombre);
        printf("%s ,", P->marca);
        printf("%s ,", P->sector);
        printf("%d ,", P->stock);
        printf("%d \n", P->precio);

    }else{
        printf("No existe este producto.\n");
    }
    
}

void agregarProducto(Map * mapaGeneral, Map * mapaMarcas, Map * mapaTipos){

    Producto * P = (Producto*)malloc(sizeof(Producto));
    // entrada de datos;
    char * nombre = (char*) malloc(30*sizeof(char));
    printf("Ingrese el nombre del producto : ");
    scanf(" %[^\n]s]", nombre);
    
    P = searchMap(mapaGeneral,nombre);
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

        insertMap(mapaGeneral, (void *)nuevoP->nombre, nuevoP);
        annadirEnMapas(mapaMarcas, nombre, marca, nuevoP);
        annadirEnMapas(mapaTipos, nombre, sector, nuevoP);

        printf("Producto agregado. \n");
        
    }
}

void mostrarTodo(Map* mapaGeneral){

    Producto *P = firstMap(mapaGeneral);

    while(P){
        printf("%s ,", P->nombre);
        printf("%s ,", P->marca);
        printf("%s ,", P->sector);
        printf("%d ,", P->stock);
        printf("%d \n", P->precio);
        P = nextMap(mapaGeneral);
    }
}

Map * importar(Map * mapaGeneral, Map * mapaPorMarca, Map * mapaPorSector){
    char archivo[101];
    char archivo2[101];
    char archivo3[101];
    FILE * file;
    FILE * file2;
    FILE * file3;

    do{

        printf("Ingresar nombre archivo: ");
        scanf("%s", &archivo);
        strcat(archivo, ".csv"); 
        file = fopen(archivo, "r");
        file2 = fopen(archivo, "r");
        file3 = fopen(archivo, "r");


    }while(!file);
    
    cargarPorNombre(file,mapaGeneral);
    cargarPorMarca(file2,mapaPorMarca);
    cargarPorTipo(file3,mapaPorSector);

    fclose(file);
    fclose(file2);
    fclose(file3);
}

int main(){
    Map * mapaGeneral=createMap(isEqualString);
    Map * mapaPorMarca=createMap(isEqualString);
    Map * mapaPorSector=createMap(isEqualString);

    importar(mapaGeneral,mapaPorMarca,mapaPorSector);

    int opcion=1;
    printf("============== SUPERMERCADO =============\n");

    while(opcion!=0){
        printf("1. Agregar producto.\n");
        printf("2. Buscar producto/s por tipo.\n");
        printf("3. Buscar producto/s por marca.\n");
        printf("4. Buscar producto/s por nombre.\n");
        printf("5. Mostrar todos los productos.\n");
        printf("6. Agregar producto a carrito.\n");
        printf("7. Concretar compra.\n");
        printf("0. Salir.\n");
        scanf("%d",&opcion);

        switch(opcion){
            case 1:agregarProducto(mapaGeneral,mapaPorMarca,mapaPorSector);break;
            case 2:buscarPorTipo(mapaPorSector,mapaGeneral);break;
            case 3:buscarPorMarca(mapaPorMarca,mapaGeneral);break;
            case 4:buscarNombre(mapaGeneral);break;
            case 5:mostrarTodo(mapaGeneral);break;
            case 6:printf("NO HECHA.\n");break;
            case 7:printf("NO HECHA.\n");break;
        }
    }
    return 0;
}
