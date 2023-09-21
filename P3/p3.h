
#ifndef P1_P1_H
#define P1_P1_H

#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<dirent.h>
#include<fcntl.h>
#include<sys/utsname.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <stdint.h>
#include <pwd.h>
#include <grp.h>
#include <sys/shm.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <ctype.h>
//P3
#include <sys/resource.h>
#include <signal.h>

#define DIMENSION_ARRAY 4096
#define TAMANO 2048

bool terminado = false;
extern char **environ;
int lastProc;

typedef struct{
    void * direccion;
    size_t tamano;
    struct tm fecha;
}mallocblock;

typedef struct{
    void * direccion;
    size_t tamano;
    struct tm fecha;
    key_t   key;
}sharedblock;

typedef struct{
    void * direccion;
    size_t tamano;
    struct tm fecha;
    char nombre[100];
    int descriptor;
}mappedblock;

typedef struct{
    mallocblock *mallocs[DIMENSION_ARRAY];
    sharedblock *shareds[DIMENSION_ARRAY];
    mappedblock *mappeds[DIMENSION_ARRAY];
}memblock;

typedef struct{
    pid_t pid_num;
    struct tm launching_time;
    char estado[200];
    int senal;
    char comando[500];
}proceso;

struct SEN{
    char * nombre;
    int signum;
};

void imprimirPrompt();
void imprimirAutores(char *trozos[], int numero_trozos);
void imprimirPid(char *trozos[], int numero_trozos);
void cambiarDirectorio(char *trozos[], int numero_trozos);
void imprimirFecha(char *trozos[], int numero_trozos);
void historial(char *trozos[], int numero_trozos, int *numero_comandos,char array_comandos[DIMENSION_ARRAY][200]);
void infosis();
void create(char *trozos[]);
void imprimirAyuda(char *trozos[], int numero_trozos);
void finalizar(char *trozos[]);
void delete(char *trozos[], int numero_trozos);
void stat_fich_dir(char *trozos[], int numero_trozos);
void list(char *trozos[], int numero_trozos);
void deltree(char *trozos[], int numero_trozos);
void procesarEntrada(char *cadena, char *trozos[],int *numero_comandos,char array[DIMENSION_ARRAY][200],int *lastMall,int *lastSha,int *lastMap,char ** env);
void allocate(char *trozos[], int n_trozos, memblock *allocates,int *lastMall,int *lastSha,int *lastMap);
void deallocate(char *trozos[], int n_trozos, memblock allocates,int *lastMall,int *lastSha,int *lastMap);
void i_o(char *trozos[], int n_trozos);
void memfill(char *trozos[],int numero_trozos);
void memdump(char *trozos[],int numero_trozos);
void memory(char * trozos[],int numero_trozos,memblock allocates, int lastMall, int lastSha, int lastMap);
void recurse(char *trozos[],int numero_trozos);
void do_AllocateCreateshared(char *tr[],sharedblock * array[DIMENSION_ARRAY],int * lastSha);
//P3
void priority(char * trozos[],int numero_trozos);
void showvar(char * trozos[],int numero_trozos,char ** env);
void changevar(char * trozos[],int numero_trozos,char ** env);
void showenv(char * trozos[],int numero_trozos,char ** env);
void listjobs(proceso * array[]);
void deljobs(char * trozos[],int numero_trozos,proceso * array[]);
void job(char * trozos[],int numero_trozos,proceso * array[]);
void comando_random(char * cadena,char * trozos[],int n_trozos,proceso *proccarr[]);







//FUNCIONES AUXILIARES
int TrocearCadena(char *cadena, char *trozos[]);
void Insertar_comando(char *cadena, char comandos[DIMENSION_ARRAY][200],  int posicion);
char *ConvierteModo3(mode_t m);
bool existe( char letratf);
int TrocearDireccion(char *cadena, char *trozos[]);
void info_long(struct stat info,char *ruta);
bool opciones_stat(char *opcion);
bool opciones_list(char *opcion);
int BorrarArbol(char *nombre);
void Insertar_allocate_malloc(mallocblock allocate,mallocblock *array[DIMENSION_ARRAY],int last);
void Insertar_allocate_shared(sharedblock allocate,sharedblock *array[DIMENSION_ARRAY],int last);
void Insertar_allocate_mapped(mappedblock allocate,mappedblock *array[DIMENSION_ARRAY],int last);

