#include "CDiccionario.h"
#include "CDiccionario.h"
#include <iostream>
#define RESET   "\033[0m"
#define ROJO    "\033[31m"
#define AZUL    "\033[34m"
#define CIAN    "\033[36m"
#define VERDE   "\033[32m"

///Coronado Y Costilla Cerino Alexis
///Pablo Zahir Casas Campos

CDiccionario::CDiccionario()
{
    archivo=NULL;
    nAtributos=0;
    tambloque=0;
    menuPrincipal();
}

CDiccionario::~CDiccionario()
{

}


using namespace std;

// Muestro las opciones principales para crear, abrir o salir del diccionario.
void CDiccionario::menuPrincipal(){
    int op,valido;
    std::cout << CIAN << "\tDiccionario" << RESET <<  std::endl;
    do{
        printf("\n---------------------------------");
        printf("\n\tQue desea hacer:\n \n \t1.Nuevo diccionario\n \t2.Abrir diccionario\n \t3.Salir\n\n>>\t");
        //printf("\n---------------------------------\n>>\t");
        valido = scanf("%d", &op);
        //printf("\n---------------------------------");

        if (valido != 1) {
        printf("\nError: solo se aceptan numeros.\n");
        scanf("%*[^\n]");
        scanf("%*c");
        op = 0;
        } else if (op < 1 || op > 3)
        {
            printf("\nLa opcion %d no existe, por favor selecciona otra opcion-\n", op);
        }

        switch(op){
            case 1: printf("\n> Nuevo Diccionario\n");nuevoDiccionario(); break;
            case 2: printf("\n> Abrir Diccionario\n");abrirDiccionario(); break;
            case 3: std::cout << VERDE << "\n\t\tSaliendo..." << RESET << std::endl; break;
            default: std::cout << ROJO << "\nNo es una opcion valida" << RESET << std::endl;
        }
    }while(op!=3);
}

// Pido el nombre del archivo, verifico si ya existe y si no, lo creo y seteo la cabecera en -1.
void CDiccionario::nuevoDiccionario(){
    char nombre[50];
    printf("\nNombre del nuevo diccionario: ");
    scanf("%s",nombre);
    archivo=fopen(nombre,"rb");
    if(archivo){
        printf("\nEl archivo ya existe\n");
        fclose(archivo);
    }else{
        printf("\nEl archivo NO existe, creando...");
        archivo=fopen(nombre,"wb+");
        if(!archivo){
            printf("\nError al crear el archivo");
            return;
        }
        printf("\nArchivo %s creado correctamente\n",nombre);
        escribeCabEntidades(-1);
        MenuEntidades();
        fclose(archivo);
        archivo = NULL;
    }
}

// Intento abrir un archivo existente en modo lectura/escritura y abro el menu de entidades.
void CDiccionario::abrirDiccionario(){
    char nombre[50];
    printf("\nNombre del archivo que deseas abrir: ");
    scanf("%s",nombre);
    archivo=fopen(nombre,"rb+");
    if(archivo!= NULL){
        std::cout << VERDE << "\nArchivo abierto correctamente" << RESET << std::endl;
        //printf("\nArchivo abierto correctamente\n");
        MenuEntidades();
        fclose(archivo);
    }else{
        std::cout << ROJO << "\nNo existe el archivo" << RESET << std::endl;
    }
}
///--------------------------------------------------------------------MENU ENTIDADES------------------------------------------------------------------------------------
// Muestro las opciones para gestionar las entidades y mando a llamar las funciones correspondientes.
void CDiccionario::MenuEntidades(){
    int op;
    do{
        printf("\n---------------------------------");
        std::cout << CIAN << "\n>>\tMenu Entidades" << RESET <<  std::endl;
        printf("\n \t1.Nueva\n \t2.Consultar\n \t3.Eliminar\n \t4.Modificar\n \t5.Atributos\n \t6.Datos\n \t7.Regresar\n\n>>\t");
        //printf("---------------------------------\n>>\t ");
        scanf("%d",&op);
        //printf("---------------------------------\n ");

        switch(op){
            case 1: printf("\n>Nueva Entidad\n");altaEntidad(); break;
            case 2: printf("\n> Consultar Entidad\n");consultarEntidades(); break;
            case 3: printf("\n> Eliminar Entidad\n");bajaEntidad(); break;
            case 4: printf("\n> Modificar Entidad\n");modificaEntidad(); break;
            case 5: printf("\n> Atributos\n");menuAtributos(); break;
            // preguntar con que entidad se quiere trabajar
            case 6:printf("\n> Datos"); menuDatos();
                    break;
            case 7: std::cout << VERDE << "\n\t\tRegresando..." << RESET << std::endl; break;
            default: std::cout << ROJO << "\n\t\tOpcion no valida..." << RESET << std::endl;
        }
    }while(op!=7);
}



