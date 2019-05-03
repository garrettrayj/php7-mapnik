#ifndef PHP_MAPNIK_MAP_H
#define PHP_MAPNIK_MAP_H

#include "php_mapnik.h"
#include <mapnik/map.hpp>

extern zend_class_entry *php_mapnik_map_ce;

struct php_mapnik_map_object {
    mapnik::Map *map;
    zend_object std;
};

static inline php_mapnik_map_object * php_mapnik_map_fetch_object(zend_object *obj) {
    return (php_mapnik_map_object *)((char *)obj - XtOffsetOf(php_mapnik_map_object, std));
}

#define Z_PHP_MAPNIK_MAP_P(zv) php_mapnik_map_fetch_object(Z_OBJ_P(zv));

void php_mapnik_map_startup(INIT_FUNC_ARGS);

#endif
