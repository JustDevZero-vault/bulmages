-- ----------------------------------------------------------------------------------------
-- (C)  Joan Miquel Torrer Rigo & Tomeu Borras Riera & Mateu Borras Marco, Agosto 2004
--  joanmi@bulma.net, tborras@conetxia.com mborras@conetxia.com 
-- Este documento esta licenciado bajo licencia GPL, el cual no escribimos aqui por pereza.
--  ----------------------------------------------------------------------------------------
--     psql xxxx < bulmafact-0_0_1.sql
--  ---------------------------------------------------------------------------------------

SET SESSION AUTHORIZATION 'postgres';
SET search_path = public, pg_catalog;


CREATE FUNCTION plpgsql_call_handler() RETURNS language_handler
    AS '/usr/lib/postgresql/lib/plpgsql.so', 'plpgsql_call_handler'
    LANGUAGE c;


-- NOTACION:
-- Considerar las siguientes opciones de codificaci�n:
-- Los nombres de tabla estan escritos SIEMPRE en singular.
-- Todos los campos de una tabla terminan siempre con el nombre de la tabla (salvo las claves foraneas).
-- Las claves foraneas tienen el mismo nombre que el campo con que se corresponden en la tabla relacionada.
-- En caso de que haya diversas claves foraneas referentes al mismo campo, el criterio es que una de lleas tenga el nombre del campo con el que se corresponde y la otra tenga un nombre significativo.
-- Los campos de clave automatica empiezan por id    
-- Los enums se simulan (normalmente) con campos numericos, el significado de los valores debe estar
-- explicado en este archivo.
    


-- Codi: Clau artificial.
-- Nom
-- Url
-- Comentaris
CREATE TABLE marca (
   idmarca serial PRIMARY KEY,
   nommarca character varying(150),
   urlmarca character varying(150),
   comentmarca character varying(2000)
);


-- Linea de producto, las lineas de producto son una catalogaci�n de productos por linea.
-- Codi: Clau artificial.
-- Descripcio
CREATE TABLE linea_prod(
   idlinea_prod serial PRIMARY KEY,
   desclinea_prod character varying(500),
   idmarca integer REFERENCES marca(idmarca)
);



-- Esta tabla contiene las descripciones de los ivas que se pueden aplicar.
-- Codi: Clau artificial.
-- Descripcio: Text descriptiu del tipus d'IVA.
-- Taxa: Tipus vigent.
CREATE TABLE tipo_iva (
   idtipo_iva serial PRIMARY KEY,
   desctipo_iva character varying(2000),
   tasatipo_iva integer
);


--COMPROVACIONS D'INTEGRITAT>Genriques:
--Els articles "contenidors" de tots els components son articles composts.
--Els articles "contingut" de tots els components son articles no composts. 
--COMPROVACIONS D'INTEGRITAT>A realitzar en moments especEn convertir un article a compost, comprovar abans que no s component de ning.
--En afegir components a un article, comprovar que el nou component no s un article compost.
--PANTALLES ESTADISTIQUES>Articles:
--Nombre d'articles que no pertanyen a cap familia.
--
--tipoarticulo: [Simple | Compost calculat | Compost escalat | Ampliat]. Determina el tipus d'article i com calcular-ne el preu. Simple = Article no compost amb preu PVP; Compost = Article compost amb preu suma dels PVPs dels components (Calculat) o PVP (escalat); Ampliat = Compost d'ell mateix ms els components amb preu suma de PVP i PVPs dels components
--