/// -------------------------------------------------------------- MENU ATRIBUTOS ------------------------------------------------------------------------------------------------

/// Verifico que haya una entidad activa y muestro el menu para gestionar sus atributos.
void CDiccionario::menuAtributos(){
    if(!activarEntidad()) return;

    int op;
    do{
        printf("\n---------------------------------");
        std::cout << CIAN << "\n>>\tMenu Atributo" << RESET <<  std::endl;
        printf("\n \t1.Nuevo \n \t2.Consultar \n \t3.Eliminar \n \t4.Modificar \n \t5.Regresar\n\n>>\t");
        //printf("---------------------------------\n>>\t ");
        scanf("%d",&op);
        switch(op){
            case 1: nuevoAtributo(); break;
            case 2: consultarAtributo(); break;
            case 3: if(activa.data != -1)
                        std::cout << ROJO << "\n\t\tEROOR!! la entidad ya tiene registros, no se puede eliminar\n" << RESET << std::endl;
                    else
                        bajaAtributo();
                    break;
            case 4:  if(activa.data != -1)
                        std::cout << ROJO << "\n\t\tEROOR!! la entidad ya tiene registros, no se puede eliminar\n" << RESET << std::endl;
                    else
                        modificaAtributo();
                    break;
            case 5: std::cout << VERDE << "\n\t\tRegresando..." << RESET << std::endl; break;
            default: std::cout << ROJO << "\n\t\tOpcion no valida..." << RESET << std::endl;
        }
    }while(op!=5);
}

///------------------------------------------------------- MENU DATOS -----------------------------------------------------------------------------------------------------------

// Muestro el menu para gestionar los registros de informacion.
void CDiccionario::menuDatos(){
    int op;
    do{
        printf("\n---------------------------------");
        std::cout << CIAN << "\n>>\tMenu Datos" << RESET <<  std::endl;
        printf("\n \t1.Nuevo \n \t2.Consultar \n \t3.Eliminar \n \t4.Modificar \n \t5.Regresar\n");
        //printf("---------------------------------\n>>\t ");
        scanf("%d",&op);
        switch(op){
            case 1: printf("\n> Nuevo Registro");altaBloque(); break;
            case 2: printf("\n> Consulta Registro"); consultaBloques(); break;
            case 3: printf("\n> Elimina Registro"); //eliminaBloque(void *bloq); break;
            case 4:printf("\n> Modifica Registro"); modificaBloque();break;
            case 5: std::cout << VERDE << "\n\t\tRegresando..." << RESET << std::endl; break;
            default: std::cout << ROJO << "\n\t\tOpcion no valida..." << RESET << std::endl;
        }
    }while(op!=5);
}
///--------------------------------------------------------- ENTIDADES ----------------------------------------------------------------------
// Me muevo al inicio del archivo y guardo la direccion de la cabecera de las entidades.
void CDiccionario::escribeCabEntidades(long cab){
    fseek(archivo,0,SEEK_SET);
    fwrite(&cab,sizeof(long),1,archivo);
}

// Capturo una nueva entidad y si no existe, la escribo en el disco y la enlazo.
void CDiccionario::altaEntidad(){
    long dir;
    Entidad nueva=capturaEntidad();

    if(buscaEntidad(nueva)==-1){
        dir=escribeEntidad(nueva);
        insertaEntidad(nueva,dir);
    }else{
        std::cout << ROJO << "\n\tError: La entidad ya existe" << RESET << std::endl;
    }
}

