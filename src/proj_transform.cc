#include "php.h"
#include "exception.h"
#include "box2d.h"
#include "projection.h"
#include "proj_transform.h"
#include <mapnik/projection.hpp>
#include <mapnik/proj_transform.hpp>

zend_class_entry *proj_transform_ce;
zend_object_handlers proj_transform_object_handlers;

// PHP object handling

void proj_transform_free_storage(zend_object *object TSRMLS_DC)
{
    proj_transform_object *obj;
    obj = proj_transform_fetch_object(object);
    delete obj->proj_transform;
    zend_object_std_dtor(object TSRMLS_DC);
}

zend_object * proj_transform_new(zend_class_entry *ce TSRMLS_DC) {
    // Allocate sizeof(custom) + sizeof(properties table requirements)
    proj_transform_object *intern;
    intern = (proj_transform_object*)
        ecalloc(1, sizeof(proj_transform_object) + zend_object_properties_size(ce));

    zend_object_std_init(&intern->std, ce TSRMLS_CC);
    object_properties_init(&intern->std, ce);

    intern->std.handlers = &proj_transform_object_handlers;

    return &intern->std;
}

// Class Methods

ZEND_BEGIN_ARG_INFO_EX(argInfo_projTransform_construct, 0, ZEND_RETURN_VALUE, 2)
    ZEND_ARG_INFO(0, source)
    ZEND_ARG_INFO(0, destination)
ZEND_END_ARG_INFO()

PHP_METHOD(ProjTransform, __construct)
{
    proj_transform_object *obj = Z_PHP_MAPNIK_PROJ_TRANSFORM_P(getThis());
    mapnik::proj_transform* proj_transform = NULL;
    obj->proj_transform = NULL;

    zval* source_zval;
    zval* destination_zval;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 2, 2)
        Z_PARAM_OBJECT_OF_CLASS(source_zval, projection_ce)
        Z_PARAM_OBJECT_OF_CLASS(destination_zval, projection_ce)
    ZEND_PARSE_PARAMETERS_END();

    projection_object *source_obj = Z_PHP_MAPNIK_PROJECTION_P(source_zval);
    mapnik::projection *source = new mapnik::projection(source_obj->projection->params());

    projection_object *destination_obj = Z_PHP_MAPNIK_PROJECTION_P(destination_zval);
    mapnik::projection *destination = new mapnik::projection(destination_obj->projection->params());

    proj_transform = new mapnik::proj_transform(*source, *destination);
    obj->proj_transform = proj_transform;
}

ZEND_BEGIN_ARG_INFO_EX(argInfo_projTransform_forward, 0, 0, 1)
    ZEND_ARG_INFO(0, box2d)
ZEND_END_ARG_INFO()

PHP_METHOD(ProjTransform, forward)
{
    proj_transform_object *obj = Z_PHP_MAPNIK_PROJ_TRANSFORM_P(getThis());

    zval* box2d_zval;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_OBJECT_OF_CLASS(box2d_zval, box2d_ce)
    ZEND_PARSE_PARAMETERS_END();

    box2d_object *box2d_obj = Z_PHP_MAPNIK_BOX2D_P(box2d_zval);
    mapnik::box2d<double> *box2d = box2d_obj->box2d;

    mapnik::proj_transform *proj_transform = obj->proj_transform;

    proj_transform->forward(*box2d);

    // Return Box2D object
    object_init_ex(return_value, box2d_ce);
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
        throw_mapnik_exception("Creating Box2D return value failed");
        RETURN_FALSE;
    }

    zval_ptr_dtor(&ctor);
    zval_ptr_dtor(&dummy_return_value);
    efree(args);
}

ZEND_BEGIN_ARG_INFO_EX(argInfo_projTransform_backward, 0, 0, 1)
    ZEND_ARG_INFO(0, box2d)
ZEND_END_ARG_INFO()

PHP_METHOD(ProjTransform, backward)
{
    proj_transform_object *obj = Z_PHP_MAPNIK_PROJ_TRANSFORM_P(getThis());

    zval* box2d_zval;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_OBJECT_OF_CLASS(box2d_zval, box2d_ce)
    ZEND_PARSE_PARAMETERS_END();

    box2d_object *box2d_obj = Z_PHP_MAPNIK_BOX2D_P(box2d_zval);
    mapnik::box2d<double> *box2d = box2d_obj->box2d;

    mapnik::proj_transform *proj_transform = obj->proj_transform;

    proj_transform->backward(*box2d);

    // Return Box2D object
    object_init_ex(return_value, box2d_ce);
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
        throw_mapnik_exception("Creating Box2D return value failed");
        RETURN_FALSE;
    }

    zval_ptr_dtor(&ctor);
    zval_ptr_dtor(&dummy_return_value);
    efree(args);
}

// Register methods

zend_function_entry proj_transform_methods[] = {
    PHP_ME(ProjTransform, __construct, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(ProjTransform, forward, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(ProjTransform, backward, NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

// Extension class startup

void proj_transform_startup(INIT_FUNC_ARGS)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Mapnik", "ProjTransform", proj_transform_methods);
    proj_transform_ce = zend_register_internal_class(&ce TSRMLS_CC);
    proj_transform_ce->create_object = proj_transform_new;

    memcpy(&proj_transform_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    proj_transform_object_handlers.offset = XtOffsetOf(struct proj_transform_object, std);
    proj_transform_object_handlers.free_obj = &proj_transform_free_storage;
    proj_transform_object_handlers.clone_obj = NULL;
}