-- Codi: Clau artificial.
-- Nom: Descripci� curta de l'article.
-- Descripcio: Descripci� completa (llarga) de l'article.
-- CBarres: Codi de barres.
-- Tipus: [Simple | Compost calculat | Compost escalat | Ampliat]. Determina el tipus d'article i com --  calcular-ne el preu. Simple = Article no compost amb preu PVP; Compost = Article compost amb preu suma dels PVPs dels components (Calculat) o PVP (escalat); Ampliat = Compost d'ell mateix m�s els components amb preu suma de PVP i PVPs dels components.
-- Descompte: Descompte "invisible" que s'aplica al preu resultant. �til sobretot per a diferenciar el preu dels articles composts del preu de compra per separat.
-- Especificacions: Camp de text per a comentaris i informacions varies.
-- Icona: Icona de l'article.
-- Fotografia: Fotografia de l'article.
-- Poster: Fotografia de gran tamany de l'article.
-- Marge: (Per defecte) Percentatge de c�lcul del PVP sobre el PVD. V�lid nom�s quan no estigui definit un marge espec�fic per aquest article i el prove�dor corresponent.
-- Sobrecost: (Per defecte) Import a sumar en el c�lcul del PVP en concepte de despeses de transport o altres.
-- Model: Refer�ncia, o nom identificatiu del fabricant.
CREATE TABLE articulo (
    idarticulo serial PRIMARY KEY,
    codarticulo character varying(12),
    nomarticulo character varying(50),
    descarticulo character varying(500),
    cbarrasarticulo character varying(22),
    tipoarticulo integer,
    descuentoarticulo float,
    especificacionesarticulo character varying(2000),
    iconoarticulo oid,
    fotoarticulo oid,
    posterarticulo oid,
    margenarticulo float,
    sobrecostearticulo float,
    modeloarticulo character varying(1000),
    
    idtipo_iva integer REFERENCES tipo_iva (idtipo_iva),
    idlinea_prod integer REFERENCES linea_prod(idlinea_prod)
);





-- COMPROVACIONS D'INTEGRITAT>Genriques:
-- Totes les families tenen, al menys, un ascendent de primer nivell i no son descendents de si mateixes. 
-- COMPROVACIONS DE RUTINA:
-- En assignar una nova familia a un article, verificar els catlegs no exclusius de la familia als que no pertany l'article i preguntar a l'usuari si vol que s'hi afegeixi. 
-- En desassignar un article d'una familia, verificar els catlegs no exclusius de la familia sortint als que tamb pertany l'article i no cap de les families a les que encara pertany i preguntar a l'usuari si vol treure'l de les mateixes i de quines.
-- La catalogaci
-- Codi: Clau artificial.
-- Nom: Descripci� curta de la familia.
-- Descripcio: Descripci� completa (llarga) de la familia.
-- Icona: Icona de l'article.
CREATE TABLE familia (
    idfamilia serial PRIMARY KEY,
    nombrefamilia character varying(50),
    descfamilia character varying(1000),
    iconofamilia oid
);


-- Yo me imagino que un catalogo es una agrupaci�n de articulos para poder hacer listados
-- de ofertas y cosas de estas.
--La catalogaci� per families tendr� prefer�ncia sobre la catalogaci� per articles. Si un article pertany a un cat�leg sel�leccionat per� no a cap familia sel�leccionada pel seu cat�leg, no ser� llistat.
--Codi: Clau artificial.
--Nom: Descripci� curta del cat�leg.
--Descripcio: Descripci� detallada del cat�leg.
--Exclusiu: (Boole�) La pertinen�a a un cat�leg exclusiu anula (que no esborra) la pertinen�a de l'article o familia a qualsevol cat�leg no exclusiu. Un article o familia nom�s pot pert�nyer a un �nic cat�leg exclusiu. �til per a cat�legs de descatalogaci� (descatalogats temporals, definitius, sense stock, pendents de revisi�...).
CREATE TABLE catalogo (
   idcatalogo serial PRIMARY KEY,
   nombrecatalogo character varying(50),
   desccatalogo character varying(1000),
   exclusivocatalogo boolean NOT NULL
);


--Numero: N�mero de divisi� (clau artificial).
--Descripcio: Nom o descripci� de la divisi�.
--Contactes: Nom de persona o persones de contacte.
--Comentaris
--Telf: Tel�fon.
--Fax: Fax.
--Email
CREATE TABLE division (
   iddivision serial PRIMARY KEY,
   descdivision character varying(1000),
   contactosdivisioon character varying(500),
   comentdivision character varying(2000),
   teldivision character varying(20),
   faxdivision character varying(20),
   maildivision character varying(100)
);


