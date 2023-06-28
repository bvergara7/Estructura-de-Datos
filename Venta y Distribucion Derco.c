#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define max  1000
#define maxbuffer 100

struct SedeCentral {
    struct Objeto *objetos [max];
    struct NodoVenta *ventas ;
    struct NodoRegion *regiones, *tail;
};

struct NodoVenta {
    struct Venta *venta;
    struct NodoVenta *sig, *ant;
};

struct Venta {
    char nombre[50];
    char rut[100];
    int codigo;
    char nombreComuna[50];
    int monto;
    int cantVendida ;
    struct Objeto *comprado;

};

struct Objeto {
    char nombre[50];
    char marca[50];
    int tipo;
    int precio;
    int idObjeto;
};

struct NodoRegion {
    struct Region *sedeEnRegion;
    struct NodoRegion *sig,*ant;
};

struct Region {
    int ip;
    char nombreRegion[50];
    struct nodoComuna *comunas;
};

struct NodoComuna {
    struct Comuna *comuna;
    struct NodoComuna *izq, *der;
};

struct Comuna{
    int distanciaTiempoDelaSede;
    char nombre[50];
    struct NodoBodega *bodegas;
};

struct NodoBodega {
    struct Bodega *inventario;
    struct NodoBodega *ant, *sig;
};

struct Bodega {
    int stock ;
    struct Objeto *objeto;
};


////////////////////////////////////////    OBJETOS   //////////////////////////////////////////////////
int buscarObjeto(struct Objeto *objetos[max], int idBuscado)
{
    int i;
    for (i = 0; i < max ; i++)
    {
        if(objetos[i] == NULL)
            return i;

        if (objetos[i]->idObjeto == idBuscado)
            return i;
    }
    return -1;
}

int agregarObjeto (struct Objeto *objetos[max], int codigoProducto, int cant, struct Objeto *producto){

    int posicion = buscarObjeto(objetos,codigoProducto);

    if(posicion == -1 || objetos[posicion] != NULL) return 0;

    objetos[posicion] = producto;
    return 1;
}

void leerDatosObjetoNuevo(struct Objeto *objeto){

    printf("Ingrese el id del objeto\n");
    scanf("%d",&(objeto->idObjeto) );
    getchar();

    printf("Ingrese el nombre del objeto\n");
    scanf("%[^\n]s",objeto -> nombre);
    getchar();

    printf("Ingrese la marca\n");
    scanf("%[^\n]s",objeto->marca);
    getchar();

    printf("Ingrese el tipo: \n1: Auto \n2: Accesorio \n3:Repuesto \n");
    scanf("%d",&(objeto->tipo));
    getchar();

    printf("Ingrese el precio\n");
    scanf("%d", &(objeto->precio));
    getchar();
}

int establecerObjeto(struct Objeto *objetos[max], int codigoProducto) {

    int posicion = buscarObjeto(objetos,codigoProducto);

    if(posicion == -1 || objetos[posicion] != NULL) return 0;

    struct Objeto *objetoBuscado = (struct Objeto *) malloc ( sizeof(struct Objeto));


    leerDatosObjetoNuevo(objetoBuscado);
    objetos[posicion] = objetoBuscado ;

}

void compactar(struct Objeto *objetos[max], int pos){
    int i;

    for (i = pos; i+1 < max; i++){

        objetos[i] = objetos[i+1];

        if(objetos[i] == NULL)
            return;
    }
}

int eliminarObjeto(struct Objeto *objetos[max], int idBuscado){
    int i;

    for (i = 0; i < max; i++) {
        if (objetos[i] != NULL) {
            if (objetos[i]->idObjeto == idBuscado) {
                objetos[i] = NULL;
                compactar(objetos, i);
                return 1;
            }
        }
        else
            return 0;
    }
    return 0;
}

void mostrarObjetos (struct Objeto *objetos[max])
{
    int i;
    if(objetos[0] != NULL) {
        printf("Objetos ingresados:\n");
        printf("          Marca      Codigo\n");

        for (i = 0; i < max && objetos[i] != NULL; i++) {
            printf("%15s ", objetos[i]->marca);
            printf("%d\n", objetos[i]->idObjeto);
        }
    }
    else{
        printf("No hay objetos\n");
    }
}

