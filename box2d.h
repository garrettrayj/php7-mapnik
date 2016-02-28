#ifndef PHP_MAPNIK_BOX2D_H
#define PHP_MAPNIK_BOX2D_H

#include "php.h"
#include <mapnik/map.hpp>

extern zend_class_entry *php_mapnik_box2d_ce;

struct php_mapnik_box2d_object {
    mapnik::box2d<double> *box2d;
    zend_object std;
};

static inline php_mapnik_box2d_object * php_mapnik_box2d_fetch_object(zend_object *obj) {
    return (php_mapnik_box2d_object *)((char *)obj - XtOffsetOf(struct php_mapnik_box2d_object, std));
}

#define Z_PHP_MAPNIK_BOX2D_P(zv) php_mapnik_box2d_fetch_object(Z_OBJ_P(zv));

void php_mapnik_box2d_startup(INIT_FUNC_ARGS);

#endif