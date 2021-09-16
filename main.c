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
    char sector[20];
    int stock;
    int precio;
}Producto;

typedef struct{
    const char * nombreCarro;
    int cantidadProductos;
}carroCompras;

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

int is_equal_int(void * key1, void * key2) {
    if(*(int*)key1 == *(int*)key2) return 1;
    return 0;
}

int main(){

    Map * mapa = createMap(is_equal_int);
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