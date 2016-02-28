#ifndef PHP_MAPNIK_AGG_RENDERER_H
#define PHP_MAPNIK_AGG_RENDERER_H

#include "php.h"
#include <mapnik/agg_renderer.hpp>
#include <mapnik/image.hpp>

extern zend_class_entry *php_mapnik_agg_renderer_ce;

struct php_mapnik_agg_renderer_object {
    mapnik::agg_renderer<mapnik::image_rgba8> *agg_renderer;
    zend_object std;
};

static inline php_mapnik_agg_renderer_object * php_mapnik_agg_renderer_fetch_object(zend_object *obj) {
    return (php_mapnik_agg_renderer_object *)((char *)obj - XtOffsetOf(php_mapnik_agg_renderer_object, std));
}

#define Z_PHP_MAPNIK_AGG_RENDERER_P(zv) php_mapnik_agg_renderer_fetch_object(Z_OBJ_P(zv));

void php_mapnik_agg_renderer_startup(INIT_FUNC_ARGS);

#endif