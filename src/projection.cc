#include "php.h"
#include "exception.h"
#include "projection.h"
#include <mapnik/projection.hpp>

zend_class_entry *php_mapnik_projection_ce;
zend_object_handlers php_mapnik_projection_object_handlers;

// PHP object handling

void php_mapnik_projection_free_storage(zend_object *object TSRMLS_DC)
{
    php_mapnik_projection_object *obj;
    obj = php_mapnik_projection_fetch_object(object);
    delete obj->projection;
    zend_object_std_dtor(object TSRMLS_DC);
}

zend_object * php_mapnik_projection_new(zend_class_entry *ce TSRMLS_DC) {
    // Allocate sizeof(custom) + sizeof(properties table requirements)
    php_mapnik_projection_object *intern;
    intern = (php_mapnik_projection_object*)
        ecalloc(1, sizeof(php_mapnik_projection_object) + zend_object_properties_size(ce));

    zend_object_std_init(&intern->std, ce TSRMLS_CC);
    object_properties_init(&intern->std, ce);

    intern->std.handlers = &php_mapnik_projection_object_handlers;

    return &intern->std;
}

// Class Methods

ZEND_BEGIN_ARG_INFO_EX(argInfo_projection_construct, 0, 0, 1)
    ZEND_ARG_INFO(0, parameters)
ZEND_END_ARG_INFO()

PHP_METHOD(Projection, __construct)
{
    php_mapnik_projection_object *obj = Z_PHP_MAPNIK_PROJECTION_P(getThis());
    mapnik::projection* projection = NULL;

    zend_string *parameters;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_STR(parameters)
    ZEND_PARSE_PARAMETERS_END();

    try {
        std::string params_str(parameters->val, parameters->len);
        projection = new mapnik::projection(params_str);
    } catch (const mapnik::proj_init_error & ex) {
        php_mapnik_throw_exception(ex.what());
    } catch (const std::exception & ex) {
        php_mapnik_throw_exception(ex.what());
    } catch (...) {
        php_mapnik_throw_exception("Unknown exception thrown while constructing projection.");
    }

    obj->projection = projection;
}

// Register methods

zend_function_entry php_mapnik_projection_methods[] = {
    PHP_ME(Projection, __construct, argInfo_projection_construct, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

// Extension class startup

void php_mapnik_projection_startup(INIT_FUNC_ARGS)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Mapnik", "Projection", php_mapnik_projection_methods);
    php_mapnik_projection_ce = zend_register_internal_class(&ce TSRMLS_CC);
    php_mapnik_projection_ce->create_object = php_mapnik_projection_new;

    memcpy(&php_mapnik_projection_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    php_mapnik_projection_object_handlers.offset = XtOffsetOf(struct php_mapnik_projection_object, std);
    php_mapnik_projection_object_handlers.free_obj = &php_mapnik_projection_free_storage;
    php_mapnik_projection_object_handlers.clone_obj = NULL;
}
