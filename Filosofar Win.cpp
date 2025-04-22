// Filosofar Win.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//
#include "filosofar2.h"
#include <iostream>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <tchar.h>
//para pasar a las funciones de semaforos
#define SEMTENIZ 1
#define SEMTEDER 2
#define SEMANTESALA 3
#define SEMMEMCSITIOS 4
//faltan mas a definir
//Definiciones de las Funciones 
typedef int(__cdecl* TFI2_inicio)(int, unsigned long long, struct DatosSimulaciOn*, int const*);
typedef int(__cdecl* TFI2_inicioFilOsofo)(int);
typedef int(__cdecl* TFI2_pausaAndar)(void);
typedef int(__cdecl* TFI2_puedoAndar)(void);
typedef int(__cdecl* TFI2_aDondeVoySiAndo)(int*, int*);
typedef int(__cdecl* TFI2_andar)(void);
typedef int(__cdecl* TFI2_entrarAlComedor)(int);
typedef int(__cdecl* TFI2_cogerTenedor)(int);
typedef int(__cdecl* TFI2_comer)(void);
typedef int(__cdecl* TFI2_dejarTenedor)(int);
typedef int(__cdecl* TFI2_entrarAlTemplo)(int);
typedef int(__cdecl* TFI2_meditar)(void);
typedef int(__cdecl* TFI2_finFilOsofo)(void);
typedef int(__cdecl* TFI2_fin)(void);
typedef void(__cdecl* Tpon_error)(char*);
//Declaracion de punteros Globales
TFI2_inicio FI2_inicio;
TFI2_inicioFilOsofo FI2_inicioFilOsofo;
TFI2_pausaAndar FI2_pausaAndar;
TFI2_puedoAndar FI2_puedoAndar;
TFI2_andar FI2_andar;
TFI2_entrarAlComedor FI2_entrarAlComedor;
TFI2_cogerTenedor FI2_cogerTenedor;
TFI2_comer FI2_comer;
TFI2_dejarTenedor FI2_dejarTenedor;
TFI2_entrarAlTemplo FI2_entrarAlTemplo;
TFI2_meditar FI2_meditar;
TFI2_finFilOsofo FI2_finFilOsofo;
TFI2_fin FI2_fin;
Tpon_error pon_error;
TFI2_aDondeVoySiAndo FI2_aDondeVoySiAndo;

//Declaracion el Struct con los parametros
typedef struct param{
    int numF, numV, vel;
    unsigned long long clave = 28545168465992;
}Parametros;
Parametros parametrosGlob = { 0 };
//esto al hacer un struct es como un POO de Java, asi todo lo del puente lo tengo aqui
typedef struct puente{
    HANDLE hsem = nullptr;
    LONG aforo = LONG_MAX;
    //Crear el semaforo con el aforo de la dss
    bool iniciarSem(int aforoM) 
    {
        if (aforoM > 0)
        {
            aforo = aforoM;
        }
        hsem = CreateSemaphore(nullptr, aforo, aforo, TEXT("SemPuente"));
        if (!hsem)
        {
            std::cerr << "[!]\t Error en la creacion de SemPuente";
            return false;
        }
        return true;
    }
    //elimiar
    void eliminarSem() 
    {
        if (hsem)
        {
            CloseHandle(hsem);
            hsem = nullptr;
        }
    }
    //falta decremntar aumentar y comprobar cosas
}puente;
puente P;

