/* AUTHOR 1: Álvaro Pazos García LOGIN 1: alvaro.pazosg@udc.es
* AUTHOR 2: Aaron Eiroa López LOGIN 2:  aaron.eiroa@udc.es
* GROUP: 1.3
*/

#include "p3.h"

void imprimirPrompt() {
    printf("-> ");
}

void leer2(char *cad) {
    fgets(cad, PATH_MAX, stdin);
}

int TrocearCadena(char *cadena, char *trozos[]) {
    int i = 1;

    if ((trozos[0] = strtok(cadena, " \n\t")) == NULL)
        return 0;
    while ((trozos[i] = strtok(NULL, " \n\t")) != NULL)
        i++;
    return i;
}

int TrocearCadenaComillas(char *cadena, char *trozos[]) {
    int i = 1;

    if ((trozos[0] = strtok(cadena, " ' ")) == NULL)
        return 0;
    while ((trozos[i] = strtok(NULL, " ' ")) != NULL)
        i++;
    return i;
}

void imprimirAutores(char *trozos[], int numero_trozos) {

    if (numero_trozos == 1) {
        printf("Alvaro Pazos Garcia: alvaro.pazosg@udc.es \nAaron Eiroa Lopez: aaron.eiroa@udc.es\n");
    } else if (numero_trozos > 1) {
        if (strcmp(trozos[1], "-l") == 0) {
            printf("alvaro.pazosg@udc.es \naaron.eiroa@udc.es\n");
        } else if (strcmp(trozos[1], "-n") == 0) {
            printf("Alvaro Pazos Garcia \nAaron Eiroa Lopez\n");
        }
    }
}

void imprimirPid(char *trozos[], int numero_trozos) {
    if (numero_trozos == 1)
        printf("Pid de shell: %d\n", getpid());
    else if (numero_trozos > 1) {
        if (strcmp(trozos[1], "-p") == 0) {
            printf("Pid del padre del shell: %d\n", getppid());
        }
    }
}

void cambiarDirectorio(char *trozos[], int numero_trozos) {
    char nombre_ruta[PATH_MAX];
    if (numero_trozos == 1) {
        getcwd(nombre_ruta, PATH_MAX);
        printf("%s\n", nombre_ruta);
    } else {
        if (!chdir(trozos[1])) {
            return;
        } else {
            perror("No se pudo cambiar de directorio");
        }
    }
}

void imprimirFecha(char *trozos[], int numero_trozos) {

    time_t hora = time(NULL);
    struct tm fecha = *localtime(&hora);
    if (strcmp(trozos[0], "fecha") == 0) {
        if (numero_trozos == 1) {
            printf("%d:%d:%d\n%d/%d/%d\n", fecha.tm_hour, fecha.tm_min, fecha.tm_sec,
                   fecha.tm_mday, fecha.tm_mon + 1, fecha.tm_year + 1900);
        } else if (numero_trozos > 1) {
            if (strcmp(trozos[1], "-d") == 0)
                printf("%d/%d/%d\n", fecha.tm_mday, fecha.tm_mon + 1, fecha.tm_year + 1900);
            if (strcmp(trozos[1], "-h") == 0)
                printf("%d:%d:%d\n", fecha.tm_hour, fecha.tm_min, fecha.tm_sec);
        }
    }
}

void Insertar_comando(char *cadena, char comandos[DIMENSION_ARRAY][200], int posicion) {
    if (posicion > DIMENSION_ARRAY)return;
    strcpy(comandos[posicion], cadena);
}

void historial(char *trozos[], int numero_trozos, int *numero_comandos, char array_comandos[DIMENSION_ARRAY][200]) {
    int N, i, j;
    if (*numero_comandos > DIMENSION_ARRAY) {
        perror("Limite de comandos alcanzado");
        return;
    }
    if (numero_trozos > 1) {
        if (strcmp(trozos[1], "-c") == 0) {
            for (i = 0; i <= *numero_comandos; i++) {
                for (j = 0; j < 100; j++) {
                    strcpy(&array_comandos[i][j], "\0");
                    *numero_comandos = -1;
                }
            }
            *numero_comandos = 0;
            return;
        } else {
            //Pasa el -N a un entero positivo con la funcion atoi para pasar de char a int
            N = (-1) * atoi(trozos[1]);
            for (i = 0; i < N && array_comandos[i] != NULL; i++) {
                printf("%d -> %s\n", i, array_comandos[i]);
            }
            return;
        }

    }
    if (numero_trozos == 1) {
        for (i = 0; i <= *numero_comandos; i++) {
            printf("%d -> %s\n", i, array_comandos[i]);
        }

    } else printf("Use el comando: 'ayuda hist' para obtener informarcion sobre el comando\n");
}

char *comando_N(char *trozos[], int numero_trozos, int numero_comandos, char comandos[DIMENSION_ARRAY][200]) {
    unsigned int comandoN;
    if (numero_trozos == 2) {
        comandoN = atoi(trozos[1]);
        if (comandoN > numero_comandos) {
            perror("Posicion no valida");
            return "ayuda comando";
        }
        printf("Ejecutando comando %d:\n", comandoN);
        return comandos[comandoN];
    }
    return "ayuda comando";
}

void infosis() {
    struct utsname datos;
    uname(&datos);
    printf("Usuario:%s   SO:%s   Kernel:%s   Arquitectura:%s   Kernel version:%s\n", datos.nodename, datos.sysname,
           datos.release, datos.machine, datos.version);
}

void create(char *trozos[]) {
    char nuevo_dir[100], nuevo_fic[100];
    strcpy(nuevo_dir, trozos[1]);
    strcpy(nuevo_fic, trozos[2]);
    //Crear fichero
    if (strcmp(nuevo_dir, "-f") == 0) {
        if (open(nuevo_fic, O_RDWR | O_CREAT | O_TRUNC, 0777) < 0) perror("No se pudo crear el fichero");
    }
        //Crear directorio
    else if (mkdir(nuevo_dir, 0777) < 0) perror("No se pudo crear el directorio");
}

