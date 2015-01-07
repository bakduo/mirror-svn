# mirror-svn
Mirror-svn mantiene la relación entre las actualizaciones de svn de algún/os proyecto/s respecto de la web de desarrollo.


El servicio de srv_svn mantiene la relación entre las actualizaciones del svn de algún/os proyecto/s respecto de la web de desarrollo.

Una actualización desde cualquier punto terminal en la base de desarrollo acciona un evento de actualización.

Para ello es necesario designar un usuario que tenga permisos para actualizar el repo y que tenga una cuenta valida o tener acceso al directorio del svn con el usuario.

Utilidades:
Archivo binario compilado en C
Archivo script daemon que carga init start | stop | status
Archivo script que maneja los eventos.
Archivo daemon.pid almacena el pid principal del proceso.
Requiere tener instalado inotify-tools

para realizar la carga del servicio en el sistema y que arranque por default.

Como root ejecutar:
insserv srv-sync-svn

Este comando deja el servicio instalado <Esta basado en los servicios de Debian Wheezy>

Los archivos sync-svn.sh y srv_svn pueden o no estar en el mismo directorio.

Procedemos a un ejemplo de instalación.

Utilizamos como usuario : www-data
en la carpeta /home/www/desarrollo copiamos  sync-svn.sh y srv_svn

Variables para configurar el servicio srv-sync-svn :

SVN_SRV=/home/www/desarrollo/srv_svn 
PID_DAEMON=/home/www/desarrollo/daemon.pid 
NAME_SYNC=sync-svn.sh 
PATH_SYNC=$SVN_SRV/$NAME_SYNC 
USUARIO=www-data

Variables para configurar el script  sync-svn.sh

tener en cuenta que home_dev es ele directorio donde esta actualmente el / del svn gral la db.

svn_srv="/var/www/desarrollo" 
repo_svn="/home/svn/db" 
salida_log="$svn_srv/log-www-data.log" 
interactive_value="$svn_srv/svn_si" 
usuario=www-data 
password=www-data  => * 
usuario=www-data 
password=www-data


*Para el caso que pida “yes” confirmación de guardar clave cuando hacemos la actualización vía web.

y como root ejecutamos:
/etc/init.d/srv-sync-svn start

Para ejecutar el servicio.