//Carga de la DLL
HMODULE cargarDLL() {
    HMODULE dll = LoadLibraryA("filosofar2.dll");
    if (!dll)
    {
        std::cerr << "[!]\tError al cargar la DLL con codigo : " << GetLastError() << " \n";
        return NULL;
        
    }
    else { return dll;}
}
//Lectura de argumentos
void leerArgs(int argc, char* argv[]) {
    if (argc!=4)
    {
        std::cerr << "[!]\tSe requieren 3 parametros para el funcionamiento del programa" << std::endl;
        exit(1);
    }
    if (atoi(argv[1])<1 || atoi(argv[1]) > 64)
    {
        std::cerr << "[!]\tDebe de haber entre 1 y 21 filosofos" << std::endl;
        exit(1);
    }
    if (atoi(argv[2])<1)
    {
        std::cerr << "[!]\tLas vueltas no pueden ser negativas" << std::endl;
        exit(1);
    }if (atoi(argv[3])<0)
    {
        std::cerr << "[!]\tLa velocidad minima es 0" << std::endl;
        exit(1);
    }
    parametrosGlob.numF = std::atoi(argv[1]);
    parametrosGlob.numV = std::atoi(argv[2]);
    parametrosGlob.vel = std::atoi(argv[3]);
    std::cout << "Numero de filosofos: " << parametrosGlob.numF << std::endl;
    std::cout << "Numero Vueltas: " << parametrosGlob.numV << std::endl;
    std::cout << "Velocidad: " << parametrosGlob.vel << std::endl;
}
//Funcion de Carga
int cargarFunciones(HMODULE dll) {
    FI2_inicio = (TFI2_inicio)GetProcAddress(dll, "FI2_inicio");
    FI2_inicioFilOsofo = (TFI2_inicioFilOsofo)GetProcAddress(dll, "FI2_inicioFilOsofo");
    FI2_pausaAndar = (TFI2_pausaAndar)GetProcAddress(dll, "FI2_pausaAndar");
    FI2_puedoAndar = (TFI2_puedoAndar)GetProcAddress(dll, "FI2_puedoAndar");
    FI2_andar = (TFI2_andar)GetProcAddress(dll, "FI2_andar");
    FI2_entrarAlComedor = (TFI2_entrarAlComedor)GetProcAddress(dll, "FI2_entrarAlComedor");
    FI2_cogerTenedor = (TFI2_cogerTenedor)GetProcAddress(dll, "FI2_cogerTenedor");
    FI2_comer = (TFI2_comer)GetProcAddress(dll, "FI2_comer");
    FI2_dejarTenedor = (TFI2_dejarTenedor)GetProcAddress(dll, "FI2_dejarTenedor");
    FI2_entrarAlTemplo = (TFI2_entrarAlTemplo)GetProcAddress(dll, "FI2_entrarAlTemplo");
    FI2_meditar = (TFI2_meditar)GetProcAddress(dll, "FI2_meditar");
    FI2_finFilOsofo = (TFI2_finFilOsofo)GetProcAddress(dll, "FI2_finFilOsofo");
    FI2_fin = (TFI2_fin)GetProcAddress(dll, "FI2_fin");
    pon_error = (Tpon_error)GetProcAddress(dll, "pon_error");
    FI2_aDondeVoySiAndo = (TFI2_aDondeVoySiAndo)GetProcAddress(dll, "FI2_aDondeVoySiAndo");

    if (!FI2_inicio || !FI2_comer || !FI2_fin) {
        std::cerr << "[!]\tError al obtener una o más funciones clave.\n";
        return 1;
    }
    else {
        return 0;
    }
}
static HANDLE* sems = NULL;
static int semCont = 0;
//funcion para crear un array de semaforos, lo unico que todos con los mismos huecos y estados iniciales
bool crearSems(int cont, LONG ini, LONG max) {
    sems = (HANDLE*)malloc(sizeof(HANDLE) * cont);
    if (!sems)
    {
        return false;
    }
    semCont = cont;
    for (int i = 0; i < semCont; i++)
    {
        sems[i] = CreateSemaphore(NULL, ini, max, NULL);
        if (!sems[i])
        {
            return false;
            //error
        }
    }
    return true;
}
//le pasas cuantos signal quieres hacer en un semaforo y que semaforo
bool levantarSem(int cuantas, int semId) {
    if (!ReleaseSemaphore(sems[semId],cuantas,NULL))
    {
        return false;
    }
    return true;
}
// le pasas cuantos waits quieres hacer y que semaforo
bool bloquearSem(int cuantas, int semId) {
    DWORD res;
    for (int i = 0; i < cuantas; i++)
    {
        res = WaitForSingleObject(sems[semId], INFINITE);
        if (res != WAIT_OBJECT_0)
        {
            //he intentado decrementar mas de la cuenta, se supone que esto no tiene que fallar ya que somos nosotros los que le pasamos los parametros
        }
    }
    return true;
}
//Funcion que evita la semiespera
//No se que hacer
int esperaYanda(int id) {
   
    int x,y;
    while(1){
        FI2_pausaAndar();
        if (FI2_puedoAndar()==100)
        {
            if (FI2_aDondeVoySiAndo(&x,&y)!=0)
            {
                return -1;
            }
            return FI2_andar();
        }
        Sleep(0);
    }
}
//Funcion del HILO ( filosofo )
DWORD WINAPI hiloF(LPVOID lpParam){
    int id = (int)(intptr_t)lpParam;
    FI2_inicioFilOsofo(id);
    int zona=-1,zonaAnt=-1, vueltas=0,temp;
    while(vueltas < parametrosGlob.numV)
    {
       
        zonaAnt = zona;
        zona = esperaYanda(id);
        switch (zona) {
        case ENTRADACOMEDOR:
            if (zonaAnt == ANTESALA)
            {
                FI2_entrarAlComedor(id);
            }
            break;
        case SILLACOMEDOR:
            FI2_cogerTenedor(TENEDORIZQUIERDO);
            FI2_cogerTenedor(TENEDORDERECHO);
            while(FI2_comer()==SILLACOMEDOR);
            FI2_dejarTenedor(TENEDORIZQUIERDO);
            FI2_dejarTenedor(TENEDORDERECHO);
            break;
        case SALIDACOMEDOR:
            
            break;
        case TEMPLO:
            if (zonaAnt == CAMPO)
            {
                FI2_entrarAlTemplo(id);
            }break;
        case SITIOTEMPLO:       
            while (FI2_meditar() == SITIOTEMPLO);
            parametrosGlob.numV++;

            break;
            
        case PUENTE:
            //Se usan IPCS aqui
        case CAMPO:
        }
        
    }
    FI2_finFilOsofo();
    return 0;
}


