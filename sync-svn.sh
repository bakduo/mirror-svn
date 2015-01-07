#!/bin/bash
#/*
# *  sync-svn.sh - A Bash Implementation
# *  sync-svn.sh
# *  Copyright (C) 2014  linuxknow@gmail.com
# *  This program is free software; you can redistribute it and/or modify
# *  it under the terms of the GNU General Public License as published by
# *  the Free Software Foundation; either version 2 of the License, or
# *  (at your option) any later version.
# *
# *  This program is distributed in the hope that it will be useful,
# *  but WITHOUT ANY WARRANTY; without even the implied warranty of
# *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# *  GNU General Public License for more details.
# *
# *  You should have received a copy of the GNU General Public License along
# *  with this program; if not, write to the Free Software Foundation, Inc.,
# */
#
#Aqui tiene que estar el directorio donde esta el svn actualmente
#
#touch /home/www/desarrollo/sync-svn.pid
export LANGUAGE=en_US
#repo_svn=/home/ttb/db

svn_srv="/var/www/desarrollo"
repo_svn="/home/svn/archivo.cambio"
salida_log="$svn_srv/log-www-data.log"
interactive_value="$svn_srv/svn_si"
usuario=www-data
password=www-data

while true;do
	/usr/bin/inotifywait -mrq --format "%w" -e modify $repo_svn |while read line
	do
	    fwrepo=$(echo $line |awk -F/ '{print $1}')
	    echo "Detected Changes/commits on ${fwrepo} at $(date +%c)" >> $salida_log
        if [ ! -e "$svn_srv/.subversion/servers" ];then
            /usr/bin/svn update --username $usuario --password $password --non-interactive < $interactive_value
        else
            /usr/bin/svn update --username $usuario --password $password --non-interactive
        fi
	done
    echo "Repo deberia estar actualizado" >> $salida_log
done
