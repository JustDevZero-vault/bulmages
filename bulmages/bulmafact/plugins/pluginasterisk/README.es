#######
#Plugins
#######
#README.es
#20080501

Nombre: Control de Presencia por Centralita Asterisk

Biblioteca: libpluginasterisk.so

Descripción: Presenta todas las validaciones realizadas en los usuarios y en los almacenes correspondientes. Modifica la ficha de almacenes y trabajadores para que presenten un 
cuadro de validaciones realizadas.

ParmBD: DBRev-ValAsterisk

ArchivoSQLpatch: revf-pluginasterisk.sql

ArchivoSQLdeins: 

Comentarios: Requiere el pluginalmacen y el plugincuadrante, sin los cuales no funcionará.
También incluye en el código fuente ejemplos de scritps de validacion hecho en php para ser
puestos en marcha en la centralita con agi.