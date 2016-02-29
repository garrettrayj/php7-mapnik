#include "php.h"
#include "exception.h"
#include "agg_renderer.h"
#include "image.h"
#include "map.h"
#include <mapnik/image.hpp>

zend_class_entry *php_mapnik_agg_renderer_ce;
zend_object_handlers php_mapnik_agg_renderer_object_handlers;

// PHP object handling

void php_mapnik_agg_renderer_free_storage(zend_object *object TSRMLS_DC)
{
    php_mapnik_agg_renderer_object *obj;
    obj = php_mapnik_agg_renderer_fetch_object(object);
    zend_object_std_dtor(object TSRMLS_DC);
}

zend_object * php_mapnik_agg_renderer_new(zend_class_entry *ce TSRMLS_DC) {
    // Allocate sizeof(custom) + sizeof(properties table requirements)
    php_mapnik_agg_renderer_object *intern;
    intern = (php_mapnik_agg_renderer_object*)
        ecalloc(1, sizeof(php_mapnik_agg_renderer_object) + zend_object_properties_size(ce));

    zend_object_std_init(&intern->std, ce TSRMLS_CC);
    object_properties_init(&intern->std, ce);

    intern->std.handlers = &php_mapnik_agg_renderer_object_handlers;

    return &intern->std;
}

// Class Methods

PHP_METHOD(AggRenderer, __construct)
{
    struct php_mapnik_agg_renderer_object *obj;
    mapnik::agg_renderer<mapnik::image_rgba8> *agg_renderer = NULL;

    zval* map_zval;
    zval* image_zval;

    if (::zend_parse_parameters_ex(
        ZEND_PARSE_PARAMS_QUIET,
        ZEND_NUM_ARGS() TSRMLS_CC,
        "OO",
        &map_zval,
        php_mapnik_map_ce,
        &image_zval,
        php_mapnik_image_ce) == SUCCESS
    ) {
        php_mapnik_map_object *map_obj = Z_PHP_MAPNIK_MAP_P(map_zval);
        mapnik::Map *map = map_obj->map;

        php_mapnik_image_object *image_obj = Z_PHP_MAPNIK_IMAGE_P(image_zval);
        mapnik::image_rgba8 *image = image_obj->image;

        agg_renderer = new mapnik::agg_renderer<mapnik::image_rgba8>(*map, *image);
    } else {
        php_mapnik_throw_exception("Wrong arguments passed to \\Mapnik\\AggRenderer::__construct");
        RETURN_FALSE;
    }

    assert(agg_renderer != NULL);

    obj = Z_PHP_MAPNIK_AGG_RENDERER_P(getThis());
    obj->agg_renderer = agg_renderer;
}

PHP_METHOD(AggRenderer, apply)
{
    struct php_mapnik_agg_renderer_object *obj;
    mapnik::agg_renderer<mapnik::image_rgba8> *agg_renderer;

    obj = Z_PHP_MAPNIK_AGG_RENDERER_P(getThis());
    agg_renderer = obj->agg_renderer;
    agg_renderer->apply();

    RETURN_TRUE;
}

// Register methods

zend_function_entry php_mapnik_agg_renderer_methods[] = {
    PHP_ME(AggRenderer, __construct, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(AggRenderer, apply, NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

// Extension class startup

void php_mapnik_agg_renderer_startup(INIT_FUNC_ARGS)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Mapnik", "AggRenderer", php_mapnik_agg_renderer_methods);
    php_mapnik_agg_renderer_ce = zend_register_internal_class(&ce TSRMLS_CC);
    php_mapnik_agg_renderer_ce->create_object = php_mapnik_agg_renderer_new;
    
    memcpy(&php_mapnik_agg_renderer_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    php_mapnik_agg_renderer_object_handlers.offset = XtOffsetOf(struct php_mapnik_agg_renderer_object, std);
    php_mapnik_agg_renderer_object_handlers.free_obj = &php_mapnik_agg_renderer_free_storage;
    php_mapnik_agg_renderer_object_handlers.clone_obj = NULL;
}