////////////////////////////////////////    VENTAS   //////////////////////////////////////////////////

struct NodoVenta *buscarVenta (struct NodoVenta *head, int codigo)
{
    if (head != NULL)
    {
        struct NodoVenta *rec = head;
        do
        {
            if (rec->venta->codigo == codigo)
            {
                return rec;
            }
            rec = rec -> sig;
        } while (rec != head);
    }
    return NULL;
}

void agregarVenta(struct NodoVenta **headVenta, struct Venta *ventaAgregar)
{
    struct NodoVenta *ventaNueva = NULL;
    ventaNueva = (struct NodoVenta *) malloc (sizeof(struct NodoVenta));
    ventaNueva->venta = ventaAgregar;
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

int establecerVenta(struct NodoVenta **head, struct Objeto *objetoComprado, char *rutCliente, int cant, int *codigo, char* comuna)
{
    if(buscarVenta(*head, (*codigo))==NULL){
        struct Venta *venta = ((struct Venta*) malloc(sizeof(struct Venta)));
        strcpy(venta->rut, rutCliente);

        venta->cantVendida= cant;
        venta->monto = (venta->comprado->precio) * venta->cantVendida;
        venta->comprado = objetoComprado;
        venta->codigo = (*codigo);
        strcpy(venta->nombreComuna, comuna);
        (*codigo)++;
        agregarVenta(head,venta);
        return 1;
    }
    else
        return 0;
}

int eliminarVenta (struct NodoVenta **head, int codigo)
{
    if(*head != NULL)
    {
        struct NodoVenta *rec = (*head);
        do{
            if(rec->venta->codigo == codigo)
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

void mostrarVentas(struct NodoVenta *head){
    if(head != NULL)
    {
        struct NodoVenta *rec = head;
        printf("Lista de ventas: \n");
        printf("  Codigo Venta         Cantidad Vendida        Nombre del Objeto\n");
        do
        {
            printf("%d ",rec->venta->codigo);
            printf("%15d pesos",rec->venta->cantVendida);
            printf("%25s\n", rec->venta->comprado->nombre);
            rec = rec->sig;
        }while (rec != head);
    }

    else
        printf("No hay ventas\n");
}

/*                                       REGION                              */

struct NodoRegion *buscarSedeRegion(struct NodoRegion *head, int ip){
    if(head == NULL){
        return NULL;
    }
    else{
        struct NodoRegion *p = head->sig;
        while(p){
            if(ip == p->sedeEnRegion->ip){
                return p;
            }
            p = p->sig;
        }
        return NULL;
    }
}


struct NodoRegion *crearNodoRegion(struct SedeRegion *dato){
    struct NodoRegion *nuevo;

    nuevo = ((struct NodoRegion*) malloc(sizeof(struct NodoRegion)));
    nuevo->sedeEnRegion = dato;
    nuevo->sig = nuevo->ant = NULL;

    return nuevo;
}

int agregarSedeRegion(struct NodoRegion **ptrRegiones,struct NodoRegion **ptrRegionesFinal, struct Region *regionNueva){
    struct NodoRegion *nuevaRegion = malloc(sizeof(struct NodoRegion));
    if(nuevaRegion != NULL){

        nuevaRegion->sedeEnRegion = malloc(sizeof (struct Region));
        nuevaRegion->sedeEnRegion = regionNueva;
        nuevaRegion->sig = nuevaRegion->ant =  NULL;

        if((*ptrRegiones) != NULL){
            nuevaRegion->ant = *ptrRegionesFinal;
            ( *ptrRegionesFinal)->sig = nuevaRegion;
        }

        *ptrRegionesFinal = nuevaRegion;
        return 1;
    }

    return 0;
}

int establecerRegion(struct NodoRegion **head, struct NodoRegion **tail,char *nombre, int ip){
    if(*head == NULL){
        (*head) = crearNodoRegion(NULL);
    }

    if(buscarSedeRegion(*head, ip) == NULL){
        struct Region *reg = ((struct Region*) malloc(sizeof(struct Region)));

        strcpy(reg->nombreRegion, nombre);
        reg->ip = ip;
        reg->comunas = NULL ;
        agregarSedeRegion(*head, *tail,reg);


        return 1;
    }
    return 0;
}

int eliminarRegion(struct NodoRegion **head, char *nombreBuscado){
    if((*head )!= NULL)
    {
        struct NodoRegion *rec = (*head)->sig;
        while (rec != NULL)
        {
            if (strcmp(rec->sedeEnRegion->nombreRegion, nombreBuscado) == 0)
            {
                if ((*head)->sig == rec)
                {
                    (*head)-> sig = rec->sig;
                    if((*head)->sig == NULL)
                    {
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

int modificarRegion(struct NodoRegion *head, int ipBuscada, char *nuevoNombre)
{
    if (head != NULL)
    {
        struct NodoRegion *nodoBuscado = buscarSedeRegion(head,ipBuscada);
        if (nodoBuscado != NULL)
        {
            strcpy(nodoBuscado->sedeEnRegion->nombreRegion, nuevoNombre);
            return 1;
        }
    }
    return 0;
}

void mostrarRegiones(struct NodoRegion *head)
{
    if(head != NULL)
    {
        struct NodoRegion *rec = head->sig;
        printf("Lista de todas las regiones: \n");
        while (rec != NULL)
        {
            printf("- %s\n",rec->sedeEnRegion->nombreRegion);
            rec = rec->sig;
        }
    }

    else
        printf("NO HAY REGIONES\n");
}


////////////////////////////////////////    BODEGA   //////////////////////////////////////////////////

struct Bodega *buscarBodega(struct NodoBodega *head, int idObjetoBuscado)
{
    if (head != NULL)
    {
        struct NodoBodega *rec = head;
        do
        {
            if (rec->inventario->objeto->idObjeto == idObjetoBuscado)
            {
                return rec->inventario;
            }
            rec = rec -> sig;
        } while (rec != head);
    }
    return NULL;
}

void agregarBodega(struct NodoBodega **headBodega, struct Bodega *BodegaAgregar)
{
    struct NodoBodega *bodegaNueva = NULL;
    bodegaNueva = (struct NodoBodega *) malloc (sizeof(struct NodoBodega));
    bodegaNueva->inventario = BodegaAgregar;
    if((*headBodega) != NULL)
    {
        (*headBodega)->ant->sig = bodegaNueva;
        bodegaNueva->ant =  (*headBodega)->ant;
        (*headBodega)->ant = bodegaNueva;
        bodegaNueva->sig = (*headBodega);
    }
    else
    {
        (*headBodega) = bodegaNueva;
        (*headBodega)->sig = (*headBodega)->ant = (*headBodega);
    }
}

struct Bodega *establecerBodega(struct NodoBodega **head, int stock, struct Objeto *objetoB)
{
    //Entrada
    struct Bodega *inventario = NULL;
    if(buscarBodega(*head, objetoB->idObjeto)==NULL){

        inventario = ((struct Bodega *) malloc(sizeof(struct Bodega)));
        inventario->stock = stock;
        inventario->objeto = objetoB;
        agregarBodega(head,inventario);

    }

    else
        return NULL;

    return inventario;
}

int eliminarBodega(struct NodoBodega **head, int idObjetoBuscado)
{
    if(*head != NULL)
    {
        struct NodoBodega *rec = (*head);
        do{
            if(rec->inventario->objeto->idObjeto == idObjetoBuscado)
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

void mostrarBodega(struct NodoBodega *head){
    if(head != NULL)
    {
        struct NodoBodega *rec = head;
        printf("Lista de Tnventario: \n");
        printf("  Id del Objeto         Nombre         Precio        Stock\n");
        do
        {
            printf("%d ",rec->inventario->objeto->idObjeto);
            printf("%15s ",rec->inventario->objeto->nombre);
            printf("%15d pesos",rec->inventario->objeto->precio);
            printf("%d\n", rec->inventario->stock);
            rec = rec->sig;
        }while (rec != head);
    }

    else
        printf("No hay objetos en la bodega\n");
}

////////////////////////////////////////    COMUNA   //////////////////////////////////////////////////

struct NodoComuna *buscarComuna (struct NodoComuna *raiz, struct NodoComuna **previa, char *nombreBuscado) {
    struct NodoComuna *abb = raiz;
    bool esta = false;
    while (abb != NULL && esta == false) {
        if (strcmp(abb->comuna->nombre, nombreBuscado) == 0) {
            esta = true;

        }
        else {
            *previa = abb;
            if (strcmp( nombreBuscado, abb->comuna->nombre) < 0)
                abb = abb->izq;

            else
                abb = abb->der;
        }
    }
    return abb;
}

int esHoja(struct NodoComuna *ptr)
{
    if(ptr->der == NULL && ptr->izq == NULL)
        return 1;
    return 0;
}

int agregarComuna(struct NodoComuna **comunaABB, struct Comuna *datosNuevos)
{
    struct NodoComuna *comunaNueva = NULL, *ant = NULL, *ptr = NULL;
    ptr = buscarComuna(*comunaABB, &ant, datosNuevos->nombre);
    if(ptr == NULL)
    {
        comunaNueva = (struct NodoComuna *) malloc (sizeof(struct NodoComuna));
        comunaNueva->comuna = datosNuevos;
        comunaNueva->izq = comunaNueva->der = NULL;
        if(ant == NULL)
            *comunaABB = comunaNueva;

        else
        {
            if(strcmp(datosNuevos->nombre, ant->comuna->nombre) < 0)
                ant->izq = comunaNueva;
            else
                ant->der = comunaNueva;
        }
        return 1;
    }
    return 0;
}

int establecerComuna(struct NodoComuna **raiz, char *nombre, int tiempo)
{
    struct Comuna * comunaNueva = ((struct Comuna*) malloc(sizeof(struct Comuna)));
    strcpy(comunaNueva->nombre, nombre);
    comunaNueva->bodegas = NULL;
    comunaNueva->distanciaTiempoDelaSede = tiempo ;


    if(agregarComuna(raiz, comunaNueva) == 0)
        return 0;

    return 1;
}

int eliminarComuna(struct NodoComuna **comunaABB, char *nombreComunaEliminar)
{
    struct NodoComuna *padre = NULL, *actual = NULL, *nodo = NULL;
    struct Comuna *aux;

    actual = buscarComuna(*comunaABB, &padre, nombreComunaEliminar);

    while(actual != NULL)
    {
        if(esHoja(actual) == 1)
        {

            if(actual == (*comunaABB)){
                (*comunaABB)= NULL;
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
            aux = actual->comuna;
            actual->comuna = nodo->comuna;
            nodo->comuna = aux;
            actual = nodo;
        }
    }
    return 0;
}

int modificarNombreComuna(struct NodoComuna *abbComuna, char *nombreBuscado, char *nuevoNombre)
{
    struct NodoComuna *previa = NULL;
    if (abbComuna != NULL)
    {
        struct NodoComuna *nodoBuscado = buscarComuna(abbComuna, &previa, nombreBuscado);
        if (nodoBuscado != NULL)
        {
            strcpy(nodoBuscado->comuna->nombre, nuevoNombre);
            return 1;
        }
    }
    return 0;
}

void mostrarComuna(struct NodoComuna *abb){
    if(abb != NULL){
        printf("-%s\n",abb->comuna->nombre);
        mostrarBodega(abb->comuna->bodegas);
        mostrarComuna(abb->izq);
        mostrarComuna(abb->der);
    }
}

void CrearNodoVacioSedeCentral(struct SedeCentral * sedeInicio){
    sedeInicio = (struct SedeCentral *) malloc(sizeof (struct SedeCentral));
    sedeInicio ->ventas = NULL ;
    sedeInicio ->regiones = NULL ;
}

struct NodoVenta *crearNodoVenta(struct Objeto *objeto , char rut[100], int *codigo, int monto, int cant)
{

    struct Venta *venta = ((struct Venta*) malloc(sizeof(struct Venta)));

    strcpy(venta->rut, rut);

    venta->cantVendida = cant;
    venta->monto = (objeto->precio) * venta->cantVendida;
    venta->comprado = objeto;
    venta->codigo = (*codigo);
    (*codigo)++;

    struct NodoVenta *nodoNuevo = NULL;
    nodoNuevo = (struct NodoVenta *) malloc (sizeof(struct NodoVenta));
    nodoNuevo->venta = venta;

    return nodoNuevo;
}

void AnadirNodoVenta(struct NodoVenta **head, struct Objeto*objeto , char rut[100], int *codigo, int monto, int cantVendida){
    struct NodoVenta *rec;
    struct NodoVenta *nuevaV;

    if(*head!=NULL){
        rec=*head;
        if(buscarVenta(*head, *codigo)== NULL) {
            rec = *head;
            do {
                rec = rec->sig;
            } while (rec->sig != *head);

            nuevaV = crearNodoVenta(objeto ,rut,codigo,monto, cantVendida);
            if (nuevaV != NULL) {
                rec->sig = nuevaV;
                nuevaV->ant = rec;
                nuevaV->sig = *head;
                (*head)->ant = nuevaV ;
            }
            else{
                return;
            }
        }
    }
    if(*head==NULL){
        (*head) = crearNodoVenta(objeto ,rut,codigo,monto, cantVendida);

        return;
    }
    else return;
}

/*------------------------------------------FUNCIONES PROPIAS---------------------------------------------------------*/

int calcularMontoTotal(struct NodoVenta *head, struct NodoVenta *rec, char *rutCliente)
{
    struct NodoVenta *ptr = rec;

    int monto = 0;
    do
    {
        if (ptr == rec)
            monto += ptr->venta->monto;

        else if (strcmp(ptr->venta->rut, rutCliente) == 0)
            monto += ptr->venta->monto;

        ptr = ptr->sig;
    } while (ptr != head);

    return monto;
}

char *mejorCliente(struct NodoVenta *listaVentas) {

    if(listaVentas == NULL) return NULL;

    char *rutMejorCliente = (char*) malloc(maxbuffer * sizeof(char));

    struct NodoVenta *rec = listaVentas;

    int  monto;
    int masVentas = 0;
    do
    {
        monto = calcularMontoTotal(listaVentas, rec, rec->venta->rut);

        if (monto > (masVentas))
        {
            (masVentas) = monto;
            strcpy(rutMejorCliente, rec->venta->rut);
        }
        rec = rec->sig;
    } while (rec != listaVentas);

    return rutMejorCliente;
}



//---------------------------------------------AUTOS VENDIDOS---------------------------------------------------------//



void mostrarAutosVendidos(struct NodoVenta *headVenta)
{
    struct NodoVenta *rec = headVenta;
    if(headVenta == NULL){
        printf("No hay Ventas\n");
        return;
    }

    int cont = 0;
    do
    {
        if(rec->venta->comprado->tipo == 1){

            struct Objeto * objeto = rec->venta->comprado;
            if(cont == 0 )
            {
                printf("Lista de productos: \n");
                printf("    Nomrbe     Precio               Marca   ID Objeto\n");

            }
            printf("-%10s ",objeto->nombre);
            printf("%10d Pesos",objeto->precio);
            printf("%15s",objeto->marca);
            printf("%15s\n", objeto->idObjeto);
            cont++;
        }

        rec = rec->sig;
    }while(rec != headVenta);

    if(cont == 0) printf("NO SE ENCONTRARON AUTOS VENDIDOS");
}

//-----------------------------------------SEDE MAS DISTRIBUICION-----------------------------------------------------//

void contarVentas(struct NodoVenta *headVenta, int *cantVentas, char *nombre){
    if(headVenta == NULL) return;

    struct NodoVenta *rec = headVenta;

    do {
        if(strcmp(rec->venta->nombreComuna, nombre) == 0)
            (*cantVentas)++;
        rec = rec->sig;

    }while(rec != headVenta);
}

void determinarMejorSede(struct NodoComuna *sede, struct Comuna **sedeMasVentas,struct NodoVenta *headVenta, int *masVentas)
{
    int cantVentas = 0;

    contarVentas(headVenta, &cantVentas, sede->comuna->nombre);

    if(cantVentas > (*masVentas))
    {
        (*masVentas) = cantVentas;

        if(sedeMasVentas == NULL)
            (*sedeMasVentas) = ((struct Comuna *) malloc(sizeof(struct Comuna)));

        (*sedeMasVentas) = sede->comuna;

    }

}

void buscarSedeMasVentas(struct NodoComuna *headSede, struct Comuna **sedeMasVentas,struct NodoVenta *headVenta, int *masVentas)
{
    if(headSede != NULL)
    {
        determinarMejorSede(headSede, sedeMasVentas,headVenta, masVentas);
        buscarSedeMasVentas(headSede->izq, sedeMasVentas, headVenta, masVentas);
        buscarSedeMasVentas(headSede->der, sedeMasVentas, headVenta, masVentas);
    }
}

struct Comuna *sedeMasVentas(struct NodoRegion *headRegion, struct NodoVenta *headVenta, int *masVentas)
{
    if(headRegion == NULL) return NULL;
    if(headVenta == NULL) return NULL;

    struct Comuna *sedeMasVentas = NULL;
    struct NodoRegion *rec = headRegion->sig;

    while (rec != NULL)
    {
        buscarSedeMasVentas(rec->sedeEnRegion->comunas, &sedeMasVentas,headVenta, masVentas);
        rec = rec->sig;
    }

    return sedeMasVentas;
}

int validarRUT(char *rut)
{
    int length = strlen(rut);
    int i;
    for (i = 0; i < length; i++) {
        if (!isdigit(rut[i])) {
            return 0;
        }
    }
    if (length < 7 || length > 8) {
        return 0;
    }
    return 1;
}

int contarObjeto(struct NodoVenta *head, int idBuscado)
{
    struct NodoVenta *rec = head;

    int cantidad = 0;
    do {
        if(rec->venta->comprado->idObjeto == idBuscado)
            cantidad += rec->venta->cantVendida;

        rec = rec->sig;
    }while (rec != head);

    return cantidad;
}

struct Objeto *encontrarObjetoMasVendidoPorTipo(struct Objeto *vector[max], struct NodoVenta *headVenta, int tipoBuscado)
{
    if(headVenta == NULL){
        return NULL;
    }

    struct Objeto *MasVendido = NULL;
    int cantidad, mayor = 0;
    int i;
    for(i = 0; i < max; i++)
    {
        if(vector[i] != NULL)
        {
            if(vector[i]->tipo == tipoBuscado)
            {
                cantidad = contarObjeto(headVenta, vector[i]->idObjeto);
                if(cantidad > mayor)
                {
                    mayor = cantidad;
                    MasVendido = vector[i];
                }
            }
        }
    }
    return MasVendido;
}

int cantidadDeProductosEnSede(struct NodoBodega *head)
{
    if (head == NULL) {
        return 0;
    }

    struct NodoBodega *rec = head;
    int cantidadEnBodega = 0;

    do {
        cantidadEnBodega += rec->inventario->stock;

        rec = rec->sig;
    }while (rec != head);

    return cantidadEnBodega;
}

////////////////////////////////Menus/////////////////////////////////////////
////Modificar las regiones
void menuModificar(struct Nodoregion *ptrRegiones)
{
    char modificar, sumaResta;
    int opcionModificar, cantidad, idobj;
    if(!ptrRegiones){
        printf("no hay Regiones\n");
    }else{
        struct NodoRegion *p = ptrRegiones;
        printf("===================================================\n");
        printf("ingresa el numero \n");
        while(p){
            printf("%d para la region de %s\n",p->sedeEnRegion->ip, p->sedeEnRegion->nombreRegion);
            p = p->sig;
        }
        printf("===================================================\n");
        printf("opcion:");
        scanf("%d", &opcionModificar);
        p = buscarSedeRegion(ptrRegiones, opcionModificar);
        if (p){
            printf("Quieres modificar la Region de %s?\n",p->sedeEnRegion->nombreRegion);
            getchar();
            modificar= getchar();
            modificar = tolower(modificar);
            if(modificar == 'y'){

                printf("Quieres cambiar el nombre de la region?(apretar Y para confirmar)\n");
                getchar();
                modificar= getchar();
                modificar = tolower(modificar);
                if(modificar == 'y'){
                    idobj = 1;
                    char nuevoN[50];
                    scanf("%s", nuevoN); getchar();
                    int exito = modificarRegion(ptrRegiones, opcionModificar, nuevoN);
                    if(exito == 1)
                        printf("Modificado con exito\n");
                }
                printf("Quieres anadir comunas?(apretar Y para confirmar)\n");
                getchar();
                modificar= getchar();
                modificar = tolower(modificar);
                if(modificar == 'y') {
                    char comuna[100];

                    int dist;

                    printf("Ingrese la comuna de la sede\n");
                    scanf("%s", comuna);
                    getchar();

                    printf("Ingrese el tiempo de desplazamiento en dias\n");
                    scanf("%d", &dist);
                    getchar();

                    if (establecerComuna(&(p->sedeEnRegion->comunas), comuna, dist) == 1)
                        printf("Sede agregada con exito\n");
                    else
                        printf("Sede ya existente\n");

                }

            }
        }else{
            printf("se ingreso un dato invalido\n");
        }
    }
}

///// menu sede central
void menuSedeCentral(struct SedeCentral *Derco){
    char Y;
    int obj;
    if (Derco == NULL){
        CrearNodoVacioSedeCentral(Derco);
        printf("se creo la sede central\n");
    }else{
        printf("ya esta creada la sede Central\n");
    }
    printf("quieres Modificar datos de la sede Central(Y para confirmar)\n");
    Y = getchar();
    Y = tolower(Y);
    getchar();
    if(Y == 'y'){
        printf("quieres agregar Objetos a la sede central?(Y para confirmar)\n");


        Y = getchar();
        Y = tolower(Y);
        getchar();
        if(Y == 'y'){
            int id;
            printf("Ingrese el id del objeto\n");
            scanf("%d", &id);
            getchar();


            establecerObjeto(Derco->objetos, id);
        }
    }
    printf("quieres VER los datos de la sede central?(Y para confirmar)\n");
    getchar();
    Y = getchar();
    Y = tolower(Y);
    if(Y == 'y'){
        if(Derco->objetos == NULL){
            printf("NO HAY STOCK AUTOS SEDE CENTRAL\n");
        }else{
            struct nodoAutos *p = Derco->objetos;
            mostrarObjetos(Derco->objetos);
        }
    }

}
///menu ingreso de datos
void menuIngresarDatos(struct SedeCentral* Derco){
    char nombreReg[50] = "";
    int opcionReg = 0;
    printf("==================================\n");
    printf("¿Qué Region desea ingresar?\n");
    printf("0 para la Region de Arica y Parinacota\n");
    printf("1 para la Region de Tarapaca\n");
    printf("2 para la Region de Antofagasta\n");
    printf("3 para la Region de Atacama\n");
    printf("4 para la Region de Coaquimbo\n");
    printf("5 para la Region de Valparaiso\n");
    printf("6 para la Region de O'Higgins\n");
    printf("7 para la Region de Maule\n");
    printf("8 para la Region de Bio-Bio\n");
    printf("9 para la Region de la Araucania\n");
    printf("10 region de Los Rios\n");
    printf("11 para la Region de Los Lagos\n");
    printf("12 para la Region de Magallanes\n");
    scanf("%d", &opcionReg);

    if (opcionReg >= 0 && opcionReg <=12){
        if (opcionReg == 0) {
            strcpy(nombreReg,"Arica y Parinacota");
        }else if (opcionReg == 1) {
            strcpy(nombreReg,"Tarapaca");
        } else if (opcionReg == 2) {
            strcpy(nombreReg , "Antofagasta");
        }else if (opcionReg == 3) {
            strcpy(nombreReg , "Atacama");
        }else if (opcionReg == 4) {
            strcpy(nombreReg, "Coaquimbo");
        }else if (opcionReg == 5) {
            strcpy(nombreReg , "Valparaiso");
        }else if (opcionReg == 6) {
            strcpy(nombreReg , "O'Higgins");
        }else if (opcionReg == 7) {
            strcpy(nombreReg , "O'Higgins");
        }else if (opcionReg == 8) {
            strcpy(nombreReg , "Maule");
        }else if (opcionReg == 9) {
            strcpy(nombreReg , "Bio-Bio");
        }else if (opcionReg == 10) {
            strcpy(nombreReg , "la Araucania");
        }else if (opcionReg == 11) {
            strcpy(nombreReg , "Los Lagos");
        }else if (opcionReg == 12) {
            strcpy(nombreReg , "Magallanes");
        }
        struct NodoRegion *Region;
        if(Derco->regiones == NULL) {
            establecerRegion(&(Derco->regiones), &(Derco->tail), nombreReg, opcionReg);

        }else{

            Region = buscarSedeRegion(Derco->regiones, opcionReg);
            if(Region != NULL){
                printf("Esta region ya esta\n");
            }else{
                int exito = establecerRegion(&(Derco->regiones), &(Derco->tail), nombreReg, opcionReg);
                if(exito ==1)
                    printf("Region Agregada con exito\n");


            }
        }
    }else{
        printf("no se ingreso un dato valido\n");
    }
}

////menu de eleccion a funcion realizar
void menuEntrada(struct SedeCentral *Derco){

    char opcion;
    printf("===================================================\n");
    printf("****     *******  ******        ****     *****     \n");
    printf("**  **   **       **   **     ****     ***   ***   \n");
    printf("**   **  ******   ******     **       **       **  \n");
    printf("**   **  ******   **   **    **       **       **  \n");
    printf("**  **   **       **    **    +***     ***   ***   \n");
    printf("****     *******  **     **     ****     *****     \n");
    printf("===================================================\n");



    printf("r para ingresar Region\n");
    printf("m para modificar datos de la sede en Region\n");
    printf("v ver datos sedes\n");
    printf("c sede central\n");
    printf("s para salir\n");
    printf("e para funciones extras\n");
    printf("opcion:");
    scanf("%s", opcion);
    getchar();

    opcion = tolower(opcion);


    if(opcion == 'r'){
        menuIngresarDatos(Derco);

    }else if(opcion == 'm'){
        menuModificar(&(Derco->regiones));

    }

    else if(opcion == 'v') {

        struct NodoRegion *r = Derco->regiones;
        while(r){
            printf("===================================================\n");
            printf("nombre Region: %s\n",r->sedeEnRegion->nombreRegion);
            if(r->sedeEnRegion->comunas == NULL){
                printf("No hay comunas\n");
            }else{
                mostrarComuna(r->sedeEnRegion->comunas);

            }

            printf("===================================================\n");
            r = r->sig;
        }
    }

    else if(opcion == 'c'){
        menuSedeCentral(Derco);
    }
    else if(opcion == 'e'){
        char Y;
        printf("quieres obtener el cliente que mas gasta en sus productos? (Y para confirmar)\n");
        Y = getchar();
        Y = tolower(Y);
        getchar();
        if(Y == 'y'){
            char rutMejorCliente[100];

            strcpy(rutMejorCliente,(mejorCliente(Derco->ventas)) );

            if(rutMejorCliente != NULL){
                printf("El rut del mejor cliente es %s, ", rutMejorCliente);
            }
            else{
                printf("NO SE HAN REGISTRADO VENTAS\n");
            }
        }

        printf("quieres obtener la sede con mayor distribucion? (Y para confirmar)\n");
        Y = getchar();
        Y = tolower(Y);
        getchar();
        if(Y == 'y'){
            int masVentas = 0;
            struct Comuna * mayorComuna = sedeMasVentas(Derco->regiones, Derco->ventas, &masVentas);
            printf("La comuna con la sede de mayor distribucion es %s", mayorComuna->nombre);
        }

        printf("quieres ver los autos vendidos hasta la fecha? (Y para confirmar)\n");
        Y = getchar();
        Y = tolower(Y);
        getchar();
        if(Y == 'y'){
            mostrarAutosVendidos(Derco->ventas);
        }

    }

    else{
        printf("\nSaliendo del programa");
        exit(0);
    }
    getchar();
    menuEntrada(Derco);
}


int main() {

    struct SedeCentral *Derco = (struct SedeCentral*) malloc(sizeof(struct SedeCentral));

    menuEntrada(Derco);
    return 0;
}


