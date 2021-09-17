#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "map.h"
#include "list.h"


typedef struct{
    const char * nombre;
    char * marca;
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



Map * cargar(FILE * file, Map * mapa){
    char lineaArchivo[1024];
    int i;
    int cont = 0;

    while (fgets (lineaArchivo, 1024, file) != NULL) {
        Producto * productos = (Producto*) malloc (sizeof(Producto));
        
        for(i = 0; i <= 4; i++){
            const char *aux = get_csv_field(lineaArchivo, i);

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
                productos->stock = (char *)aux;
            }

            if(i==4){
                productos->precio = (char *)aux;
            }   
        }
        
        // insert mapas
        insertMap(mapa, productos->nombre, productos);

        cont++; 
        if(cont == 100) break;
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

//void agregar_Produto(char[] nombre, char[] marca, char [] tipo, int stock int precio){
    /*
    int cont = 0; //contador 
    
    for  (i= 0  ; i< stock ; i++){ // recorrer 
            

     cont++:   
    }

    */
//}


Map * importar(){
    char archivo[101];
    FILE *file;

    do{
      printf("Ingresar nombre archivo: ");
      scanf("%s", &archivo);
      strcat(archivo, ".csv"); 
      file  = fopen(archivo, "r");
    }while(!file);
    Map* mapa = createMap(is_equal_string);
    cargar(file,mapa);
    fclose(file);
    return mapa;
}

int main(){
    Map * map;
    map = importar();

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
            case 1:buscarNombre(map);break;
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
