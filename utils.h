#ifndef UTILS_H
#define UTILS_H

#include <QDebug>
#include <QFile>
#include "json.hpp"

#ifndef st
#define st(X) do{X}while(0)
#endif

enum Graph:unsigned int {
    NONE,
    RECTANGLE       = 0x0001,
    ELLIPSE         = 0x0002,
    LINE            = 0x0004,
    CURVES          = 0x0008,
    POLYGON         = 0x0010,
    BROKEN_LINE     = 0x0020
};


// Log
//#ifndef LOG
//    #define ERROR       QMessageLogger(__FILE__, __LINE__, __FUNCTION__).critical()
//    #define INFO        QMessageLogger(__FILE__, __LINE__, __FUNCTION__).info()
//    #define DEBUG       QMessageLogger(__FILE__, __LINE__, __FUNCTION__).debug()
//    #define WARNING     QMessageLogger(__FILE__, __LINE__, __FUNCTION__).warning()
//    #define FATAL       QMessageLogger(__FILE__, __LINE__, __FUNCTION__).fatal()
//
//    #define LOG(X)      X
//#endif

#define LOAD_QSS(X, Y)			QFile file(Y);						\
								file.open(QFile::ReadOnly);			\
																	\
								if (file.isOpen()) {				\
									auto style = file.readAll();	\
									X->setStyleSheet(style);		\
									file.close();					\
								}

#define TO_STRING(x)    #x

#define ENABLE_IF_INT(Type)		typename std::enable_if_t<std::is_integral_v<_Tp>> * = nullptr
#define ENABLE_IF_REAL(Type)	typename std::enable_if_t<std::is_floating_point_v<Type>> * = nullptr

#define ENABLE_IF_CPU(Device)	typename std::enable_if_t<std::is_same_v<_Device, CPU>> * = nullptr
#define ENABLE_IF_GPU(Device)	typename std::enable_if_t<std::is_same_v<_Device, GPU>> * = nullptr


template <typename T>
void json_get(const nlohmann::json& json, const std::string & key, T& val)
{
	if (json.contains(key))
		val = json[key].get<T>();
}

#endif // UTILS_H