void LlenarMemoria (void *p, size_t cont, unsigned char byte);
void Recursiva(int n);
void *cadtop(char * cadena);
void VolcarMemoria(void *p, size_t cont);
void Do_pmap(void);
void *MapearFichero(char *fichero, int protection,mappedblock *array[DIMENSION_ARRAY],int * lastMapp);
void do_AllocateMmap(char *arg[],mappedblock *array[DIMENSION_ARRAY],int *lastMap);
void do_DeallocateDelkey(char *args[]);
ssize_t LeerFichero(char *f, void *p, size_t cont);
void do_I_O_read(char *ar[]);
ssize_t EscribirFichero(char *f, void *p, size_t cont, int overwrite);
void ImprimirListaMalloc(mallocblock * array[DIMENSION_ARRAY],int lastMall);
void ImprimirListaMmap(mappedblock * array[DIMENSION_ARRAY],int lastMmap);
void ImprimirListaShared(sharedblock * array[DIMENSION_ARRAY],int lastSha);
void print_Mallocs(mallocblock * array[DIMENSION_ARRAY],int lastMall);
void print_SharedKey(sharedblock  * array[DIMENSION_ARRAY],int lastSha,key_t key);
void print_Mappeds(mappedblock * array[DIMENSION_ARRAY],int lastSha);
void Deallocate_addres(char *direccion,memblock allocates,int lastMall,int lastSha,int lastMap);
//P3
void Cmd_fork ();
void Execute(char * trozos[],int n_trozos);
char * Ejecutable (char *s);
int OurExecvpe(char *file, char *const argv[], char * envp[]);
void insertarproceso(proceso process,proceso *proccarray[]);
int EjecutarBackGround(char *args[],int prio);
int EjecutarBackGroundEntorno(char *args[],char *entorno[],int prio);
int BuscarVariable(char *var, char *e[]);
int BuscarVariableAddrEnv(char *var, char **env);
int BuscarVariableAddrEnviron(char *var);
int CambiarVariable(char *var, char *valor, char *e[]);
void copiarArgs( char * trozos[],char * args[],int inicio,int finargs,int finEnt);
#endif

/*las siguientes funciones nos permiten obtener el nombre de una senal a partir
del número y viceversa*/

struct SEN sigstrnum[] = {
        {"HUP", SIGHUP},
        {"INT", SIGINT},
        {"QUIT", SIGQUIT},
        {"ILL", SIGILL},
        {"TRAP", SIGTRAP},
        {"ABRT", SIGABRT},
        {"IOT", SIGIOT},
        {"BUS", SIGBUS},
        {"FPE", SIGFPE},
        {"KILL", SIGKILL},
        {"USR1", SIGUSR1},
        {"SEGV", SIGSEGV},
        {"USR2", SIGUSR2},
        {"PIPE", SIGPIPE},
        {"ALRM", SIGALRM},
        {"TERM", SIGTERM},
        {"CHLD", SIGCHLD},
        {"CONT", SIGCONT},
        {"STOP", SIGSTOP},
        {"TSTP", SIGTSTP},
        {"TTIN", SIGTTIN},
        {"TTOU", SIGTTOU},
        {"URG", SIGURG},
        {"XCPU", SIGXCPU},
        {"XFSZ", SIGXFSZ},
        {"VTALRM", SIGVTALRM},
        {"PROF", SIGPROF},
        {"WINCH", SIGWINCH},
        {"IO", SIGIO},
        {"SYS", SIGSYS},
//senales que no hay en todas partes
#ifdef SIGPOLL
        {"POLL", SIGPOLL},
#endif
#ifdef SIGPWR
        {"PWR", SIGPWR},
#endif
#ifdef SIGEMT
        {"EMT", SIGEMT},
#endif
#ifdef SIGINFO
        {"INFO", SIGINFO},
#endif
#ifdef SIGSTKFLT
        {"STKFLT", SIGSTKFLT},
#endif
#ifdef SIGCLD
        {"CLD", SIGCLD},
#endif
#ifdef SIGLOST
        {"LOST", SIGLOST},
#endif
#ifdef SIGCANCEL
        {"CANCEL", SIGCANCEL},
#endif
#ifdef SIGTHAW
        {"THAW", SIGTHAW},
#endif
#ifdef SIGFREEZE
        {"FREEZE", SIGFREEZE},
#endif
#ifdef SIGLWP
        {"LWP", SIGLWP},
#endif
#ifdef SIGWAITING
        {"WAITING", SIGWAITING},
#endif
        {NULL, -1},
};    //fin array sigstrnum