// Pido al usuario el nombre de la nueva entidad y configuro todos sus punteros en nulo (-1).
Entidad CDiccionario::capturaEntidad(){
    Entidad ent;
    printf("\nNombre de la entidad: ");
    scanf(" %[^\n]",ent.nombre);
    printf("\n");
    ent.atr=-1;
    ent.sig=-1;
    ent.data=-1;
    return ent;
}

// Recorro la lista buscando una entidad por nombre; si la encuentro, regreso su direccion.
long CDiccionario::buscaEntidad(Entidad ent){
    long cab=getCabEntidades();
    Entidad actual;

    while(cab!=-1){
        actual=leeEntidad(cab);
        if(strcmp(actual.nombre,ent.nombre)==0)
            return cab;
        cab=actual.sig;
    }
    return -1;
}

// Me voy al final del archivo para estampar la nueva entidad y devuelvo el byte en el que cayo.
long CDiccionario::escribeEntidad(Entidad ent){
    long dir;
    fseek(archivo,0,SEEK_END);
    dir=ftell(archivo);
    fwrite(&ent,sizeof(Entidad),1,archivo);
    return dir;
}

// Salto a la direccion que me pasen, leo la entidad que vive ahi y la regreso.
Entidad CDiccionario::leeEntidad(long dir){
    Entidad nvo;
    fseek(archivo,dir,SEEK_SET);
    fread(&nvo,sizeof(Entidad),1,archivo);
    return nvo;
}

// Conecto la entidad nueva en la lista de disco, asegurandome de que quede ordenada alfabeticamente.
void CDiccionario::insertaEntidad(Entidad nvo,long dir){
    Entidad act,ant;
    long cab=getCabEntidades(),dirant;

    if(cab==-1){
        cab=dir;
        escribeCabEntidades(cab);
    }else{
        act=leeEntidad(cab);
        if(strcmp(act.nombre,nvo.nombre)>0){
            nvo.sig=cab;
            reescribeEntidad(dir,nvo);
            escribeCabEntidades(dir);
        }else{
            while(cab!=-1&&strcmp(nvo.nombre,act.nombre)>0){
                dirant=cab;
                ant=act;
                cab=act.sig;
                if(cab!=-1) act=leeEntidad(cab);
            }
            nvo.sig=cab;
            reescribeEntidad(dir,nvo);
            ant.sig=dir;
            reescribeEntidad(dirant,ant);
        }
    }
}

// Piso una entidad vieja en disco usando su direccion para actualizarle los datos.
void CDiccionario::reescribeEntidad(long dir,Entidad ent){
    fseek(archivo,dir,SEEK_SET);
    fwrite(&ent,sizeof(Entidad),1,archivo);
}

// Pido el nombre de la entidad a borrar, reviso si existe y luego mando a eliminarla.
void CDiccionario::bajaEntidad(){
    long dir;
    char nom[30];

    printf("\nIngrese entidad >>\t ");
    scanf("%s",nom);

    Entidad aux;
    strcpy(aux.nombre,nom);
    dir=buscaEntidad(aux);

    if(dir==-1)
        std::cout << ROJO << "\n\tERROR!! la entidad no puede borrarse " << RESET << std::endl;
    else eliminaEntidad(nom);
}

// Desenlazo la entidad de la lista saltandome su nodo para que el disco ya no la detecte.
long CDiccionario::eliminaEntidad(cadena nom){
    Entidad ant,le;
    long dirant,cab;

    cab=getCabEntidades();
    le=leeEntidad(cab);

    if(strcmp(nom,le.nombre)==0){
        escribeCabEntidades(le.sig);
        return cab;
    }else{
        while(cab!=-1&&strcmp(nom,le.nombre)!=0){
            dirant=cab;
            ant=le;
            cab=le.sig;
            if(cab!=-1) le=leeEntidad(cab);
        }
        if(strcmp(le.nombre,nom)==0){
            ant.sig=le.sig;
            reescribeEntidad(dirant,ant);
            return cab;
        }else
            return -1;
    }
}

