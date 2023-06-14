#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define max 10000
#define maxbuffer 100

struct Derco{
    struct NodoRegion *regiones;
    struct NodoProducto *productos;
};

struct NodoProducto
{
    struct Producto *datosProducto;
    struct NodoProducto *ant, *sig;
};

struct Producto{
    char *tipo;
    char *modelo;
    char *marca;
    int precio;
    char *codigoProducto;
    int recalls;
};

struct NodoRegion{
    struct Region *region;
    struct NodoRegion *sig, *ant;
};

struct Region{
    struct NodoSede *sedes;
    char *nombreRegion;
};

struct NodoSede{
    struct Sede *datosSede;
    struct NodoSede *izq, *der;
};

struct Sede{
    char *nombreSede;
    struct NodoVenta *ventas;
    struct Bodega *inventario[max];
    int tiempoTransporte;
    char *comuna;
};

struct Bodega{
    int stock;
    struct Producto *producto;
};

struct NodoVenta{
    struct Venta *datosVenta;
    struct NodoVenta *sig, *ant;
};

struct Venta{
    char *rutCliente;
    char *fecha;
    int montoTotal ;
    int cantidadVendida;
    int codigo;
    struct Producto *productoVendido;
};

//----------------------------------------------------SEDES-----------------------------------------------------------//

//-------------------------------------------------BUSCAR SEDE--------------------------------------------------------//
//      Esta función se encarga de buscar una sede especifica a través de su nombre que recibe como parámetro         //
//      formal. Se hace un ciclo preguntando si el nodo existe y si la sede buscada aún no es encontrada.             //
//      mientras no se encuentre, avanzará por el arbol hacia la izquierda o derecha dependiendo del resultado        //
//      de una comparacion de cadena, cuando se encuentra la sede deseada, el ciclo se detiene y retorna la misma.    //
//--------------------------------------------------------------------------------------------------------------------//

struct NodoSede * buscarSede (struct NodoSede *raiz, struct NodoSede **previa, char *nombreBuscado) {
    struct NodoSede *abb = raiz;
    bool esta = false;
    while (abb != NULL && esta == false) {
        if (strcmp(abb->datosSede->nombreSede, nombreBuscado) == 0) {
            esta = true;

        }
        else {
            *previa = abb;
            if (strcmp( nombreBuscado, abb->datosSede->nombreSede) < 0)
                abb = abb->izq;

            else
                abb = abb->der;
        }
    }
    return abb;
}

//-------------------------------------------------ES HOJA------------------------------------------------------------//
//        Es una funcion auxiliar de "eliminarSede", que verifica si el nodo del arbol en el que se                   //
//        encuentra es una hoja o no, retornando 1 si lo es, y 0 si no lo es.                                         //
//--------------------------------------------------------------------------------------------------------------------//

int esHoja(struct NodoSede *ptr)
{
    if(ptr->der == NULL && ptr->izq == NULL)
        return 1;
    return 0;
}

//------------------------------------------------AGREGAR SEDE--------------------------------------------------------//
//      Esta función es encargada de añadir al arbol una sede nueva, para ello llama a "buscarSede",                  //
//      la cual, en caso de que no encuentre la sede a agregar en el arbol, le daria la posicion exacta               //
//      para añadir la misma. En caso de que si se haya encontrado la sede en el arbol, no se añadiria.               //
//      Retorna 1 si se agrego existosamente y 0 si es que no.                                                        //
//--------------------------------------------------------------------------------------------------------------------//

int agregarSede(struct NodoSede **sedeRaiz, struct Sede *datosNuevos)
{
    struct NodoSede *sedeNueva = NULL, *ant = NULL, *ptr = NULL;
    ptr = buscarSede(*sedeRaiz, &ant, datosNuevos->nombreSede);
    if(ptr == NULL)
    {
        sedeNueva = (struct NodoSede *) malloc (sizeof(struct NodoSede));
        sedeNueva->datosSede = datosNuevos;
        sedeNueva->izq = sedeNueva->der = NULL;
        if(ant == NULL)
            *sedeRaiz = sedeNueva;

        else
        {
            if(strcmp(datosNuevos->nombreSede, ant->datosSede->nombreSede) < 0)
                ant->izq = sedeNueva;
            else
                ant->der = sedeNueva;
        }
        return 1;
    }
    return 0;
}

//---------------------------------------------ESTABLECER SEDE--------------------------------------------------------//
//      Funcion encargada de crear una sede nueva, reservando memoria y colocando todos                               //
//      los datos de la misma, posteriormente se llama a "agregarSede" para verificar                                 //
//      que sea una sede valida a añadir, si es añadida correctamente, retorna 1.                                     //
//--------------------------------------------------------------------------------------------------------------------//

int establecerSede(struct NodoSede **raiz, char *nombre, int tiempo, char *comuna)
{
    struct Sede * sede = ((struct Sede*) malloc(sizeof(struct Sede)));
    sede->nombreSede = (char *) malloc(strlen(nombre)*sizeof(char));
    strcpy(sede->nombreSede, nombre);
    sede-> ventas = NULL;
    sede->tiempoTransporte = tiempo;

    sede->comuna =  (char *) malloc (strlen(comuna) *sizeof(char));
    strcpy(sede->comuna, comuna);

    if(agregarSede(raiz, sede) == 0)
        return 0;

    return 1;
}

//----------------------------------------------ELIMINAR SEDE---------------------------------------------------------//
//      Esta funcion es encargada de eliminar sedes a conveniencia, recibe por paremetro la raiz                      //
//      del arbol de las sedes y el nombre de la sede a eliminar. La funcion primero verifica que                     //
//      la sede exista llamando a buscarSede, si existe entra en un ciclo donde llamara a la funcion                  //
//      "esHoja" dependiendo de si lo es o no, va a eliminar la sede del arbol a conveniencia.                        //
//--------------------------------------------------------------------------------------------------------------------//

int eliminarSede (struct NodoSede **sedesABB, char *nombreSedeEliminar)
{
    struct NodoSede *padre = NULL, *actual = NULL, *nodo = NULL;
    struct Sede* aux;

    actual = buscarSede(*sedesABB, &padre, nombreSedeEliminar);

    while(actual != NULL)
    {
        if(esHoja(actual) == 1)
        {

            if(actual == (*sedesABB)){
                (*sedesABB)= NULL;
            }
            if(padre != NULL)
            {
                if(padre->der == actual)
                    padre->der = NULL;

                else
                    padre->izq = NULL;
            }
            actual = NULL;
            return 1;
        }

        else
        {
            padre = actual;
            if(actual->der != NULL)
            {
                nodo = actual->der;
                while(nodo->izq != NULL)
                {
                    padre = nodo;
                    nodo = nodo->izq;
                }
            }

            else
            {
                nodo = actual->izq;
                while(nodo->der != NULL)
                {
                    padre = nodo;
                    nodo = nodo->der;
                }
            }
            aux = actual->datosSede;
            actual->datosSede = nodo->datosSede;
            nodo->datosSede = aux;
            actual = nodo;
        }
    }
    return 0;
}

//---------------------------------------------MODIFICAR SEDE---------------------------------------------------------//
//      Funcion encargada de cambiar el nombre a una sede existente, recibe como parametros la sede                   //
//      a la cual le cambiara el nombre y el nombre nuevo, verifica si el arbol existe y si la sede                   //
//      que tiene que modificar existe, si ambas condiciones se cumplen, se borra el nombre actual                    //
//      y se copia el nuevo.                                                                                          //
//--------------------------------------------------------------------------------------------------------------------//

int modificarNombreSede(struct NodoSede *abbSede, char *nombreBuscado, char *nuevoNombre)
{
    struct NodoSede *previa = NULL;
    if (abbSede != NULL)
    {
        struct NodoSede *nodoBuscado = buscarSede(abbSede, &previa,  nombreBuscado);
        if (nodoBuscado != NULL) {
            nodoBuscado->datosSede->nombreSede =  NULL;
            nodoBuscado->datosSede->nombreSede = (char *) malloc(strlen(nuevoNombre) * sizeof(char));

            strcpy(nodoBuscado -> datosSede -> nombreSede, nuevoNombre);
            return 1;
        }
    }
    return 0;
}

//---------------------------------------------MOSTRAR SEDES----------------------------------------------------------//
//      Esta funcion recursiva pre-orden se encarga de recorrer y mostrar los nombres de todas las                    //
//      sedes en el arbol de sedes.                                                                                   //
//--------------------------------------------------------------------------------------------------------------------//

void mostrarSedes(struct NodoSede *abb){
    if(abb != NULL){
        printf("-%s\n",abb->datosSede->nombreSede);
        mostrarSedes(abb->izq);
        mostrarSedes(abb->der);
    }
}

//------------------------------------------------REGION--------------------------------------------------------------//

