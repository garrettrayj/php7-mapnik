#include "php.h"
#include "exception.h"
#include "image.h"
#include <mapnik/image.hpp>
#include <mapnik/image_util.hpp>

zend_class_entry *php_mapnik_image_ce;
zend_object_handlers php_mapnik_image_object_handlers;

// PHP object handling

void php_mapnik_image_free_storage(zend_object *object TSRMLS_DC)
{
    php_mapnik_image_object *obj;
    obj = php_mapnik_image_fetch_object(object);
    zend_object_std_dtor(object TSRMLS_DC);
}

zend_object * php_mapnik_image_new(zend_class_entry *ce TSRMLS_DC) {
    // Allocate sizeof(custom) + sizeof(properties table requirements)
    php_mapnik_image_object *intern;
    intern = (php_mapnik_image_object*) ecalloc(1, sizeof(php_mapnik_image_object) + zend_object_properties_size(ce));

    zend_object_std_init(&intern->std, ce TSRMLS_CC);
    object_properties_init(&intern->std, ce);

    intern->std.handlers = &php_mapnik_image_object_handlers;

    return &intern->std;
}

// Class Methods

PHP_METHOD(Image, __construct)
{
    php_mapnik_image_object *obj;
    mapnik::image_rgba8 *image;

    long width, height;

    if (::zend_parse_parameters_ex(
        ZEND_PARSE_PARAMS_QUIET,
        ZEND_NUM_ARGS() TSRMLS_CC,
        "ll",
        &width,
        &height) == SUCCESS
    ) {
        image = new mapnik::image_rgba8(width, height);
    } else {
        php_mapnik_throw_exception("Wrong arguments passed to \\Mapnik\\Image::__construct");
        RETURN_FALSE;
    }

    obj = Z_PHP_MAPNIK_IMAGE_P(getThis());
    obj->image = image;
}

PHP_METHOD(Image, saveToFile)
{
    php_mapnik_image_object *obj;
    zend_string *file;

    if (::zend_parse_parameters_ex(
        ZEND_PARSE_PARAMS_QUIET,
        ZEND_NUM_ARGS() TSRMLS_CC,
        "S",
        &file) == FAILURE
    ) {
        php_mapnik_throw_exception("Wrong arguments passed to \\Mapnik\\Image::saveToFile");
        RETURN_FALSE;
    }

    obj = Z_PHP_MAPNIK_IMAGE_P(getThis());
    if (!obj->image->painted()) {
        php_mapnik_throw_exception("Mapnik image has not been painted.");
        RETURN_FALSE;
    }

    try {
        std::string file_str(file->val, file->len);
        mapnik::save_to_file(*obj->image, file_str, "png");
    } catch (const mapnik::image_writer_exception & ex) {
        php_mapnik_throw_exception(ex.what());
        RETURN_FALSE;
    } catch (const std::exception & ex) {
        php_mapnik_throw_exception(ex.what());
        RETURN_FALSE;
    } catch (...) {
        php_mapnik_throw_exception("Unknown exception thrown while saving image to file.");
        RETURN_FALSE;
    }

    RETURN_TRUE;
}

PHP_METHOD(Image, saveToString)
{
    php_mapnik_image_object *obj;
    std::string image_str;

    obj = Z_PHP_MAPNIK_IMAGE_P(getThis());
    if (!obj->image->painted()) {
        php_mapnik_throw_exception("Mapnik image has not been painted.");
        RETURN_FALSE;
    }

    try {
        image_str = mapnik::save_to_string(*obj->image, "png");
    } catch (const mapnik::image_writer_exception & ex) {
        php_mapnik_throw_exception(ex.what());
        RETURN_FALSE;
    } catch (const std::exception & ex) {
        php_mapnik_throw_exception(ex.what());
        RETURN_FALSE;
    } catch (...) {
        php_mapnik_throw_exception("Unknown exception thrown while saving image to string.");
        RETURN_FALSE;
    }

    RETURN_STRINGL(image_str.c_str(), image_str.size());
}

// Register methods

zend_function_entry php_mapnik_image_methods[] = {
    PHP_ME(Image, __construct, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Image, saveToFile, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Image, saveToString, NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

// Extension class startup

void php_mapnik_image_startup(INIT_FUNC_ARGS)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Mapnik", "Image", php_mapnik_image_methods);
    php_mapnik_image_ce = zend_register_internal_class(&ce TSRMLS_CC);
    php_mapnik_image_ce->create_object = php_mapnik_image_new;

    memcpy(&php_mapnik_image_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    php_mapnik_image_object_handlers.offset = XtOffsetOf(struct php_mapnik_image_object, std);
    php_mapnik_image_object_handlers.free_obj = &php_mapnik_image_free_storage;
    php_mapnik_image_object_handlers.clone_obj = NULL;
}