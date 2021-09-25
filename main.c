#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "map.h"
#include "list.h"

Map * importar(Map * mapaGeneral, Map * mapaPorMarca, Map * mapaPorSector);
Map * cargarPorNombre(FILE * file, Map * mapa);
Map * cargarPorMarca(FILE * file, Map * mapa);
Map * cargarPorSector(FILE * file, Map * mapa);
void agregarProducto(Map * mapaGeneral, Map * mapaMarcas, Map * mapaTipos);
void buscarPorSector(Map * mapTipo, Map * mapaGeneral);
void buscarPorMarca(Map * mapMarcas, Map * mapaGeneral);
void buscarNombre(Map * mapaGeneral);
void mostrarTodo(Map* mapaGeneral);
void agregarACarro(Map * mapaGeneral, Map * mapaCarritos, Map * mapaPorSector, Map * mapaPorMarca);
void concretarCompraCarrito(Map* mapaCarros);
void exportarProductos (Map * mapaGeneral);
int isEqualString(void * key1, void * key2);

typedef struct{
    char * nombre;
    char * marca;
    char * sector;
    int stock;
    int precio;
}producto;

typedef struct{
    char * nombreCarro;
    List * productosCarritos;
}carroCompras;

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
        printf("8. Exportar productos.\n");
        printf("0. Salir.\n");
        scanf("%d",&opcion);

        switch(opcion){
            case 1:agregarProducto(mapaGeneral,mapaPorMarca,mapaPorSector);break;
            case 2:buscarPorSector(mapaPorSector,mapaGeneral);break;
            case 3:buscarPorMarca(mapaPorMarca,mapaGeneral);break;
            case 4:buscarNombre(mapaGeneral);break;
            case 5:mostrarTodo(mapaGeneral);break;
            case 6:agregarACarro(mapaGeneral,mapaCarros,mapaPorSector,mapaPorMarca);break;
            case 7:concretarCompraCarrito(mapaCarros);break;
            case 8:exportarProductos(mapaGeneral);
        }
    }
    return 0;
}

/*
    En esta funcion leemos el archivo csv, este lo utilizamos para cargar los mapas dejando dentro de este los datos correspondientes.
*/
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
    cargarPorSector(file3,mapaPorSector);

    fclose(file);
    fclose(file2);
    fclose(file3);
}
/*
  funcion para comparar claves de tipo string retorna 1 si son iguales
*/

int isEqualString(void * key1, void * key2){
    if(strcmp((char*)key1, (char*)key2)==0) return 1;
    return 0;
}

/*
    Funcion que recibe una linea, la lee hasta la coma y retorna lo leido, todo esto hasta llegar al final de la linea.
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

/*
    Funcion que guarda lo que contiene el archivo csv en el mapa cargar por nombre, utilizando la funcion que obtiene la linea 
    del archivo para ir  distribuyendo segun el campo que corresponda. Finalmente utilizando el insertMap para guardar todo esto
    dentro del mapa.
*/

Map * cargarPorNombre(FILE * file, Map * mapa){
    
    char lineaArchivo[1024];
    int i;
    int cont = 0;

    while (fgets (lineaArchivo, 1024, file) != NULL) {
        producto * productos = (producto*) malloc (sizeof(producto));
        
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
                productos->stock = atoi(aux); // Atoi convierte un char en int.
            }

            if(i==4){
                productos->precio = atoi(aux);
            }   
        }
        
        insertMap(mapa, (void *) productos->nombre, productos);

        cont++; 
        if(cont == 100) break;
    } 
}

/*
    En esta funcion se cargara el mapa con el campo marca, el nombre de la marca sera nuestra key y el value sera una lista
    que contenga los nombres que estan asociados a esa marca. Ademas, se comprobara si la marca buscada se encuentra dentro 
    del mapa.
*/