-- Los proveedores son los que nos suminstran articulos y/o servicios.
-- COMPROVACIONS D'INTEGRITAT>Gen�riques:
-- 1 Article t� 1 sol prove�dor principal.
-- 1 Article t� 1 sol prove�dor referent.
-- CAMPOS
-- ======
-- Codi: Clau artificial.
-- Nom: Nom comercial o fiscal.
-- Nom_alternatiu: Nom comercial o fiscal.
-- CIF: Codi d'Identificaci� Fiscal.
-- CodiCli: Codi de client amb que ens facturen. �til per a identificar-nos.
-- C_Banc
-- Comentaris
-- Adre�a: Adre�a.
-- Poblaci�: Poblaci�.
-- CProv: Codi de provincia (dos primers d�gits del codi postal).
-- sCP: Tres darrers d�gits del codi postal.
-- Telf: Tel�fon.
-- Fax: Fax.
-- Email: eMail.
-- Url: Url.
-- CompteWeb: Dades de login si disposen de tenda o tarifes en l�nia
CREATE TABLE proveedor (
   idproveedor serial PRIMARY KEY,
   nomproveedor character varying(200),
   nomaltproveedor character varying(200),
   cifproveedor character varying(12),
   codicliproveedor character varying(30),
   cbancproveedor character varying(20),
   comentproveedor character varying(2000),
   dirproveedor character varying(50),
   poblproveedor character varying(50),
   cpproveedor character varying(9) NOT NULL,
   telproveedor character varying(12),
   faxproveedor character varying(12),
   emailproveedor character varying(100),
   urlproveedor character varying(100),
   clavewebproveedor character varying(100),
   iddivision integer NOT NULL REFERENCES division(iddivision)
);



--TARIFES AUTOM�TIQUES:
--Els prearticles son els articles obtinguts de tarifes de prove�dors que no tenim com a articles per� que s'enregistren (i esborren en caducar) autom�ticament per oferir-nos una recerca r�pida sobre els articles dels nostres prove�dors que no hem treballat mai.
--Refpro: Refer�ncia del prove�dor.
--Descripcio: Descripci� de l'article.
--PVD
--Data: Data d'actualitzaci� del PVD del prearticle.
--Caducitat: Temps de vida (en dies) del prearticle a comptar des de la darrera data d'actualitzaci�.
-- Aix� ha de desapareixer aix� que no pos res aqui.
--CREATE TABLE prearticulo (
--);



-- La tabla recargo indica los recargos que se aplican a un cliente. O los descuentos.
-- Codi: Clau artificial.
-- Nom: Nom descriptiu del tipus de Rec�rrec.
-- Taxa: Percentatge de Rec�rrec d'Equival�ncia.
--  Rec�rrec d'Equival�ncia.
CREATE TABLE recargo (
   idrecargo serial PRIMARY KEY,
   nomrecargo character varying(150),
   tasarecargo float
);

INSERT INTO recargo (idrecargo, nomrecargo, tasarecargo) VALUES ('1', 'Tipo 1', '4.0');

-- El cliente siempre tiene la raz�n, bueno, o por lo menos eso cree.
--Codi: Clau artificial.
--Nom: Nom comercial o fiscal.
--Nom_alternatiu: Nom comercial o fiscal.
--CIF: Codi d'Identificaci� Fiscal.
--C_Banc: Compte Bancari.
--Adr: Adre�a.
--Pobl: Poblaci�.
--CProv: Codi de provincia (dos primers d�gits del codi postal).
--sCP: Tres darrers d�gits del codi postal.
--Telf: Tel�fon.
--Fax: Fax.
--Email: eMail.
--Url: Url.
--Data_alta
--Data_Baixa
---Comentaris
CREATE TABLE cliente (
   idcliente serial PRIMARY KEY,
   nomcliente character varying(100),
   nomaltcliente character varying(300),
   cifcliente character varying(200),
   bancocliente character varying(35),
   dircliente character varying(100),
   poblcliente character varying(40),
   cpcliente character varying(10),
   telcliente character varying(20),
   faxcliente character varying(20),
   mailcliente character varying(100),
   urlcliente character varying(150),
   faltacliente date DEFAULT NOW(),
   fbajacliente date,
   comentcliente character varying(2000),
   
   idrecargo integer NOT NULL REFERENCES recargo(idrecargo)
);


