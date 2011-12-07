# -*- coding: utf-8 -*-

import sys
import os
from PyQt4.QtGui import *
from PyQt4.QtCore import *
from nuevousuariobase import *
from empresa import Empresa
from config import *
import psycopg2

class NuevoUsuario(Ui_NuevoUsuario, Empresa):

    def __init__(self, parent = None):
        Empresa.__init__(self,parent)
        self.setupUi(self)
        self.proceso = QtCore.QProcess()
        self.psql = functions.multios().search_executable('psql')

    def on_mui_botonera_accepted(self):
        # Creamos el usuario

        if (self.mui_superusuario.isChecked()):
            self.execComando(functions.as_postgres + "createuser -s -d -r  \'" + self.mui_nombre.text() + "\'" + functions.end_sql)
        else:
            self.execComando(functions.as_postgres + "createuser -S -d -r  \'" + self.mui_nombre.text() + "\'" + functions.end_sql)

        # Cambiamos el password del usuario
        self.execComando(self.psql + " template1 -c \"ALTER ROLE " + self.mui_nombre.text() + " WITH PASSWORD '" + self.mui_password.text() + "'\"")

        # Y como ya hemos acabado, cerramos la aplicacion.
        self.accept()

    def execComando(self, command):
        self.proceso.start(command)
        self.proceso.waitForFinished(-1)

def main(args):
    app=QtGui.QApplication(args)
    win=NuevoUsuario()
    win.exec_()
    sys.exit(app.exec_())

if __name__=="__main__":
    main(sys.argv)
