#ifndef PHP_MAPNIK_PROJECTION_H
#define PHP_MAPNIK_PROJECTION_H

#include "php_mapnik.h"
#include <mapnik/projection.hpp>

extern zend_class_entry *projection_ce;

struct projection_object {
    mapnik::projection *projection;
    zend_object std;
};

static inline projection_object * fetch_projection_object(zend_object *obj) {
    return (projection_object *)((char *)obj - XtOffsetOf(struct projection_object, std));
}

#define Z_PHP_MAPNIK_PROJECTION_P(zv) fetch_projection_object(Z_OBJ_P(zv));

void init_projection(INIT_FUNC_ARGS);

#endif
