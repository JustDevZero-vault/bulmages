#ifndef PDEFS_PLUGINBF_CLIENTEPROVEEDOR_H
#define PDEFS_PLUGINBF_CLIENTEPROVEEDOR_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBF_CLIENTEPROVEEDOR
#define PLUGINBF_CLIENTEPROVEEDOR_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_CLIENTEPROVEEDOR_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_CLIENTEPROVEEDOR_EXPORT

#endif


#endif