Map * cargarPorMarca(FILE * file, Map * mapa){
    char lineaArchivo[1024];
    int i;
    int cont = 0;
    // recorremos el archivo csv por linea
    while (fgets (lineaArchivo, 1024, file) != NULL) {
        producto * productos = (producto*)malloc(sizeof(producto));
        
        for(i = 0; i <= 4; i++){
            const char * aux = get_csv_field(lineaArchivo, i);
            if(i == 1){
                
                const char * M;
                char * marcaEntrante = (char*) aux;
                M = searchMap(mapa,marcaEntrante);

                // condicionales que determinan si la marca se encuentra dentro del mapa.

                if(M == NULL){//No existe la marca, por lo que se inserta como key la marca y el valor la lista con el producto.
                    List * listaMarcas = createList();
                    productos->nombre = get_csv_field(lineaArchivo, 0);
                    pushFront(listaMarcas,productos); 
                    insertMap(mapa,marcaEntrante,listaMarcas);
                }
                else{ // si la marca ya existe adicionamos el nombre producto dentro de la lista del mapa (mapa por marca).
                    
                    List * L = (List*)searchMap(mapa,marcaEntrante);
                    productos->nombre = get_csv_field(lineaArchivo, 0);
                    pushFront(L,productos);
                }
            }
        }

        cont++; 
        if(cont == 100) break;
    } 

}
/*
    En esta funcion se cargara el mapa con el campo sector, el nombre de la sector sera nuestra key y el value sera una lista
    que contenga los nombres que estan asociados a este sector. Ademas, se comprobara si el sector buscado se encuentra dentro 
    del mapa.
*/

Map * cargarPorSector(FILE * file, Map * map){
    char lineaArchivo[1024];
    int i;
    int cont = 0;
    // recorremos el archivo csv por linea
    while (fgets (lineaArchivo, 1024, file) != NULL) {
        producto * productos = (producto*)malloc(sizeof(producto));
        
        for(i = 0; i <= 4; i++){
            const char * aux = get_csv_field(lineaArchivo, i);
            if(i == 2){
                
                const char * M;
                char * sectorEntrante = (char*) aux;
                M = searchMap(map,sectorEntrante);
                
                // condicionales que determinan si el sector se encuentra dentro del mapa.

                if(M == NULL){ //No existe el sector , por lo que se  inserta como key el sectro y el valor la lista con el producto
                    List * listaSector = createList();
                    productos->nombre = get_csv_field(lineaArchivo, 0);
                    pushFront(listaSector,productos);
                    insertMap(map,sectorEntrante,listaSector);
                }
                else{ // en el caso de que ya exista solo adicionamos el nombre del producto dentro de la lista del mapa (mapa por sector).
                    
                    List * L = (List*)searchMap(map,sectorEntrante);
                    productos->nombre = get_csv_field(lineaArchivo, 0); // obtenemos el nombre del producto.
                    pushFront(L,productos);// insertamos el nombre del producto en la lista
                }
            }
        }
        cont++; 
        if(cont == 100) break;
    }
}

