#ifndef PHP_MAPNIK_PROJ_TRANSFORM_H
#define PHP_MAPNIK_PROJ_TRANSFORM_H

#include "php_mapnik.h"
#include <mapnik/proj_transform.hpp>

extern zend_class_entry *php_mapnik_proj_transform_ce;

struct php_mapnik_proj_transform_object {
    mapnik::proj_transform *proj_transform;
    zend_object std;
};

static inline php_mapnik_proj_transform_object * php_mapnik_proj_transform_fetch_object(zend_object *obj) {
    return (php_mapnik_proj_transform_object *)((char *)obj - XtOffsetOf(struct php_mapnik_proj_transform_object, std));
}

#define Z_PHP_MAPNIK_PROJ_TRANSFORM_P(zv) php_mapnik_proj_transform_fetch_object(Z_OBJ_P(zv));

void php_mapnik_proj_transform_startup(INIT_FUNC_ARGS);

#endif