void imprimirAyuda(char *trozos[], int numero_trozos) {

    if (strcmp(trozos[0], "ayuda") == 0) {
        if (numero_trozos == 1) {
            printf("'ayuda cmd' donde cmd es uno de los siguientes comandos:\n"
                   "fin salir bye fecha pid autores hist comando carpeta infosis ayuda\n"
                   "create stat list delete deltree\n"
                   "allocate deallocate i-o memdump memfill memory recurse \n"
                   "priority showvar changevar showenv fork execute listjobs deljobs job\n");

        } else if (numero_trozos > 1) {
            if (strcmp(trozos[1], "autores") == 0) {
                printf("autores [-n|-l]	Muestra los nombres y logins de los autores\n");
            }
            if (strcmp(trozos[1], "pid") == 0) {
                printf("pid [-p]: Muestra el pid del shell o de su proceso padre\n");
            }
            if (strcmp(trozos[1], "carpeta") == 0) {
                printf("carpeta [dir]: Cambia (o muestra) el directorio actual del shell\n");
            }
            if (strcmp(trozos[1], "fecha") == 0) {
                printf("fecha [-d|-h]: Muestra la fecha y o la hora actual\n");
            }
            if (strcmp(trozos[1], "hist") == 0) {
                printf("hist [-c|-N]: Muestra el historico de comandos, con -c lo borra\n");
            }
            if (strcmp(trozos[1], "comando") == 0) {
                printf("comando [-N]: Repite el comando N (del historico)\n");
            }
            if (strcmp(trozos[1], "infosis") == 0) {
                printf("infosis: Muestra informacion de la maquina donde corre el shell\n");
            }
            if (strcmp(trozos[1], "hist") == 0) {
                printf("hist [-c|-N]:	Muestra (o borra)el historico de comandos\n"
                       "-N: muestra los N primeros\n"
                       "-c: borra el historico\n");
            }
            if (strcmp(trozos[1], "create") == 0) {
                printf("create [-f] [name]:	Crea un directorio o un fichero (-f)\n");
            }
            if (strcmp(trozos[1], "delete") == 0) {
                printf("delete [name1 name2 ..]: Borra ficheros o directorios vacios\n");
            }
            if (strcmp(trozos[1], "stat") == 0) {
                printf("stat [-long][-link][-acc] name1 name2 ..\tlista ficheros\n\t-long: listado largo\n\t-acc: acesstime\n\t-link:"
                       "si es enlace simbolico, el path contenido\n");
            }
            if (strcmp(trozos[1], "list") == 0) {
                printf("list [-reca] [-recb] [-hid][-long][-link][-acc] n1 n2 ..\tlista contenidos de directorios"
                       "\n\t-hid: incluye los ficheros ocultos\n\t-reca: recursivo (antes)"
                       "\n\t-recb: recursivo (despues)\n\tresto parametros como stat\n");
            }
            if (strcmp(trozos[1], "deltree") == 0) {
                printf("deltree [name1 name2 ..]: Borra ficheros o directorios no vacios recursivamente");
            }
            if (strcmp(trozos[1], "allocate") == 0) {
                printf("allocate [-malloc|-shared|-createshared|-mmap] ...	Asigna un bloque de memoria\n"
                       "\t-malloc size asigna un bloque malloc de tamano tam\n"
                       "\t-createshared cl tam: asigna (creando) el bloque de memoria compartida de clave cl y tamano tam\n"
                       "\t-shared cl: asigna el bloque de memoria compartida de clave cl\n"
                       "\t-mmap fich perm: mapea el fichero fich, perm son los permisos\n");
            }
            if (strcmp(trozos[1], "deallocate") == 0) {
                printf("deallocate [-malloc|-shared|-mmap]..	Desasigna un bloque de memoria\n"
                       "\t-malloc size desasigna el bloque malloc de tamano tam\n"
                       "\t-shared cl: desasigna el bloque de memoria compartida de clave cl\n"
                       "\t-mmap fich: desmapea el fichero mapeado fich\n"
                       "\taddr: desasigna el bloque de memoria en la direccion addr\n");
            }
            if (strcmp(trozos[1], "i-o") == 0) {
                printf("i-o [read|write] [-o] fiche addr cont 	Lee (escribe) cont bytes desde (a) fiche a (desde) addr\n");
            }

            if (strcmp(trozos[1], "recurse") == 0) {
                printf("recurse [n]	Invoca a la funcion recursiva n veces\n");
            }
            if (strcmp(trozos[1], "memory") == 0) {
                printf("memory [-blocks|-funcs|-vars|-all|-pmap] ..	Muestra muestra detalles de la memoria del proceso\n"
                       "\t-blocks: los bloques de memoria asignados\n"
                       "\t-funcs: las direcciones de las funciones\n"
                       "\t-vars: las direcciones de las variables\n"
                       "\t:-all: todo\n"
                       "\t-pmap: muestra la salida del comando pmap(o similar)\n");
            }
            if (strcmp(trozos[1], "memdump") == 0) {
                printf("memdump addr cont 	Vuelca en pantallas los contenidos (cont bytes) de la posicion de memoria addr\n");

            }
            if (strcmp(trozos[1], "memfill") == 0) {
                printf("memfill addr cont byte Llena la memoria a partir de addr con byte\n");
            }
            if (strcmp(trozos[1], "priority") == 0) {
                printf("priority [pid] [valor] 	Muestra o cambia la prioridad del proceso pid a valor\n");
            }
            if (strcmp(trozos[1], "showvar") == 0) {
                printf("showvar var1	Muestra el valor y las direcciones de la variable de entorno var\n");
            }
            if (strcmp(trozos[1], "changevar") == 0) {
                printf("changevar [-a|-e|-p] var valor Cambia el valor de una variable de entorno"
                       "\n-a: accede por el tercer arg de main"
                       "\n-e: accede mediante environ"
                       "\n-p: accede mediante putenv\n");
            }
            if (strcmp(trozos[1], "showenv") == 0) {
                printf("showenv [-environ|-addr] 	 Muestra el entorno del proceso\n");
            }
            if (strcmp(trozos[1], "fork") == 0) {
                printf("fork 	Hace una llamada fork para crear un proceso\n");
            }
            if (strcmp(trozos[1], "execute") == 0) {
                printf("execute prog args....	Ejecuta, sin crear proceso,prog con argumentos\n");
            }
            if (strcmp(trozos[1], "listjobs") == 0) {
                printf("listjobs 	Lista los procesos en segundo plano\n");
            }
            if (strcmp(trozos[1], "deljobs") == 0) {
                printf("deljobs [-term][-sig]	Elimina los procesos de la lista procesos en sp\n"
                       "\t-term: los terminados\n"
                       "\t-sig: los terminados por senal\n");
            }
            if (strcmp(trozos[1], "job") == 0) {
                printf("job [-fg] pid	Muestra informacion del proceso pid."
                       "\n-fg: lo pasa a primer plano\n");
            }

            if ((strcmp(trozos[1], "fin") == 0) || (strcmp(trozos[1], "salir") == 0) ||
                (strcmp(trozos[1], "bye") == 0)) {
                printf("%s: Termina la ejecucion del shell\n", trozos[1]);
            }
        }
    }
}

void finalizar(char *trozos[]) {
    if (strcmp(trozos[0], "bye") == 0 || strcmp(trozos[0], "salir") == 0 || strcmp(trozos[0], "fin") == 0)
        terminado = true;
}

char LetraTF(mode_t m) {

    switch (m & S_IFMT) { //and bit a bit con los bits de formato,0170000
        case S_IFSOCK:
            return 's'; //socket
        case S_IFLNK:
            return 'l'; //symbolic link
        case S_IFREG:
            return '-'; // fichero normal
        case S_IFBLK:
            return 'b'; //block device
        case S_IFDIR:
            return 'd'; //directorio
        case S_IFCHR:
            return 'c'; //char device
        case S_IFIFO:
            return 'p'; //pipe
        default:
            return '?'; //desconocido, no deberia aparecer
    }
}

char *ConvierteModo3(mode_t m) {
    char *permisos;

    if ((permisos = (char *) malloc(12)) == NULL)
        return NULL;
    strcpy(permisos, "---------- ");

    permisos[0] = LetraTF(m);
    if (m & S_IRUSR) permisos[1] = 'r';    /*propietario*/
    if (m & S_IWUSR) permisos[2] = 'w';
    if (m & S_IXUSR) permisos[3] = 'x';
    if (m & S_IRGRP) permisos[4] = 'r';    /*grupo*/
    if (m & S_IWGRP) permisos[5] = 'w';
    if (m & S_IXGRP) permisos[6] = 'x';
    if (m & S_IROTH) permisos[7] = 'r';    /*resto*/
    if (m & S_IWOTH) permisos[8] = 'w';
    if (m & S_IXOTH) permisos[9] = 'x';
    if (m & S_ISUID) permisos[3] = 's';    /*setuid, setgid y stickybit*/
    if (m & S_ISGID) permisos[6] = 's';
    if (m & S_ISVTX) permisos[9] = 't';

    return permisos;
}

void delete(char *trozos[], int numero_trozos) {
    int n = 1;
    char ruta[100];
    char letratf;

    struct stat info;

    while (n < numero_trozos) {
        stat(trozos[n], &info);
        letratf = LetraTF(info.st_mode);
        strcpy(ruta, trozos[n]);//nombre de fichero

        if (letratf == '-') {//Es un fichero normal
            if (unlink(ruta) != 0) {
                perror("No se pudo eliminar el fichero");
            }
        } else if (letratf == 'd') {//Es un directorio
            if (rmdir(ruta) < 0) {
                perror("No se pudo eliminar el directorio");
            }
        }
        n++;
    }
}

bool existe(const char letratf) {

    if (letratf == '?') {
        return false;
    } else {
        return true;
    }

}

int TrocearDireccion(char *cadena, char *trozos[]) {
    int i = 1;

    if ((trozos[0] = strtok(cadena, "/")) == NULL)
        return 0;
    while ((trozos[i] = strtok(NULL, "/")) != NULL)
        i++;
    return i;
}

void info_long(struct stat info, char *ruta) {

    struct passwd user;
    struct group group;
    char perm[12];
    mode_t permisos;
    permisos = info.st_mode;
    strcpy(perm, ConvierteModo3(permisos));
    user = *getpwuid(info.st_uid);
    group = *getgrgid(info.st_gid);
    getgrgid(info.st_gid);
    printf("Ultimo acceso:%s"
           "%ld    (%ld)\t%s\t%s\t%s\t%ld\t%s\n", ctime((const time_t *) &info.st_atime),
           (long) info.st_nlink, (long) info.st_ino,
           user.pw_name, group.gr_name,
           perm, (long) info.st_size, ruta);
}

bool opciones_stat(char *opcion) {
    return (strcmp(opcion, "-long") == 0 || strcmp(opcion, "-link") == 0 || strcmp(opcion, "-acc") == 0);

}

void stat_fich_dir(char *trozos[], int numero_trozos) {
    int i = 1, aux;
    char ruta[100], letratf, dir_act[PATH_MAX], *opcion, real[PATH_MAX], *path[PATH_MAX], *result;
    struct stat info;

    opcion = trozos[1];
    if (numero_trozos == 1) {
        getcwd(dir_act, PATH_MAX);
        printf("%s\n", dir_act);
    } else {
        while (i < numero_trozos) {
            stat(trozos[i], &info);
            letratf = LetraTF(info.st_mode);

            strncpy(ruta, trozos[i], strlen(trozos[i]) + 1);//nombre de fichero

            if (existe(letratf)) {

                if (letratf == '-' && !opciones_stat(opcion)) {//Es un fichero normal
                    printf("   %ld %s\n", info.st_size, ruta);
                }
                if (letratf == 'd' && !opciones_stat(opcion)) {//Es un directorio
                    printf("   %ld %s\n", info.st_size, ruta);
                }
                if (strcmp(opcion, "-long") == 0) {
                    info_long(info, ruta);


                }
                if (strcmp(opcion, "-link") == 0) {
                    if (letratf == 'l') {
                        result = realpath(trozos[i], real);
                        aux = TrocearDireccion(result, path);

                        //Printea la parte de long
                        info_long(info, ruta);
                        //Printea el peso del link el nombre del link y a dnd apunta
                        printf(" -> %s\n", path[aux - 1]);
                        break;
                    }
                }
                if (strcmp(opcion, "-acc") == 0) {
                    printf("   %ld %s\n", info.st_size, ruta);
                }

            } else {
                if (!opciones_stat(ruta)) {
                    printf("****error al acceder a %s:\n", ruta);
                    perror("");
                } else if (opciones_stat(ruta) &&
                           numero_trozos - 1 == i) {//si el argumento es el ultimo trozo solo se pone la direccion
                    getcwd(dir_act, PATH_MAX);
                    printf("%s\n", dir_act);
                    i = numero_trozos;

                }
            }
            i++;
        }
    }
}