//---------------------------------------------BUSCAR REGION----------------------------------------------------------//
//      Funcion a cargo de buscar si existe o no una region a traves del nombre de la misma. Primero, se              //
//      verifica si la lista de regiones existe, luego se hace un ciclo hasta encontrar, gracias a una                //
//      comparacion, la region deseada, retornando su nodo en caso de encontrarla y nulo en caso contrario.           //
//--------------------------------------------------------------------------------------------------------------------//

struct NodoRegion *buscarRegion (struct NodoRegion *head, char* nombreBuscado)
{
    if (head != NULL)
    {
        struct NodoRegion *rec = head -> sig;
        while (rec != NULL)
        {
            if (strcmp(rec -> region -> nombreRegion, nombreBuscado) == 0)
            {
                return rec;
            }
            rec = rec -> sig;
        }
    }
    return NULL;
}

//--------------------------------------------CREAR NODO REGION-------------------------------------------------------//
//      Esta funcion reserva memoria y asigna los datos del nodo de una region. Finalmente, retorna el nodo.          //
//--------------------------------------------------------------------------------------------------------------------//

struct NodoRegion *crearNodoRegion(struct Region *dato){
    struct NodoRegion *nuevo;

    nuevo = ((struct NodoRegion*) malloc(sizeof(struct NodoRegion)));
    nuevo->region = dato;
    nuevo->sig = nuevo->ant = NULL;

    return nuevo;
}

//----------------------------------------------AGREGAR REGION--------------------------------------------------------//
//      Recibe como parametro la region a agregar y la lista de regiones, si esta existe, se hace un ciclo            //
//      hasta encontrar la ultima posicion de la misma, añadiendo la nueva region a la ultima posicion de             //
//      esta lista. Si la region es agregada correctamente, retorna 1, si no retorna 0.                               //
// -------------------------------------------------------------------------------------------------------------------//

int agregarRegion(struct NodoRegion *head, struct NodoRegion *nuevo){
    struct NodoRegion *rec = head;
    if(rec != NULL){
        while(rec-> sig != NULL){
            rec = rec->sig;
        }
        rec->sig = nuevo;
        nuevo->ant = rec;
        return 1;
    }
    return 0;
}

//---------------------------------------------ESTABLECER REGION------------------------------------------------------//
//      Verifica si la lista de regiones existe, si no, se crea empezando por un nodo fantasma, posteriormente
//      se busca si el nombre de la region a agregar existe o no, si no existe, se reserva memoria establecen
//      los datos y se llama a "agregarRegion" para añadir la misma a la lista. Se retorna 1 si se establecio
//      correctamente y 0 si no lo hace.
//--------------------------------------------------------------------------------------------------------------------//

int establecerRegion(struct NodoRegion **head, char *nombre){
    if(*head == NULL){
        (*head) = crearNodoRegion(NULL);
    }

    if(buscarRegion(*head, nombre) == NULL){
        struct Region *reg = ((struct Region*) malloc(sizeof(struct Region)));
        reg->nombreRegion = (char*) malloc(strlen(nombre)*sizeof(char));
        strcpy(reg->nombreRegion, nombre);
        reg->sedes = NULL ;
        agregarRegion(*head,crearNodoRegion(reg));
        return 1;
    }
    return 0;
}

//---------------------------------------------ELIMINAR REGION--------------------------------------------------------//
//      Recibe como parametro el nombre de la region que se desea eliminar. Se verifica si la lista existe,           //
//      si lo hace, se crea un ciclo, se compara el nombre de todas las regiones con el buscado, si se                //
//      encuentra, se elimina la region buscada y se conectan los nodos que hay antes y despues de esta.              //
//      Retorna 1 si se eliminó con exito y 0 si no.                                                                  //
//--------------------------------------------------------------------------------------------------------------------//

int eliminarRegion(struct NodoRegion **head, char *nombreBuscado){
    if((*head )!= NULL) {
        struct NodoRegion *rec = (*head)->sig;
        while (rec != NULL) {
            if (strcmp(rec->region->nombreRegion, nombreBuscado) == 0) {
                if ((*head)->sig == rec) {
                    (*head)-> sig = rec->sig;
                    if((*head)->sig == NULL){
                        (*head) = NULL;
                        return 1;
                    }

                }
                if (rec->sig != NULL)
                    rec->sig->ant = rec->ant;


                rec->ant->sig = rec->sig;
                rec = NULL;
                return 1;
            }
            rec = rec->sig;
        }

    }
    return 0;
}

//--------------------------------------------MODIFICAR REGION--------------------------------------------------------//
//      Se encarga de modificar el nombre de una region existente. Primero se verifica que la lista de                //
//      regiones exista, se llama a "buscarRegion", si la region que se desea modificar existe, se copia              //
//      el nombre nuevo en la region. Si el nombre es cambiado exitosamente, se retorna 1, si no 0.                   //
//--------------------------------------------------------------------------------------------------------------------//

int modificarRegion(struct NodoRegion *head, char *nombreBuscado, char *nuevoNombre)
{
    if (head != NULL)
    {

        struct NodoRegion *nodoBuscado = buscarRegion(head,nombreBuscado);
        if (nodoBuscado != NULL)
        {
            nodoBuscado->region->nombreRegion = (char*) malloc(strlen(nuevoNombre)*sizeof(char));

            strcpy(nodoBuscado->region->nombreRegion, nuevoNombre);
            return 1;
        }
    }
    return 0;
}

//------------------------------------------MOSTRAR REGIONES----------------------------------------------------------//
//      Se verifica que la lista de regiones exista, si existe, se hace un ciclo mostrando cada region existente      //
//      si la lista no existe, se muestra que no hay regiones.                                                        //
//--------------------------------------------------------------------------------------------------------------------//

void mostrarRegiones(struct NodoRegion *head)
{
    if(head != NULL)
    {
        struct NodoRegion *rec = head->sig;
        printf("Lista de todas las regiones: \n");
        while (rec != NULL)
        {
            printf("- %s\n",rec->region->nombreRegion);
            rec = rec->sig;
        }
    }

    else
        printf("NO HAY REGIONES\n");
}

//-------------------------------------------------VENTAS-------------------------------------------------------------//

//----------------------------------------------BUSCAR VENTA----------------------------------------------------------//
//      Primero verifica si la lista de ventas existe, si esta validacion es verdadera, recorre la lista              //
//      hasta encontrar el codigo de la venta deseada, el cual es recibido como parametro. Retorna el nodo            //
//      buscado em caso de encontrarlo y nulo en caso de que no.                                                      //
//--------------------------------------------------------------------------------------------------------------------//

struct NodoVenta *buscarVenta (struct NodoVenta *head, int codigo)
{
    if (head != NULL)
    {
        struct NodoVenta *rec = head;
        do
        {
            if (rec->datosVenta->codigo == codigo)
            {
                return rec;
            }
            rec = rec -> sig;
        } while (rec != head);
    }
    return NULL;
}

//---------------------------------------------AGREGAR VENTA----------------------------------------------------------//
//      Se reserva memoria para el nodo de la venta a agregar, si la lista de ventas existe, se añade                 //
//      la nueva venta en la posicion anterior del head y se enlaza con los nodos correspondientes.                   //
//      Si la lista no existe se establece la nueva venta como head.                                                  //
//--------------------------------------------------------------------------------------------------------------------//

void agregarVenta(struct NodoVenta **headVenta, struct Venta *ventaAgregar)
{
    struct NodoVenta *ventaNueva = NULL;
    ventaNueva = (struct NodoVenta *) malloc (sizeof(struct NodoVenta));
    ventaNueva->datosVenta = ventaAgregar;
    if((*headVenta) != NULL)
    {
        (*headVenta)->ant->sig = ventaNueva;
        ventaNueva->ant =  (*headVenta)->ant;
        (*headVenta)->ant = ventaNueva;
        ventaNueva->sig = (*headVenta);
    }
    else
    {
        (*headVenta) = ventaNueva;
        (*headVenta)->sig = (*headVenta)->ant = (*headVenta);
    }
}

//---------------------------------------------ESTABLECER VENTA-------------------------------------------------------//
//      Se verifica que la venta que uno quiere establecer exista, si no existe se establecen los datos               //
//      de la misma y llama a "agregarVenta" para colocarla en la lista de ventas. Retorna 1 si se                    //
//      establecio correctamente y 0 si no.                                                                           //
//--------------------------------------------------------------------------------------------------------------------//

int establecerVenta(struct NodoVenta **head, struct Producto *productoComprado, char *rutCliente,char *fecha, int cant, int *codigo)
{
    if(buscarVenta(*head, (*codigo))==NULL){
        struct Venta *venta = ((struct Venta*) malloc(sizeof(struct Venta)));

        venta->rutCliente = (char *) malloc(strlen(rutCliente) * sizeof(char));
        strcpy(venta->rutCliente, rutCliente);

        venta->fecha = (char *) malloc(strlen(fecha) * sizeof(char));
        strcpy(venta->fecha, fecha);

        venta->cantidadVendida = cant;
        venta->montoTotal = (productoComprado->precio) * venta->cantidadVendida;
        venta->productoVendido = productoComprado;
        venta->codigo = (*codigo);
        (*codigo)++;
        agregarVenta(head,venta );
        return 1;
    }
    else
        printf("Venta ya existente\n");
    return 0;
}

