#ifndef PHP_MAPNIK_IMAGE_H
#define PHP_MAPNIK_IMAGE_H


#include "php_mapnik.h"
#include <mapnik/map.hpp>

extern zend_class_entry *php_mapnik_image_ce;

struct php_mapnik_image_object {
    mapnik::image_rgba8 *image;
    zend_object std;
};

static inline php_mapnik_image_object * php_mapnik_image_fetch_object(zend_object *obj) {
    return (php_mapnik_image_object *)((char *)obj - XtOffsetOf(struct php_mapnik_image_object, std));
}

#define Z_PHP_MAPNIK_IMAGE_P(zv) php_mapnik_image_fetch_object(Z_OBJ_P(zv));

void php_mapnik_image_startup(INIT_FUNC_ARGS);

#endif