bool opciones_list(char *opcion) {
    return (strcmp(opcion, "-hid") == 0 || strcmp(opcion, "-reca") == 0 ||
            strcmp(opcion, "-recb") == 0 || strcmp(opcion, "-long") == 0 ||
            strcmp(opcion, "-link") == 0 || strcmp(opcion, "-acc") == 0);
}

void list(char *trozos[], int numero_trozos) {

    DIR *dir;
    struct dirent *cont_dir;
    struct stat info;
    char actual[PATH_MAX], letratf;
    char opcion[6];
    int i = 1;

    strcpy(opcion, trozos[1]);

    if (numero_trozos == 1) {//introduce solo el comando list o introduce mas comandos
        getcwd(actual, PATH_MAX);
        printf("%s\n", actual);
    } else {

        while (i < numero_trozos) {

            stat(trozos[i], &info);
            letratf = LetraTF(info.st_mode);
            if (existe(letratf)) {

                if (!opciones_list(trozos[1]) &&
                    letratf == '-') {//archivos que existen y no son directorios ni argumentos
                    printf("\t%ld  %s\n", info.st_size, trozos[i]);
                    // i = numero_trozos;

                } else if (letratf == 'd' &&
                           !opciones_list(trozos[1])) {//existe y es un directorio y no va precedido de un argumento

                    if ((dir = opendir(trozos[i])) == NULL) {
                        perror("No se pudo acceder al directorio");
                    } else {

                        printf("*********%s\n", trozos[i]);
                        while ((cont_dir = readdir(dir)) != NULL) {
                            stat(cont_dir->d_name, &info);
                            if (strcmp(cont_dir->d_name, ".") != 0 && strcmp(cont_dir->d_name, "..") != 0) {
                                printf("\t%jd  %s\n", (intmax_t) info.st_size, cont_dir->d_name);
                            }

                        }
                    }
                } else { //tienen argumento
                    if (strcmp(opcion, "-long") == 0) {

                        letratf = LetraTF(info.st_mode);//hacer letratf de trozos i +1
                        if (letratf == 'd') {
                            dir = opendir(trozos[i]);
                            while ((cont_dir = readdir(dir)) != NULL) {
                                stat(cont_dir->d_name, &info);
                                info_long(info, actual);

                            }
                            closedir(dir);
                        } else {
                            stat(cont_dir->d_name, &info);
                            info_long(info, actual);
                        }
                    }
                    if (strcmp(opcion, "-link") == 0) {

                        if (letratf == 'd') {

                            dir = opendir(trozos[i]);

                            printf("*********%s\n", trozos[i]);
                            while ((cont_dir = readdir(dir)) != NULL) {
                                stat(cont_dir->d_name, &info);

                                if (strcmp(cont_dir->d_name, ".") != 0 && strcmp(cont_dir->d_name, "..") != 0) {
                                    printf("\t%jd  %s\n", (intmax_t) info.st_size, cont_dir->d_name);
                                }

                            }
                            closedir(dir);
                        } else {

                            printf("\t%ld  %s\n", info.st_size, trozos[i]);
                        }

                    }
                    if (strcmp(opcion, "-acc") == 0) {

                        if (letratf == 'd') {

                            dir = opendir(trozos[i]);

                            printf("*********%s\n", trozos[i]);
                            while ((cont_dir = readdir(dir)) != NULL) {
                                stat(cont_dir->d_name, &info);

                                if (strcmp(cont_dir->d_name, ".") != 0 && strcmp(cont_dir->d_name, "..") != 0) {
                                    printf("\t%jd  %s\n", (intmax_t) info.st_size, cont_dir->d_name);
                                }

                            }
                            closedir(dir);
                        } else {

                            printf("\t%ld  %s\n", info.st_size, trozos[i]);
                        }

                    }
                    if (strcmp(opcion, "-reca") == 0) {
                        printf("*********%s\n", trozos[i + 1]);

                        while ((cont_dir = readdir(dir)) != NULL) {

                            stat(cont_dir->d_name, &info);
                            letratf = LetraTF(info.st_mode);
                            printf("\t%ld  %s\n", info.st_size, cont_dir->d_name);


                        }

                        closedir(dir);


                    }
                    if (strcmp(opcion, "-recb") == 0) {

                    }
                    if (strcmp(opcion, "-hid") == 0) {


                        if (letratf == 'd') {

                            dir = opendir(trozos[i]);

                            printf("*********%s\n", trozos[i]);
                            while ((cont_dir = readdir(dir)) != NULL) {
                                stat(cont_dir->d_name, &info);
                                printf("\t%ld  %s\n", info.st_size, cont_dir->d_name);


                            }
                            closedir(dir);
                        } else {
                            stat(trozos[i], &info);
                            printf("\t%ld  %s\n", info.st_size, trozos[i]);
                        }
                    }

                }
            } else {//introduce un archivo no existente
                if (!opciones_list(trozos[i])) {
                    printf("****error al acceder a %s:\n", trozos[i]);
                    perror("");

                } else if ((opciones_list(trozos[i]) && numero_trozos - 1 == i) ||
                           opciones_list(trozos[numero_trozos - 1])) {

                    getcwd(actual, PATH_MAX);
                    printf("%s\n", actual);
                    i = numero_trozos;
                }
            }
            i++;
        }
    }

}

int BorrarArbol(char *nombre) {

    DIR *d = opendir(nombre);
    size_t tamano = strlen(nombre);
    struct dirent *dir_info;
    struct stat info;
    char letratf;
    int result = -1, aux = -1;

    if (d) {
        result = 0;
        //Accedemos mientras se puede seguir leyendo el directorio d
        while ((dir_info = readdir(d)) && !result) {
            //NO accedemos a los archivos . ni .. para no acabar en un bucle infinito
            if (!strcmp(dir_info->d_name, ".") || !strcmp(dir_info->d_name, ".."))continue;

            result = -1;
            size_t aux_size;
            char *sig;
            aux_size = tamano + strlen(dir_info->d_name) + 2;
            sig = malloc(aux_size + 1);

            if (sig) {
                snprintf(sig, aux_size, "%s/%s", nombre, dir_info->d_name);
                //comprueba q el archivo existe
                if (stat(sig, &info) == 0) {
                    letratf = *ConvierteModo3(info.st_mode);
                    //Si nos encontramos otro fichero
                    if (letratf == 'd')
                        //Hacemos una llamada recursiva
                        aux = BorrarArbol(sig);
                    else
                        aux = unlink(sig);
                }
                free(sig);
            }
            result = aux;
        }
        closedir(d);
    }
    if (!result)
        return result = rmdir(nombre);

    return result;
}

void deltree(char *trozos[], int numero_trozos) {

    char archivo;
    char letra;
    int i = 1;
    struct stat info;

    while (i < numero_trozos) {
        archivo = *trozos[i];
        stat(trozos[i], &info);
        letra = LetraTF(info.st_mode);
        if (letra == '-' || (letra == 'd')) {//CASOS DIRECTORIOS NO VACIOS O FICHEROS
            if (letra == '-') {
                if (unlink(&archivo) != 0) {
                    perror("No se pudo eliminar el fichero");
                }
            } else if (rmdir(&archivo) != 0) {//DIRECTORIO NO VACIO
                BorrarArbol(trozos[i]);
            }
        } else {
            if (rmdir(trozos[i]) < 0) {
                perror("No existe el directorio");
            }
        }
        i++;
    }

}

void procesarEntrada(char *cadena, char *trozos[], int *numero_comandos,
                     char comandos[DIMENSION_ARRAY][200], int *lastMall, int *lastSha, int *lastMap, char **env) {
    int numero_trozos;
    numero_trozos = TrocearCadena(cadena, trozos);
    static proceso *proccarr[DIMENSION_ARRAY];

    static memblock allocates;
    if (trozos[0] == NULL) {
        return;
        numero_comandos--;
    }
    if (strcmp(trozos[0], "autores") == 0) {
        imprimirAutores(trozos, numero_trozos);
    } else if (strcmp(trozos[0], "pid") == 0) {
        imprimirPid(trozos, numero_trozos);
    } else if (strcmp(trozos[0], "carpeta") == 0) {
        cambiarDirectorio(trozos, numero_trozos);
    } else if (strcmp(trozos[0], "fecha") == 0) {
        imprimirFecha(trozos, numero_trozos);
    } else if (strcmp(trozos[0], "hist") == 0) {
        historial(trozos, numero_trozos, numero_comandos, comandos);
    } else if (strcmp(trozos[0], "comando") == 0) {
        char cmdN[100];
        strcpy(cmdN, comando_N(trozos, numero_trozos, *numero_comandos, comandos));
        numero_trozos = TrocearCadena(cmdN, trozos);
        if (strcmp(trozos[0], "comando") == 0) {
            printf("No se puede ejecutar comando %s", trozos[0]);
            return;
        }
        procesarEntrada(cmdN, trozos, numero_comandos, comandos, lastMall, lastSha, lastMap, env);
    } else if (strcmp(trozos[0], "infosis") == 0) {
        infosis();
    } else if (strcmp(trozos[0], "ayuda") == 0) {
        imprimirAyuda(trozos, numero_trozos);
    } else if (strcmp(trozos[0], "bye") == 0 || strcmp(trozos[0], "fin") == 0 || strcmp(trozos[0], "salir") == 0) {
        finalizar(trozos);
    } else if (strcmp(trozos[0], "create") == 0) {
        create(trozos);
    } else if (strcmp(trozos[0], "list") == 0) {
        list(trozos, numero_trozos);
    } else if (strcmp(trozos[0], "delete") == 0) {
        delete(trozos, numero_trozos);
    } else if (strcmp(trozos[0], "stat") == 0) {
        stat_fich_dir(trozos, numero_trozos);
    } else if (strcmp(trozos[0], "deltree") == 0) {
        deltree(trozos, numero_trozos);
    } else if (strcmp(trozos[0], "allocate") == 0) {
        allocate(trozos, numero_trozos, &allocates, lastMall, lastSha, lastMap);
    } else if (strcmp(trozos[0], "deallocate") == 0) {
        deallocate(trozos, numero_trozos, allocates, lastMall, lastSha, lastMap);
    } else if (strcmp(trozos[0], "i-o") == 0) {
        i_o(trozos, numero_trozos);
    } else if (strcmp(trozos[0], "memfill") == 0) {
        memfill(trozos, numero_trozos);
    } else if (strcmp(trozos[0], "memdump") == 0) {
        memdump(trozos, numero_trozos);
    } else if (strcmp(trozos[0], "memory") == 0) {
        memory(trozos, numero_trozos, allocates, *lastMall, *lastSha, *lastMap);
    } else if (strcmp(trozos[0], "recurse") == 0) {
        recurse(trozos, numero_trozos);
    } else if (strcmp(trozos[0], "priority") == 0) {
        priority(trozos, numero_trozos);
    } else if (strcmp(trozos[0], "showvar") == 0) {
        showvar(trozos, numero_trozos, env);
    } else if (strcmp(trozos[0], "changevar") == 0) {
        changevar(trozos, numero_trozos, env);
    } else if (strcmp(trozos[0], "showenv") == 0) {
        showenv(trozos, numero_trozos, env);
    } else if (strcmp(trozos[0], "fork") == 0) {
        Cmd_fork();
    } else if (strcmp(trozos[0], "execute") == 0) {
        Execute(trozos,numero_trozos);
    } else if (strcmp(trozos[0], "listjobs") == 0) {
        listjobs(proccarr);
    } else if (strcmp(trozos[0], "deljobs") == 0) {
        deljobs(trozos, numero_trozos, proccarr);
    } else if (strcmp(trozos[0], "job") == 0) {
        job(trozos, numero_trozos, proccarr);
    } else {
        comando_random(cadena, trozos, numero_trozos, proccarr);
    }

}