//--------------------------------------------ELIMINAR VENTA----------------------------------------------------------//
//      Esta funcion elimina una venta a traves del codigo de la misma, primero verifica que la lista                 //
//      de ventas exista, posteriormente recorre la lista hasta encontrar el nodo de la venta a eliminar.             //
//      Si la venta es eliminada con exito se retorna 1, si no 0.                                                     //
//--------------------------------------------------------------------------------------------------------------------//

int eliminarVenta (struct NodoVenta **head, int codigo)
{
    if(*head != NULL)
    {
        struct NodoVenta *rec = (*head);
        do{
            if(rec->datosVenta->codigo == codigo)
            {
                if(rec == (*head))
                {
                    if(rec == (*head)->sig){
                        (*head) = NULL;
                        return 1;
                    }
                    (*head) = (*head)->sig;
                }
                rec->ant->sig = rec->sig;
                rec->sig->ant = rec->ant;

                rec = NULL;
                return 1;
            }
            rec = rec->sig;
        }while(rec != *head);
    }
    return 0;
}

//--------------------------------------------MOSTRAR VENTA-----------------------------------------------------------//
//      Verifica que la lista de ventas exista, si existe, muestra todas las ventas y sus datos.                      //
//-------------------------------------------------------------------------------------------------------------------//

void mostrarVentas(struct NodoVenta *head){
    if(head != NULL)
    {
        struct NodoVenta *rec = head;
        printf("Lista de ventas: \n");
        printf("  Codigo Venta          Fecha         Monto Venta        Modelo\n");
        do
        {
            printf("%d ",rec->datosVenta->codigo);
            printf("%15s ",rec->datosVenta->fecha);
            printf("%15d pesos",rec->datosVenta->montoTotal);
            printf("%25s\n", rec->datosVenta->productoVendido->modelo);
            rec = rec->sig;
        }while (rec != head);
    }

    else
        printf("No hay ventas\n");
}

//----------------------------------------------PRODUCTOS-------------------------------------------------------------//

//-------------------------------------------BUSCAR PRODUCTO----------------------------------------------------------//
//      Este funcion primero verifica si la lista de productos exista, si lo hace, la recorre en un ciclo             //
//      hasta en contrar el codigo del producto deseado, si el producto se encuentra, se retorna su nodo,             //
//      si no, se retorna nulo.                                                                                       //
//--------------------------------------------------------------------------------------------------------------------//

struct NodoProducto *buscarProducto (struct NodoProducto *head, char *codigoBuscado)
{
    if (head != NULL)
    {
        struct NodoProducto *rec = head;
        do
        {
            if (strcmp(rec -> datosProducto -> codigoProducto, codigoBuscado) == 0)
            {
                return rec;
            }
            rec = rec -> sig;
        } while (rec != head);
    }
    return NULL;
}

//-------------------------------------------AGREGAR PRODUCTO---------------------------------------------------------//
//      Se reserva memoria para el nodo de la producto a agregar, si la lista de ventas existe, se añade              //
//      la nueva producto en la posicion anterior del head y se enlaza con los nodos correspondientes.                //
//      Si la lista no existe se establece la nueva venta como head.                                                  //
//--------------------------------------------------------------------------------------------------------------------//

void agregarProducto(struct NodoProducto **headProducto, struct Producto *productoAgregar)
{
    struct NodoProducto *productoNuevo = NULL;
    productoNuevo = (struct NodoProducto *) malloc (sizeof(struct NodoProducto));
    productoNuevo->datosProducto = productoAgregar;
    if((*headProducto) != NULL)
    {
        (*headProducto)->ant->sig = productoNuevo;
        productoNuevo->ant =  (*headProducto)->ant;
        (*headProducto)->ant = productoNuevo;
        productoNuevo->sig = (*headProducto);
    }
    else
    {
        (*headProducto) = productoNuevo;
        (*headProducto)->sig = (*headProducto)->ant = (*headProducto);
    }
}

//---------------------------------------------ESTABLECER PRODUCTO----------------------------------------------------//
//      Se verifica que la producto que uno quiere establecer exista, si no existe se establecen los datos            //
//      de la misma y llama a "agregarProducto" para colocarla en la lista de producto. Retorna 1 si se               //
//      establecio correctamente y 0 si no.                                                                           //
//--------------------------------------------------------------------------------------------------------------------//

struct Producto *establecerProducto(struct NodoProducto **head, char *codigo, char *tipo, char *modelo, char *marca, int precio)
{
    //Entrada
    struct Producto *prod= NULL;
    if(buscarProducto(*head, codigo)==NULL){
        prod = ((struct Producto*) malloc(sizeof(struct Producto)));
        prod->tipo = (char *) malloc(strlen(tipo) * sizeof(char));
        strcpy(prod->tipo, tipo);

        prod->modelo = (char *) malloc(strlen(modelo) * sizeof(char));
        strcpy(prod->modelo, modelo);

        prod->marca = (char *) malloc(strlen(marca) * sizeof(char));
        strcpy(prod->marca, marca);

        prod->precio = precio;

        prod->codigoProducto = (char *) malloc(strlen(codigo) * sizeof(char));
        strcpy(prod->codigoProducto, codigo);

        prod->recalls = 0;
        agregarProducto(head,prod );

    }
    else
        printf("Producto ya existente\n");

    return prod;
}

//---------------------------------------------MODIFICAR PRODUCTO-----------------------------------------------------//
//      Este funcion verifica que el producto exista, luego modifica, el modelo, marca o el precio, si                //
//      se modifica exitosamente, retorna 1, si no retorna 0.                                                         //
//--------------------------------------------------------------------------------------------------------------------//

int modificarProducto(struct NodoProducto *head, char *codProducto, int opcion, char * nuevoCampo) {
    struct NodoProducto * productoBuscado = buscarProducto(head, codProducto);

    if(productoBuscado != NULL) {

        if (opcion == 1)
            strcpy(productoBuscado->datosProducto->modelo, nuevoCampo);

        else if (opcion == 2)
            strcpy(productoBuscado->datosProducto->marca, nuevoCampo);

        else if (opcion == 3)
            productoBuscado->datosProducto->precio = atoi(nuevoCampo);

        else
            return 0;

        return 1;
    }
    return 0;
}

//--------------------------------------------MOSTRAR PRODUCTI--------------------------------------------------------//
//      Verifica que la lista de producto exista, si existe, muestra todas las producto y sus datos.                  //
//--------------------------------------------------------------------------------------------------------------------//

void mostrarProductos(struct NodoProducto *head)
{
    if(head != NULL)
    {
        struct NodoProducto *rec = head;
        printf("Lista de productos: \n");
        printf("    Modelo     Precio               Marca   Codigo Producto\n");
        do
        {
            printf("-%10s ",rec->datosProducto->modelo);
            printf("%10d Pesos",rec->datosProducto->precio);
            printf("%15s",rec->datosProducto->marca);
            printf("%15s\n", rec->datosProducto->codigoProducto);
            rec = rec->sig;
        }while (rec != head);
    }

    else
        printf("NO HAY PRODUCTOS\n");
}

//-----------------------------------------------INVENTARIO-----------------------------------------------------------//

//--------------------------------------------BUSCAR INVENTARIO-------------------------------------------------------//
//      Busca la posicion del codigo de un producto en el inventario, retornandolo. Si no se encuentra retorna -1     //
//--------------------------------------------------------------------------------------------------------------------//

int buscarInventario(struct Bodega *inventario[max], char *codigoBuscado)
{
    int i;
    for (i = 0; i < max ; i++)
    {
        if(inventario[i] == NULL)
            return i;

        if (strcmp(inventario[i]->producto->codigoProducto, codigoBuscado) == 0)
            return i;
    }
    return -1;
}

//--------------------------------------------AGREGAR INVENTARIO------------------------------------------------------//
//      Primero llama a "buscarInventario" para corroborar si el item existe en el inventario,                        //
//      si existe le suma su stock, si no, se agrega al inventario. Retorna 1 si se agrego exitosamente               //
//      retorna 0 si no.                                                                                              //
//--------------------------------------------------------------------------------------------------------------------//

int agregarItem (struct Bodega *inventario[max], char *codigoProducto, int cant, struct Producto *producto){

    int posicion = buscarInventario(inventario,codigoProducto);

    if(posicion == -1) return 0;

    if(inventario[posicion] != NULL)
        inventario[posicion]->stock += cant;

    else{
        inventario[posicion] = (struct Bodega*) malloc(sizeof(struct Bodega));
        inventario[posicion]->producto = producto;
        inventario[posicion]->stock = cant;
    }
    return 1;
}

//--------------------------------------------LLER DATOS PRODUCTO-----------------------------------------------------//
//      Funcion en la que se ingresan datos para un producto nuevo.                                                   //
//--------------------------------------------------------------------------------------------------------------------//

