#ifndef PHP_MAPNIK_BOX2D_H
#define PHP_MAPNIK_BOX2D_H

#include "php_mapnik.h"
#include <mapnik/box2d.hpp>

extern zend_class_entry *box2d_ce;

struct box2d_object {
    mapnik::box2d<double> *box2d;
    zend_object std;
};

static inline box2d_object * box2d_fetch_object(zend_object *obj) {
    return (box2d_object *)((char *)obj - XtOffsetOf(struct box2d_object, std));
}

#define Z_PHP_MAPNIK_BOX2D_P(zv) box2d_fetch_object(Z_OBJ_P(zv));

void box2d_startup(INIT_FUNC_ARGS);

#endif