///P2
void Insertar_allocate_malloc(mallocblock bloque, mallocblock *array[DIMENSION_ARRAY], int last) {
    mallocblock *nuevo = malloc(sizeof(mallocblock));
    nuevo->direccion = bloque.direccion;
    nuevo->tamano = bloque.tamano;
    nuevo->fecha = bloque.fecha;

    if (last == 0) array[0] = nuevo;
    else if (last < DIMENSION_ARRAY) array[last] = nuevo;
}

void Insertar_allocate_shared(sharedblock bloque, sharedblock *array[DIMENSION_ARRAY], int last) {
    sharedblock *nuevo = malloc(sizeof(sharedblock));
    nuevo->direccion = bloque.direccion;
    nuevo->tamano = bloque.tamano;
    nuevo->fecha = bloque.fecha;
    nuevo->key = bloque.key;
    if (last == 0) array[0] = nuevo;
    else if (last < DIMENSION_ARRAY) array[last] = nuevo;
}

void Insertar_allocate_mapped(mappedblock bloque, mappedblock *array[DIMENSION_ARRAY], int last) {
    mappedblock *nuevo = malloc(sizeof(mappedblock));
    nuevo->direccion = bloque.direccion;
    nuevo->tamano = bloque.tamano;
    nuevo->fecha = bloque.fecha;

    if (last == 0) array[0] = nuevo;
    else if (last < DIMENSION_ARRAY) array[last] = nuevo;
}

void Deallocate_malloc(size_t tamano, mallocblock *array[DIMENSION_ARRAY], int *lastMall) {
    int i, eliminado;
    mallocblock aux;
    for (i = *lastMall - 1; i >= 0; i--) {
        if (array[i]->tamano == tamano) {
            free(array[i]);
            eliminado = i;
            *lastMall = *lastMall - 1;
            break;
        }
    }
    for (i = eliminado + 1; i <= *lastMall; i++) {
        aux = *array[i];
        *array[i - 1] = aux;
    }
}

void Deallocate_shared(key_t clave, sharedblock *array[DIMENSION_ARRAY], int *lastSha) {
    int i, eliminado;
    sharedblock aux;
    for (i = *lastSha - 1; i >= 0; i--) {
        if (array[i]->key == clave) {
            free(array[i]);
            eliminado = i;
            *lastSha = *lastSha - 1;
            break;
        }
    }
    for (i = eliminado + 1; i <= *lastSha; i++) {
        aux = *array[i];
        *array[i - 1] = aux;
    }
}

void Deallocate_mmap(const char *nombre, mappedblock *array[DIMENSION_ARRAY], int *lastMap) {
    int i, eliminado;
    mappedblock aux;
    for (i = *lastMap - 1; i >= 0; i--) {
        if (array[i]->nombre == nombre) {
            free(array[i]);
            eliminado = i;
            *lastMap = *lastMap - 1;
            break;
        }
    }
    for (i = eliminado + 1; i <= *lastMap; i++) {
        aux = *array[i];
        *array[i - 1] = aux;
    }
}

void *pointerDir1(mallocblock bloque) {
    return bloque.direccion;
}

void *pointerDir2(sharedblock bloque) {
    return bloque.direccion;
}

void *pointerDir3(mappedblock bloque) {
    return bloque.direccion;
}

void Deallocate_address(void *direccion, memblock allocates, int *lastMall, int *lastSha, int *lastMap) {
    int i, eliminado = -1;
    void *dir = direccion;
    mallocblock aux1;
    sharedblock aux2;
    mappedblock aux3;

    //Comprueba en mallocs
    for (i = *lastMall - 1; i >= 0; i--) {
        if (pointerDir1(*allocates.mallocs[i]) == dir) {
            free(allocates.mappeds[i]);
            eliminado = i;
            *lastMall = *lastMall - 1;
            break;
        }
    }
    if (eliminado != -1) {
        for (i = eliminado + 1; i <= *lastMall; i++) {
            aux1 = *allocates.mallocs[i];
            *allocates.mallocs[i - 1] = aux1;
        }
    }
    //Comprueba en shareds
    for (i = *lastSha - 1; i >= 0; i--) {
        if (pointerDir2(*allocates.shareds[i]) == dir) {
            free(allocates.shareds[i]);
            eliminado = i;
            *lastSha = *lastSha - 1;
            break;
        }
    }
    if (eliminado != -1) {
        for (i = eliminado + 1; i <= *lastMall; i++) {
            aux2 = *allocates.shareds[i];
            *allocates.shareds[i - 1] = aux2;
        }
    }
    //Comprueba en mappeds
    for (i = *lastMap - 1; i >= 0; i--) {
        if (pointerDir3(*allocates.mappeds[i]) == dir) {
            free(allocates.mappeds[i]);
            eliminado = i;
            *lastMap = *lastMap - 1;
            break;
        }
    }
    if (eliminado != -1) {
        for (i = eliminado + 1; i <= *lastMall; i++) {
            aux3 = *allocates.mappeds[i];
            *allocates.mappeds[i - 1] = aux3;
        }
    }

}

char *Nmes_to_Smes(int mesN) {
    switch (mesN) {
        case 0:
            return "Ene";
        case 1:
            return "Feb";
        case 2:
            return "Mar";
        case 3:
            return "Abr";
        case 4:
            return "May";
        case 5:
            return "Jun";
        case 6:
            return "Jul";
        case 7:
            return "Ago";
        case 8:
            return "Sep";
        case 9:
            return "Oct";
        case 10:
            return "Nov";
        default:
            return "Dic";
    }
}

void allocate(char *trozos[], int n_trozos, memblock *allocates, int *lastMall, int *lastSha, int *lastMap) {
    int i;
    time_t fecha = time(NULL);
    mallocblock newmalloc;
    mappedblock newmapped;
    void *aux;

    if (n_trozos > 2) {
        if (strcmp(trozos[1], "-malloc") == 0) {
            if (atoll(trozos[2]) > 0) {
                aux = malloc(atoll(trozos[2]));
                if (aux == NULL) perror("Memoria Llena");
                else {
                    newmalloc.direccion = aux;
                    newmalloc.tamano = (size_t) (unsigned) atoll(trozos[2]);
                    newmalloc.fecha = *localtime(&fecha);
                    Insertar_allocate_malloc(newmalloc, allocates->mallocs, *lastMall);
                    printf("Asignados %lu bytes a %p\n", newmalloc.tamano, newmalloc.direccion);
                    *lastMall = *lastMall + 1;
                    free(aux);
                }
            }
        }
        if (strcmp(trozos[1], "-shared") == 0) {
            print_SharedKey(allocates->shareds, *lastSha, (key_t) atoll(trozos[2]));
        }
        if (strcmp(trozos[1], "-createshared") == 0) {
            do_AllocateCreateshared(trozos, allocates->shareds, lastSha);
        }
        if (strcmp(trozos[1], "-mmap") == 0) {
            newmapped.direccion = aux;
            newmapped.tamano = (size_t) trozos[2];
            newmapped.fecha = *localtime(&fecha);
            Insertar_allocate_mapped(newmapped, allocates->mappeds, *lastMap);
            MapearFichero(trozos[2], 777, allocates->mappeds, lastMap);
            printf("Asignados %lu bytes a %p\n", newmalloc.tamano, newmalloc.direccion);

        }
    } else {
        if (n_trozos == 2 && strcmp(trozos[1], "-mmap") == 0) {
            do_AllocateMmap(trozos, allocates->mappeds, lastMap);
        }
        if (n_trozos == 2 && strcmp(trozos[1], "-malloc") == 0) {
            printf("***********Listado de bloques asignados malloc al proceso %d \n", getpid());
            for (i = 0; i < *lastMall; i++) {
                ImprimirListaMalloc(allocates->mallocs, *lastMall);
            }
        }
        if (n_trozos == 2 && (strcmp(trozos[1], "-createshared") == 0 || strcmp(trozos[1], "-shared") == 0)) {
            ImprimirListaShared(allocates->shareds, *lastSha);
        } else if (n_trozos == 1)printf("Prueba con allocate [-malloc|-shared|-createshared|-mmap] ...\n");
    }
}

