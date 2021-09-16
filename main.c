#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "map.h"
#include "list.h"


typedef struct{
    const char * nombre;
    const char * marca;
    const char * sector;
    int stock;
    int precio;
}Producto;

typedef struct{
    const char * nombreCarro;
    int cantidadProductos;
}carroCompras;

/*
typedef struct{
    char* marca;
    char* sector[20];
    int stock;
    int precio;
}infoProducto;

typedef struct{
    char* nombre;
    infoProducto * info;
}Producto;
*/




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



Map * cargar(FILE * file){
    char lineaArchivo[1024];
    int i;
    int cont = 0;

    // crear mapas
    
    Map* mapaNombre = createMap(is_equal_int);
    Map* mapaMarca = createMap(is_equal_int);
    Map* mapaSector = createMap(is_equal_int);

    while (fgets (lineaArchivo, 1024, file) != NULL) {
        Producto * productos = (Producto*) malloc (sizeof(Producto));
        
        for(i = 0; i <= 4; i++){
            const char *aux = get_csv_field(lineaArchivo, i);

            if(i==0){
                productos->nombre = (char *)aux;
                
            }
            
            if(i==1){
                productos->marca = (char *)aux;
                printf("%s", productos->marca);
            }
            
            if(i==2){
                productos->sector = (char *)aux;
            }
            
            if(i==3){
                productos->stock = (char *)aux;
            }

            if(i==4){
                productos->precio = (char *)aux;
            }   
        }
        
        // insert mapas

        insertMap(mapaNombre, productos->nombre, productos);
        insertMap(mapaMarca, productos->marca, productos);
        insertMap(mapaSector, productos->sector, productos);

    
        cont++; 
        if(cont == 100) break;
    } 
    muestrePapito(mapaMarca);
}

void muestrePapito(Map* mapaSector){

    /*
    printf("Libros ordeandos por nombre:\n");
    b = firstMap(books_by_title);
    while (b) {
        printf("%s (%d)\n", b->title, b->year);
        b = nextMap(books_by_title);
    }
    */
    printf("Recorriendo el mapa: \n");
    Producto * wea = firstMap(mapaSector);
    printf("%s \n", wea->nombre);
    printf("%s \n", wea->marca);
    printf("%s \n", wea->sector);
    printf("%d \n", wea->stock);
    /*
    while(mapaSector){

        printf("%s \n", wea->sector);
        nextMap(mapaSector);
    }
    */

    
    
}
//void agregar_Produto(char[] nombre, char[] marca, char [] tipo, int stock int precio){
    /*
    int cont = 0; //contador 
    
    for  (i= 0  ; i< stock ; i++){ // recorrer 
            

     cont++:   
    }

    */
//}
int is_equal_int(void * key1, void * key2) {
    if(*(int*)key1 == *(int*)key2) return 1;
    return 0;
}

Map * importar(){
    char archivo[101];
    FILE *file;

    do{
      printf("Ingresar nombre archivo: ");
      scanf("%s", &archivo);
      strcat(archivo, ".csv"); 
      file  = fopen(archivo, "r");
    }while(!file);
    cargar(file);
    fclose(file);
}

int main(){

    importar();

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
        printf("0. Salir del acuenta.\n");
        scanf("%d",&opcion);

        switch(opcion){
            case 1:printf("NO HECHA.\n");break;
            case 2:printf("NO HECHA.\n");break;
            case 3:printf("NO HECHA.\n");break;
            case 4:printf("NO HECHA.\n");break;
            case 5:printf("NO HECHA.\n");break;
            case 6:printf("NO HECHA.\n");break;
            case 7:printf("NO HECHA.\n");break;
        }

    }
    return 0;
}