-- Numero: N�mero de sucursal (Clau artificial).
-- Nom: Nom de la sucursal.
-- Adr: Adre�a.
-- Pobl: Poblaci�.
-- CProv: Codi de provincia (dos primers d�gits del codi postal). 
-- sCP: Tres darrers d�gits del codi postal.
-- Telf: Tel�fon.
-- Fax: Fax.
-- Email: eMail.
-- Url: Url.
-- Comentaris
CREATE TABLE sucursal (
   idsucursal serial PRIMARY KEY,
   nomsucursal character varying(100),
   dirsucursal character varying(150),
   poblsucursal character varying(100),
   cpsucursal character varying(15),
   telsucursal character varying(20),
   faxsucursal character varying(20),
   mailsucursal character varying(50),
   comentsucursal character varying(2000),
   
   idcliente integer NOT NULL REFERENCES cliente(idcliente)
);



-- Any: Any en que s'efectua la comanda.
-- Numero: N�mero de comanda (comen�ant de 1 cada any).
-- Descripcio: Breu descripci� o comentari opcional.
-- Data: Data d'emisi� de la comanda.
CREATE TABLE pedido (
   idpedido serial PRIMARY KEY,
   numpedido integer,
   anopedido integer,
   fechapedido date,
   descpedido character varying(500),
   
   idproveedor integer NOT NULL REFERENCES proveedor(idproveedor)
);






-- Any: Any de facturaci�.
-- Numero: N�mero de factura.
-- Data Comentaris
-- Factura de prove�dor.
CREATE TABLE fra_pro (
   idfra_pro serial PRIMARY KEY,
   anofra_pro integer,
   numfra_pro integer,
   fcrefra_pro date,
   comentfra_pro character varying(2000)
);



-- Albaran de proveedor
-- Any: Any en que s'efectua la comanda.
-- NumCompra: Numero de Compra (Clau artificial per poder registrar recepcions que ens arribin sense l'albar� postposant la cumplimentaci� del n�mero d'albar�).
-- NumAlbara: N�mero d'albar�.
-- Data: Data de l'albar�
-- Recepcio: Data de recepci�.
-- Comentaris
CREATE TABLE alb_pro (
   idalb_pro serial PRIMARY KEY,
   anoalb_pro integer,
   ncompraalb_pro integer,
   nalbalb_pro integer,
   fcrealb_pro date,
   frecepalb_pro date,
   comentalb_pro character varying(2000),
   
   idfra_pro integer REFERENCES fra_pro(idfra_pro)
);



-- Linea de pedido
-- Numero: N�mero de l�nia.
-- Descripcio: Descripcio de l'article.
-- Quantitat
-- PVD
-- Previsi�: Data prevista de recepci�
CREATE TABLE lpedido (
   numlpedido integer PRIMARY KEY,
   desclpedido character varying(150),
   cantlpedido integer,
   pvdlpedido float,
   prevlpedido date,
   
   idpedido integer NOT NULL REFERENCES pedido(idpedido),
   idalb_pro integer REFERENCES alb_pro(idalb_pro),
   idarticulo integer REFERENCES articulo(idarticulo)
);




-- modalidad_g es la talba que contiene las modalidades de garanti9a.
-- Codi: Clau artificial.
-- Nom
-- Descripcio
CREATE TABLE modalidad_g (
   idmodalidad_g serial PRIMARY KEY,
   nommodalitat_g character varying(150),
   descmodalidad_g character varying(2000)
);


-- Codi
-- Descripcio
-- RetardC: Temps a transc�rrer per a l'inici de la cobertura.
-- TempsC: Temps de Cobertura.
-- Comentaris
-- Condicions de garantia. Defineixen un periode d'aplicabilitat d'una determinada modalitat de garantia per a una determinada l�nia de productes d'un mateix fabricant.
CREATE TABLE cond_garantia (
  idcond_garantia serial PRIMARY KEY,
  desccond_garantia character varying(2000),
  retardcond_garantia timestamp,
  tempsccond_garantia timestamp,
  comentcond_garantia character varying(2000),
  
  idmodalidad_g integer NOT NULL REFERENCES modalidad_g(idmodalidad_g)
);



