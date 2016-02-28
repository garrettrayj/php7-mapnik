#ifndef PHP_MAPNIK_EXCEPTION_H
#define PHP_MAPNIK_EXCEPTION_H

#include "php.h"

extern zend_class_entry *php_mapnik_exception_ce;

void php_mapnik_exception_startup(INIT_FUNC_ARGS);

void php_mapnik_throw_exception_with_message(const char *message, int code TSRMLS_DC);

#define php_mapnik_throw_exception(message) php_mapnik_throw_exception_with_message(message, 0)

#endif