void deallocate(char *trozos[], int n_trozos, memblock allocates, int *lastMall, int *lastSha, int *lastMap) {

    if (n_trozos > 2) {
        if (strcmp(trozos[1], "-malloc") == 0) {
            Deallocate_malloc((size_t) atoll(trozos[2]), allocates.mallocs, lastMall);
        }

        if (strcmp(trozos[1], "-shared") == 0) {
            Deallocate_shared((key_t) atoll(trozos[2]), allocates.shareds, lastSha);
        }
        if (strcmp(trozos[1], "-delkey") == 0) {
            do_DeallocateDelkey(trozos);
        }

        if (strcmp(trozos[1], "-mmap") == 0) {
            Deallocate_mmap(trozos[2], allocates.mappeds, lastMap);
        }

    } else {
        if (n_trozos == 2) {
            if (n_trozos == 2 && strcmp(trozos[1], "-malloc") == 0) {
                print_Mallocs(allocates.mallocs, *lastMall);
            }
            if (n_trozos == 2 && (strcmp(trozos[1], "-shared") == 0 || strcmp(trozos[1], "-createshared") == 0)) {
                ImprimirListaShared(allocates.shareds, *lastMall);
            }
            if (strcmp(trozos[1], "-mmap") == 0) {
                print_Mappeds(allocates.mappeds, *lastMap);
            } else Deallocate_address(cadtop(trozos[1]), allocates, lastMall, lastSha, lastMap);

        } else if (n_trozos == 1)printf("Prueba con allocate [-malloc|-shared|-createshared|-mmap] ...\n");
    }
}

void i_o(char *trozos[], int n_trozos) {
    size_t bytes;
    if (n_trozos > 3) {
        if (strcmp(trozos[1], "read") == 0) {
            do_I_O_read(trozos);
        } else if (strcmp(trozos[1], "write") == 0 && n_trozos >= 5) {
            if (n_trozos >= 6 && strcmp(trozos[5], "-o") == 0) {
                bytes = EscribirFichero(trozos[3], cadtop(trozos[4]), (size_t) atoll(trozos[5]), 1);
                printf("Escritos %lu bytes en %s desde 0x%p", (size_t) bytes, trozos[2], cadtop(trozos[4]));
            } else {
                bytes = EscribirFichero(trozos[2], cadtop(trozos[3]), (size_t) atoll(trozos[4]), 0);
                printf("Escritos %lu bytes en %s desde 0x%p", (size_t) bytes, trozos[2], cadtop(trozos[3]));
            }


        }
    } else {
        printf("Faltan parametros\n");
        return;
    }
}

void memdump(char *trozos[], int numero_trozos) {
    size_t tamano;
    void *direccion;
    direccion = cadtop(trozos[1]);
    if (numero_trozos >= 2) {
        tamano = 25;
        if (numero_trozos == 2) {
            printf("Volcando %ld bytes desde la direccion 0x%s\n", tamano, trozos[1]);
            VolcarMemoria(direccion, tamano);
        } else {
            tamano = (size_t) strtol(trozos[2], NULL, 10);
            printf("Volcando %ld bytes desde la direccion 0x%s\n", tamano, trozos[1]);
            VolcarMemoria(direccion, tamano);
        }
    }
}

void memfill(char *trozos[], int numero_trozos) {
    size_t tamano;
    unsigned char caracter;
    void *p;
    char *trozosaux[3];
    p = cadtop(trozos[1]);
    if (numero_trozos >= 2) {
        tamano = (size_t) 128;
        caracter = 'A';

        if (numero_trozos == 2) {
            printf("Llenando 128 bytes de memoria con el byte A(41) a partir de la direccion 0x%s\n", trozos[1]);
            LlenarMemoria(p, tamano, caracter);
        } else if (numero_trozos == 3) {
            tamano = (size_t) strtol(trozos[2], NULL, 10);
            printf("Llenando %ld bytes de memoria con el byte A(41) a partir de la direccion 0x%s\n", tamano,
                   trozos[1]);
            LlenarMemoria(p, tamano, 'A');

        } else {
            tamano = (size_t) strtol(trozos[2], NULL, 10);
            if (isdigit(*trozos[3])) {
                caracter = (unsigned char) *trozos[3];

            } else {
                caracter = (unsigned char) atoi(trozos[3]);
            }

            if ((TrocearCadenaComillas(trozos[3], trozosaux)) ==
                1) {//comprobamos que pasan entre comillas un caracter 'A'
                //caracter = (unsigned char) atoi(trozosaux[0]);
                caracter = *trozosaux[0];
                printf("Llenando %ld bytes de memoria con el byte %c(%02hhx) a partir de la direccion 0x%s\n", tamano,
                       caracter, caracter, trozos[1]);
                LlenarMemoria(p, tamano, *trozosaux[0]);

            } else {
                printf("Llenando %ld bytes de memoria con el byte %c(%02hhx) a partir de la direccion 0x%s\n", tamano,
                       caracter, caracter, trozos[1]);
                LlenarMemoria(p, tamano, *trozos[3]);
            }


        }
    }
}

//Variables globales para funcion memory
int global1 = 0, global2 = 1, global3 = 2;

void memory(char *trozos[], int n_trozos, memblock allocates, int lastMall, int lastSha, int lastMap) {
    int local1 = 0;
    char local2 = 'a';
    float local3 = 1.3;
    static int estatica1 = 0;
    static char estatica2 = 'a';
    static float estatica3 = 1.3;

    if (n_trozos == 1) {
        printf("Variables locales     %p   %p  %p \n", &local1, &local2, &local3);
        printf("Variables globales    %p   %p  %p \n", &global1, &global2, &global3);
        printf("Variables estaticas   %p   %p  %p \n", &estatica1, &estatica2, &estatica3);
        printf("Funciones programa    %p   %p  %p \n", create, cadtop, delete);
        printf("Funciones libreria    %p   %p  %p \n", malloc, fgets, scanf);
        printf("*****Lista de bloques asignados para el proceso %d \n", getpid());

        ImprimirListaMalloc(allocates.mallocs, lastMall);
        ImprimirListaShared(allocates.shareds, lastSha);
        ImprimirListaMmap(allocates.mappeds, lastSha);

    } else if (n_trozos >= 2) {
        if (strcmp(trozos[1], "-blocks") == 0) {
            printf("*****Lista de bloques asignados para el proceso %d \n", getpid());
            ImprimirListaMalloc(allocates.mallocs, lastMall);
            ImprimirListaShared(allocates.shareds, lastSha);
            ImprimirListaMmap(allocates.mappeds, lastMap);
        }
    } else if (strcmp(trozos[1], "-funcs") == 0) {
        printf("Funciones programa  %p   %p  %p \n", create, cadtop, delete);
        printf("Funciones libreria  %p   %p  %p \n", malloc, fgets, scanf);
    } else if (strcmp(trozos[1], "-vars") == 0) {
        printf("Variables locales   %p   %p  %p \n", &local1, &local2, &local3);
        printf("Variables globales   %p   %p  %p \n", &global1, &global2, &global3);
        printf("Variables estaticas   %p   %p  %p \n", &estatica1, &estatica2, &estatica3);
    } else if (strcmp(trozos[1], "-all") == 0) {
        memory(&trozos[1], 1, allocates, lastMall, lastSha, lastMap);
    } else if (strcmp(trozos[1], "-pmap") == 0) {
        Do_pmap();
    } else {
        printf("Opcion %s no contemplada\n", trozos[1]);
    }

}

void *cadtop(char *cadena) {
    void *p;
    p = (void *) strtoull(cadena, NULL, 16);
    return p;
}

void recurse(char *trozos[], int numero_trozos) {
    int n_veces;
    if (numero_trozos > 1) {
        n_veces = (int) strtol(trozos[1], NULL, 10);
        Recursiva(n_veces);
    }
}

void Recursiva(int n) {
    char automatico[TAMANO];
    static char estatico[TAMANO];

    printf("parametro:%3d(%p) array %p, arr estatico %p\n", n, &n, automatico, estatico);

    if (n > 0)
        Recursiva(n - 1);
}

void do_AllocateMmap(char *arg[], mappedblock *array[DIMENSION_ARRAY], int *lastMmap) {
    char *perm;
    void *p;
    int protection = 0;

    if (arg[0] == NULL) {
        ImprimirListaMmap(array, *lastMmap);
        return;
    }
    if ((perm = arg[1]) != NULL && strlen(perm) < 4) {
        if (strchr(perm, 'r') != NULL) protection |= PROT_READ;
        if (strchr(perm, 'w') != NULL) protection |= PROT_WRITE;
        if (strchr(perm, 'x') != NULL) protection |= PROT_EXEC;
    }
    if ((p = MapearFichero(arg[0], protection, array, lastMmap)) == NULL)
        perror("Imposible mapear fichero");
    else
        printf("fichero %s mapeado en %p\n", arg[0], p);
    *lastMmap = *lastMmap + 1;
}