void leerDatosProductoNuevo(char *modelo, char *marca, int *precio)
{
    printf("Ingrese el modelo\n");
    scanf("%[^\n]s", modelo);
    getchar();

    printf("Ingrese la marca\n");
    scanf("%[^\n]s",marca);
    getchar();

    printf("Ingrese el precio\n");
    scanf("%d", precio);
    getchar();
}

//------------------------------------------ESTABLECER INVENTARIO-----------------------------------------------------//
//      Esta funcion verifica que el producto exista llamando a "buscarProducto", si no lo encuentra, le              //
//      establece datos llamando a "leerDatosProductoNuevo" y establece el producto en la lista productos,            //
//      finalmente agrega el producto al inventario. Si se establece exitosamente, retorna 1, si no 0.                //
//--------------------------------------------------------------------------------------------------------------------//

int establecerItem(struct Bodega *inventario[max], char *codigoProducto, char *tipo, int cant, struct NodoProducto **head) {

    struct Producto *productoBuscado ;
    struct NodoProducto *nodoBuscado = buscarProducto(*head, codigoProducto);
    if(nodoBuscado != NULL){
        productoBuscado = nodoBuscado->datosProducto;

    }
    else{
        char modelo[maxbuffer];
        char marca[maxbuffer];
        int precio;
        leerDatosProductoNuevo(modelo, marca, &precio);

        productoBuscado = establecerProducto(head, codigoProducto, tipo, modelo, marca, precio);
    }
    if (agregarItem(inventario, codigoProducto, cant, productoBuscado) == 1) {

        return 1;
    }
    else{

        return 0;
    }
}

//------------------------------------------COMPACTAR INVENTARIO------------------------------------------------------//
//      Esta funcion itera en el vector de inventario, eliminando espacios vacios.                                    //
//--------------------------------------------------------------------------------------------------------------------//

void compactar(struct Bodega *inventario[max], int pos){
    int i;

    for (i = pos; i+1 < max; i++){

        inventario[i] = inventario[i+1];

        if(inventario[i] == NULL)
            return;
    }
}

//----------------------------------------ELIMINAR ITEM INVENTARIO----------------------------------------------------//
//      Se itera el inventario hasta encontrar el codigo del producto a eliminar, si lo encuentra lo retira           //
//      del vector de inventario y retorna 1. Retorna 0 si no esta en el inventario.                                  //
//--------------------------------------------------------------------------------------------------------------------//

int eliminarItem(struct Bodega *inventario[max], char *codigoBuscado){
    int i;

    for (i = 0; i < max; i++) {
        if (inventario[i] != NULL) {
            if (strcmp(inventario[i]->producto->codigoProducto, codigoBuscado) == 0) {
                inventario[i] = NULL;
                compactar(inventario, i);
                return 1;
            }
        }
        else
            return 0;
    }
    return 0;
}

//--------------------------------------------MOSTRAR INVENTARIO------------------------------------------------------//
//      Itera el inventario para mostrar todos los productos dentro, si no hay productos se muestra que el            //
//      inventario esta vacio.                                                                                        //
//--------------------------------------------------------------------------------------------------------------------//

void mostrarInventario (struct Bodega *inventario[max])
{
    int i;
    if(inventario[0] != NULL) {
        printf("Inventario:\n");
        printf("          Modelo      Stock     Codigo\n");

        for (i = 0; i < max && inventario[i] != NULL; i++) {
            printf("%15s ", inventario[i]->producto->modelo);
            printf("%10d ", inventario[i]->stock);
            printf("%10s\n", inventario[i]->producto->codigoProducto);
        }
    }
    else{
        printf("Inventario vacio\n");
    }
}

//------------------------------------------MODIFICAR INVENTARIO------------------------------------------------------//
//      Esta funcion modifica el stock de un producto en el inventario, restando segun la cantidad de ventas.         //
//--------------------------------------------------------------------------------------------------------------------//

int modificarInventario(struct Bodega * item, int cantVendida){
    if(item->stock < cantVendida) return 0;

    item->stock -=cantVendida;
    return 1;
}

//---------------------------------------------- MENU ----------------------------------------------------------------//

//-------------------------------------------VENDER PRODUCTO----------------------------------------------------------//
//      Primero verifica si existen; la sede, el producto y si este ultimo esta en la sede, luego,                    //
//      se ve si hay stock. Finalmente llama a las funciones "modificarInventario" y "establecerVenta"                //
//      para reducir el stock y añadir la venta a la lista, respectivamente. Retorna 1 si la venta se                 //
//        hizo con exito, 0 si no.                                                                                    //
//--------------------------------------------------------------------------------------------------------------------//

int venderProducto(struct Sede*sede,struct NodoProducto*listaProductos , char *codigoProducto, char *rut, char *fecha,int cantVendida, int *codigoVentas){
    //si la sede existe
    if(sede == NULL) return 0;

    struct NodoProducto * nodoBuscado = buscarProducto(listaProductos, codigoProducto);
    //si el producto existe
    if(nodoBuscado== NULL) return 0;

    int posicion = buscarInventario(sede->inventario,codigoProducto);

    if(posicion == -1) return 0;
    //si el producto esta en esta sede
    if(sede->inventario[posicion] != NULL){
        //si hay stock
        if(modificarInventario(sede->inventario[posicion], cantVendida)== 0) return 0;

        if(establecerVenta(&(sede->ventas),nodoBuscado->datosProducto,rut,fecha,cantVendida, codigoVentas) == 0)
            return 0;
    }
    return 1;
}

//-------------------------------------------OPCIONES INVENTARIO------------------------------------------------------//
//      Esta funcion llama a diversas funciones que tratan con el inventario, como "eliminarItem",                    //
//      "buscarItem" y "mostrarInventario". El usuario debe colocar una opcion valida para llamar                     //
//      a una de estas funciones, si la opcion no es valida, se muestra un mensaje advirtiendolo                      //
//      se termina la funcion.                                                                                        //
//--------------------------------------------------------------------------------------------------------------------//

void opcionesInventario(struct NodoSede *nodoSede)
{
    int numero;
    printf("Que quiere hacer? \n1:Eliminar item \n2:Buscar item  \n3:Mostrar inventario \n");
    scanf("%d",&numero); getchar();

    if(numero == 1)
    {
        printf("Ingrese el codigo del item\n");
        char codigo[maxbuffer];
        scanf("%[^\n]s",codigo); getchar();
        if(eliminarItem(nodoSede->datosSede->inventario, codigo) == 1)
            printf("El item se elimino con exito\n");
        else
            printf("Item no encontrado\n");
    }

    else if(numero == 2)
    {
        printf("Ingrese el codigo del item\n");
        char codigo[maxbuffer];
        scanf("%[^\n]s",codigo); getchar();

        int posicion = buscarInventario(nodoSede->datosSede->inventario, codigo);
        if(posicion != -1 && nodoSede->datosSede->inventario[posicion] != NULL)
        {
            printf("Modelo: %s, ", nodoSede->datosSede->inventario[posicion]->producto->modelo);
            printf("Codigo: %s, ",nodoSede->datosSede->inventario[posicion]->producto->codigoProducto);
            printf("Stock: %d, ",nodoSede->datosSede->inventario[posicion]->stock);
            printf("Precio: %d.\n",nodoSede->datosSede->inventario[posicion]->producto->precio);
        }

        else
            printf("El item no existe\n");
    }

    else if(numero == 3)
        mostrarInventario(nodoSede->datosSede->inventario);

    else
        printf("Opcion no valida\n");
}

//---------------------------------------------OPCIONES VENTA---------------------------------------------------------//
//      Esta funcion llama a diversas funciones que tratan con el inventario, como "modificarVenta",                  //
//      "buscarVenta" y "mostrarVenta". El usuario debe colocar una opcion valida para llamar                         //
//      a una de estas funciones, si la opcion no es valida, se muestra un mensaje advirtiendolo                      //
//      se termina la funcion.                                                                                        //
//--------------------------------------------------------------------------------------------------------------------//

