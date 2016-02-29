#include "php.h"
#include "exception.h"
#include "box2d.h"
#include "projection.h"
#include "proj_transform.h"
#include <mapnik/projection.hpp>
#include <mapnik/proj_transform.hpp>

zend_class_entry *php_mapnik_proj_transform_ce;
zend_object_handlers php_mapnik_proj_transform_object_handlers;

// PHP object handling

void php_mapnik_proj_transform_free_storage(zend_object *object TSRMLS_DC)
{
    php_mapnik_proj_transform_object *obj;
    obj = php_mapnik_proj_transform_fetch_object(object);
    zend_object_std_dtor(object TSRMLS_DC);
}

zend_object * php_mapnik_proj_transform_new(zend_class_entry *ce TSRMLS_DC) {
    // Allocate sizeof(custom) + sizeof(properties table requirements)
    php_mapnik_proj_transform_object *intern;
    intern = (php_mapnik_proj_transform_object*)
        ecalloc(1, sizeof(php_mapnik_proj_transform_object) + zend_object_properties_size(ce));

    zend_object_std_init(&intern->std, ce TSRMLS_CC);
    object_properties_init(&intern->std, ce);

    intern->std.handlers = &php_mapnik_proj_transform_object_handlers;

    return &intern->std;
}

// Class Methods

PHP_METHOD(ProjTransform, __construct)
{
    php_mapnik_proj_transform_object *obj;
    mapnik::proj_transform* proj_transform = NULL;

    zval* source_zval;
    zval* destination_zval;

    if (::zend_parse_parameters_ex(
        ZEND_PARSE_PARAMS_QUIET,
        ZEND_NUM_ARGS() TSRMLS_CC,
        "OO",
        &source_zval,
        php_mapnik_projection_ce,
        &destination_zval,
        php_mapnik_projection_ce) == FAILURE
    ) {
        php_mapnik_throw_exception("Wrong arguments passed to \\Mapnik\\ProjTransform::__construct");
        RETURN_FALSE;
    }

    php_mapnik_projection_object *source_obj = Z_PHP_MAPNIK_PROJECTION_P(source_zval);
    mapnik::projection source = *source_obj->projection;

    php_mapnik_projection_object *destination_obj = Z_PHP_MAPNIK_PROJECTION_P(destination_zval);
    mapnik::projection destination = *destination_obj->projection;

    proj_transform = new mapnik::proj_transform(source, destination);

    obj = Z_PHP_MAPNIK_PROJ_TRANSFORM_P(getThis());
    obj->proj_transform = proj_transform;
}

PHP_METHOD(ProjTransform, forward)
{
    php_mapnik_proj_transform_object *obj;

    zval* box2d_zval;

    if (::zend_parse_parameters_ex(
        ZEND_PARSE_PARAMS_QUIET,
        ZEND_NUM_ARGS() TSRMLS_CC,
        "O",
        &box2d_zval,
        php_mapnik_box2d_ce) == FAILURE
    ) {
        php_mapnik_throw_exception("Wrong arguments passed to \\Mapnik\\ProjTransform::forward");
        RETURN_FALSE;
    }

    php_mapnik_box2d_object *box2d_obj = Z_PHP_MAPNIK_BOX2D_P(box2d_zval);
    mapnik::box2d<double> *box2d = box2d_obj->box2d;

    obj = Z_PHP_MAPNIK_PROJ_TRANSFORM_P(getThis());
    mapnik::proj_transform *proj_transform = obj->proj_transform;

    proj_transform->forward(*box2d);

    // Return Box2D object
    object_init_ex(return_value, php_mapnik_box2d_ce);
    zval ctor;
    ZVAL_STRING(&ctor, "__construct");
    zval dummy_return_value;
    zval * args = NULL;
    args = (zval*)safe_emalloc(sizeof(zval), 4, 0);

    // MinX argument
    zval minX;
    ZVAL_DOUBLE(&minX, box2d->minx());
    args[0] = minX;

    // MinY argument
    zval minY;
    ZVAL_DOUBLE(&minY, box2d->miny());
    args[1] = minY;

    // MaxX argument
    zval maxX;
    ZVAL_DOUBLE(&maxX, box2d->maxx());
    args[2] = maxX;

    // MaxY argument
    zval maxY;
    ZVAL_DOUBLE(&maxY, box2d->maxy());
    args[3] = maxY;

    // call_user_function(HashTable *function_table, zval **object_pp, zval *function_name, zval *retval_ptr, zend_uint param_count, zval *params[] TSRMLS_DC);

    if (call_user_function(
        NULL,
        return_value,
        &ctor,
        &dummy_return_value,
        4,
        args TSRMLS_CC) == FAILURE
    ) {
        php_mapnik_throw_exception("Creating Box2D return value failed");
        RETURN_FALSE;
    }

    zval_ptr_dtor(&ctor);
    zval_ptr_dtor(&dummy_return_value);
    efree(args);
}