//Funcion para poder añadir las marcas o sectores a sus respectivos mapas donde la key es el nombre de la marca o sector
// y el value una lista con los productos.
void annadirEnMapas(Map * mapa, char * nombreProducto, char * nombreMoS, producto * nuevoP){
    const char * M;
    char * MoSEntrante = nombreMoS; // char que guarda la marca o el sector.

    //Se guarda el mapa
    M = searchMap(mapa,MoSEntrante);            
    if(M == NULL){//No existe la marca o sector, por lo que se inserta como key la marca o sector y el valor la lista con el producto
        List * lista = createList();
        nuevoP->nombre = nombreProducto;
        pushFront(lista,nuevoP);
        insertMap(mapa,MoSEntrante,lista);
    }
    else{ //Si existe la marca o sector, se inicia una lista en la lista que tiene la marca o sector y se mete el producto dentro de esta.
        
        List * L = (List*)searchMap(mapa,MoSEntrante);
        nuevoP->nombre = nombreProducto;
        pushFront(L,nuevoP);
    }

}
/*
Esta funcion busca en el mapa el producto deseado , en caso de que no este se muestra por pantalla.
*/
void buscarPorSector(Map * mapSector, Map * mapaGeneral){

    char * nombre = (char*) malloc(70*sizeof(char));
    printf("Ingrese el tipo de producto deseado : ");
    scanf(" %[^\n]s]", nombre);
    List * L;
    L = searchMap(mapSector,nombre);

    if(L){
        producto * P = firstList(L);
        while(P != NULL){
            
            printf("%s ,", P->nombre);
            producto * datos = (producto*)searchMap(mapaGeneral,P->nombre);
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
/*
    Esta funcion busca el producto por el nombre de la marca, en caso de que no exista aquel producto, se muestra por pantalla 
    que no existe.
*/
void buscarPorMarca(Map * mapMarcas, Map * mapaGeneral){

    char * nombre = (char*) malloc(70*sizeof(char));
    printf("Ingrese la marca del producto: ");
    scanf(" %[^\n]s]", nombre);
    List * L;
    L = searchMap(mapMarcas,nombre);
    
    if(L){
        producto * P = firstList(L);
        while(P != NULL){
            
            printf("%s ,", P->nombre);
            producto * datos = (producto*)searchMap(mapaGeneral,P->nombre);
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
/*
    Esta funcion busca y muestra el nombre del producto, en caso de que no este el producto, se muestra por pantalla 
    que el producto no existe.
*/
void buscarNombre(Map * mapaGeneral){

    char * nombre = (char*) malloc(70*sizeof(char));
    printf("Ingrese el nombre del producto : ");
    scanf(" %[^\n]s]", nombre);
    producto * P;
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

/* 
    Esta función lo que hace es agrega el producto el cual se ingresa el nombre del producto, si aquel producto existe se aumenta el 
    stock, en caso contrario se pide todos lo datos del producto y agrega el producto con sus respectivos datos. Este producto a la vez
    se agrega en todo los mapas:  mapa general, mapa marcas y mapa tipos.
*/

void agregarProducto(Map * mapaGeneral, Map * mapaMarcas, Map * mapaTipos){

    producto * P = (producto*)malloc(sizeof(producto));
    //Se pide el nombre del producto
    char * nombre = (char*) malloc(30*sizeof(char));
    printf("Ingrese el nombre del producto : ");
    scanf(" %[^\n]s]", nombre);
    
    //Se revisa si este existe o no, si existe se aumenta el stock, sino se agrega.
    P = searchMap(mapaGeneral,nombre);

    if(P != NULL){
        printf("Este producto ya existe, aumentando stock.\n"); 
        (P->stock)++;

    }
    else{
        
        producto *nuevoP = (producto *) malloc (sizeof (producto));

        printf("Este producto no existe, agregue datos.\n"); 
        // Solicitamos las especificaciones del producto
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
        nuevoP->stock = 1; // inicializamos el stock en 1 porque es un producto nuevo

        //A los mapas como, mapa general, mapa marcas y mapa tipos
        insertMap(mapaGeneral, (void *)nuevoP->nombre, nuevoP);

        annadirEnMapas(mapaMarcas, nombre, marca, nuevoP);
        annadirEnMapas(mapaTipos, nombre, sector, nuevoP);

        printf("Producto agregado. \n");
        
    }
}
/*
    La funcion "mostrarTodo" como dice el nombre muestra todo los productos con detalles como marca, stock, etc, que se encuentran
    en el mapa general
*/
void mostrarTodo(Map* mapaGeneral){

    //Se coloca el primer elemento del mapa general
    producto *P = firstMap(mapaGeneral);

    //Y se muestra por pantalla, pasando al siguiente elemento hasta terminar el mapa
    while(P != NULL){
        printf("%s, ", P->nombre);
        printf("%s, ", P->marca);
        printf("%s, ", P->sector);
        printf("%d, ", P->stock);
        printf("%d \n", P->precio);
        P = nextMap(mapaGeneral);
    }
}

/*
    La funcion "agregarACarro" pide el nombre del carro, verifica si este existe, si no existe se crea uno pero si existe
    se trabaja ahi mismo. Luego se pide el nombre del producto, se revisa si esta en el mapa general, si es asi se agrega
    al carro elminando el stock del mapa general y del producto del mapa marcas y mapa stock, si no muestra un mensaje por
    pantalla y acaba el programa
*/
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
        producto * P = (producto*)malloc(sizeof(producto));
        P = searchMap(mapaGeneral,nuevoProducto);

        if(P != NULL){
            printf("Producto existe en mapa general\n");
            List * L = (List*)searchMap(mapaCarritos,nuevoNombreCarro);
            producto * productoListaCarrito = (producto*)malloc(sizeof(producto));
            productoListaCarrito = firstList(L); // PRODUCTO QUE ESTÁ EN LA LISTA DEL CARRO

            producto * productoParaCarro = (producto*)malloc(sizeof(producto));
            productoParaCarro->nombre = P->nombre;
            productoParaCarro->marca = P->marca;
            productoParaCarro->sector = P->sector;
            productoParaCarro->precio = P->precio;
            

            //VERIFICAR SI EL PRODUCTO YA EXISTE EN LA LISTA DEL CARRO
            while(productoListaCarrito != NULL){
                if(strcmp(productoListaCarrito->nombre, nuevoProducto) == 0){
                    printf("Producto ya existe en el carro.\n");
                    
                    //EN ESTE PUNTO, EXISTE EN MAPA GENERAL Y MAPA DEL CARRO.
                    productoListaCarrito->stock += 1;
                    P->stock--;
                    if(P->stock <= 0){

                        printf("Agotaste este producto.\n");
                        
                        // ELIMINANDO PRODUCTOS DE MAPA POR MARCA
                        List * L;
                        L = searchMap(mapaPorMarca,P->marca);
                        producto * productoMapaMarcas = firstList(L);

                        while(productoMapaMarcas != NULL){
                            
                            if(strcmp(P->nombre,productoMapaMarcas->nombre) == 0){
                                popCurrent(L);
                                break;
                            }
                                    
                            productoMapaMarcas = nextList(L);
                            if(L == NULL)break;
                        }
                        // ELIMINANDO PRODUCTOS DE MAPA POR SECTOR/TIPO

                        List * L2;
                        L2 = searchMap(mapaPorSector,P->sector);
                        producto * productoMapaSector = firstList(L2);

                        while(productoMapaSector != NULL){
                            
                            if(strcmp(P->nombre,productoMapaSector->nombre) == 0){
                                popCurrent(L2);
                                break;
                            }
                                    
                            productoMapaSector = nextList(L2);
                            if(L2 == NULL)break;
                        }

                        // ELIMINANDO PRODUCTOS DE MAPA GENERAL
                        eraseMap(mapaGeneral,P->nombre);
                        break;
                    }else{
                        productoListaCarrito++;
                        printf("Producto del carro aumentado en stock.\n");
                        break;
                    }
                }
                productoListaCarrito = nextList(L);
                if(productoListaCarrito == NULL){
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
        producto * P = (producto*)malloc(sizeof(producto));
        P = searchMap(mapaGeneral,nuevoProducto);
        
        if(P != NULL){//EXISTE EL PRODUCTO
            
            List * L = createList();
            producto * productoParaCarro = (producto*)malloc(sizeof(producto));
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
                producto * productoMapaMarcas = firstList(L);

                while(productoMapaMarcas != NULL){
                    
                    if(strcmp(P->nombre,productoMapaMarcas->nombre) == 0){
                        popCurrent(L);
                        break;
                    }
                            
                    productoMapaMarcas = nextList(L);
                    if(L == NULL)break;
                }
                // ELIMINANDO PRODUCTOS DE MAPA POR SECTOR/TIPO

                List * L2;
                L2 = searchMap(mapaPorSector,P->sector);
                producto * productoMapaSector = firstList(L2);

                while(productoMapaSector != NULL){
                    
                    if(strcmp(P->nombre,productoMapaSector->nombre) == 0){
                        popCurrent(L2);
                        break;
                    }
                            
                    productoMapaSector = nextList(L2);
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
/*
    La funcion "concretarCompraCarrito" recibe el nombre de un carrito, comprueba si este existe, si no es asi muestra un mensaje
    por pantalla y se sale de la funcion, en cambio si es asi, muestra por pantalla los productos del carrito junto con su precio,
    cantidad y el precio total, para por ultimo preguntar al usuario si efectuara la compra
*/
void concretarCompraCarrito(Map* mapaCarros){

    //Se pide nombre del carro. 
    char * carro = (char*) malloc(30*sizeof(char));
    printf("Ingrese el nombre de su carro de compras: ");
    scanf(" %[^\n]s]", carro);

    //Se verifica si existe en el mapa de carros.
    carroCompras * C = (carroCompras*)malloc(sizeof(carroCompras));
    C = searchMap(mapaCarros,carro);

    if(C != NULL){ //El carro existe.

        //Obtenemos la lista que contienen los productos del carro.
        List * L = (List*)searchMap(mapaCarros,carro);
        producto * productosCarro = (producto*)malloc(sizeof(producto));
        productosCarro = firstList(L);
        int totalPago=0;
        int totalProductos=0;

        while(productosCarro != NULL){

            //Se muestran los datos del producto
            printf("%s, ", productosCarro->nombre);
            printf("%s, ", productosCarro->marca);
            printf("%s, ", productosCarro->sector);
            printf("%d, ", productosCarro->stock);
            printf("%d \n", productosCarro->precio);
            
            //Se guarda el total de productos y el total a pagar por cada iteracion.
            totalPago += totalPago + (productosCarro->precio * productosCarro->stock);
            totalProductos = totalProductos + productosCarro->stock;
            
            //Se pasa al siguiente producto de la lista del carro.
            productosCarro = nextList(L);
        }
        printf("=======================================\n");
        printf("       Total productos = %d.\n", totalProductos);
        printf("  Precio total productos = $%d.\n", totalPago);
        printf("=======================================\n");

        //Se verifica si se quiere completar la compra, si es asi se elimina el carro del mapa de carros.
        char * pagar = (char*)malloc(10*sizeof(char));
        char * palabra = (char*)malloc(3*sizeof(char));
        palabra = "SI";
        
        printf("Si desea hacer su compra escriba SI: ");
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
    
}

/*
    La funcion "exportarProductos" crea nuevo archivo ".csv" en donde iran todos los datos del mapa general
*/
void exportarProductos (Map * mapaGeneral){
    char * nombreArchivo = (char*) malloc(30*sizeof(char));
    FILE * nuevoFile;

    //Se pide el nombre para el nuevo archivo ".csv"
    do{
        printf("Ingrese el nombre de su nuevo archivo .csv: ");
        scanf(" %[^\n]s", nombreArchivo);
        if(nombreArchivo == "Productos"){
            printf("Este archivo .csv ya existe, intente con otro nombre.\n");
        }
    }while(nombreArchivo == "Productos");
    
    //Se muestra por pantalla el nombre del archivo
    printf("Nombre de su archivo: %s.csv\n",nombreArchivo);
    
    nombreArchivo = strcat(nombreArchivo,".csv"); 
    nuevoFile=fopen(nombreArchivo,"w+");
    producto * P = (producto*)malloc(sizeof(producto));
    P = firstMap(mapaGeneral);
    
    //Se imprime todos los datos del mapa general en el nuevo archivo ".csv"
    while(P!=NULL){
        
        fprintf(nuevoFile,"%s,%s,%s,%d,%d\n",P->nombre,P->marca,P->sector,P->stock,P->precio);
        P = nextMap(mapaGeneral);
    }
    
}