void opcionesVentas(struct NodoSede *nodoSede, struct NodoProducto * listaProductos, int *codigoVentas ) {

    int numero;
    int codigoEliminar;
    char codigoProducto[maxbuffer];
    int stockRecuperado= 0;

    printf("Que quiere hacer? \n1:Modificar venta (reingresar la venta) \n2:Buscar venta  \n3:Mostrar ventas \n\n");
    scanf("%d", &numero);
    getchar();
    if (numero == 1){
        printf("Ingrese el codigo de la venta a reingresar\n");

        scanf("%d", &codigoEliminar);
        getchar();
        struct NodoVenta* nodoBuscado = buscarVenta((nodoSede->datosSede->ventas), codigoEliminar);
        if(nodoBuscado!= NULL) {

            stockRecuperado = nodoBuscado->datosVenta->cantidadVendida;
            strcpy(codigoProducto, nodoBuscado->datosVenta->productoVendido->codigoProducto);
        }

        if (eliminarVenta(&(nodoSede->datosSede->ventas), codigoEliminar) == 1) {
            int pos = buscarInventario(nodoSede->datosSede->inventario, codigoProducto);
            if(pos != -1 && nodoSede->datosSede->inventario[pos]!= NULL)
                if (modificarInventario(nodoSede->datosSede->inventario[pos], -stockRecuperado)==1) {

                    int cantidad;
                    char rutCliente[maxbuffer];
                    char fecha[maxbuffer];


                    printf("Ingrese cantidad vendida\n");
                    scanf("%d", &cantidad);
                    getchar();
                    printf("Ingrese el rut del cliente\n");
                    scanf("%[^\n]s", rutCliente);
                    getchar();
                    printf("Ingrese fecha\n");
                    scanf("%[^\n]s", fecha);
                    getchar();

                    if (venderProducto(nodoSede->datosSede, listaProductos, codigoProducto, rutCliente, fecha, cantidad,
                                       codigoVentas) == 1) {
                        printf("La venta se ha registrado con exito\n");
                        printf("Se ha generado un nuevo codigo de venta \n");
                    } else {
                        printf("No se pudo efectuar la venta\n");
                    }
                }
        }
        else
            printf("Region no existente\n");
    }

    else if(numero == 2)
    {
        int codigoVenta;
        char *rutBuscado;
        printf("Ingrese el codigo de la venta\n");
        scanf("%d",&codigoVenta); getchar();

        struct NodoVenta* nodoVenta = buscarVenta(nodoSede->datosSede->ventas, codigoVenta);


        if(nodoVenta != NULL)
            printf("%s  $%d  %s", nodoVenta->datosVenta->fecha, nodoVenta->datosVenta->montoTotal,nodoVenta->datosVenta->productoVendido->modelo);

        else
            printf("No hay ventas con ese rut\n");
    }

    else if(numero == 3)
    {
        mostrarVentas(nodoSede->datosSede->ventas);

    }

    else
        printf("Opcion no valida\n");
}

//--------------------------------------------NAVEGAR SEDES-----------------------------------------------------------//
//      Esta funcion permite navegar sobre opciones dentro de las sedes, como el inventario y las                     //
//      ventas de las mismas. El usuario debe colocar una opcion valida para llamar                                   //
//      a una de estas funciones, si la opcion no es valida, se muestra un mensaje advirtiendolo                      //
//      se termina la funcion.                                                                                        //
//--------------------------------------------------------------------------------------------------------------------//

void navegarSede(struct NodoSede *nodoSede,struct NodoProducto * listaProductos, int *codigoVentas){

    while(true){
        int numero;
        printf("Sobre que quiere trabajar? \n1: Inventario \n2: Registro de ventas \n\n");
        scanf("%d",&numero); getchar();

        if(numero == 1)
            opcionesInventario(nodoSede);

        else if(numero == 2)
            opcionesVentas(nodoSede,listaProductos, codigoVentas);

        else
            printf("Opcion no valida\n");

        int continuar = 1;
        while(continuar == 1){

            printf("\nDesea hacer otra operacion sobre las sedes de esta region?  \n1: Si \n0: No \n\n");
            scanf("%d", &continuar);
            getchar();

            if(continuar == 1) continuar = 0;

            else if(continuar == 0) return;

            else{
                continuar = 1;
                printf("Numero incorrecto\n");
            }
        }
    }

}

//--------------------------------------------NAVEGAR REGION----------------------------------------------------------//
//      Esta funcion permite navegar sobre opciones dentro de las regiones, como diversas modificaciones al           //
//      arbol de las sedes. El usuario debe colocar una opcion valida para llamar a una de estas funciones,           //
//      si la opcion no es valida, se muestra un mensaje advirtiendolo se termina la funcion.                         //
//--------------------------------------------------------------------------------------------------------------------//

void navegarRegion(struct NodoRegion *nodoRegion, struct NodoProducto *listaProductos, int *codigoVentas)
{


    while(true)
    {
        int numero;
        char sedeBuscada[maxbuffer];

        printf("Que desea hacer?\n 1: Agregar Sede\n 2: Eliminar Sede\n 3: Buscar Informacion\n");
        printf(" 4: Modificar Sede\n 5: Mostrar Sedes\n 6: Navegar Sede\n");
        scanf("%d",&numero);
        getchar();

        if(numero == 1){
            char comuna[maxbuffer];
            int tiempo;
            printf("Ingrese el nombre de la sede\n");
            scanf("%[^\n]s",sedeBuscada);
            getchar();

            printf("Ingrese la comuna de la sede\n");
            scanf("%[^\n]s",comuna);
            getchar();

            printf("Ingrese el tiempo de desplazamiento en dias\n");
            scanf("%d",&tiempo);
            getchar();

            if(establecerSede(&(nodoRegion->region->sedes), sedeBuscada, tiempo, comuna) == 1)
                printf("Sede agregada con exito\n");
            else
                printf("Sede ya existente\n");
        }
        else if(numero == 2){
            printf("Ingrese el nombre de la sede\n");
            scanf("%[^\n]s",sedeBuscada);
            getchar();

            if(eliminarSede(&(nodoRegion->region->sedes), sedeBuscada) == 1)
                printf("Sede eliminada con exito\n");
            else
                printf("Sede no existente\n");
        }
        else if(numero == 3){
            printf("Ingrese el nombre de la sede\n");
            scanf("%[^\n]s",sedeBuscada);
            getchar();

            struct NodoSede *previa = NULL;
            struct NodoSede *nodoSede = buscarSede(nodoRegion->region->sedes, &previa, sedeBuscada);

            if(nodoSede != NULL)
                printf("-Comuna: %s, nombre: %s, tiempo de desplazamiento: %d\n",nodoSede->datosSede->comuna,nodoSede->datosSede->nombreSede,nodoSede->datosSede->tiempoTransporte);

            else
                printf("No se encontro la sede\n");
        }
        else if(numero == 4){
            printf("Ingrese el nombre de la sede\n");
            scanf("%[^\n]s",sedeBuscada);
            getchar();

            char nuevoNombre[maxbuffer];
            printf("Ingrese nuevo nombre\n");
            scanf("%[^\n]s",nuevoNombre);
            getchar();

            if(modificarNombreSede(nodoRegion->region->sedes, sedeBuscada, nuevoNombre) == 1)
                printf("Sede modificada con exito\n");
            else
                printf("No se pudo modificar la sede\n");
        }
        else if(numero == 5){
            if((nodoRegion->region->sedes) != NULL){
                printf("Lista de todas las sedes:\n");
                mostrarSedes(nodoRegion->region->sedes);
            }
            else
                printf("No se pudo mostrar la sede");
        }
        else if(numero == 6){
            printf("Ingrese el nombre de la sede\n");
            scanf("%[^\n]s",sedeBuscada);
            getchar();

            struct NodoSede *previa = NULL;
            struct NodoSede *nodoSede = buscarSede(nodoRegion->region->sedes, &previa, sedeBuscada);
            if (nodoSede != NULL)
                navegarSede(nodoSede, listaProductos, codigoVentas);
            else
                printf("La sede no existe\n");
        }
        else
            printf("Opcion no valida\n");

        int continuar = 1;
        while(continuar == 1){

            printf("\nDesea hacer otra operacion sobre las sedes de esta region?  \n1: Si \n0: No \n\n");
            scanf("%d", &continuar);
            getchar();

            if(continuar == 1) continuar = 0;

            else if(continuar == 0) return;

            else{
                continuar = 1;
                printf("Numero incorrecto\n");
            }
        }
    }
}

//--------------------------------------------MANEJAR REGION----------------------------------------------------------//
//      Esta funcion llama a distintas funciones que manejan a las regiones, es llamada en el main.                   //
//      Dependiendo de si se cumplen o no las funciones exitosamente, se muestra un mensaje u otro.                   //
//--------------------------------------------------------------------------------------------------------------------//

void manejarRegion(struct NodoRegion **listaRegiones, int opcion, struct NodoProducto * listaProductos, int *codigoVentas){
    char nombre[maxbuffer];
    if(opcion ==1){

        printf("Ingrese nombre de la region\n");
        scanf("%[^\n]s",nombre);
        getchar();
        if(establecerRegion(listaRegiones, nombre)==1){
            printf("Agregada con exito\n");

        }
        else
            printf("Region ya existente\n");
    }

    else if(opcion == 2){

        printf("Ingrese nombre de la region\n");
        scanf("%[^\n]s",nombre);
        getchar();

        if(eliminarRegion(listaRegiones, nombre)==1){
            printf("Eliminada con exito\n");
        }
        else
            printf("Region no existente\n");
    }

    else if(opcion ==3){
        mostrarRegiones(*listaRegiones);
    }

    else if(opcion == 4){
        printf("Ingrese nombre de la region\n");
        scanf("%[^\n]s", nombre);
        getchar();

        printf("Ingrese nuevo nombre para la region\n");
        char nuevoNombre[maxbuffer];
        scanf("%[^\n]s", nuevoNombre); getchar();

        if(modificarRegion(*listaRegiones, nombre, nuevoNombre ) == 1) {
            printf("Se ha modificado el nombre con exito\n");
        }
        else
            printf("Region no existente\n");
    }

    else if(opcion == 5) {
        printf("Ingrese nombre de la region\n");
        scanf("%[^\n]s", nombre);
        getchar();

        struct NodoRegion*nodo = buscarRegion(*listaRegiones, nombre);
        if(nodo!= NULL) {
            navegarRegion(nodo, listaProductos, codigoVentas);
        }
        else
            printf("Region no existente\n");
    }

    else{
        printf("OPCION NO VALIDA\n");
    }

}

