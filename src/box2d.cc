#include "php.h"
#include "exception.h"
#include "box2d.h"
#include <mapnik/box2d.hpp>

zend_class_entry *box2d_ce;
zend_object_handlers box2d_object_handlers;

// Class methods

PHP_METHOD(Box2D, __construct)
{
    box2d_object *obj;
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

PHP_METHOD(Box2D, minX)
{
    box2d_object *obj = Z_PHP_MAPNIK_BOX2D_P(getThis());

    RETURN_DOUBLE(obj->box2d->minx());
}

PHP_METHOD(Box2D, minY)
{
    box2d_object *obj = Z_PHP_MAPNIK_BOX2D_P(getThis());

    RETURN_DOUBLE(obj->box2d->miny());
}

PHP_METHOD(Box2D, maxX)
{
    box2d_object *obj = Z_PHP_MAPNIK_BOX2D_P(getThis());

    RETURN_DOUBLE(obj->box2d->maxx());
}

PHP_METHOD(Box2D, maxY)
{
    box2d_object *obj = Z_PHP_MAPNIK_BOX2D_P(getThis());

    RETURN_DOUBLE(obj->box2d->maxy());
}

// Reflection info

ZEND_BEGIN_ARG_INFO_EX(argInfo_box2d_construct, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
    ZEND_ARG_INFO(0, minX)
    ZEND_ARG_INFO(0, minY)
    ZEND_ARG_INFO(0, maxX)
    ZEND_ARG_INFO(0, maxY)
ZEND_END_ARG_INFO()

// Register methods

zend_function_entry box2d_methods[] = {
    PHP_ME(Box2D, __construct, argInfo_box2d_construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
    PHP_ME(Box2D, minX, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Box2D, minY, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Box2D, maxX, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Box2D, maxY, NULL, ZEND_ACC_PUBLIC)
    { NULL, NULL, NULL }
};

// Internal object handling

void free_box2d(zend_object *object TSRMLS_DC)
{
    box2d_object *obj;
    obj = fetch_box2d_object(object);
    delete obj->box2d;
    zend_object_std_dtor(object TSRMLS_DC);
}

zend_object * create_box2d(zend_class_entry *ce TSRMLS_DC) {
    box2d_object *intern;
    intern = (box2d_object*) ecalloc(1, sizeof(box2d_object) + zend_object_properties_size(ce));

    zend_object_std_init(&intern->std, ce TSRMLS_CC);
    object_properties_init(&intern->std, ce);

    intern->std.handlers = &box2d_object_handlers;

    return &intern->std;
}

// Extension class initializer

void init_box2d(INIT_FUNC_ARGS)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Mapnik", "Box2D", box2d_methods);
    box2d_ce = zend_register_internal_class(&ce TSRMLS_CC);
    box2d_ce->create_object = create_box2d;

    memcpy(&box2d_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    box2d_object_handlers.offset = XtOffsetOf(struct box2d_object, std);
    box2d_object_handlers.free_obj = &free_box2d;
    box2d_object_handlers.clone_obj = NULL;
}
