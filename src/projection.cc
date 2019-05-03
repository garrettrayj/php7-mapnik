#include "exception.h"
#include "projection.h"
#include <mapnik/projection.hpp>

zend_class_entry *projection_ce;
zend_object_handlers projection_object_handlers;

// Class Methods

PHP_METHOD(Projection, __construct)
{
    projection_object *obj = Z_PHP_MAPNIK_PROJECTION_P(getThis());
    mapnik::projection* projection = NULL;

    zend_string *parameters;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_STR(parameters)
    ZEND_PARSE_PARAMETERS_END();

    try {
        std::string params_str(parameters->val, parameters->len);
        projection = new mapnik::projection(params_str);
    } catch (const mapnik::proj_init_error & ex) {
        throw_mapnik_exception(ex.what());
    } catch (const std::exception & ex) {
        throw_mapnik_exception(ex.what());
    } catch (...) {
        throw_mapnik_exception("Unknown exception thrown while constructing projection.");
    }

    obj->projection = projection;
}

// Reflection info

ZEND_BEGIN_ARG_INFO_EX(argInfo_projection_construct, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, parameters)
ZEND_END_ARG_INFO()

// Register methods

zend_function_entry projection_methods[] = {
    PHP_ME(Projection, __construct, argInfo_projection_construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
    {NULL, NULL, NULL}
};

// Internal object handling

void free_projection(zend_object *object TSRMLS_DC)
{
    projection_object *obj;
    obj = fetch_projection_object(object);
    delete obj->projection;
    zend_object_std_dtor(object TSRMLS_DC);
}

zend_object * create_projection(zend_class_entry *ce TSRMLS_DC) {
    projection_object *intern;
    intern = (projection_object*)
        ecalloc(1, sizeof(projection_object) + zend_object_properties_size(ce));

    zend_object_std_init(&intern->std, ce TSRMLS_CC);
    object_properties_init(&intern->std, ce);

    intern->std.handlers = &projection_object_handlers;

    return &intern->std;
}

// Extension class initializer

void init_projection(INIT_FUNC_ARGS)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Mapnik", "Projection", projection_methods);
    projection_ce = zend_register_internal_class(&ce TSRMLS_CC);
    projection_ce->create_object = create_projection;

    memcpy(&projection_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    projection_object_handlers.offset = XtOffsetOf(struct projection_object, std);
    projection_object_handlers.free_obj = &free_projection;
    projection_object_handlers.clone_obj = NULL;
}