// Pregunto que entidad actualizar, pido los datos nuevos, borro la vieja y la inserto modificada.
void CDiccionario::modificaEntidad(){
    Entidad nueva,aux;
    long dir;

    printf("Que entidad desea modificar: ");
    scanf(" %[^\n]",aux.nombre);

    if(buscaEntidad(aux)!=-1){
        printf("Ingrese la nueva info: ");
        nueva=capturaEntidad();
        if(buscaEntidad(nueva)==-1){
            dir=eliminaEntidad(aux.nombre);
            reescribeEntidad(dir,nueva);
            insertaEntidad(nueva,dir);
        }else
            std::cout << ROJO << "\n\tNo se puede actualizar" << RESET << std::endl;
    }else
        std::cout << ROJO << "\n\tNo existe la entidad" << RESET << std::endl;
}


/// ---------------------------------------------------------------------- REGISTROS ------------------------------------------------------------------------------------------------


// Salto a un byte especifico, descargo el bloque de memoria del paciente y lo desgloso en pantalla.
void CDiccionario::consultarRegistro(){
    if(!archivo){
        printf("\nPrimero abre un archivo");
        return;
    }

    long pos;
    int total=(TAM*sizeof(char))+sizeof(int)+sizeof(float)+sizeof(double)+sizeof(long);
    void *paciente=malloc(total);

    printf("\nPosicion: ");
    scanf("%ld",&pos);
    fseek(archivo,pos,SEEK_SET);

    if(fread(paciente,total,1,archivo)!=1){
        printf("\nNo hay datos");
        free(paciente);
        return;
    }

    char *ptr=(char*)paciente;
    int cont=0;

    printf("\nNombre: %s",ptr);
    cont+=TAM;
    printf("\nEdad: %d",*(int*)(ptr+cont));
    cont+=sizeof(int);
    printf("\nPeso: %.2f",*(float*)(ptr+cont));
    cont+=sizeof(float);
    printf("\nEstatura: %.2lf",*(double*)(ptr+cont));
    cont+=sizeof(double);
    printf("\nID: %ld",*(long*)(ptr+cont));

    free(paciente);
}

// Leo y saco la direccion guardada en el primer byte del archivo para saber donde inicia todo.
long CDiccionario::getCabEntidades(){
    long dir;
    fseek(archivo,0,SEEK_SET);
    fread(&dir,sizeof(long),1,archivo);
    return dir;
}

// Recorro toda la lista de entidades brincando con sus punteros e imprimo su informacion.
void CDiccionario::consultarEntidades(){
    Entidad actual;
    long cab=getCabEntidades();
    while(cab!=-1){
        actual=leeEntidad(cab);
        printf("\n|%s\t\t| atr > %ld \t| data > %ld \t| sig > %ld \t|\n",actual.nombre,actual.atr,actual.data,actual.sig);
        cab=actual.sig;
    }
}

//---------------------------------------====================== FUNCIONES NUEVAS DE ATRIBUTOS ======================---------------------------------------------------------------------------------
///acl
// Checo si hay entidades, pregunto por cual trabajar y la cargo en memoria como mi entidad 'activa'.
bool CDiccionario::activarEntidad(){
    if(getCabEntidades()==-1)
    {
        cout << ROJO <<"\nERROR:" << RESET;
        cout << "No hay entidades registradas\n";
        return false;
    }

    Entidad temp;
    printf("\nEn cual entidad desea trabajar? ");
    scanf(" %[^\n]",temp.nombre);

    long d=buscaEntidad(temp);
    if(d!=-1)
    {
        diractiva=d;
        activa=leeEntidad(d);
        return true;
    }
    cout << ROJO << "\nERROR:" << RESET;
    cout << "La entidad no existe\n";
    return false;
}

void CDiccionario::altaAtributo()
{
    Atributo nuevoA = capturaAtributo();
    if(buscaAtributo(nuevoA.nombre)!=-1)
    {
        long dir = escribeAtributo(nuevoA);
        insertaAtributo(nuevoA, dir);
    }else
    cout << "Ese atributo no existe";
}



// Voy a la direccion que me pasen en disco y descargo la estructura de ese atributo en especifico.
Atributo CDiccionario::leeAtributo(long dir){
    Atributo nvo;
    fseek(archivo,dir,SEEK_SET);
    fread(&nvo,sizeof(Atributo),1,archivo);
    return nvo;
}

