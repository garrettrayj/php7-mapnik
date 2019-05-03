#include "php.h"
#include "exception.h"
#include "box2d.h"
#include <mapnik/box2d.hpp>

zend_class_entry *php_mapnik_box2d_ce;
zend_object_handlers php_mapnik_box2d_object_handlers;

// PHP object handling

void php_mapnik_box2d_free_storage(zend_object *object TSRMLS_DC)
{
    php_mapnik_box2d_object *obj;
    obj = php_mapnik_box2d_fetch_object(object);
    delete obj->box2d;
    zend_object_std_dtor(object TSRMLS_DC);
}

zend_object * php_mapnik_box2d_new(zend_class_entry *ce TSRMLS_DC) {
    // Allocate sizeof(custom) + sizeof(properties table requirements)
    php_mapnik_box2d_object *intern;
    intern = (php_mapnik_box2d_object*) ecalloc(1, sizeof(php_mapnik_box2d_object) + zend_object_properties_size(ce));

    zend_object_std_init(&intern->std, ce TSRMLS_CC);
    object_properties_init(&intern->std, ce);

    intern->std.handlers = &php_mapnik_box2d_object_handlers;

    return &intern->std;
}

// Class Method: Mapnik\Box2D::__construct

ZEND_BEGIN_ARG_INFO_EX(argInfo_box2d_construct, 0, 0, 0)
    ZEND_ARG_INFO(0, minX)
    ZEND_ARG_INFO(0, minY)
    ZEND_ARG_INFO(0, maxX)
    ZEND_ARG_INFO(0, maxY)
ZEND_END_ARG_INFO()

PHP_METHOD(Box2D, __construct)
{
    php_mapnik_box2d_object *obj;
    mapnik::box2d<double> *box2d = NULL;
    double minX, minY, maxX, maxY;

    if (ZEND_NUM_ARGS() == 0) {
        box2d = new mapnik::box2d<double>();
    } else {
        ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 4, 4)
            Z_PARAM_DOUBLE(minX)
            Z_PARAM_DOUBLE(minY)
            Z_PARAM_DOUBLE(maxX)
            Z_PARAM_DOUBLE(maxY)
        ZEND_PARSE_PARAMETERS_END();

        box2d = new mapnik::box2d<double>(minX, minY, maxX, maxY);
    }

    obj = Z_PHP_MAPNIK_BOX2D_P(getThis());
    obj->box2d = box2d;
}

// Class Method: Mapnik\Box2D::minX

PHP_METHOD(Box2D, minX)
{
    php_mapnik_box2d_object *obj = Z_PHP_MAPNIK_BOX2D_P(getThis());

    RETURN_DOUBLE(obj->box2d->minx());
}

// Class Method: Mapnik\Box2D::minY

PHP_METHOD(Box2D, minY)
{
    php_mapnik_box2d_object *obj = Z_PHP_MAPNIK_BOX2D_P(getThis());

    RETURN_DOUBLE(obj->box2d->miny());
}

// Class Method: Mapnik\Box2D::maxX

PHP_METHOD(Box2D, maxX)
{
    php_mapnik_box2d_object *obj = Z_PHP_MAPNIK_BOX2D_P(getThis());

    RETURN_DOUBLE(obj->box2d->maxx());
}

// Class Method: Mapnik\Box2D::maxY

PHP_METHOD(Box2D, maxY)
{
    php_mapnik_box2d_object *obj = Z_PHP_MAPNIK_BOX2D_P(getThis());

    RETURN_DOUBLE(obj->box2d->maxy());
}

// Register methods

zend_function_entry php_mapnik_box2d_methods[] = {
    PHP_ME(Box2D, __construct, argInfo_box2d_construct, ZEND_ACC_PUBLIC)
    PHP_ME(Box2D, minX, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Box2D, minY, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Box2D, maxX, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Box2D, maxY, NULL, ZEND_ACC_PUBLIC)
    { NULL, NULL, NULL }
};

// Extension class startup

void php_mapnik_box2d_startup(INIT_FUNC_ARGS)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Mapnik", "Box2D", php_mapnik_box2d_methods);
    php_mapnik_box2d_ce = zend_register_internal_class(&ce TSRMLS_CC);
    php_mapnik_box2d_ce->create_object = php_mapnik_box2d_new;

    memcpy(&php_mapnik_box2d_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    php_mapnik_box2d_object_handlers.offset = XtOffsetOf(struct php_mapnik_box2d_object, std);
    php_mapnik_box2d_object_handlers.free_obj = &php_mapnik_box2d_free_storage;
    php_mapnik_box2d_object_handlers.clone_obj = NULL;
}