int main(int argc, char* argv[])
{
    HMODULE dll = cargarDLL();
    if (cargarFunciones(dll)==1)
    {
        FreeLibrary(dll);
        return 1;
    }
    leerArgs(argc, argv);
    int dE[]{
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0,0
    };
    DatosSimulaciOn dss;
    int res = FI2_inicio(parametrosGlob.vel,parametrosGlob.clave,&dss,dE);
    if (!P.iniciarSem(dss.maxFilOsofosEnPuente))
    {
        return 1;
    }
    if (res != 0)
    {
        std::cerr << "[!]\t Error al iniciar la simulacion de codigo: " << res << "\n";
        FreeLibrary(dll);

    }
    else {
        std::vector<HANDLE>hilos;
        for ( int i = 0; i < parametrosGlob.numF; i++)
        {
            DWORD threadId;
            HANDLE h = CreateThread(
                NULL,
                0,
                hiloF,
                (LPVOID)(intptr_t)i,
                0,
                &threadId
            );
            if (!h)
            {
                std::cerr << "[!]\tError creando hilo para el filosofo:" << i << std::endl;
                continue;
            }
            hilos.push_back(h);
        }
        WaitForMultipleObjects(hilos.size(), hilos.data(), TRUE, INFINITE);
        for (HANDLE h : hilos)CloseHandle(h);
    }
    P.eliminarSem();
    FI2_fin();
    FreeLibrary(dll);
    return 0;
}

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
