#ifndef PHP_MAPNIK_H
#define PHP_MAPNIK_H 1
#define PHP_MAPNIK_VERSION "1"
#define PHP_MAPNIK_EXTENSION_NAME "mapnik"

#include <config.h>

extern "C" {
    #include "php.h"
};

extern zend_module_entry mapnik_module_entry;
#define phpext_mapnik_ptr &mapnik_module_entry

#endif