-- Condicions de Venta, Agrupa articles, formes de pagament i clients.
CREATE TABLE condiciones_v (
   idcondiciones_v serial PRIMARY KEY
);

-- Numero
-- Descompte
-- AnyInici: Any d'inici d'aplicaci� (NULL => Peri�dic)
-- MesInici: Mes d'inici d'aplicaci� (NULL => Peri�dic)
-- DiaInici: Dia d'inici d'aplicaci� (NULL => Des de sempre)
-- AnyFi: Any de fi d'aplicaci� (NULL => Peri�dic)
-- MesFi: Mes de fi d'aplicaci� (NULL => Peri�dic)
-- DiaFi: Dia de fi d'aplicaci� (NULL => Indefinit)
-- Comentari
CREATE TABLE oferta (
   numoferta integer PRIMARY KEY,
   descoferta float,
   fechainicio timestamp,
   fechafin timestamp,
   periodicidad integer
);


-- Codi: Clau artificial.
-- Descripcio: Nom identificatiu o descripci� breu.
-- Dies_1T: Dies abans del primer termini computant els blocs de 30 com a mesos naturals.
-- Descompte: Descompte autom�tic per l'�s d'aquesta forma de pagament.
CREATE TABLE forma_pago (
   idforma_pago serial PRIMARY KEY,
   descforma_pago character varying(500),
   dias1tforma_pago integer,
   descuentoforma_pago float
);


-- Numero
-- Proporcio: Percentatge a abonar en aquest termini.
-- Dies: Dies a transcorrer a partir de la data de facturaci� computant els blocs de 30 com a mesos naturals.
CREATE TABLE termino_fp (
   idtermino_fp serial PRIMARY KEY,
   proportermino_fp float,
   diastermino_fp integer
);


-- Codi: Clau artificial.
-- Nom: Nom identificatiu del magatzem.
-- Adr: Adre�a.
-- Pobl: Poblaci�.
-- CProv: Codi de provincia (dos primers d�gits del codi postal).
-- sCP: Tres darrers d�gits del codi postal.
-- Telf: Tel�fon.
-- Fax: Fax.
CREATE TABLE almacen (
 idalmacen serial PRIMARY KEY,
 nomalmacen character varying(50),
 diralmacen character varying(150),
 poblalmacen character varying(50),
 cpalmacen character varying(20),
 telalmacen character varying(20),
 faxalmacen character varying(20)
);


-- Entendemos que un presupuesto es una relaci�n de materiales y trabajos cuantificada que
-- hacemos a petici�n de un cliente determinado
-- Numero
-- Data: Data d'emisi� del presupost.
-- PersContacte: Nom de persona de contacte (si cal).
-- TelfContacte: Tel�fon.
-- Venciment: Data m�xima de validesa del presupost.
-- Comentaris
--  Pressupost a clients.
CREATE TABLE presupuesto (
   idpresupuesto serial PRIMARY KEY,
   numpresupuesto integer,
   fpresupuesto date,
   contactpresupuesto character varying(90),
   telpresupuesto character varying(20),
   vencpresupuesto date,
   comentpresupuesto character varying(3000),
   idusuari integer,
   
   idcliente integer REFERENCES cliente(idcliente)
);


-- Descuento de presupuesto.
-- Numero
--Concepte: Descripci� del motiu de descompte.
--Proporcio: Percentatge a descomptar.
-- Descompte de pressupost a clients.
CREATE TABLE dpresupuesto (
   iddpresupuesto serial PRIMARY KEY,
   conceptpresupuesto character varying(2000),
   proporciondpresupuesto float,
   idpresupuesto integer REFERENCES presupuesto(idpresupuesto)
);



-- Linea de presupuesto
-- Numero
-- Descripcio: Descripci� de l'article en el moment de ser presupostat.
-- Quantitat
-- PVP: Preu de l'article en el moment de ser pressupostat
-- Descompte: Percentatge de descompte en l�nia.
-- Linia de pressupost a clients.
CREATE TABLE lpresupuesto (
   idlpresupuesto integer PRIMARY KEY,
   desclpresupuesto character varying(150),
   cantlpresupuesto float,
   pvplpresupuesto float,
   descuentolpresupuesto float,
   
   idpresupuesto integer NOT NULL REFERENCES presupuesto(idpresupuesto),
   idarituclo integer REFERENCES articulo(idarticulo)
);