//--------------------------------------------MANEJAR PRODUCTOS-------------------------------------------------------//
//      Esta funcion llama a distintas funciones que manejan a las productos, es llamada en el main.                  //
//      Dependiendo de si se cumplen o no las funciones exitosamente, se muestra un mensaje u otro.                   //
//--------------------------------------------------------------------------------------------------------------------//

void manejarProducto (struct NodoProducto *productos, int opcion){

    if (opcion == 1) {
        // BUSCAR PRODUCTO//
        printf("Ingrese el codigo del producto\n");
        char codigoProducto[25];
        scanf("%[^\n]s", codigoProducto);
        getchar();

        if (buscarProducto(productos, codigoProducto) != NULL) {
            printf("Producto Encontrado\n");
            printf("%s,%s,%d \n",productos->datosProducto->modelo, productos->datosProducto->marca, productos->datosProducto->precio);

        }
        else {
            printf("Producto No Encontrado\n");
        }

    }
    else if (opcion == 2) {
        // MOSTRAR PRODUCTO //
        mostrarProductos(productos);
    }

    else if (opcion == 3) {
        printf("Ingrese el codigo del producto\n");

        char codigoProducto[25];
        scanf("%[^\n]s", codigoProducto);
        getchar();

        printf("Los campos del producto son:\n");
        printf("1: Nombre \n2: Marca \n3: Precio \n");
        int opcionCampo;
        scanf("%d", &opcionCampo);
        getchar();
        if(opcionCampo == 1){
            printf("Ingrese nuevo nombre\n");
        }
        else if(opcionCampo == 2){
            printf("Ingrese nueva marca\n");
        }
        else if(opcionCampo == 3){
            printf("Ingrese nuevo precio\n");
        }

        else{
            printf("Opcion no válida \n");
        }

        char nuevo[40];
        scanf("%[^\n]s", nuevo);
        getchar();

        if(modificarProducto(productos, codigoProducto, opcionCampo, nuevo) == 1)
        {
            printf("Se modifico el producto con exito\n");
        }
        else {
            printf("No existe el producto\n");
        }

    }
    else
    {
        printf("OPCION NO VALIDA\n");

    }
}

//----------------------------------------MANEJAR AGREGAR PRODUCTOS---------------------------------------------------//
//      Esta funcion llamara a "establecerProducto" siempre y cuando se escriba la region y sede donde                //
//      ira, se asegura que existan estas ultimas. Se especifica que tipo de producto es (auto, accesorio             //
//      o repuesto)                                                                                                   //
//--------------------------------------------------------------------------------------------------------------------//


void manejarAgregarProducto(struct NodoRegion *regiones, struct NodoProducto **productos)
{
    char region[maxbuffer];
    printf("Ingrese nombre de la region\n");
    scanf("%[^\n]s", region);
    getchar();
    struct NodoRegion *nodoRegion = buscarRegion(regiones, region);

    if (nodoRegion != NULL) {
        char sede[maxbuffer];
        printf("Ingrese nombre de la sede\n");
        scanf("%[^\n]s", sede);
        getchar();
        struct NodoSede *previa = NULL;
        struct NodoSede *nodoSede = buscarSede(nodoRegion->region->sedes, &previa, sede);

        if (nodoSede != NULL)
        {
            char tipo[maxbuffer];
            char codigoProducto[maxbuffer];
            int opcion;
            int continuar = 1;
            while(continuar == 1)
            {
                printf("Seleccione el tipo de producto\n 1: Auto\n 2: Repuesto \n3: Accesorio\n\n");
                scanf("%d", &opcion); getchar();

                if (opcion == 1)
                    strcpy(tipo, "Auto");

                else if (opcion == 2)
                    strcpy(tipo, "Repuesto");

                else if (opcion == 3)
                    strcpy(tipo, "Accesorio");

                else
                    printf("Opcion no valida\n\n");

                if(opcion > 0 && opcion < 4) continuar = 0;
            }

            printf("Ingrese el codigo del producto\n");
            scanf("%[^\n]s", codigoProducto);
            getchar();

            int cant;
            printf("Ingrese la cantidad\n");
            scanf("%d", &cant);
            getchar();

            if (establecerItem(nodoSede->datosSede->inventario, codigoProducto, tipo, cant,productos) == 1) {
                printf("Distribucion del producto exitosa\n");
            } else
                printf("No se pudo efectuar la distribucion \n");
        }
        else
            printf("Sede no existente\n");
    }
    else
        printf("La region no existe\n");
}

//-----------------------------------------MANEJAR VENDER PRODUCTOS---------------------------------------------------//
//      Esta funcion maneja las ventas de un producto, para que se venda exitosamente se tiene que                    //
//      ingresar la region y sede del producto, ademas de sus datos. Si la ubicacion no existe, se                    //
//      mostrará.                                                                                                     //
//--------------------------------------------------------------------------------------------------------------------//

void manejarVenderProducto(struct NodoRegion *listaRegiones, struct NodoProducto *listaProductos, int *codigoVentas)
{

    char region[maxbuffer];
    printf("Ingrese nombre de la region\n");
    scanf("%[^\n]s", region);
    getchar();
    struct NodoRegion *nodoRegion = buscarRegion(listaRegiones, region);

    if (nodoRegion != NULL) {
        char sede[maxbuffer];
        printf("Ingrese nombre de la sede\n");
        scanf("%[^\n]s", sede);
        getchar();
        struct NodoSede *previa = NULL;
        struct NodoSede *nodoSede = buscarSede(nodoRegion->region->sedes, &previa, sede);

        if (nodoSede != NULL)
        {
            char codigoProducto[maxbuffer];
            char rut[maxbuffer];
            char fecha[maxbuffer];
            int cantVendida;

            printf("Ingrese el codigo del producto a vender\n");
            scanf("%[^\n]s", codigoProducto); getchar();

            printf("Ingrese el rut del cliente\n");
            scanf("%[^\n]s",rut); getchar();

            printf("Ingrese la cantidad a vender\n");
            scanf("%d",&cantVendida); getchar();

            printf("Ingrese la fecha\n");
            scanf("%[^\n]s",fecha);  getchar();

            if (venderProducto(nodoSede->datosSede, listaProductos, codigoProducto, rut, fecha, cantVendida, codigoVentas) == 1)
                printf("\nLa venta se ha realizado exitosamente\n");

            else
                printf("No se pudo efectuar la venta\n");
        }
        else
            printf("Sede no existente\n");
    }
    else
        printf("La region no existe\n");

}

//---------------------------------------------FUNCIONES EXTRA--------------------------------------------------------//

//----------------------------------------------MEJOR CLIENTE---------------------------------------------------------//
//      Las siguientes tres funciones se encargan calcular, analizar y mostrar al cliente que mas gastó               //
//      dinero en una sede, para ello se verifica que la sede y de la misma region existan, luego se                  //
//      itera dos veces, una para almacenar el rut de un cliente y la otra para ver cuantas veces el                  //
//      cliente compra y asi calcular la suma total de su gasto, finalmente se compara cada cliente                   //
//      hasta que se identifique el que mas gastó y asi retornar su rut.                                              //
//--------------------------------------------------------------------------------------------------------------------//
int calcularMontoTotal(struct NodoVenta *head, struct NodoVenta *rec, char *rutCliente)
{
    struct NodoVenta *ptr = rec;

    int monto = 0;
    do
    {
        if (ptr == rec)
            monto += ptr->datosVenta->montoTotal;

        else if (strcmp(ptr->datosVenta->rutCliente, rutCliente) == 0)
            monto += ptr->datosVenta->montoTotal;

        ptr = ptr->sig;
    } while (ptr != head);

    return monto;
}

char *mejorClientePorSede(struct Sede *sede, int *masVentas) {
    if(sede == NULL || sede->ventas == NULL) return NULL;

    char *rutMejorCliente = (char*) malloc(maxbuffer * sizeof(char));

    struct NodoVenta *rec = sede->ventas;

    int  monto;
    do
    {
        monto = calcularMontoTotal(sede->ventas, rec, rec->datosVenta->rutCliente);

        if (monto > (*masVentas))
        {
            (*masVentas) = monto;
            strcpy(rutMejorCliente, rec->datosVenta->rutCliente);
        }
        rec = rec->sig;
    } while (rec != sede->ventas);

    return rutMejorCliente;
}