void *MapearFichero(char *fichero, int protection, mappedblock *array[DIMENSION_ARRAY], int *lastMapp) {
    int df, map = MAP_PRIVATE, modo = O_RDONLY;
    struct stat s;
    time_t t = time(NULL);
    void *p;
    mappedblock bloque;

    if (protection & PROT_WRITE)
        modo = O_RDWR;
    if (stat(fichero, &s) == -1 || (df = open(fichero, modo)) == -1)
        return NULL;
    if ((p = mmap(NULL, s.st_size, protection, map, df, 0)) == MAP_FAILED)
        return NULL;
    strcpy(bloque.nombre, fichero);
    bloque.tamano = s.st_size;
    bloque.fecha = *localtime(&t);
    Insertar_allocate_mapped(bloque, array, *lastMapp);
    return p;
}

///P3
void priority(char *trozos[], int numero_trozos) {
    int prioridad, cambio_prio;
    if (numero_trozos == 1) {
        printf("Prioridad del proceso %d es %d\n", getpid(), getpriority(PRIO_PROCESS, getpid()));
    } else if (numero_trozos == 2) {
        prioridad = getpriority(PRIO_PROCESS, (id_t) atoll(trozos[1]));
        if (prioridad == -1) {
            printf("Imposible obtener prioridad del proceso %s\n", trozos[1]);

        } else {
            printf("Prioridad del proceso %s es %d\n", trozos[1], prioridad);
        }

    } else if (numero_trozos >= 3) {
        cambio_prio = setpriority(PRIO_PROCESS, (id_t) atoi(trozos[1]), atoi(trozos[2]));
        if (cambio_prio == -1) {
            printf("Imposible cambiar la prioridad del proceso %s\n", trozos[1]);
        }
    }
}

void Cmd_fork() {
    pid_t pid;

    if ((pid = fork()) == 0) {
        printf("ejecutando proceso %d\n", getpid());
    } else if (pid != -1)
        waitpid(pid, NULL, 0);
}

void showvar(char *trozos[], int numero_trozos, char **env) {
    int i, b;
    char newenv[250];
    if (numero_trozos == 1) {
        for (i = 0; env[i] != NULL; i++) {
            printf("%p-> main arg3[%d]=(%p) %s\n", &env[i], i, env[i], env[i]);
        }
    } else {
        i = BuscarVariable(trozos[1], env);
        if (i == -1) {
            printf("No se encontro la variable %s\n", trozos[1]);
            return;
        }
        b = BuscarVariable(trozos[1], environ);
        strcpy(newenv, getenv(trozos[1]));
        printf("%s\n", newenv);
        printf("Con arg3 main %s(%p) @%p\n", env[i], &env[i], env[i]);
        printf("Con environ   %s(%p) @%p\n", environ[b], &environ[b], environ[b]);
        printf("Con getenv %s(%p) \n", newenv, newenv);
    }
}

void changevar(char *trozos[], int numero_trozos, char **env) {
    char aux[250];
    int i, x;
    if (numero_trozos == 1) printf("Usa -> ayuda changevar");
    else if (numero_trozos >= 4) {
        if (strcmp(trozos[1], "-a") == 0) CambiarVariable(trozos[2], trozos[3], env);
        else if (strcmp(trozos[1], "-e") == 0) CambiarVariable(trozos[2], trozos[3], environ);
        else if (strcmp(trozos[1], "-p") == 0) {
            strcpy(aux, trozos[2]);
            strcat(aux, "=");
            strcat(aux, trozos[3]);
            i = putenv(aux);
            x = BuscarVariable(trozos[2], env);
            env[x] = aux;
            x = BuscarVariable(trozos[2], environ);
            environ[x] = aux;
            if (i != 0) printf("No se pudo cambiar la variable\n");
        }
    }
}

void showenv(char *trozos[], int numero_trozos, char **env) {
    int i, pos = 0;
    if (numero_trozos == 1) {
        for (i = 0; env[i] != NULL; i++) {
            printf("%p-> main arg3[%d]=(%p) %s\n", &env[i], i, env[i], env[i]);
        }

    } else if (numero_trozos >= 2) {
        if (strcmp(trozos[1], "-addr") == 0) {
            pos = BuscarVariableAddrEnv(trozos[2], env);
            i = BuscarVariableAddrEnviron(trozos[2]);
            if (i == -1) {
                printf("No se encontro la variable\n");
                return;
            } else {
                printf("environ:   %p (almacenado en %p)\n", environ[pos], &environ);
                printf("main arg3: %p (almacenado en %p)\n", env[i], &env);
            }

        } else if (strcmp(trozos[1], "-environ") == 0) {
            for (i = 0; environ[i] != NULL; i++) {
                printf("%p->environ[%d]=(%p) %s\n", &environ[i], i, environ[i], environ[i]);
            }
        }
    }
}

void Execute(char * trozos[],int n_trozos){
    if (n_trozos > 1) {
        char *entorno[10] = {NULL};
        char *args[10] = {NULL};
        char *prio;
        int i=0,aux=1;
        if(strchr(trozos[n_trozos-1],'@')!=NULL){
            prio=trozos[n_trozos-1]+1;
            setpriority(PRIO_PROCESS,getpid(),atoi(prio));
        }
        while (aux<n_trozos) {
            if ( (int) trozos[aux][0] <= 90 && (int) trozos[aux][0] >= 65) {
                entorno[i] = environ[BuscarVariable(trozos[aux],environ)];
                i++;
            } else if (strchr(trozos[aux], '@') == NULL) {
                args[aux-i-1]=trozos[aux];
            }
            aux++;
        }
        if(i!=0) {
            if (OurExecvpe(Ejecutable(args[0]), args, entorno) == -1) printf("NO ejecutado\n");
        }
        else if(OurExecvpe(args[0],args,environ)==-1) printf("NO ejecutado\n");
    }
}

char *Ejecutable(char *s) {
    char path[PATH_MAX];
    static char aux2[PATH_MAX];
    struct stat st;
    char *p;
    if (s == NULL || (p = getenv("PATH")) == NULL)
        return s;
    if (s[0] == '/' || !strncmp(s, "./", 2) || !strncmp(s, "../", 3))
        return s;       //is an absolute pathname
    strncpy(path, p, PATH_MAX);
    for (p = strtok(path, ":"); p != NULL; p = strtok(NULL, ":")) {
        sprintf(aux2, "%s/%s", p, s);
        if (lstat(aux2, &st) != -1)
            return aux2;
    }
    return s;
}

int OurExecvpe(char *file, char *const argv[], char * envp[]) {
    //En error da -1 y un errno
    return execve(Ejecutable(file),argv, envp);
}

void listjobs(proceso *array[]) {
    int i;
    proceso aux;
    char *user = getlogin();
    printf("\t\t\t LISTADO DE PROCESOS\n");
    for (i = 0; i < lastProc; i++) {
        aux = *array[i];
        if (waitpid(aux.pid_num, &aux.senal, WNOHANG | WUNTRACED | WCONTINUED) == aux.pid_num){
            if (WIFEXITED(aux.senal)) {
                strcpy(aux.estado, "TERMINADO");
                aux.senal = WEXITSTATUS(aux.senal);
            } else if (WIFSIGNALED(aux.senal)) {
                strcpy(aux.estado, "SENALADO");
                aux.senal = WTERMSIG(aux.senal);
            } else if (WIFSTOPPED(aux.senal)) {
                strcpy(aux.estado, "STOPPED");
                aux.senal = WTERMSIG(aux.senal);
            } else if (WIFCONTINUED(aux.senal))
                strcpy(aux.estado, "ACTIVO");
        }
        printf("\t%d\t\t%s p=%d %d/%d/%d %d:%d:%d %s(%03d) %s\n", aux.pid_num, user,getpriority(PRIO_PROCESS,aux.pid_num),
               1900 + aux.launching_time.tm_year, 1 + aux.launching_time.tm_mon, aux.launching_time.tm_mday,
               aux.launching_time.tm_hour, aux.launching_time.tm_min, aux.launching_time.tm_sec,
               aux.estado, aux.senal, aux.comando);
        *array[i]=aux;
    }
}

