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

// Class methods

PHP_METHOD(Box2D, __construct)
{
    php_mapnik_box2d_object *obj;
    mapnik::box2d<double> *box2d;
    double minX, minY, maxX, maxY;

    if (ZEND_NUM_ARGS() == 0) {
        box2d = new mapnik::box2d<double>();
    } else if (::zend_parse_parameters_ex(
        ZEND_PARSE_PARAMS_QUIET,
        ZEND_NUM_ARGS() TSRMLS_CC,
        "dddd",
        &minX,
        &minY,
        &maxX,
        &maxY) == SUCCESS
    ) {
        box2d = new mapnik::box2d<double>(minX, minY, maxX, maxY);
    } else {
        php_mapnik_throw_exception("Wrong arguments passed to \\Mapnik\\Box2D::__construct");
        RETURN_FALSE;
    }

    obj = Z_PHP_MAPNIK_BOX2D_P(getThis());
    obj->box2d = box2d;
}

zend_function_entry php_mapnik_box2d_methods[] = {
    PHP_ME(Box2D, __construct, NULL, ZEND_ACC_PUBLIC)
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