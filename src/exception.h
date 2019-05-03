#ifndef PHP_MAPNIK_EXCEPTION_H
#define PHP_MAPNIK_EXCEPTION_H

#include "php_mapnik.h"

extern zend_class_entry *exception_ce;

void init_exception(INIT_FUNC_ARGS);

void throw_mapnik_exception_with_message(const char *message, int code TSRMLS_DC);

#define throw_mapnik_exception(message) throw_mapnik_exception_with_message(message, 0)

#endif
