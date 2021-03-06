#ifndef PHP_MAPNIK_PROJ_TRANSFORM_H
#define PHP_MAPNIK_PROJ_TRANSFORM_H

#include "php_mapnik.h"
#include <mapnik/proj_transform.hpp>

extern zend_class_entry *proj_transform_ce;

struct proj_transform_object {
    mapnik::proj_transform *proj_transform;
    zend_object std;
};

static inline proj_transform_object * fetch_proj_transform_object(zend_object *obj) {
    return (proj_transform_object *)((char *)obj - XtOffsetOf(struct proj_transform_object, std));
}

#define Z_PHP_MAPNIK_PROJ_TRANSFORM_P(zv) fetch_proj_transform_object(Z_OBJ_P(zv));

void init_proj_transform(INIT_FUNC_ARGS);

#endif
