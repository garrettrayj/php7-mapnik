#ifndef PHP_MAPNIK_MAP_H
#define PHP_MAPNIK_MAP_H

#include "php_mapnik.h"
#include <mapnik/map.hpp>

extern zend_class_entry *map_ce;

struct map_object {
    mapnik::Map *map;
    zend_object std;
};

static inline map_object * map_fetch_object(zend_object *obj) {
    return (map_object *)((char *)obj - XtOffsetOf(map_object, std));
}

#define Z_PHP_MAPNIK_MAP_P(zv) map_fetch_object(Z_OBJ_P(zv));

void map_startup(INIT_FUNC_ARGS);

#endif
