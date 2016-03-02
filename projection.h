#ifndef PHP_MAPNIK_PROJECTION_H
#define PHP_MAPNIK_PROJECTION_H

#include "php.h"
#include <mapnik/projection.hpp>

extern zend_class_entry *php_mapnik_projection_ce;

struct php_mapnik_projection_object {
    mapnik::projection *projection;
    zend_object std;
};

static inline php_mapnik_projection_object * php_mapnik_projection_fetch_object(zend_object *obj) {
    return (php_mapnik_projection_object *)((char *)obj - XtOffsetOf(struct php_mapnik_projection_object, std));
}

#define Z_PHP_MAPNIK_PROJECTION_P(zv) php_mapnik_projection_fetch_object(Z_OBJ_P(zv));

void php_mapnik_projection_startup(INIT_FUNC_ARGS);

#endif