void manejarMejorCliente(struct NodoRegion *head)
{
    char region[maxbuffer];
    printf("Ingrese nombre de la region\n");
    scanf("%[^\n]s", region);
    getchar();
    struct NodoRegion *nodoRegion = buscarRegion(head, region);

    if (nodoRegion != NULL) {
        char sede[maxbuffer];
        printf("Ingrese nombre de la sede\n");
        scanf("%[^\n]s", sede);
        getchar();
        struct NodoSede *previa = NULL;
        struct NodoSede *nodoSede = buscarSede(nodoRegion->region->sedes, &previa, sede);

        if (nodoSede != NULL) {
            int montoMasVentas = 0;
            char *rutMejorCliente = (char*) malloc(maxbuffer *sizeof(char));

            strcpy(rutMejorCliente,(mejorClientePorSede(nodoSede->datosSede, &montoMasVentas)) );

            if(rutMejorCliente != NULL){
                printf("El rut del mejor cliente es %s, ", rutMejorCliente);
                printf("con un gasto total de $%d\n", montoMasVentas);
            }
            else{
                printf("NO SE HAN REGISTRADO VENTAS\n");
            }
        }
        else
            printf("Sede no existente\n");
    }
    else
        printf("La region no existe\n");

}

//---------------------------------------------AUTOS VENDIDOS---------------------------------------------------------//
//      En las siguientes dos funciones se verifica y muestran los autos vendidos en una sede, iterando               //
//      en la lista de ventas y productos.                                                                            //
//--------------------------------------------------------------------------------------------------------------------//

int seVende(struct NodoVenta *head, struct Producto *productoBuscado)
{
    int contVendidos = 0;
    if (head != NULL)
    {
        struct NodoVenta *rec = head;
        do
        {
            if (rec -> datosVenta -> productoVendido == productoBuscado)
            {
                contVendidos++;
                return contVendidos;
            }
            rec = rec -> sig;

        }while (rec != head);
    }
    return contVendidos;
}

void mostrarAutosVendidosEnSede(struct Sede *sede, struct NodoProducto *headProducto )
{
    struct NodoProducto *rec = headProducto;
    if(headProducto == NULL){
        printf("No hay productos\n");
        return;
    }

    int cont = 0;
    do
    {
        if(strcmp(rec->datosProducto->tipo, "Auto" ) == 0)
        {

            int esta = seVende(sede->ventas, rec->datosProducto);
            if(esta == 1)
            {
                if(cont == 0 )
                {
                    printf("Lista de productos: \n");
                    printf("    Modelo     Precio               Marca   Codigo Producto\n");

                }
                printf("-%10s ",rec->datosProducto->modelo);
                printf("%10d Pesos",rec->datosProducto->precio);
                printf("%15s",rec->datosProducto->marca);
                printf("%15s\n", rec->datosProducto->codigoProducto);
                cont++;
            }
        }
        rec = rec->sig;
    }while(rec != headProducto);

    if(cont == 0) printf("NO SE ENCONTRARON AUTOS VENDIDOS");
}

//-----------------------------------------SEDE MAS DISTRIBUICION-----------------------------------------------------//
//      En las siguientes cuatro funciones se recorre las regiones, sedes y ventas, buscando y calculando             //
//      cual es la sede con mayor cantidad de ventas                                                                  //
//--------------------------------------------------------------------------------------------------------------------//

void contarVentas(struct NodoVenta *headVenta, int *cantVentas){
    if(headVenta == NULL) return;


    struct NodoVenta *rec = headVenta;

    do {
        (*cantVentas)++;
        rec = rec->sig;

    }while(rec != headVenta);
}

void determinarMejorSede(struct NodoSede *sede, struct Sede **sedeMasVentas, int *masVentas)
{
    int cantVentas = 0;


    contarVentas(sede->datosSede->ventas, &cantVentas);

    if(cantVentas > (*masVentas))
    {
        (*masVentas) = cantVentas;

        if(sedeMasVentas == NULL)
            (*sedeMasVentas) = ((struct Sede *) malloc(sizeof(struct Sede)));

        (*sedeMasVentas) = sede->datosSede;

    }

}

void buscarSedeMasVentas(struct NodoSede *headSede, struct Sede **sedeMasVentas, int *masVentas)
{
    if(headSede != NULL)
    {
        determinarMejorSede(headSede, sedeMasVentas, masVentas);
        buscarSedeMasVentas(headSede->izq, sedeMasVentas, masVentas);
        buscarSedeMasVentas(headSede->der, sedeMasVentas, masVentas);
    }
}

struct Sede *sedeMasVentas(struct NodoRegion *headRegion, int *masVentas)
{
    if(headRegion == NULL) return NULL;

    struct Sede *sedeMasVentas = NULL;
    struct NodoRegion *rec = headRegion->sig;


    while (rec != NULL)
    {
        buscarSedeMasVentas(rec->region->sedes, &sedeMasVentas, masVentas);
        rec = rec->sig;
    }

    return sedeMasVentas;
}

//---------------------------------------------AUTO MAS RECALLS-------------------------------------------------------//
//      Esta funcion sirve para calcular el auto con mas recalls, recorriendo la lista de ventas y                    //
//      comparando los recalls de cada auto. Retorna el auto con mas recalls.                                         //
//--------------------------------------------------------------------------------------------------------------------//

struct Producto *autoConMasRecalls(struct NodoSede *sede){
    if(sede == NULL || sede->datosSede->ventas == NULL) return NULL;

    struct Producto *autoMasRecall = NULL;
    struct NodoVenta *rec = sede->datosSede->ventas;
    int contRecalls = 0;
    do
    {
        if(strcmp(rec->datosVenta->productoVendido->tipo, "Auto") == 0){
            if (rec->datosVenta->productoVendido->recalls > contRecalls){
                contRecalls = rec->datosVenta->productoVendido->recalls;
                autoMasRecall = rec->datosVenta->productoVendido;
            }
        }
        rec = rec->sig;

    } while (rec != sede->datosSede->ventas);

    return autoMasRecall;
}

//--------------------------------------------LLAMAR RECALLS---------------------------------------------------------//
//      Esta funcion añade recalls a un auto.                                                                         //
//--------------------------------------------------------------------------------------------------------------------//

int llamarRecalls(struct NodoProducto *head, struct Sede *sede, char* codigoProducto){
    struct NodoProducto * productoBuscado = buscarProducto(head, codigoProducto);
    if(productoBuscado!= NULL){
        (productoBuscado->datosProducto->recalls)++;
        return 1;
    }
    return 0;
}

//----------------------------------------------MEJOR MARCA-----------------------------------------------------------//
//      En las siguientes dos funciones se calcula y retorna la marca que mas fue vendida en una sede.                //
//      Para calcularla se itera en la lista de ventas y se compara las ventas de cada marca, la marca                //
//      mas vendida es retornada, si no hay se retorna nulo.                                                          //
//--------------------------------------------------------------------------------------------------------------------//

int calcularMarcaVendida(struct NodoVenta *headVenta, struct NodoVenta *venta, char *marca)
{
    struct NodoVenta *ptr = venta;
    int cant = 0;
    do {

        if(ptr == venta)
            cant++;

        else if(strcmp(marca, ptr->datosVenta->productoVendido->marca) == 0)
            cant++;

        ptr = ptr->sig;

    }while(ptr != headVenta);

    return cant;
}

char *marcaMasVendidaSede(struct NodoVenta *headVenta)
{
    if (headVenta == NULL) return NULL;

    char *marcaMasVendida = (char *) malloc(maxbuffer * sizeof(char));
    struct NodoVenta *rec = headVenta;
    int cantMarcaVendida, mejorMarca = 0;

    do {
        cantMarcaVendida = calcularMarcaVendida(headVenta, rec, rec->datosVenta->productoVendido->marca);

        if (cantMarcaVendida > mejorMarca) {
            mejorMarca = cantMarcaVendida;
            strcpy(marcaMasVendida, rec->datosVenta->productoVendido->marca);
        }
        rec = rec->sig;
    } while (rec != headVenta);

    return marcaMasVendida;
}

//------------------------------------------SACAR DE CIRCULACION------------------------------------------------------//
//      En las siguientes funciones se recorre las sedes, productos e inventario y elimina el                         //
//      producto buscado del inventario y la lista de productos, si se elimina correctamente                          //
//      retorna 1, si no, retorna 0.                                                                                  //
//--------------------------------------------------------------------------------------------------------------------//


int eliminarProducto (struct NodoProducto **head, char *codigoProducto)
{
    if((*head) != NULL)
    {
        struct NodoProducto *rec = (*head);
        do
        {
            if(strcmp(rec->datosProducto->codigoProducto,codigoProducto) == 0)
            {
                if(rec == (*head))
                {
                    if(rec== (*head)->sig){
                        (*head)= NULL;
                        return 1;
                    }
                    (*head) = (*head)->sig;
                }
                rec->ant->sig = rec->sig;
                rec->sig->ant = rec->ant;
                rec = NULL;
                return 1;
            }
            rec = rec->sig;
        }while(rec != *head);
    }
    return 0;
}