void deljobs(char *trozos[], int numero_trozos, proceso *array[]) {
    int i, eliminar, eliminados = 0;
    proceso aux;
    proceso *arr[DIMENSION_ARRAY];
    if (numero_trozos == 1) {
        listjobs(array);
    } else {
        if (numero_trozos == 2) {
            if ((strcmp(trozos[1], "-term") == 0 || strcmp(trozos[1], "-sig") == 0) && numero_trozos == 2) {
                for (i = 0; i < lastProc; i++) {
                    *arr=*array;
                    aux=*array[i];
                    if (waitpid(aux.pid_num, &aux.senal, WNOHANG | WUNTRACED | WCONTINUED) == aux.pid_num){
                        if (WIFEXITED(aux.senal)) {
                            strcpy(aux.estado, "TERMINADO");
                            aux.senal = WEXITSTATUS(aux.senal);
                        } else if (WIFSIGNALED(aux.senal)) {
                            strcpy(aux.estado, "SENALADO");
                            aux.senal = WTERMSIG(aux.senal);
                        } else if (WIFSTOPPED(aux.senal)) {
                            strcpy(aux.estado, "STOPPED");
                            aux.senal = WTERMSIG(aux.senal);
                        } else if (WIFCONTINUED(aux.senal))
                            strcpy(aux.estado, "ACTIVO");
                    }
                    if (strcmp(aux.estado, "TERMINADO") == 0 && aux.senal==0) {
                        if(array[i+1]!=NULL){
                            if(lastProc==1) free(array[0]);
                            else {
                                eliminar=i;
                                array[i]=NULL;
                                for (eliminar = i+1; eliminar < lastProc; eliminar++) {
                                    array[eliminar-1]=array[eliminar];
                                }
                            }
                        }
                    }
                    if ( aux.senal == 15) {
                        free(array[i]);
                        eliminados++;
                        if(array[i+1]!=NULL){
                            if(lastProc==1) free(array[0]);
                            else {
                                eliminar=i;
                                array[i]=NULL;
                                for (eliminar = i+1; eliminar < lastProc; eliminar++) {
                                    array[eliminar-1]=array[eliminar];
                                }
                            }
                        }
                    }
                }
            }
        }
        else if(numero_trozos==3){
            for (i = 0; i < lastProc; i++) {
                *arr = *array;
                aux = *array[i];
                if (waitpid(aux.pid_num, &aux.senal, WNOHANG | WUNTRACED | WCONTINUED) == aux.pid_num) {
                    if (WIFEXITED(aux.senal)) {
                        strcpy(aux.estado, "TERMINADO");
                        aux.senal = WEXITSTATUS(aux.senal);
                    } else if (WIFSIGNALED(aux.senal)) {
                        strcpy(aux.estado, "SENALADO");
                        aux.senal = WTERMSIG(aux.senal);
                    } else if (WIFSTOPPED(aux.senal)) {
                        strcpy(aux.estado, "STOPPED");
                        aux.senal = WTERMSIG(aux.senal);
                    } else if (WIFCONTINUED(aux.senal))
                        strcpy(aux.estado, "ACTIVO");
                }
                if (strcmp(aux.estado, "TERMINADO") == 0) {
                    if (array[i + 1] != NULL) {
                        if (lastProc == 1) free(array[0]);
                        else {
                            eliminar = i;
                            array[i] = NULL;
                            for (eliminar = i + 1; eliminar < lastProc; eliminar++) {
                                array[eliminar - 1] = array[eliminar];
                            }
                        }
                    }
                }
            }

        }
        lastProc = lastProc - eliminados;
    }
}

void job(char *trozos[], int numero_trozos, proceso *array[]) {
    int i;
    proceso aux;
    if (numero_trozos > 2) {
        if (strcmp(trozos[1], "-fg") == 0) {
            for (i = 0; i < lastProc; i++) {
                aux = *array[i];
                if (atoll(trozos[2]) == aux.pid_num){
                    printf("Ejecutando el proceso %d en primer plano\n",aux.pid_num);
                    break;
                }
            }
            wait(&aux.pid_num);
            printf("Proceso terminado\n");
        }
    }
}

void comando_random(char *cadena, char *trozos[], int n_trozos, proceso *proccarr[]) {
    int error = 0, i = 0, aux = 0;
    char *prio="-100";
    char comando[500]={"\0"};
    char *args[10] = {NULL}, *entorno[10] = {NULL};
    proceso nuevo;
    pid_t p = 0;
    time_t t = time(NULL);
    //Copiar las variables de entorno
    while (aux<n_trozos) {
        if ( (int) trozos[aux][0] <= 90 && (int) trozos[aux][0] >= 65) {
            entorno[i] = environ[BuscarVariable(trozos[i],environ)];
            i++;
            strcat(comando, trozos[aux]);
            strcat(comando, " ");
        } else if (strchr(trozos[aux], '@') == NULL && strchr(trozos[aux], '&') == NULL) {
            strcat(comando, trozos[aux]);
            strcat(comando, " ");
        }
        aux++;
    }
    aux = i;
    copiarArgs(trozos, args, i, n_trozos, aux);
    //CASOS Q TIENEN & = ejecutar en background
    if (strchr(trozos[n_trozos - 1], '&') != NULL) {
        //HAY Q PONERLE LA PRIORIDAD DE @
        if (strchr(trozos[n_trozos - 2], '@') != NULL) {
            prio = trozos[n_trozos - 2]+1;
        }
        ///SE EJECUTA CON ENTORNO
        if(aux!=0){
            if((p=EjecutarBackGroundEntorno(args,entorno, atoi(prio)))>0);
        }
        else p = EjecutarBackGround(args,atoi(prio));
        if(p>0) {
            strcpy(nuevo.estado, "ACTIVO");
            nuevo.pid_num = p;
            nuevo.senal = 0;
            nuevo.launching_time = *localtime(&t);
            strcpy(nuevo.comando, comando);
            insertarproceso(nuevo, proccarr);
            lastProc += 1;
        }
    }
        //Ejecutar en foreground
    else {
        if (strchr(trozos[n_trozos - 1], '@') != NULL) {
            prio=trozos[n_trozos - 1]+1;
            nice(atoi(prio));
        }
        if ( fork() == 0){//crea hijo
            //CASOS SIN &
            //HAY ENTORNO
            if (aux > 0) {
                    if ((error = execve(Ejecutable(args[0]), args, entorno)) < 0) {
                        printf("NO ejecutado\n");
                    }
            }
                //NO HAY ENTORNO
            else {
                    if (execv(Ejecutable(args[0]), args) < 0) {
                        printf("NO ejecutado\n");
                    }
                }
            }

        wait(NULL);//termina a q acabe el hijo
    }
    if (p < 0)printf("NO ejecutado\n");
}

int main(__attribute__((unused))int argc, __attribute__((unused))char *argv[], char **env) {
    char cadena[PATH_MAX];
    char *trozos[20] = {NULL};
    static char array_comandos[DIMENSION_ARRAY][200];
    //int *numero_comandos=malloc(sizeof(int));
    int *lastMall = malloc(sizeof(int)), *lastSha = malloc(sizeof(int)), *lastMap = malloc(sizeof(int)),*numero_comandos=malloc(sizeof(int));
    lastProc = 0;
    *lastMall = 0;
    *lastSha = 0;
    *lastMap = 0;
    *numero_comandos = 0;

    while (!terminado) {
        imprimirPrompt();
        leer2(cadena);
        Insertar_comando(cadena, array_comandos, *numero_comandos);
        procesarEntrada(cadena, trozos, numero_comandos, array_comandos, lastMall, lastSha, lastMap, env);
        *numero_comandos+=1;
    }
    free(lastMall);
    free(lastSha);
    free(lastMap);
    free(numero_comandos);
    return 0;
}

void LlenarMemoria(void *p, size_t cont, unsigned char byte) {
    unsigned char *arr = (unsigned char *) p;
    size_t i;
    for (i = 0; i < cont; i++)
        arr[i] = byte;
}

void VolcarMemoria(void *p, size_t cont) {
    unsigned char *arr = (unsigned char *) p;
    size_t i;
    for (i = 0; i < cont; i++) {
        printf("  %c", arr[i]);
    }
    printf("\n");
    for (i = 0; i < cont; i++) {
        printf("%02hhx ", arr[i]);
    }
    printf("\n");
}

void Do_pmap(void) { //sin argumentos
    pid_t pid;       //hace el pmap (o equivalente) del proceso actual
    char elpid[32];
    char *argv[4] = {"pmap", elpid, NULL};

    sprintf(elpid, "%d", (int) getpid());
    if ((pid = fork()) == -1) {
        perror("Imposible crear proceso");
        return;
    }
    if (pid == 0) {
        if (execvp(argv[0], argv) == -1)
            perror("cannot execute pmap (linux, solaris)");

        argv[0] = "procstat";
        argv[1] = "vm";
        argv[2] = elpid;
        argv[3] = NULL;
        if (execvp(argv[0], argv) == -1)//No hay pmap, probamos procstat FreeBSD
            perror("cannot execute procstat (FreeBSD)");

        argv[0] = "procmap", argv[1] = elpid;
        argv[2] = NULL;
        if (execvp(argv[0], argv) == -1)  //probamos procmap OpenBSD
            perror("cannot execute procmap (OpenBSD)");

        argv[0] = "vmmap";
        argv[1] = "-interleave";
        argv[2] = elpid;
        argv[3] = NULL;
        if (execvp(argv[0], argv) == -1) //probamos vmmap Mac-OS
            perror("cannot execute vmmap (Mac-OS)");
        exit(1);
    }
    waitpid(pid, NULL, 0);
}

void *ObtenerMemoriaShmget(key_t clave, size_t tam, sharedblock *array[DIMENSION_ARRAY], int lastSha) {
    void *p;
    int aux, id, flags = 0777;
    struct shmid_ds s;
    sharedblock bloque;
    time_t fecha = time(NULL);

    if (tam)     //tam distito de 0 indica crear
        flags = flags | IPC_CREAT | IPC_EXCL;
    if (clave == IPC_PRIVATE)  //no nos vale
    {
        errno = EINVAL;
        return NULL;
    }
    if ((id = shmget(clave, tam, flags)) == -1)
        return (NULL);
    if ((p = shmat(id, NULL, 0)) == (void *) -1) {
        aux = errno;
        if (tam)
            shmctl(id, IPC_RMID, NULL);
        errno = aux;
        return (NULL);
    }
    shmctl(id, IPC_STAT, &s);
    bloque.direccion = p;
    bloque.tamano = tam;
    bloque.key = clave;
    bloque.fecha = *localtime(&fecha);
    Insertar_allocate_shared(bloque, array,
                             lastSha);// Guardar en la lista   InsertarNodoShared (&L, p, s.shm_segsz, clave);
    return (p);
}