// Escribo los datos de un atributo al fondo del archivo y me guardo en que byte cayo.
long CDiccionario::escribeAtributo(Atributo atr){
    long dir;
    fseek(archivo,0,SEEK_END);
    dir=ftell(archivo);
    fwrite(&atr,sizeof(Atributo),1,archivo);
    return dir;
}

// Le caigo encima a un atributo viejo reescribiendo la zona del archivo con datos nuevos.
void CDiccionario::reescribeAtributo(long dir,Atributo atr){
    fseek(archivo,dir,SEEK_SET);
    fwrite(&atr,sizeof(Atributo),1,archivo);
}

// Pido la configuracion completa para el nuevo atributo y pongo su apuntador siguiente en nulo (-1).
Atributo CDiccionario::capturaAtributo(){
    Atributo nuevaA;
    printf("Dame el nombre: \n");
    scanf("%s",nuevaA.nombre);
    printf("\n\tIngresa de que tipo es:\n  \n\t1.char, \n\t2.int, \n\t3.float, \n\t4.double, \n\t5.long\n");
    scanf("%d",&nuevaA.tipo);

    if(nuevaA.tipo==1){
        printf("cuantas letras?: ");
        scanf("%d",&nuevaA.tamano);
    }else{
        switch(nuevaA.tipo){
            case 2: nuevaA.tamano=sizeof(int); break;
            case 3: nuevaA.tamano=sizeof(float); break;
            case 4: nuevaA.tamano=sizeof(double); break;
            case 5: nuevaA.tamano=sizeof(long); break;
        }
    }
    do
    {
        printf("este atributo es clave primario? (s/n): ");
        scanf(" %c",&nuevaA.lskp);
    }while(nuevaA.lskp != 's' && nuevaA.lskp != 'n');
    if(nuevaA.lskp != 's')
    {
        do
        {
            printf("permite Nulos? (s/n): ");
            scanf(" %c",&nuevaA.nulo);
        }while(nuevaA.nulo != 's' && nuevaA.nulo != 'n');
    }
     else
     {
        cout << "\nNO se permiten nulos si es clave primario\n";
        nuevaA.nulo = 's';
     }
    printf("Indique descripcion Para el Producto: ");
    scanf(" %[^\n]",nuevaA.descripcion);

    nuevaA.sig=-1;
    return nuevaA;
}

// Capturo la informacion del atributo y si el nombre esta libre, lo meto al archivo y lo enlazo.
void CDiccionario::nuevoAtributo(){
    Atributo nuevoA=capturaAtributo();
    if(buscaAtributo(nuevoA.nombre)==-1){
        long dir=escribeAtributo(nuevoA);
        insertaAtributo(nuevoA,dir);
        printf("\nAtributo guardado.\n");
    }else{
        printf("ese atributo no existe\n");
    }
}

// Acomodo el atributo nuevo en la lista ligada de mi entidad activa ordenandolo por abecedario.
void CDiccionario::insertaAtributo(Atributo nvo,long dir){
    if(activa.atr==-1){
        activa.atr=dir;
        reescribeEntidad(diractiva,activa);
    }else{
        Atributo actual=leeAtributo(activa.atr);
        if(strcmp(actual.nombre,nvo.nombre)>0){
            nvo.sig=activa.atr;
            reescribeAtributo(dir,nvo);
            activa.atr=dir;
            reescribeEntidad(diractiva,activa);
        }else{
            long cab=activa.atr;
            long dirAnt;
            Atributo atrAnt;
            while(cab!=-1&&strcmp(nvo.nombre,actual.nombre)>0){
                dirAnt=cab;
                atrAnt=actual;
                cab=actual.sig;
                if(cab!=-1) actual=leeAtributo(cab);
            }
            if(cab!=-1){
                nvo.sig=cab;
                reescribeAtributo(dir,nvo);
            }
            atrAnt.sig=dir;
            reescribeAtributo(dirAnt,atrAnt);
        }
    }
}

