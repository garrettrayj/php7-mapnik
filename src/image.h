#ifndef PHP_MAPNIK_IMAGE_H
#define PHP_MAPNIK_IMAGE_H


#include "php_mapnik.h"
#include <mapnik/map.hpp>

extern zend_class_entry *image_ce;

struct image_object {
    mapnik::image_rgba8 *image;
    zend_object std;
};

static inline image_object * image_fetch_object(zend_object *obj) {
    return (image_object *)((char *)obj - XtOffsetOf(struct image_object, std));
}

#define Z_PHP_MAPNIK_IMAGE_P(zv) image_fetch_object(Z_OBJ_P(zv));

void image_startup(INIT_FUNC_ARGS);

#endif
