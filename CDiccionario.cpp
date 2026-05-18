#include "CDiccionario.h"
//Coronado Y Costilla Cerino Alexis y Pablo Zahir Casas Campos

// Inicializo los punteros y variables de la clase, y mando a llamar al menu principal.
CDiccionario::CDiccionario(){
    archivo=NULL;
    nAtributos=0;
    tambloque=0;
    menuPrincipal();
}

// Muestro las opciones principales para crear, abrir o salir del diccionario.
void CDiccionario::menuPrincipal(){
    int op;
    do{
        printf("\nQue desea hacer:\n1.Nuevo diccionario\n2.Abrir diccionario\n3.Salir\n");
        scanf("%d",&op);
        switch(op){
            case 1: nuevoDiccionario(); break;
            case 2: abrirDiccionario(); break;
            case 3: printf("\nSaliendo..."); break;
            default: printf("\nNo es una opcion");
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
    }
}

// Intento abrir un archivo existente en modo lectura/escritura y abro el menu de entidades.
void CDiccionario::abrirDiccionario(){
    char nombre[50];
    printf("\nNombre del archivo: ");
    scanf("%s",nombre);
    archivo=fopen(nombre,"rb+");
    if(archivo!=NULL){
        printf("\nArchivo abierto correctamente");
        MenuEntidades();
        fclose(archivo);
    }else{
        printf("\nNo existe el archivo");
    }
}

// Muestro las opciones para gestionar las entidades y mando a llamar las funciones correspondientes.
void CDiccionario::MenuEntidades(){
    int op;
    do{
        printf("\n1.Nueva 2.Consultar 3.Eliminar 4.Modificar 5.Atributos 6.Datos 7.Regresar\n");
        scanf("%d",&op);
        switch(op){
            case 1: altaEntidad(); break;
            case 2: consultarEntidades(); break;
            case 3: bajaEntidad(); break;
            case 4: modificaEntidad(); break;
            case 5: menuAtributos(); break;
            case 6: menuDatos(); break;
            case 7: printf("\nRegresando..."); break;
            default: printf("\nOpcion no valida");
        }
    }while(op!=7);
}

// ---------------- ATRIBUTOS ----------------

// Verifico que haya una entidad activa y muestro el menu para gestionar sus atributos.
void CDiccionario::menuAtributos(){
    if(!activarEntidad()) return;

    int op;
    do{
        printf("\n1.Nuevo 2.Consultar 3.Eliminar 4.Modificar 5.Regresar\n");
        scanf("%d",&op);
        switch(op){
            case 1: nuevoAtributo(); break;
            case 2: consultarAtributo(); break;
            case 3: bajaAtributo(); break;
            case 4: modificaAtributo(); break;
            case 5: printf("\nRegresando..."); break;
            default: printf("\nOpcion no valida");
        }
    }while(op!=5);
}

// ---------------- DATOS ----------------

// Muestro el menu para gestionar los registros de informacion.
void CDiccionario::menuDatos(){
    int op;
    do{
        printf("\n1.Nuevo 2.Consultar 3.Eliminar 4.Modificar 5.Regresar\n");
        scanf("%d",&op);
        switch(op){
            case 1: nuevoRegistro(); break;
            case 2: consultarRegistro(); break;
            case 3: eliminaRegistro(); break;
            case 4: modificaRegistro(); break;
            case 5: printf("\nRegresando..."); break;
            default: printf("\nOpcion no valida");
        }
    }while(op!=5);
}

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
        printf("Error: La entidad ya existe");
    }
}

// Pido al usuario el nombre de la nueva entidad y configuro todos sus punteros en nulo (-1).
Entidad CDiccionario::capturaEntidad(){
    Entidad ent;
    printf("\nNombre de la entidad: ");
    scanf(" %[^\n]",ent.nombre);
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

    printf("Ingrese nombre: ");
    scanf("%s",nom);

    Entidad aux;
    strcpy(aux.nombre,nom);
    dir=buscaEntidad(aux);

    if(dir==-1) printf("ERROR");
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
        }else return -1;
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
        }else printf("No se puede actualizar");
    }else printf("No existe la entidad");
}

// ---------------- REGISTROS ----------------