// Voy recorriendo los atributos ligados a mi entidad activa para ver si ya tengo uno con ese nombre.
long CDiccionario::buscaAtributo(char  *atr){
    long cab=activa.atr;
    Atributo actual;
    while(cab!=-1){
        actual=leeAtributo(cab);
        if(strcmp(actual.nombre,atr)==0){
            return cab;
        }
        cab=actual.sig;
    }
    return -1;
}

// Imprimo todo el detalle de los atributos que pertenecen a la entidad que tengo activa en ese momento.
void CDiccionario::consultarAtributo(){
    Atributo nvo;
    long cab=activa.atr;
    if(cab == -1){
        printf("\nNo hay ningun atributo\n");
    }else{
    while(cab!=-1){
        nvo=leeAtributo(cab);
        printf("| %s | %d | %d | %c | %c | %s |\n",nvo.nombre,nvo.tipo,nvo.tamano,nvo.lskp,nvo.nulo,nvo.descripcion);
        cab=nvo.sig;
        }
    }
}

//Busco un atributo a eliminar, verifico si el atributo existe y si existe lo elimino
void CDiccionario::bajaAtributo(){
    long dir;
    char nombre[30];
    printf("Ingrese el nombre del atributo a eliminar:");
    scanf("%s", nombre);
    dir=buscaAtributo(nombre);
    if(dir==-1){
        printf("Error: El atributo no existe");
    } else {
        eliminaAtributo(nombre);
    }
}

// Elimino el atributo que pertenece a una entidad activa
long CDiccionario::eliminaAtributo(char cad[30]){
    long cab=activa.atr;
    long dirant=-1;
    Atributo ant, actual;
    actual=leeAtributo(cab);
    if(strcmp(actual.nombre, cad)==0){
        activa.atr=actual.sig;
        reescribeEntidad(diractiva,activa);
    }
    else {
        while(cab!=-1&&strcmp(actual.nombre,cad)<0){
            dirant=cab;
            ant=actual;
            cab=actual.sig;
            if(cab!=-1){
                actual=leeAtributo(cab);
            }
        }
        if(strcmp(cad,actual.nombre)==0){
            ant.sig=actual.sig;
            reescribeAtributo(dirant,ant);
        }
    }
    return(cab);
}

void CDiccionario::modificaAtributo()
{
   Atributo nvo, aux;
   long dir, dir2;
   printf("Atributo a modificar:\n");
   scanf("%s", aux.nombre);

   if(buscaAtributo(aux.nombre)!=-1)
   {
	printf("Ingresa nueva informacion\n");
	nvo=capturaAtributo();
	dir2=buscaAtributo(nvo.nombre);

	if(strcmp(nvo.nombre, aux.nombre)==0 || dir2==-1)
	{
		dir=eliminaAtributo(aux.nombre);
		reescribeAtributo(dir,nvo);
		insertaAtributo(nvo, dir);
	}else
	printf("No se pudo modificar por que ya existe un atributo con ese nombre.\n");
   }else
   printf("No existe el atributo\n");

}

//-------------------------------------------------------------------------BLOQUES-----------------------------------------------------------------------------------------------------

/// verificaciones->
/// 1.- Si hay Bloques no se permite: modificar/eliminar atributos.
/// 2.- Solo entramos a Bloques si hay 1 sola clave primaria.

void CDiccionario::cargaAtributos()
{
    long cab = activa.atr;
    int id_n = 1;
    int contPk = 0;

    tambloque = sizeof(long);
    nAtributos = 0;

    if (activa.atr == -1)
    {
        cout << ROJO << "\nERROR:" << RESET;
        printf("La entidad activa no tiene atributos\n");
        return;
    }
    while (cab != -1)
    {
        Atributo actual = leeAtributo(cab);

        if (actual.lskp == 'S' || actual.lskp == 's') {
            contPk++;       //se valida que solo pueda tener una sola clave primaria
            arrAtributo[0] = actual;
        } else {
            arrAtributo[id_n] = actual;
            id_n++;
        }
        tambloque += actual.tamano;
        nAtributos++;
        cab = actual.sig;
    }
    if (contPk == 0)
    {
        printf("\nError: La entidad no tiene clave primaria (KP).\n");
        return;
    } else if (contPk > 1)
    {
        cout << ROJO << "\nERROR:" << RESET;
        cout << "La entidad tiene mas de una clave primaria\nPara poder acceder a bloques la entidad solo debera contar con una clave primaria" << RESET;
        return;
    }
}

