#ifndef NUMEROCUENTA_H
#define NUMEROCUENTA_H

#include <qstring.h>

/**\class numerocuenta
\brief C�digo Cuenta Cliente
@author David Guti�rrez Rubio

Esta clase implementa el C�digo Cuenta Cliente (CCC) de un banco. Su funci�n m�s importante es
 \ref cuentaesvalida
 */
class numerocuenta
  {
  public:
    numerocuenta(QString,QString,QString,QString);//Banco, entidad, dc y numcuenta.
    numerocuenta(QString,QString,QString);//Banco, entidad y numcuenta. El dc lo calcula automaticamente.
    numerocuenta(QString);//Banco, entidad, dc y numcuenta todo seguido, con guiones opcionales.
    ~numerocuenta();
    QString getdc();//Calcula y devuelve los digitos de control del ccc
/**  \brief Comprueba si es un n�mero de cuenta v�lido-
\retval true Si es un c�digo CCC correcto.
\retval false En caso contrario.
 */
    bool cuentaesvalida(){return((getdc()==dc));};
///Devuelve el CCC separando sus miembros por la cadena \c sep    
    QString getcodigo(QString sep=""){return(banco+sep+entidad+sep+dc+sep+numcuenta);};
    QString getnumcuenta(){return(numcuenta);};///Devuelve el n�mero de cuenta
    QString getbanco(){return(banco);};///Devuelve el n�mero de banco
    QString getentidad(){return(entidad);};///Devuelve el n�mero de entidad
 
/**\brief Fija el n�mero de cuenta

Si el argumento tiene menos de 10 d�gitos, se rellena con ceros a la izquierda
*/    
    void setnumcuenta(QString cad){numcuenta=cad.rightJustify(10,'0');};

/**\brief Fija el n�mero de banco

Si el argumento tiene menos de 4 d�gitos, se rellena con ceros a la izquierda
*/    
    void setbanco(QString cad){banco=cad.rightJustify(4,'0');;};
    /**\brief Fija el n�mero de entidad

Si el argumento tiene menos de 4 d�gitos, se rellena con ceros a la izquierda
*/    

    void setentidad(QString cad){entidad=cad.rightJustify(4,'0');;};
    
      
    private:
    QString banco;
    QString entidad;
    QString dc;
    QString numcuenta;

  };

#endif
