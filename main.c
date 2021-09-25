#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "map.h"
#include "list.h"

typedef struct{
    char * nombre;
    char * marca;
    char * sector;
    int stock;
    int precio;
}Producto;

typedef struct{
    char * nombreCarro;
    List * productosCarritos;
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
            if(L == NULL)break;
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
            if(L == NULL)break;
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
    if(P != NULL){
        printf("%s ,", P->nombre);
        printf("%s ,", P->marca);
        printf("%s ,", P->sector);
        printf("%d ,", P->stock);
        printf("%d \n", P->precio);

    }else{
        printf("No existe este producto.\n");
    }
    
}
/* esta función lo que hace es agrega el producto el cual se ingresa el nombre del producto, si aquel producto existe se aumenta el 
stock, en caso contrario se agrega el producto con sus respectivos datos.
*/
void agregarProducto(Map * mapaGeneral, Map * mapaMarcas, Map * mapaTipos){

    Producto * P = (Producto*)malloc(sizeof(Producto));
    // entrada de datos;
    char * nombre = (char*) malloc(30*sizeof(char));
    printf("Ingrese el nombre del producto : ");
    scanf(" %[^\n]s]", nombre);
    
    P = searchMap(mapaGeneral,nombre);
    if(P != NULL){
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

    while(P != NULL){
        printf("%s ,", P->nombre);
        printf("%s ,", P->marca);
        printf("%s ,", P->sector);
        printf("%d ,", P->stock);
        printf("%d \n", P->precio);
        P = nextMap(mapaGeneral);
    }
}

void agregarACarro(Map * mapaGeneral, Map * mapaCarritos, Map * mapaPorSector, Map * mapaPorMarca){
    char * nuevoNombreCarro = (char*) malloc(30*sizeof(char));
    printf("Ingrese un nombre para su carro: ");
    scanf(" %[^\n]s]", nuevoNombreCarro);
    carroCompras * C = (carroCompras*)malloc(sizeof(carroCompras));
    C = searchMap(mapaCarritos,nuevoNombreCarro);

    //BUSQUEDA DE CARRO EN MAPA CARRITOS
    if(C != NULL){//EXISTE EL CARRO
        
        //SE PIDE EL NOMBRE DEL PRODUCTO
        printf("El carro ya existe.\n");
        char * nuevoProducto = (char*) malloc(30*sizeof(char));
        printf("Ingrese el nombre del producto a insertar en el carrito: ");
        scanf(" %[^\n]s]", nuevoProducto);
        
        //SE BUSCA EN EL MAPA GENERAL SI EXISTE
        Producto * P = (Producto*)malloc(sizeof(Producto));
        P = searchMap(mapaGeneral,nuevoProducto);

        if(P != NULL){
            printf("Producto existe en mapa general\n");
            List * L = (List*)searchMap(mapaCarritos,nuevoNombreCarro);
            Producto * productoParaCarro = (Producto*)malloc(sizeof(Producto));
            productoParaCarro->nombre = P->nombre;
            productoParaCarro->marca = P->marca;
            productoParaCarro->sector = P->sector;
            productoParaCarro->precio = P->precio;
            productoParaCarro->stock = 1;


            Producto * productoGeneral = (Producto*)malloc(sizeof(Producto));
            productoGeneral = firstList(L); // PRODUCTO QUE ESTÁ EN LA LISTA DEL CARRO

            //VERIFICAR SI EL PRODUCTO YA EXISTE EN LA LISTA DEL CARRO
            while(productoGeneral != NULL){
                if(strcmp(productoGeneral->nombre, nuevoProducto) == 0){
                    printf("Producto ya existe en el carro.\n");
                    
                    //EN ESTE PUNTO, EXISTE EN MAPA GENERAL Y MAPA DEL CARRO.
                    P->stock--;
                    if(P->stock <= 0){

                        printf("Agotaste este producto.\n");
                        
                        // ELIMINANDO PRODUCTOS DE MAPA POR MARCA
                        List * L;
                        L = searchMap(mapaPorMarca,P->marca);
                        Producto * A = firstList(L);

                        while(A != NULL){
                            
                            if(strcmp(P->nombre,A->nombre) == 0){
                                popCurrent(L);
                                break;
                            }
                                    
                            A = nextList(L);
                            if(L == NULL)break;
                        }
                        // ELIMINANDO PRODUCTOS DE MAPA POR SECTOR/TIPO

                        List * L2;
                        L2 = searchMap(mapaPorSector,P->sector);
                        Producto * B = firstList(L2);

                        while(B != NULL){
                            
                            if(strcmp(P->nombre,B->nombre) == 0){
                                popCurrent(L2);
                                break;
                            }
                                    
                            B = nextList(L2);
                            if(L2 == NULL)break;
                        }

                        // ELIMINANDO PRODUCTOS DE MAPA GENERAL
                        eraseMap(mapaGeneral,P->nombre);
                        break;
                    }else{
                        productoGeneral->stock++;
                        printf("Producto del carro aumentado en stock.\n");
                        break;
                    }
                }
                productoGeneral = nextList(L);
                if(productoGeneral == NULL){
                    printf("Producto no existe en el carro.\n");
                    pushFront(L,productoParaCarro);
                    printf("Producto annadido al carro.\n");
                }
            }
            
        }else{ //NO EXISTE PRODUCTO
            printf("No existe ese producto.\n");
            
        }
        
    }else{ //NO EXISTE EL CARRO

        //SE CREA NUEVO CARROCOMPRAS
        printf("Nuevo carro agregado.\n");
        carroCompras* nuevoCarroCompras = (carroCompras*) malloc(30*sizeof(carroCompras));
        //SE PIDE EL NOMBRE DEL PRODUCTO, SE BUSCA EN MAPAGENERAL SI EXISTE
        char * nuevoProducto = (char*) malloc(30*sizeof(char));
        printf("Ingrese el nombre del producto a insertar en el carrito: ");
        scanf(" %[^\n]s]", nuevoProducto);
        
        //BUSQUEDA DE PRODUCTO INGRESADO EN MAPA GENERAL
        Producto * P = (Producto*)malloc(sizeof(Producto));
        P = searchMap(mapaGeneral,nuevoProducto);
        
        if(P != NULL){//EXISTE EL PRODUCTO
            
            List * L = createList();
            Producto * productoParaCarro = (Producto*)malloc(sizeof(Producto));
            productoParaCarro->nombre = P->nombre;
            productoParaCarro->marca = P->marca;
            productoParaCarro->sector = P->sector;
            productoParaCarro->precio = P->precio;
            productoParaCarro->stock = 1;

            pushFront(L,productoParaCarro);
            P->stock--;
            if(P->stock <= 0){
                printf("Agotaste este producto.\n");
                
                // ELIMINANDO PRODUCTOS DE MAPA POR MARCA
                List * L;
                L = searchMap(mapaPorMarca,P->marca);
                Producto * A = firstList(L);

                while(A != NULL){
                    
                    if(strcmp(P->nombre,A->nombre) == 0){
                        popCurrent(L);
                        break;
                    }
                            
                    A = nextList(L);
                    if(L == NULL)break;
                }
                // ELIMINANDO PRODUCTOS DE MAPA POR SECTOR/TIPO

                List * L2;
                L2 = searchMap(mapaPorSector,P->sector);
                Producto * B = firstList(L2);

                while(B != NULL){
                    
                    if(strcmp(P->nombre,B->nombre) == 0){
                        popCurrent(L2);
                        break;
                    }
                            
                    B = nextList(L2);
                    if(L2 == NULL)break;
                }

                // ELIMINANDO PRODUCTOS DE MAPA GENERAL
                eraseMap(mapaGeneral,P->nombre);
                
            }
            
            printf("Producto %s agregado a carro %s.\n",nuevoProducto,nuevoNombreCarro);
            
            insertMap(mapaCarritos,nuevoNombreCarro,L);

        }else{ // NO EXISTE PRODUCTO
            printf("No existe ese producto.\n");
    
        }
        
    }
}

void concretarCompraCarrito(Map* mapaCarros){

    // pedir nombre del carro y verificar si existe en mapa 
    char * carro = (char*) malloc(30*sizeof(char));
    printf("Ingrese el nombre de su carro de compras: ");
    scanf(" %[^\n]s]", carro);
    carroCompras * C = (carroCompras*)malloc(sizeof(carroCompras));
    C = searchMap(mapaCarros,carro);

    if(C != NULL){

        List * listaProductosCarro = (List*)searchMap(mapaCarros,carro);
        Producto * productosCarro = (Producto*)malloc(sizeof(Producto));
        productosCarro = firstList(listaProductosCarro);
        int totalPago=0;
        int totalProductos=0;

        while(productosCarro != NULL){

            // SE MUESTRAN LOS PRODUCTOS QUE ESTAN EN EL CARRO SELECCIONADO
            printf("%s , ", productosCarro->nombre);
            printf("%s , ", productosCarro->marca);
            printf("%s , ", productosCarro->sector);
            printf("%d , ", productosCarro->stock);
            printf("%d \n", productosCarro->precio);
            // SE OBTIENE EL PAGO TOTAL DE LOS PRODUCTOS DE LA LISTA
            totalPago += productosCarro->precio;
            // OBTENEMOS LA CANTIDAD DE PRODUCTOS QUE SE ENCUENTRAN EN LA LISTA
            totalProductos++;

            productosCarro = nextList(listaProductosCarro);

        }
        printf("===============================\n");
        printf("Total productos = %d.\n", totalProductos);
        printf(" precio total productos = $ %d.\n", totalPago);
        printf("===============================\n");

        char * pagar = (char*)malloc(10*sizeof(char));
        
        char * palabra = (char*)malloc(3*sizeof(char));
        palabra = "SI";
        
        printf("Si desea hacer su compra escriba SI:\n");
        scanf("%s", pagar);
        
        if(strcmp(pagar,palabra) == 0){
            printf("La compra ha sido completada.\n");
            eraseMap(mapaCarros,carro);
        }else{
            printf("No se concreto la compra.\n");
        }
    }else{
        printf("No existe el carrito deseado\n");
    }
    
    // Luego debemos eliminar los carritos con los productos que se encuentran en el mapa. 
    
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

//funcion general 
int main(){
    Map * mapaGeneral =createMap(isEqualString);
    Map * mapaPorMarca =createMap(isEqualString);
    Map * mapaPorSector =createMap(isEqualString);
    Map * mapaCarros =createMap(isEqualString);

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
            case 6:agregarACarro(mapaGeneral,mapaCarros,mapaPorSector,mapaPorMarca);break;
            case 7:concretarCompraCarrito(mapaCarros);break;
        }
    }
    return 0;
}