void* CDiccionario::capturaBloque()
{
    void* bloque = malloc(tambloque);
    long desp = 0;


    *(long *)((char *)bloque + desp) = -1;
    desp += sizeof(long);

    for(int i = 0; i < nAtributos; i++)
    {
        switch(arrAtributo[i].tipo){
            case 1:
                printf("Ingresa %s: ", arrAtributo[i].nombre);
                scanf(" %[^\n]", (char *)bloque + desp);
                break;
            case 2: // int
                printf("Ingresa %s: ", arrAtributo[i].nombre);
                scanf("%d", (int *)((char *)bloque + desp));
                break;
            case 3: // float
                printf("Ingresa %s: ", arrAtributo[i].nombre);
                scanf("%f", (float *)((char *)bloque + desp));
                break;
            case 4: // double
                printf("Ingresa %s: ", arrAtributo[i].nombre);
                scanf("%lf", (double *)((char *)bloque + desp));
                break;
            case 5: // long
                printf("Ingresa %s: ", arrAtributo[i].nombre);
                scanf("%ld", (long *)((char *)bloque + desp));
                break;
        }
        desp += arrAtributo[i].tamano;
    }
    return bloque;
}


long CDiccionario::buscaBloque(void *key)
{

    if (activa.data == -1) {
        return -1;
    }

    long cab = activa.data;
    void *actual;

    while (cab != -1) {
        actual = leeBloque(cab);

        if (comparaBloques(key, actual) == 0) {

            free(actual);
            return cab;
        }

        long sig = *(long *)((char *)actual);
        free(actual);
        cab = sig;
    }

    return -1;
}


int CDiccionario::comparaBloques(void *b1, void *b2){
    long desp = sizeof(long); // saltarse el sig, comparar desde el KP
    switch(arrAtributo[0].tipo){
        case 1:
            return strcmp((char *)b1 + desp, (char *)b2 + desp);
        case 2:
            return *(int *)((char *)b1 + desp) - *(int *)((char *)b2 + desp);
        case 3:
            return (*(float *)((char *)b1+desp) > *(float *)((char *)b2+desp))
                 - (*(float *)((char *)b1+desp) < *(float *)((char *)b2+desp));
        case 4:
            return (*(double *)((char *)b1+desp) > *(double *)((char *)b2+desp))
                 - (*(double *)((char *)b1+desp) < *(double *)((char *)b2+desp));
        case 5:
            return (*(long *)((char *)b1+desp) > *(long *)((char *)b2+desp))
                 - (*(long *)((char *)b1+desp) < *(long *)((char *)b2+desp));
    }
    return 0;
}

void CDiccionario::altaBloque()
{
    void *nuevo = capturaBloque();
    if (buscaBloque(nuevo) == -1)
    {
        long dir = escribeBloque(nuevo);
        insertaBloque(nuevo, dir);
    }
    else
    {
        cout << "Error: el bloque ya existe";
    }
}

void *CDiccionario::leeBloque(long dir){
    void *bloque = malloc(tambloque);
    fseek(archivo, dir, SEEK_SET);
    fread(bloque, tambloque, 1, archivo);
    return bloque;
}
long CDiccionario::escribeBloque(void *bloque)
{
    fseek(archivo, 0, SEEK_END);
    long dir = ftell(archivo);
    fwrite(bloque, tambloque, 1, archivo);
    return (dir);
}

void CDiccionario::reescribeBloque(void *bloque, long dir){
    fseek(archivo, dir, SEEK_SET);
    fwrite(bloque, tambloque, 1, archivo);
}

