#ifndef OPERATIONINTERFACE_GLOBAL_H
#define OPERATIONINTERFACE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(OPERATIONINTERFACE_LIBRARY)
#define OPERATIONINTERFACESHARED_EXPORT Q_DECL_EXPORT
#else
#define OPERATIONINTERFACESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // OPERATIONINTERFACE_GLOBAL_H
