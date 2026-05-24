#ifndef CDICCIONARIO_H
#define CDICCIONARIO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 50
typedef char cadena [30];

typedef struct
{
    long atr;
    long sig;
    cadena nombre;
    long data;
}Entidad;

typedef struct
{
    cadena nombre;
    int tipo;
    int tamano;
    char lskp;
    char nulo;
    char descripcion[50];
    long sig;
}Atributo;

class CDiccionario
{
    public:
        CDiccionario();
        virtual ~CDiccionario();

    void menuPrincipal();

    void nuevoDiccionario();
    void abrirDiccionario();
    void MenuEntidades();
    ///                                                         Funciones Entidad
    void altaEntidad();
    void consultarEntidades();
    void bajaEntidad();
    void modificaEntidad();
    int pideEntidad();

    long getCabEntidades();
    void escribeCabEntidades(long cab);
    Entidad capturaEntidad();
    long buscaEntidad(Entidad ent);
    Entidad leeEntidad(long dir);
    long escribeEntidad(Entidad ent);
    void reescribeEntidad(long dir, Entidad ent);
    void insertaEntidad(Entidad nvo, long dir);
    long eliminaEntidad(cadena nom);
    ///                                                         Funciones publicas de atributos

    void menuAtributos();


    bool activarEntidad();
    Atributo capturaAtributo();
    void altaAtributo();
    long buscaAtributo(char *atr);
    void insertaAtributo(Atributo nvo, long dir);
    void nuevoAtributo();
    void consultarAtributo();
    long eliminaAtributo(char cab[30]);
    void modificaAtributo();
    Atributo leeAtributo(long dir);
    long escribeAtributo(Atributo atr);
    void reescribeAtributo(long dir, Atributo atr);
    void bajaAtributo();

    void menuDatos();

    void nuevoRegistro();
    void consultarRegistro();
    void eliminaRegistro();
    void modificaRegistro();

    ///                                                             Funciones de bloques

    void *capturaBloque();
    void consultaBloques();
    int comparaBloques(void *b1, void *b2);
    void altaBloque();
    void *leeBloque(long dir);
    long escribeBloque(void *bloque);
    void reescribeBloque(void *bloque, long dir);
    void insertaBloque(void *nvo, long dirnvo);
    void eliminaBloque(void *bloq);
    void modificaBloque();
    void cargaAtributos();
    long buscaBloque(void *key);
    void *pidaClaveBloque();


    protected:

    private:
        FILE *archivo;
    Entidad activa;
    long diractiva;
    long tambloque;
    int nAtributos;
    Atributo arrAtributo[50];
};

#endif // CDICCIONARIO_H




