#include "agg_renderer.h"
#include "exception.h"
#include "image.h"
#include "map.h"

#include <mapnik/datasource.hpp>
#include <mapnik/image.hpp>

zend_class_entry *agg_renderer_ce;
zend_object_handlers agg_renderer_object_handlers;

// PHP object handling

void free_agg_renderer(zend_object *object TSRMLS_DC)
{
    agg_renderer_object *obj;
    obj = agg_renderer_fetch_object(object);
    delete obj->agg_renderer;
    zend_object_std_dtor(object TSRMLS_DC);
}

zend_object * create_agg_renderer(zend_class_entry *ce TSRMLS_DC) {
    agg_renderer_object *intern;
    intern = (agg_renderer_object*) ecalloc(1, sizeof(agg_renderer_object) + zend_object_properties_size(ce));

    zend_object_std_init(&intern->std, ce TSRMLS_CC);
    object_properties_init(&intern->std, ce);

    intern->std.handlers = &agg_renderer_object_handlers;

    return &intern->std;
}

// Class methods

PHP_METHOD(AggRenderer, __construct)
{
    struct agg_renderer_object *obj;
    mapnik::agg_renderer<mapnik::image_rgba8> *agg_renderer = NULL;

    zval* map_zval;
    zval* image_zval;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 2, 2)
        Z_PARAM_OBJECT_OF_CLASS(map_zval, php_mapnik_map_ce)
        Z_PARAM_OBJECT_OF_CLASS(image_zval, php_mapnik_image_ce)
    ZEND_PARSE_PARAMETERS_END();

    php_mapnik_map_object *map_obj = Z_PHP_MAPNIK_MAP_P(map_zval);
    mapnik::Map *map = map_obj->map;

    php_mapnik_image_object *image_obj = Z_PHP_MAPNIK_IMAGE_P(image_zval);
    mapnik::image_rgba8 *image = image_obj->image;

    agg_renderer = new mapnik::agg_renderer<mapnik::image_rgba8>(*map, *image);

    obj = Z_PHP_MAPNIK_AGG_RENDERER_P(getThis());
    obj->agg_renderer = agg_renderer;
}

PHP_METHOD(AggRenderer, apply)
{
    struct agg_renderer_object *obj = Z_PHP_MAPNIK_AGG_RENDERER_P(getThis());
    mapnik::agg_renderer<mapnik::image_rgba8> *agg_renderer;

    agg_renderer = obj->agg_renderer;

    try {
        agg_renderer->apply();
    } catch (const mapnik::datasource_exception & ex) {
        php_mapnik_throw_exception(ex.what());
        return;
    } catch (const std::runtime_error & ex) {
        php_mapnik_throw_exception(ex.what());
        return;
    }

    RETURN_TRUE;
}

// Argument(s) reflection info

ZEND_BEGIN_ARG_INFO_EX(argInfo_aggRenderer_construct, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 2)
    ZEND_ARG_OBJ_INFO(0, map, Mapnik\\Map, 0)
    ZEND_ARG_OBJ_INFO(0, image, Mapnik\\Image, 0)
ZEND_END_ARG_INFO()

// Register class methods

zend_function_entry agg_renderer_methods[] = {
    PHP_ME(AggRenderer, __construct, argInfo_aggRenderer_construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
    PHP_ME(AggRenderer, apply, NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

// Extension class initializer

void init_agg_renderer(INIT_FUNC_ARGS)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Mapnik", "AggRenderer", agg_renderer_methods);
    agg_renderer_ce = zend_register_internal_class(&ce TSRMLS_CC);
    agg_renderer_ce->create_object = create_agg_renderer;

    memcpy(&agg_renderer_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    agg_renderer_object_handlers.offset = XtOffsetOf(struct agg_renderer_object, std);
    agg_renderer_object_handlers.free_obj = &free_agg_renderer;
    agg_renderer_object_handlers.clone_obj = NULL;
}