PHP_METHOD(ProjTransform, backward)
{
    php_mapnik_proj_transform_object *obj;

    zval* box2d_zval;

    if (::zend_parse_parameters_ex(
        ZEND_PARSE_PARAMS_QUIET,
        ZEND_NUM_ARGS() TSRMLS_CC,
        "O",
        &box2d_zval,
        php_mapnik_box2d_ce) == FAILURE
    ) {
        php_mapnik_throw_exception("Wrong arguments passed to \\Mapnik\\ProjTransform::backward");
        RETURN_FALSE;
    }

    php_mapnik_box2d_object *box2d_obj = Z_PHP_MAPNIK_BOX2D_P(box2d_zval);
    mapnik::box2d<double> *box2d = box2d_obj->box2d;

    obj = Z_PHP_MAPNIK_PROJ_TRANSFORM_P(getThis());
    mapnik::proj_transform *proj_transform = obj->proj_transform;

    proj_transform->backward(*box2d);

    // Return Box2D object
    object_init_ex(return_value, php_mapnik_box2d_ce);
    zval ctor;
    ZVAL_STRING(&ctor, "__construct");
    zval dummy_return_value;
    zval * args = NULL;
    args = (zval*)safe_emalloc(sizeof(zval), 4, 0);

    // MinX argument
    zval minX;
    ZVAL_DOUBLE(&minX, box2d->minx());
    args[0] = minX;

    // MinY argument
    zval minY;
    ZVAL_DOUBLE(&minY, box2d->miny());
    args[1] = minY;

    // MaxX argument
    zval maxX;
    ZVAL_DOUBLE(&maxX, box2d->maxx());
    args[2] = maxX;

    // MaxY argument
    zval maxY;
    ZVAL_DOUBLE(&maxY, box2d->maxy());
    args[3] = maxY;

    // call_user_function(HashTable *function_table, zval **object_pp, zval *function_name, zval *retval_ptr, zend_uint param_count, zval *params[] TSRMLS_DC);

    if (call_user_function(
        NULL,
        return_value,
        &ctor,
        &dummy_return_value,
        4,
        args TSRMLS_CC) == FAILURE
    ) {
        php_mapnik_throw_exception("Creating Box2D return value failed");
        RETURN_FALSE;
    }

    zval_ptr_dtor(&ctor);
    zval_ptr_dtor(&dummy_return_value);
    efree(args);
}

// Register methods

zend_function_entry php_mapnik_proj_transform_methods[] = {
    PHP_ME(ProjTransform, __construct, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(ProjTransform, forward, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(ProjTransform, backward, NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

// Extension class startup

void php_mapnik_proj_transform_startup(INIT_FUNC_ARGS)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Mapnik", "ProjTransform", php_mapnik_proj_transform_methods);
    php_mapnik_proj_transform_ce = zend_register_internal_class(&ce TSRMLS_CC);
    php_mapnik_proj_transform_ce->create_object = php_mapnik_proj_transform_new;

    memcpy(&php_mapnik_proj_transform_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    php_mapnik_proj_transform_object_handlers.offset = XtOffsetOf(struct php_mapnik_proj_transform_object, std);
    php_mapnik_proj_transform_object_handlers.free_obj = &php_mapnik_proj_transform_free_storage;
    php_mapnik_proj_transform_object_handlers.clone_obj = NULL;
}