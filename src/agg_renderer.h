#ifndef PHP_MAPNIK_AGG_RENDERER_H
#define PHP_MAPNIK_AGG_RENDERER_H

#include "php_mapnik.h"
#include <mapnik/agg_renderer.hpp>
#include <mapnik/image.hpp>

extern zend_class_entry *agg_renderer_ce;

struct agg_renderer_object {
    mapnik::agg_renderer<mapnik::image_rgba8> *agg_renderer;
    zend_object std;
};

static inline agg_renderer_object * fetch_agg_renderer_object(zend_object *obj) {
    return (agg_renderer_object *)((char *)obj - XtOffsetOf(agg_renderer_object, std));
}

#define Z_PHP_MAPNIK_AGG_RENDERER_P(zv) fetch_agg_renderer_object(Z_OBJ_P(zv));

void init_agg_renderer(INIT_FUNC_ARGS);

#endif
