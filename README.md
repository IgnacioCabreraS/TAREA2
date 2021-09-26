# Tarea2

Tarea 2 Estructura de Datos Grupo GC

1.- Analisis de tarea y como lo haremos.

El problema nos pide crear un programa que permita guardar productos y sus respectivas caracteristicas en un almacen y poder buscarlos facilmente.
Creamos un struct "producto" para guardar el nombre del producto, marca, sector, stock y precio, y un struct "carroCompras" para tener el nombre del carro
y los productos correspondientes.

Usaremos 4 mapas:
.- Mapa general = tendra todos los datos de los productos y lo usaremos en la mayoria del codigo.
.- Mapa de Marcas = contiene las marcas como key, y el valor es una lista con los productos de la misma marca. (usa Tipo producto pero solo se guarda la marca y el nombre del producto)
.- Mapa de Sector = contiene los sectores como key, el valor es una lista con los productos del mismo sector. (usa Tipo producto pero solo se guarda el sector y el nombre del producto)
.- Mapa Carrito = guarda los carros que cree el usuario, tienen asociados listas con los productos.

Esta cantidad de mapas se debe a que al momento de realizar una busqueda es mas eficiente (y facil) con el search map.

2.- Cómo compilar y ejecutar la tarea?

R: Para compilar y ejecutar nuestra tarea debemos abrir la terminal de visual estudio y escribir los siguientes comandos:

gcc -g main.c list.c -o Tarea2
./Tarea2.exe

Usamos la ultima version de MinGW para poder hacer la tarea.

3.- Las opciones que funcionan correctamente y las que no lo hacen indicando posibles causas:

R: Todas las funciones realizan bien su tarea.

Observacion: al momento mostrar todos los productos del mapa general, estos se muestran al reves de la forma que estan en el csv, pasa lo mismo
cuando exportamos un archivo csv y se guarda los productos dentro.

4.- Aspectos positivos y a mejorar por cada uno de los integrantes (coevaluación);

Manuel Cabrera: trabajo bien, aporto al arreglar algunos warnings, el menu, ademas de algunos detalles de la funcion "Agregar Producto", pero le falto meterse mas en el tema de los Mapas al principio y dedicar mas tiempo a la tarea en si.

Ignacio Cabrera: paso mucho tiempo creando el codigo, aporto en la mayoria de funciones, falta repasar un poco el uso de mapas.

Alexander Guillaume: Ayudo a realizar algunas funciones, pero su debilidad fue al momento de ocupar el mapa ya que le dificulto su uso y como se manejaba.

Marco Rojas: Participo bien dentro del codigo, en los aspectos a mejorar falta profundizar en conocimientos sobre los mapas.

5.- Puntos de premio/castigo a cada integrante dependiendo del trabajo realizado.

1) Marco: 0 
2) Alexander: 0 
3) Manuel: 0 
4) Ignacio: 0