-- FACTURACIO>Albarans:
-- Albarans pendents: S'entendran com albarans pendents tots aquells dels quals no existeixi ticket, factura ni nofactura.
-- Numero
-- Data
-- Factura a clients.
CREATE TABLE factura (
   numfactura integer PRIMARY KEY,
   fechafactura date
);




-- FACTURACIO>Albarans:
-- Albarans pendents: S'entendran com albarans pendents tots aquells dels quals no existeixi ticket, factura ni nofactura.
-- Numero: Numero de nofactura
-- Data
-- Concepte: Descripcio del concepte pel qual no es poden facturar els albarans aqui agrupats (garantia, -- contracte de manteniment, regals a clients, etc...).
-- Observacions
-- Agrupacio d'albarans no facturables en funci� d'un determinat concepte.
CREATE TABLE nofactura (
   numnofactura integer PRIMARY KEY,
   fechanofactura date,
   conceptnofactura character varying(150),
   observnofactura character varying(150)
);



-- COMPROVACIONS D'INTEGRITAT>Gen�riques:
-- Tots els albarans d'una factura corresponen al mateix client.
-- FACTURACIO>Albarans:
-- Albarans pendents: S'entendran com albarans pendents tots aquells dels quals no existeixi ticket, factura ni nofactura.
-- Numero
-- Data
-- Albar� a clients.
CREATE TABLE albaran (
   numalbaran integer PRIMARY KEY,
   fechaalbaran date,
   idusuario integer,

--   idpresupuesto integer REFERENCES presupuesto(idpresupuesto),
   idcliente integer REFERENCES cliente(idcliente),
   idforma_pago integer REFERENCES forma_pago(idforma_pago),
   numfactura integer REFERENCES factura(numfactura),
   numnofactura integer REFERENCES nofactura(numnofactura)
);



-- Descuento albaran
-- Numero
-- Concepte: Descripci� del motiu de descompte.
-- Proporcio: Percentatge a descomptar.
-- Descompte d'albar� a clients.
CREATE TABLE dalbaran (
   numdalbaran integer PRIMARY KEY,
   conceptdalbaran character varying(500),
   propordalbaran float,
   
   numalbaran integer REFERENCES albaran(numalbaran)
);


-- Numero
-- Descripcio
-- Quantitat
-- PVP: Preu de l'article en el moment de la compra o de ser presupostat.
-- Descompte
-- L�nia d'albar� a clients.
CREATE TABLE lalbaran (
   numlalbaran integer PRIMARY KEY,
   desclalbaran character varying(100),
   cantlalbaran float,
   pvplalbaran float,
   descontlalbaran float,
   
   numalbaran integer REFERENCES albaran(numalbaran),
   idarticulo integer REFERENCES articulo(idarticulo)
);



-- Numero
-- Data: Data del venciment.
-- Import: Import a abonar.
-- Abonat
-- Venciment d'un pagament.
CREATE TABLE vencimiento (
   idvencimiento serial PRIMARY KEY,
   fechavencimiento date,
   importevencimiento float,
   abonadovencimiento float,
   numfactura integer REFERENCES factura(numfactura)
);


-- FACTURACIO>Albarans:
-- Albarans pendents: S'entendran com albarans pendents tots aquells dels quals no existeixi ticket, factura ni nofactura.
CREATE TABLE ticket (
   numticket integer PRIMARY KEY,
   fechaticket date
);

-- COMPROVACIONS D'INTEGRITAT>Gen�riques:
-- 1 Article t� 1 sol prove�dor principal.
-- 1 Article t� 1 sol prove�dor referent.
CREATE TABLE suministra (
   idsuministra serial PRIMARY KEY,
   refpro integer,
   pvdsuministra float,
   beneficiosuministra float,
   sobrecostesuministra float,
   principalsuministra float,
   referentsuministra float,
   
   idproveedor integer REFERENCES proveedor(idproveedor)
--   idarticulo integer REFERENCES articulo(idarticulo)
);