void do_AllocateCreateshared(char *tr[], sharedblock *array[DIMENSION_ARRAY], int *lastSha) {
    key_t cl;
    size_t tam;
    void *p;

    if (tr[2] == NULL || tr[3] == NULL) {
        ImprimirListaShared(array, *lastSha);
        return;
    }

    cl = (key_t) strtoul(tr[2], NULL, 10);
    tam = (size_t) strtoul(tr[3], NULL, 10);
    if (tam == 0) {
        printf("No se asignan bloques de 0 bytes\n");
        return;
    }
    if ((p = ObtenerMemoriaShmget(cl, tam, array, *lastSha)) != NULL) {
        printf("Asignados %lu bytes en %p\n", (unsigned long) tam, p);
        *lastSha += 1;
    } else
        printf("Imposible asignar memoria compartida clave %lu:%s\n", (unsigned long) cl, strerror(errno));
}

void do_DeallocateDelkey(char *args[]) {
    key_t clave;
    int id;
    char *key = args[2];

    if (key == NULL || (clave = (key_t) strtoul(key, NULL, 10)) == IPC_PRIVATE) {
        printf("      delkey necesita clave_valida\n");
        return;
    }
    if ((id = shmget(clave, 0, 0666)) == -1) {
        perror("shmget: imposible obtener memoria compartida");
        return;
    }
    if (shmctl(id, IPC_RMID, NULL) == -1)
        perror("shmctl: imposible eliminar memoria compartida\n");
}

ssize_t LeerFichero(char *f, void *p, size_t cont) {
    struct stat s;
    ssize_t n;
    int df, aux;

    if (stat(f, &s) == -1 || (df = open(f, O_RDONLY)) == -1)
        return -1;
    if (cont == -1)   // si pasamos -1 como bytes a leer lo leemos entero
        cont = s.st_size;
    if ((n = read(df, p, cont)) == -1) {
        aux = errno;
        close(df);
        errno = aux;
        return -1;
    }
    close(df);
    return n;
}

void do_I_O_read(char *ar[]) {
    void *p;
    size_t cont = -1;
    ssize_t n;
    if (ar[2] == NULL || ar[3] == NULL) {
        printf("faltan parametros\n");
        return;
    }
    p = cadtop(ar[3]);  //convertimos de cadena a puntero
    if (ar[3] != NULL)
        cont = (size_t) atoll(ar[4]);

    if ((n = LeerFichero(ar[2], p, cont)) == -1)
        perror("Imposible leer fichero");
    else
        printf("leidos %lld bytes de %s en %p\n", (long long) n, ar[2], p);
}

ssize_t EscribirFichero(char *f, void *p, size_t cont, int overwrite) {
    ssize_t n;
    int df, aux, flags = O_CREAT | O_EXCL | O_WRONLY;

    if (overwrite)
        flags = O_CREAT | O_WRONLY | O_TRUNC;

    if ((df = open(f, flags, 0777)) == -1)
        return -1;

    if ((n = write(df, p, cont)) == -1) {
        aux = errno;
        close(df);
        errno = aux;
        return -1;
    }
    close(df);
    return n;
}

void print_SharedKey(sharedblock *array[DIMENSION_ARRAY], int lastSha, key_t key) {
    printf("***********Listado de bloques asignados shared al proceso %d \n", getpid());
    ImprimirListaShared(array, lastSha);
}

void print_Mallocs(mallocblock *array[DIMENSION_ARRAY], int lastMall) {
    printf("***********Listado de bloques asignados malloc al proceso %d \n", getpid());
    ImprimirListaMalloc(array, lastMall);
}

void print_Mappeds(mappedblock *array[DIMENSION_ARRAY], int lastMap) {
    printf("***********Listado de bloques asignados mmap al proceso %d \n", getpid());
    ImprimirListaMmap(array, lastMap);
}

void ImprimirListaMalloc(mallocblock *mallocs[DIMENSION_ARRAY], int lastMall) {
    int i;
    if (mallocs == NULL) return;
    for (i = 0; i < lastMall; i++) {
        printf("\t%p\t\t\t%zu %s %d %d:%d malloc\n", mallocs[i]->direccion,
               mallocs[i]->tamano,
               Nmes_to_Smes(mallocs[i]->fecha.tm_mon), mallocs[i]->fecha.tm_mday,
               mallocs[i]->fecha.tm_hour, mallocs[i]->fecha.tm_min);
    }
}

void ImprimirListaMmap(mappedblock *maps[DIMENSION_ARRAY], int lastMap) {
    int i;
    if (maps[0] == NULL) return;
    for (i = 0; i < lastMap; i++) {
        printf("\t%p\t\t\t%zu %s %d %d:%d (insert) (descriptor %d)\n", maps[i]->direccion,
               maps[i]->tamano,
               Nmes_to_Smes(maps[i]->fecha.tm_mon), maps[i]->fecha.tm_mday,
               maps[i]->fecha.tm_hour, maps[i]->fecha.tm_min, maps[i]->descriptor);
    }
}

void ImprimirListaShared(sharedblock *shareds[DIMENSION_ARRAY], int lastSha) {
    int i;
    if (shareds == NULL) return;
    for (i = 0; i < lastSha; i++) {
        printf("\t%p\t\t\t%zu %s %d %d:%d shared (key %d)\n", shareds[i]->direccion,
               shareds[i]->tamano,
               Nmes_to_Smes(shareds[i]->fecha.tm_mon), shareds[i]->fecha.tm_mday,
               shareds[i]->fecha.tm_hour, shareds[i]->fecha.tm_min, shareds[i]->key);
    }
}

//P3 AUX
int BuscarVariable(char *var, char *e[]) {  /*busca una variable en el entorno que se le pasa como parámetro*/

    int pos = 0;
    char aux[100];//MAXVAR

    strcpy(aux, var);
    strcat(aux, "=");

    while (e[pos] != NULL)
        if (!strncmp(e[pos], aux, strlen(aux)))
            return (pos);
        else
            pos++;
    errno = ENOENT;   /*no hay tal variable*/
    return (-1);
}

int BuscarVariableAddrEnv(char *var, char **env) {  /*busca una variable en el entorno que se le pasa como parámetro*/

    int pos = 0;
    char aux[100], aux2[100];//MAXVAR


    while (env[pos] != NULL) {
        sprintf(aux, "%p", &env[pos]);
        sprintf(aux2, "%p", env[pos]);
        if (strcmp(aux, var) == 0 || strcmp(aux2, var) == 0) return (pos);
        sprintf(aux, "%p", &environ[pos]);
        sprintf(aux2, "%p", environ[pos]);
        if (strcmp(aux, var) == 0 || strcmp(aux2, var) == 0) return (pos);
        pos++;
    }
    errno = ENOENT;   /*no hay tal variable*/
    return (-1);
}

int BuscarVariableAddrEnviron(char *var) {  /*busca una variable en el entorno que se le pasa como parámetro*/

    int pos = 0;
    char aux[100], aux2[100];//MAXVAR


    while (environ[pos] != NULL) {
        sprintf(aux, "%p", &environ[pos]);
        sprintf(aux2, "%p", environ[pos]);
        if (strcmp(aux, var) == 0 || strcmp(aux2, var) == 0) return (pos);
        sprintf(aux, "%p", &environ[pos]);
        sprintf(aux2, "%p", environ[pos]);
        if (strcmp(aux, var) == 0 || strcmp(aux2, var) == 0) return (pos);
        pos++;
    }
    errno = ENOENT;   /*no hay tal variable*/
    return (-1);
}

int CambiarVariable(char *var, char *valor, char *e[]) {
    /*cambia una variable en el entorno que se le pasa como parámetro*/
    /*lo hace directamente, no usa putenv*/
    int pos;
    char *aux;

    if ((pos = BuscarVariable(var, e)) == -1)
        return (-1);

    if ((aux = (char *) malloc(strlen(var) + strlen(valor) + 2)) == NULL)
        return -1;
    strcpy(aux, var);
    strcat(aux, "=");
    strcat(aux, valor);
    e[pos] = aux;
    return (pos);
}

void insertarproceso(proceso process, proceso *proccarray[]) {
    proceso *nuevo = malloc(sizeof(proceso));
    *nuevo = process;
    if (lastProc > DIMENSION_ARRAY) {
        printf("Array lleno");
        return;
    }
    proccarray[lastProc] = nuevo;
}

int EjecutarBackGround(char *args[],int prio) {
    pid_t pid;
    pid = fork();
    if(prio!=-100) nice(prio);
    if (pid == 0) {
        if (execvp(args[0], args) < 0) {
            printf("Comando no valido\n");
            return -1;
        }
    }
    if (pid < 0)
        return -1;

    return pid;
}

int EjecutarBackGroundEntorno(char *args[],char *entorno[],int prio) {
    pid_t pid;
    if(prio!=-100) nice(prio);
    pid = fork();

    if (pid == 0) {
        if (OurExecvpe(args[0], args,entorno) < 0) {
            printf("Comando no valido\n");
            return -1;
        }
    }
    if (pid < 0)
        return -1;

    return pid;
}

void copiarArgs(char *trozos[], char *args[], int inicio, int finargs, int finEnt) {
    for (; inicio < finargs; inicio++) {
        if (strchr(trozos[inicio], '&') == NULL && strchr(trozos[inicio], '@') == NULL) {
            args[inicio - finEnt] = trozos[inicio];
        }
    }
}