void eliminarEnSedes(struct NodoSede *headSede, char *codigo)
{
    if(headSede != NULL)
    {
        int num = (eliminarItem(headSede->datosSede->inventario, codigo));
        eliminarEnSedes(headSede->izq, codigo);
        eliminarEnSedes(headSede->der, codigo);
    }
}

int sacarDeCiculacion (struct NodoRegion *headR, struct NodoProducto **headP, char *codigo)
{
    int seElimino = 0;


    if(headR != NULL)
    {
        struct NodoRegion *rec = headR -> sig;

        if(eliminarProducto(headP, codigo) == 1)
        {
            seElimino = 1;

            while (rec != NULL) {
                eliminarEnSedes(rec->region->sedes, codigo);
                rec = rec->sig;
            }
        }
    }
    return seElimino;
}

//--------------------------------------------------MAIN--------------------------------------------------------------//
//      En la funcion principal se muestran las distintas opciones para que el usuario trabaje con el programa        //
//      se leen las opciones, se llaman las funciones correspondientes y maneja el programa en general.               //
//--------------------------------------------------------------------------------------------------------------------//

int main(){

    struct Derco *derco = (struct Derco *) malloc(sizeof(struct Derco));
    derco->regiones = NULL;
    derco->productos = NULL;
    int codigoVentas = 10000001;

    printf("\nBienvenido al menu Derco \n");

    while (true)
    {
        int numero;
        printf("Sobre que quiere trabajar?\n1: Regiones \n2: Productos \n3: Agregar / Vender / Reparar productos \n");
        printf("4: Otras opciones\n\n");

        scanf("%d", &numero); getchar();

        if (numero == 1) {
            int opcion;
            printf("1: Agregar region \n2: Eliminar region \n3: Listar regiones \n4: Modificar region \n5: Navegar region\n\n");

            scanf("%d", &opcion); getchar();

            manejarRegion(&(derco->regiones), opcion, derco->productos, &codigoVentas);
        }
        else if (numero == 2) {
            int opcion;
            printf("1: Buscar producto\n2: Lista productos Derco\n3: Modificar producto\n\n");
            scanf("%d", &opcion); getchar();

            manejarProducto(derco->productos, opcion);
        }
        else if (numero == 3) {
            printf("1: Agregar un producto a una sede \n2: Vender productos de una sede \n3: Mandar un producto a reparacion\n");
            scanf("%d", &numero);
            getchar();

            if (numero == 1) {
                manejarAgregarProducto(derco->regiones, &(derco->productos));

            } else if (numero == 2) {
                manejarVenderProducto(derco->regiones, derco->productos, &codigoVentas);

            } else if (numero == 3){
                char region[maxbuffer];
                printf("Ingrese nombre de la region\n");
                scanf("%[^\n]s", region); getchar();

                struct NodoRegion *nodoRegion = buscarRegion(derco->regiones, region);

                if (nodoRegion != NULL) {
                    char sede[maxbuffer];
                    printf("Ingrese nombre de la sede\n");
                    scanf("%[^\n]s", sede); getchar();

                    struct NodoSede *previa = NULL;
                    struct NodoSede *nodoSede = buscarSede(nodoRegion->region->sedes, &previa, sede);

                    if (nodoSede != NULL) {
                        char codigo[maxbuffer];
                        printf("Ingrese codigo del producto\n");
                        scanf("%[^\n]s", codigo); getchar();

                        if(llamarRecalls(derco->productos, nodoSede->datosSede, codigo) == 1){
                            printf("Se ha registrado la reparacion del producto exitosamente\n");
                        }
                        else{
                            printf("No se pudo registrar la reparación del producto\n");
                        }
                    }
                    else
                        printf("Sede no existente\n");
                }
                else
                    printf("La region no existe\n");

            } else
                printf("Opcion no valida\n\n");
        }
        else if (numero == 4){
            int opcion;
            printf("1: Buscar mejor cliente (Sede) \n2: Mostrar los autos vendidos (Sede) \n");
            printf("3: Buscar sede con mas ventas \n4: Mostrar autos con mas recalls \n");
            printf("5: Buscar marca mas vendida (Sede) \n6: Sacar producto de circulacion \n");

            scanf("%d", &opcion); getchar();

            if (opcion == 1)
                manejarMejorCliente(derco->regiones);

            else if (opcion == 2){
                char region[maxbuffer];
                printf("Ingrese nombre de la region\n");
                scanf("%[^\n]s", region); getchar();

                struct NodoRegion *nodoRegion = buscarRegion(derco->regiones, region);
                if (nodoRegion != NULL) {
                    char sede[maxbuffer];
                    printf("Ingrese nombre de la sede\n");
                    scanf("%[^\n]s", sede); getchar();

                    struct NodoSede *previa = NULL;
                    struct NodoSede *nodoSede = buscarSede(nodoRegion->region->sedes, &previa, sede);
                    if (nodoSede != NULL) {
                        mostrarAutosVendidosEnSede(nodoSede->datosSede, derco->productos);
                    }
                    else
                        printf("Sede no existente\n");
                }
                else
                    printf("La region no existe\n");
            }
            else if (opcion == 3) {
                int masVentas = 0;

                struct Sede *sedeMasDis = sedeMasVentas(derco->regiones, &masVentas);

                if (sedeMasDis != NULL) {
                    if (masVentas == 1)
                        printf("La sede con mayor distribucion es %s con %d venta \n", sedeMasDis->nombreSede,
                               masVentas);
                    else

                        printf("La sede con mayor distribucion es %s con %d ventas \n", sedeMasDis->nombreSede,
                               masVentas);
                }
                else
                    printf("No se pudo determinar la sede con mayor distribucion\n");
            }

            else if(opcion == 4){
                char region[maxbuffer];
                printf("Ingrese nombre de la region\n");
                scanf("%[^\n]s", region);
                getchar();
                struct NodoRegion *nodoRegion = buscarRegion(derco->regiones, region);

                if (nodoRegion != NULL) {
                    char sede[maxbuffer];
                    printf("Ingrese nombre de la sede\n");
                    scanf("%[^\n]s", sede);
                    getchar();
                    struct NodoSede *previa = NULL;
                    struct NodoSede *nodoSede = buscarSede(nodoRegion->region->sedes, &previa, sede);

                    struct Producto *masRecalls = autoConMasRecalls(nodoSede);
                    if(masRecalls != NULL){
                        printf("El auto con mas recalls de la sede %s es %s\n", nodoSede->datosSede->nombreSede, masRecalls->modelo);
                    }
                    else{
                        printf("No se pudo determinar el auto con mas recalls");
                    }
                }
                else
                    printf("La region no existe\n");
            }
            else if(opcion == 5){
                char region[maxbuffer];
                printf("Ingrese nombre de la region\n");
                scanf("%[^\n]s", region);
                getchar();
                struct NodoRegion *nodoRegion = buscarRegion(derco->regiones, region);

                if (nodoRegion != NULL){
                    char sede[maxbuffer];
                    printf("Ingrese nombre de la sede\n");
                    scanf("%[^\n]s", sede);
                    getchar();

                    struct NodoSede *previa = NULL;
                    struct NodoSede *nodoSede = buscarSede(nodoRegion->region->sedes, &previa, sede);

                    if(nodoSede!= NULL){
                        char *marcaMasVendida = marcaMasVendidaSede(nodoSede->datosSede->ventas);

                        if(marcaMasVendida != NULL)
                            printf("La marca mas vendida en la sede %s es %s\n", nodoSede->datosSede->nombreSede, marcaMasVendida);

                        else
                            printf("No se pudo determinar la marca mas vendida\n");
                    }
                    else
                        printf("Sede no existente\n");
                }
                else
                    printf("La region no existe\n");
            }
            else if(opcion == 6){
                char codigo[maxbuffer];
                printf("Ingrese codigo del producto\n");
                scanf("%[^\n]s", codigo);
                getchar();

                if(sacarDeCiculacion(derco->regiones, &(derco->productos), codigo )== 1){
                    printf("Producto sacado de circulacion con exito\n");
                }
                else{
                    printf("No se logro eliminar el producto\n");
                }
            }

            else
                printf("Opcion no valida\n\n");
        }
        else
            printf("Opcion no valida\n");

        int continuar = 1;

        while(continuar == 1){
            printf("\nDesea volver al inicio?  \n1: Si \n0: No \n\n");

            scanf("%d", &continuar);
            getchar();

            if(continuar == 1) continuar = 0;

            else if(continuar ==0){
                printf("Muchas gracias.\n");
                return 0;
            }
            else{
                continuar = 1;
                printf("Numero incorrecto\n");
            }
        }
    }
}