-- Con cada pedido/albaran podemos asignar un numero de serie a los productos que han llegado.
-- Esta relaci�n es con los pedidos y no con los articulos pq los articulos son genericos
-- y los pedidos instancian los stocks de los pedidos.
--Numero: Numero de srie d'un article.
--Tipus: (Intern|Majorista|Fabricant) Indica si s un nmero intern (posat per nosaltres), del majorista o del fabricant.
-- Intern 1
-- Majorista 2
-- Fabricante 3
-- Segun mis calculos un numero de serie debe ir s�lo con un pedido o s�lo con un albaran
-- Asi que los campos pueden ser NULL pq o bien va con uno o bien va con el otro.
CREATE TABLE num_serie (
    numnum_serie character varying(30) PRIMARY KEY,
    tiponumserie integer,
    
    numlpedido integer REFERENCES lpedido (numlpedido),
    numlalbaran integer REFERENCES lalbaran (numlalbaran)
);


CREATE TABLE famC (
   idcatalogo integer REFERENCES catalogo(idcatalogo),
   idfamilia integer REFERENCES familia(idfamilia)
);


CREATE TABLE artC (
   idcatalogo integer REFERENCES catalogo(idcatalogo),
   idarticulo integer REFERENCES articulo(idarticulo)
);


CREATE TABLE hija (
   idfamilia integer REFERENCES familia(idfamilia),
   famhija integer REFERENCES familia(idfamilia)
);

CREATE TABLE agrupa (
   idfamilia integer REFERENCES familia(idfamilia),
   idarticulo integer REFERENCES articulo(idarticulo)
);

CREATE TABLE componente (
   idarticulo integer REFERENCES articulo(idarticulo),
   articulocomponente integer REFERENCES articulo(idarticulo),
   id integer,
   prioridad integer,
   descripcion character varying(200),
   cantidad float,
   escalable boolean
);

CREATE TABLE stock (
   idarticulo integer REFERENCES articulo(idarticulo),
   idalmacen integer REFERENCES almacen(idalmacen),
   minimo float,
   maximo float,
   actual float
);

CREATE TABLE dxfam (
   idcliente integer REFERENCES cliente(idcliente),
   idfamilia integer REFERENCES familia(idfamilia),
   descdxfam character varying(150)
);

CREATE TABLE dxart (
   idcliente integer REFERENCES cliente(idcliente),
   idarticulo integer REFERENCES articulo(idarticulo),
   descdxart character varying(150)
);

-- Esta tabla tiene algo que ver con las ofertas
-- pero aun no he descubierto que es (si alguien logra saberlo algun dia
-- por favor que satisfaga mi curiosidad.
CREATE TABLE dxof (
  idarticulo integer REFERENCES articulo(idarticulo),
  numoferta integer REFERENCES oferta(numoferta)
);

-- Esta debe de ser la tabla de representantes.
CREATE TABLE representa (
   idusuario integer, -- no hace referencia pq esta en base de datos externa y vinculada
   idcliente integer REFERENCES cliente(idcliente),
   permisrepresnta integer
);


-- Presupuesto - forma de pago
-- Esta tabla indica las formas de pago que tiene un presupuesto
CREATE TABLE prfp (
  idpresupuesto integer REFERENCES presupuesto(idpresupuesto),
  idforma_pago integer REFERENCES forma_pago(idforma_pago),
  descprfp float
);

-- Forma de pago posible para los articulos
CREATE TABLE afp (
   idarticulo integer REFERENCES articulo(idarticulo),
   idforma_pago integer REFERENCES forma_pago(idforma_pago)
);

CREATE TABLE albpre (
   idpresupuesto integer REFERENCES presupuesto(idpresupuesto),
   numalbaran integer REFERENCES albaran(numalbaran)
);

CREATE TABLE meta (
    id character varying(10) NOT NULL,
    valor character varying(50),
    PRIMARY KEY ("id")
);


CREATE TABLE usuarios (
    login character varying(15) NOT NULL,
    nombre character varying(35),
    apellido1 character varying(35),
    apellido2 character varying(35),
    clave character varying(35),
    permisos text,
    PRIMARY KEY ("login")
    
);