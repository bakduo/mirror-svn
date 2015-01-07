/*
 *  main - A C Implementation
 *  main.c
 *  Copyright (C) 2014  linuxknow@gmail.com
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 */

/*
Para iniciar servicio
./srv_svn start -d pathdemonio.pid pathejecutable que controla sync y nombre del ejecutable

./srv_svn start -d $(pwd)/daemon.pid /home/soporte/proyecto-c/control-servicio-svn/sync-svn.sh sync-svn.sh

Para servicio 
./srv_svn stop -d $(pwd)/daemon.pid

 */

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int global;
int c_pid;
int p_pid;
int s_pid;

int killProcess(const char *pathpid){
    long pid_process;
    char line[80];
    int error;
	FILE *fp = fopen (pathpid, "r");
	while(fgets(line, 80, fp) != NULL)
    {
	 sscanf (line, "%ld", &pid_process);
	 if (pid_process>0){
		error = kill(pid_process,SIGKILL);
		printf("Proceso : %ld estado: %d \n",pid_process,error);
	 }
	 system("/usr/bin/killall sync-svn.sh");
	 system("/usr/bin/killall inotifywait");
   }
   return 0;
}

int controlProcess(const char *pathbin,const char *pathpid,const char *usuario)
{
    pid_t child_pid;
    pid_t sid = 0;
    int status;
    int local = 0;
    child_pid = fork();
    char cmd_str[512];
    strcpy (cmd_str,"");
    strcat (cmd_str,"su - ");
    strcat (cmd_str,usuario);
	strcat (cmd_str," -c '");
 	strcat (cmd_str,pathbin);
 	strcat (cmd_str,"' &");
    if (child_pid >= 0) /* es valido? */
    {
        if (child_pid == 0) /* fork() ok */
        {
            local++;
            global++;
            c_pid = getpid();
            p_pid = getppid();
            umask(0);//utilidad umask
    		sid = setsid();//nueva sesion
    		if(sid < 0)
    		{
        		exit(1);
    		}
    		close(STDIN_FILENO);
    		close(STDOUT_FILENO);
    		close(STDERR_FILENO);
            FILE *fp = fopen (pathpid, "w+");
            fprintf(fp, "%d\n", c_pid);
            fprintf(fp, "%d", p_pid);
            fclose(fp);
    	    system(cmd_str);	
            return 0;
         }
         else /*Proceso padres */
         {
             wait(&status); /* Espera para poder terminar */
             return 0;
         }
    }
    else /* failure */
    {
        perror("Ocurrio un error al crear proceso");
        return 0;
    }
}

int main(int argc, char *argv[]){

 	char daemon_str[512];
 	char usuario_str[512];
 	int id = getuid();
 	if (id!=0){
 		printf("Debe ejecutar como root.\n");
 		exit(1);
 	}
 	//Solo se permiten comando de inicio y parada
 	if (argc == 4 && strcmp(argv[1],"stop")==0 && strcmp(argv[2],"-d")==0){
 		printf("Terminando programa.\n");
 		strcpy (daemon_str,"");
 	    strcat (daemon_str,argv[3]);
 		killProcess(daemon_str);
 	}else{
 		if (argc < 2){
 			printf("No ingreso la cantidad de argumento permitidos\n");
 			exit(1);
 		}else{
 			if (argc == 7 && strcmp(argv[1],"start")==0 && strcmp(argv[2],"-d")==0 && strcmp(argv[4],"-u")==0){
 				strcpy (daemon_str,"");
 				strcat (daemon_str,argv[3]);
 				strcpy(usuario_str,"");
 	    		strcat(usuario_str,argv[5]);
 				controlProcess(argv[6],daemon_str,usuario_str);
 			}else{
 				printf("Solo se permiten como parametros start | stop y el path del demonio -d /var/run/daemon.pid -u www-data\n");
 				exit(1);
 			}
 		} 		
 	}
 	printf("\n");
	return 0;
}