void CDiccionario::consultaBloques()
{
    for(int i=0; i<nAtributos; i++)
        printf("%s", arrAtributo[i].nombre);

    long cab = activa.data;
    while(cab != -1)
    {
       void *bloque = leeBloque(cab);
        cout << *((long *)((char*)bloque + 0));
        long desp = sizeof(long);
        for(int i=0; i<nAtributos; i++)
        {
            switch(arrAtributo[i].tipo)
            {
                case 1: cout << ((char*)(char *)(bloque + desp)); break;
                case 2: cout << *((int *)((char *)bloque + desp)); break;
                case 3: cout << *(float *)(bloque + desp); break;
                case 4: cout << *(double *)(bloque + desp); break;
                case 5: cout << *(long *)(bloque + desp); break;
            }
            desp += arrAtributo[i].tamano;
        }
        cout<<endl;
        long sig = *(long *)(bloque);
        free(bloque);
        cab = sig ;
    }
}

void CDiccionario::insertaBloque(void *nvo, long dirnvo){
    if(activa.data == -1){
        activa.data = dirnvo;
        reescribeEntidad(diractiva, activa);
    }else{
        void *act = leeBloque(activa.data);
        if(comparaBloques(nvo, act) < 0){
            *(long *)((char *)nvo) = activa.data;
            reescribeBloque(nvo, dirnvo);
            activa.data = dirnvo;
            reescribeEntidad(diractiva, activa);
            free(act);
        }else{
            long cab = activa.data;
            long dirant;
            void *bloqueant;
            while(cab != -1 && comparaBloques(nvo, act) > 0){
                dirant = cab;
                bloqueant = act;
                cab = *(long *)((char *)act);
                if(cab != -1) act = leeBloque(cab);
            }
            *(long *)((char *)nvo) = cab;
            reescribeBloque(nvo, dirnvo);
            *(long *)((char *)bloqueant) = dirnvo;
            reescribeBloque(bloqueant, dirant);
            free(bloqueant);
        }
    }
}

void CDiccionario::eliminaBloque(void *bloq){
    cargaAtributos();
    void *llave = capturaBloque();

    if(activa.data == -1){
        printf("\nError: no hay bloques.\n");
        free(llave); return;
    }

    void *act = leeBloque(activa.data);
    if(comparaBloques(llave, act) == 0){
        activa.data = *(long *)((char *)act);
        reescribeEntidad(diractiva, activa);
        free(act); free(llave); return;
    }

    long dirant = activa.data;
    void *bloqueant = act;
    long cab = *(long *)((char *)act);

    while(cab != -1){
        act = leeBloque(cab);
        if(comparaBloques(llave, act) == 0){
            *(long *)((char *)bloqueant) = *(long *)((char *)act);
            reescribeBloque(bloqueant, dirant);
            free(act); free(bloqueant); free(llave);
            printf("\nBloque eliminado.\n");
            return;
        }
        free(bloqueant);
        dirant = cab;
        bloqueant = act;
        cab = *(long *)((char *)act);
    }
    printf("\nError: bloque no encontrado.\n");
    free(bloqueant);
    free(llave);
}

void CDiccionario::modificaBloque()
{
    void *nombre = pidaClaveBloque();
    long dirA =buscaBloque(nombre);
    if(dirA != -1)
    {
        cout <<"\nIngresa la nueva informacion\n";
        void *nuevo = capturaBloque();
        long dirB = buscaBloque(nuevo);
        if(dirB == -1 || dirB == dirA)
        {
            if (dirB == dirA)
            {
                reescribeBloque(nuevo, dirA);
            }
            eliminaBloque(nombre);
            insertaBloque(nuevo, dirA);
            cout << ROJO << "Registro modificado" << RESET;
        }else
            std::cout << ROJO << "Existe un registro con esta clave" << RESET <<  std::endl;

    }else
    cout << ROJO << "No existe el bloque" << RESET;
}

void *CDiccionario::pidaClaveBloque()
{
    void *bloqPK = malloc(tambloque);
    long desp = sizeof(long);
    printf("\nIngresa la clave %s ", arrAtributo[0].nombre);
    switch(arrAtributo[0].tipo)
    {
        case 1: scanf(" %[^\n]", (char *)bloqPK + desp); break;
        case 2: scanf("%d", ((int *)((char *)bloqPK + desp))); break;
        case 3: scanf("%f", ((float *)((char *)bloqPK + desp))); break;
        case 4: scanf("%lf", ((double *)((char *)bloqPK + desp))); break;
        case 5: scanf("%ld", ((long *)((char *)bloqPK + desp))); break;
    }
    return bloqPK;
}