// Creo un bloque de memoria para acomodar el registro de un paciente y lo estampo al final del archivo.
void CDiccionario::nuevoRegistro(){
    if(!archivo){
        printf("\nPrimero crea o abre un archivo");
        return;
    }

    int total=(TAM*sizeof(char))+sizeof(int)+sizeof(float)+sizeof(double)+sizeof(long);
    void *paciente=malloc(total);
    char *ptr=(char*)paciente;
    int cont=0;

    printf("\nNombre: ");
    scanf(" %[^\n]",ptr+cont);
    cont+=TAM;

    printf("Edad: ");
    scanf("%d",(int*)(ptr+cont));
    cont+=sizeof(int);

    printf("Peso: ");
    scanf("%f",(float*)(ptr+cont));
    cont+=sizeof(float);

    printf("Estatura: ");
    scanf("%lf",(double*)(ptr+cont));
    cont+=sizeof(double);

    printf("ID: ");
    scanf("%ld",(long*)(ptr+cont));

    fseek(archivo,0,SEEK_END);
    fwrite(paciente,total,1,archivo);

    printf("\nRegistro guardado");
    free(paciente);
}

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
        printf("\n%s|%ld|%ld|%ld\n",actual.nombre,actual.atr,actual.data,actual.sig);
        cab=actual.sig;
    }
}

// ====================== FUNCIONES NUEVAS DE ATRIBUTOS ====================== //

// Checo si hay entidades, pregunto por cual trabajar y la cargo en memoria como mi entidad 'activa'.
bool CDiccionario::activarEntidad(){
    if(getCabEntidades()==-1){
        printf("\nError: No hay entidades registradas.\n");
        return false;
    }
    Entidad temp;
    printf("\nEn cual entidad desea trabajar? ");
    scanf(" %[^\n]",temp.nombre);

    long d=buscaEntidad(temp);
    if(d!=-1){
        diractiva=d;
        activa=leeEntidad(d);
        return true;
    }
    printf("\nError: La entidad no existe.\n");
    return false;
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
    printf("Dame el nombre: ");
    scanf("%s",nuevaA.nombre);
    printf("Ingresa de que tipo es: \n  1)char, 2)int, 3)float, 4)double, 5)long\n");
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
    printf("este atributo es clave primario? (S/N): ");
    scanf(" %c",&nuevaA.lskp);
    printf("permite Nulos? (S/N): ");
    scanf(" %c",&nuevaA.nulo);
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
long CDiccionario::buscaAtributo(char *atr){
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
    while(cab!=-1){
        nvo=leeAtributo(cab);
        printf("%s, %d, %d, %c, %c, %s\n",nvo.nombre,nvo.tipo,nvo.tamano,nvo.lskp,nvo.nulo,nvo.descripcion);
        cab=nvo.sig;
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

// Cascaron en proceso para borrar registros.
void CDiccionario::eliminaRegistro(){
    printf("\nEliminar registro...");
}

// Cascaron en proceso para modificar registros.
void CDiccionario::modificaRegistro(){
    printf("\nModificar registro...");
}

//Funciones de bloques

void *CDiccionario::capturaBloque(){
    void *bloque = malloc(tambloque);
    long desp = 0;

    // inicializar sig en -1
    *(long *)((char *)bloque + desp) = -1;
    desp += sizeof(long);

    for(int i = 0; i < nAtributos; i++){
        switch(arrAtributo[i].tipo){
            case 1: // char/cadena
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

void CDiccionario::reescribeBloque(void *bloque, long dir){
    fseek(archivo, dir, SEEK_SET);
    fwrite(bloque, tambloque, 1, archivo);
}

void CDiccionario::reescribeBloque(void *bloque, long dir)
{
    fseek(arch, dir, SEEK_SET);
    fwrite(bloque, tamBloque, 1, arch);
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

void CDiccionario::eliminaBloque(){
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
    free(bloqueant); free(llave);
}

void CDiccionario::modificaBloque(){
    cargaAtributos();
    void *llave = capturaBloque(); // captura solo KP para buscar

    long dir = buscaBloque(llave);
    if(dir == -1){
        printf("\nError: bloque no encontrado.\n");
        free(llave); return;
    }

    void *bloque = leeBloque(dir);
    long desp = sizeof(long) + arrAtributo[0].tamano; // saltar sig + KP

    for(int i = 1; i < nAtributos; i++){
        printf("Nuevo valor para %s: ", arrAtributo[i].nombre);
        switch(arrAtributo[i].tipo){
            case 1:
                scanf(" %[^\n]", (char *)bloque + desp);
                break;
            case 2:
                scanf("%d", (int *)((char *)bloque + desp));
                break;
            case 3:
                scanf("%f", (float *)((char *)bloque + desp));
                break;
            case 4:
                scanf("%lf", (double *)((char *)bloque + desp));
                break;
            case 5:
                scanf("%ld", (long *)((char *)bloque + desp));
                break;
        }
        desp += arrAtributo[i].tamano;
    }
    reescribeBloque(bloque, dir);
    free(bloque); free(llave);
    printf("\nBloque modificado.\n");
}
