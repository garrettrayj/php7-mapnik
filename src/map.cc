#include "php.h"
#include "exception.h"
#include "box2d.h"
#include "map.h"
#include <mapnik/box2d.hpp>
#include <mapnik/load_map.hpp>
#include <mapnik/map.hpp>
#include <mapnik/projection.hpp>

zend_class_entry *php_mapnik_map_ce;
zend_object_handlers php_mapnik_map_object_handlers;

// PHP object handling

void php_mapnik_map_free_storage(zend_object *object TSRMLS_DC)
{
    php_mapnik_map_object *obj;
    obj = php_mapnik_map_fetch_object(object);
    delete obj->map;
    zend_object_std_dtor(object TSRMLS_DC);
}

zend_object * php_mapnik_map_new(zend_class_entry *ce TSRMLS_DC)
{
    // Allocate sizeof(custom) + sizeof(properties table requirements)
    php_mapnik_map_object *intern;
    intern = (php_mapnik_map_object*) ecalloc(1, sizeof(php_mapnik_map_object) + zend_object_properties_size(ce));

    zend_object_std_init(&intern->std, ce TSRMLS_CC);
    object_properties_init(&intern->std, ce);

    intern->std.handlers = &php_mapnik_map_object_handlers;

    return &intern->std;
}

// Class Method: Mapnik\Map::__construct

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_construct, 0, 0, 3)
    ZEND_ARG_INFO(0, width)
    ZEND_ARG_INFO(0, height)
    ZEND_ARG_INFO(0, srs)
ZEND_END_ARG_INFO()

PHP_METHOD(Map, __construct)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    mapnik::Map* map = NULL;

    zend_long width, height;
    zend_string *srs;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 3)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(width)
        Z_PARAM_LONG(height)
        Z_PARAM_STR(srs)
    ZEND_PARSE_PARAMETERS_END();

    try {
        if (ZEND_NUM_ARGS() == 0) {
            map = new mapnik::Map();
        } else if (ZEND_NUM_ARGS() == 2) {
            map = new mapnik::Map(width, height);
        } else if (ZEND_NUM_ARGS() == 3) {
            map = new mapnik::Map(width, height, srs->val);
        } else {
            php_mapnik_throw_exception("Wrong arguments passed to \\Mapnik\\Map::__construct");
        }
    } catch (const std::exception & ex) {
        php_mapnik_throw_exception(ex.what());
    } catch ( ... ) {
        php_mapnik_throw_exception("Unknown exception during Mapnik map instantiation.");
    }

    obj->map = map;
}

// Class Method: Mapnik\Map::loadXmlString

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_loadXmlString, 0, 0, 3)
    ZEND_ARG_INFO(0, xml)
    ZEND_ARG_INFO(0, strict)
    ZEND_ARG_INFO(0, basePath)
ZEND_END_ARG_INFO()

PHP_METHOD(Map, loadXmlString)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());

    zend_string *xml, *base_path;
    zend_bool strict = false;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 3)
        Z_PARAM_STR(xml)
        Z_PARAM_OPTIONAL
        Z_PARAM_BOOL(strict)
        Z_PARAM_STR(base_path)
    ZEND_PARSE_PARAMETERS_END();

    try {
        std::string xml_str(xml->val, xml->len);
        std::string base_path_str(base_path->val, base_path->len);
        mapnik::load_map_string(*obj->map, xml_str, strict, base_path_str);
    } catch (const mapnik::proj_init_error & ex) {
        php_mapnik_throw_exception(ex.what());
    } catch (const std::runtime_error & ex) {
        php_mapnik_throw_exception(ex.what());
    } catch (const std::exception & ex) {
        php_mapnik_throw_exception(ex.what());
    } catch (...) {
        php_mapnik_throw_exception("Unknown exception thrown while loading XML string.");
    }
}

// Class Method: Mapnik\Map::loadXmlFile

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_loadXmlFile, 0, 0, 3)
    ZEND_ARG_INFO(0, filename)
    ZEND_ARG_INFO(0, strict)
    ZEND_ARG_INFO(0, basePath)
ZEND_END_ARG_INFO()

PHP_METHOD(Map, loadXmlFile)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());

    zend_string *filename, *base_path;
    zend_bool strict = false;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 3)
        Z_PARAM_STR(filename)
        Z_PARAM_OPTIONAL
        Z_PARAM_BOOL(strict)
        Z_PARAM_STR(base_path)
    ZEND_PARSE_PARAMETERS_END();

    try {
        std::string filename_str(filename->val, filename->len);
        std::string base_path_str(base_path->val, base_path->len);
        mapnik::load_map(*obj->map, filename_str, strict, base_path_str);
    } catch (const mapnik::proj_init_error & ex) {
        php_mapnik_throw_exception(ex.what());
    } catch (const std::runtime_error & ex) {
        php_mapnik_throw_exception(ex.what());
    } catch (const std::exception & ex) {
        php_mapnik_throw_exception(ex.what());
    } catch (...) {
        php_mapnik_throw_exception("Unknown exception thrown while loading XML file.");
    }
}

// Class Method: Mapnik\Map::zoom

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_zoom, 0, 0, 1)
    ZEND_ARG_INFO(0, zoom)
ZEND_END_ARG_INFO()

PHP_METHOD(Map, zoom)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    double zoom;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_DOUBLE(zoom)
    ZEND_PARSE_PARAMETERS_END();

    obj->map->zoom(zoom);
}

// Class Method: Mapnik\Map::zoomAll

PHP_METHOD(Map, zoomAll)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());

    obj->map->zoom_all();
}

// Class Method: Mapnik\Map::pan

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_pan, 0, 0, 2)
    ZEND_ARG_INFO(0, x)
    ZEND_ARG_INFO(0, y)
ZEND_END_ARG_INFO()

PHP_METHOD(Map, pan)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    zend_long x, y;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 2, 2)
        Z_PARAM_LONG(x)
        Z_PARAM_LONG(y)
    ZEND_PARSE_PARAMETERS_END();

    obj->map->pan(x, y);
}

// Class Method: Mapnik\Map::panAndZoom

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_panAndZoom, 0, 0, 3)
    ZEND_ARG_INFO(0, x)
    ZEND_ARG_INFO(0, y)
    ZEND_ARG_INFO(0, zoom)
ZEND_END_ARG_INFO()

PHP_METHOD(Map, panAndZoom)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    zend_long x, y;
    double zoom;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 3, 3)
        Z_PARAM_LONG(x)
        Z_PARAM_LONG(y)
        Z_PARAM_DOUBLE(zoom)
    ZEND_PARSE_PARAMETERS_END();

    obj->map->pan_and_zoom(x, y, zoom);
}

// Class Method: Mapnik\Map::zoomToBox

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_zoomToBox, 0, 0, 1)
    ZEND_ARG_OBJ_INFO(0, box2d, Mapnik\\Box2D, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Map, zoomToBox)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    php_mapnik_box2d_object *box2d_obj;
    zval* box2d_zval;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_OBJECT_OF_CLASS(box2d_zval, php_mapnik_box2d_ce)
    ZEND_PARSE_PARAMETERS_END();

    box2d_obj = Z_PHP_MAPNIK_BOX2D_P(box2d_zval);
    obj->map->zoom_to_box(*box2d_obj->box2d);
}

// Class Method: Mapnik\Map::registerFonts

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_registerFonts, 0, 0, 1)
    ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

PHP_METHOD(Map, registerFonts)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    zend_string *path;
    bool fonts_registered = false;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_STR(path)
    ZEND_PARSE_PARAMETERS_END();

    try {
        std::string path_str(path->val, path->len);
        fonts_registered = obj->map->register_fonts(path_str);
    } catch (const std::exception & ex) {
        php_mapnik_throw_exception(ex.what());
        return;
    } catch (...) {
        php_mapnik_throw_exception("Unknown exception thrown while registering fonts path.");
        return;
    }

    if (fonts_registered) {
        RETURN_TRUE;
    }

    RETURN_FALSE;
}

// Class Method: Mapnik\Map::getBasePath

PHP_METHOD(Map, getBasePath)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());

    RETURN_STRING(obj->map->base_path().c_str());
}

// Class Method: Mapnik\Map::setBasePath

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_setBasePath, 0, 0, 1)
    ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

PHP_METHOD(Map, setBasePath)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    zend_string *path;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_STR(path)
    ZEND_PARSE_PARAMETERS_END();

    obj->map->set_base_path(path->val);
}

// Class Method: Mapnik\Map::getWidth

PHP_METHOD(Map, getWidth)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());

    RETURN_LONG(obj->map->width());
}

// Class Method: Mapnik\Map::setWidth

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_setWidth, 0, 0, 1)
    ZEND_ARG_INFO(0, width)
ZEND_END_ARG_INFO()

PHP_METHOD(Map, setWidth)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    zend_long width;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_LONG(width)
    ZEND_PARSE_PARAMETERS_END();

    obj->map->set_width(width);
}

// Class Method: Mapnik\Map::getHeight

PHP_METHOD(Map, getHeight)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());

    RETURN_LONG(obj->map->height());
}

// Class Method: Mapnik\Map::setHeight

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_setHeight, 0, 0, 1)
    ZEND_ARG_INFO(0, height)
ZEND_END_ARG_INFO()

PHP_METHOD(Map, setHeight)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    zend_long height;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_LONG(height)
    ZEND_PARSE_PARAMETERS_END();

    obj->map->set_height(height);
}

// Class Method: Mapnik\Map::resize

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_resize, 0, 0, 2)
    ZEND_ARG_INFO(0, width)
    ZEND_ARG_INFO(0, height)
ZEND_END_ARG_INFO()

PHP_METHOD(Map, resize)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    zend_long width, height;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 2, 2)
        Z_PARAM_LONG(width)
        Z_PARAM_LONG(height)
    ZEND_PARSE_PARAMETERS_END();

    obj->map->resize(width, height);
}

// Class Method: Mapnik\Map::getSrs

PHP_METHOD(Map, getSrs)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());

    RETURN_STRING(obj->map->srs().c_str());
}

// Class Method: Mapnik\Map::setSrs

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_setSrs, 0, 0, 1)
    ZEND_ARG_INFO(0, srs)
ZEND_END_ARG_INFO()

PHP_METHOD(Map, setSrs)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    zend_string *srs;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_STR(srs)
    ZEND_PARSE_PARAMETERS_END();

    obj->map->set_srs(srs->val);
}

// Class Method: Mapnik\Map::getBackgroundImage

PHP_METHOD(Map, getBackgroundImage)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    boost::optional<std::string> background_image = obj->map->background_image();

    if (background_image) {
        RETURN_STRING(background_image.get().c_str());
    }
}

// Class Method: Mapnik\Map::setBackgroundImage

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_setBackgroundImage, 0, 0, 1)
    ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()

PHP_METHOD(Map, setBackgroundImage)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    zend_string *image_filename;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_STR(image_filename)
    ZEND_PARSE_PARAMETERS_END();

    obj->map->set_background_image(image_filename->val);
}

// Class Method: Mapnik\Map::getBackgroundImageOpacity

PHP_METHOD(Map, getBackgroundImageOpacity)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());

    RETURN_DOUBLE(obj->map->background_image_opacity());
}

// Class Method: Mapnik\Map::setBackgroundImageOpacity

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_setBackgroundImageOpacity, 0, 0, 1)
    ZEND_ARG_INFO(0, opacity)
ZEND_END_ARG_INFO()

PHP_METHOD(Map, setBackgroundImageOpacity)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    double opacity;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_DOUBLE(opacity)
    ZEND_PARSE_PARAMETERS_END();

    obj->map->set_background_image_opacity(opacity);
}

// Class Method: Mapnik\Map::getMaximumExtent

PHP_METHOD(Map, getMaximumExtent)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    boost::optional<mapnik::box2d<double>> maximum_extent = obj->map->maximum_extent();

    if (maximum_extent) {
        // Return Box2D object
        object_init_ex(return_value, php_mapnik_box2d_ce);
        zval ctor;
        ZVAL_STRING(&ctor, "__construct");
        zval dummy_return_value;
        zval * args = NULL;
        args = (zval*)safe_emalloc(sizeof(zval), 4, 0);

        zval minX;
        ZVAL_DOUBLE(&minX, maximum_extent.get().minx());
        args[0] = minX;

        zval minY;
        ZVAL_DOUBLE(&minY, maximum_extent.get().miny());
        args[1] = minY;

        zval maxX;
        ZVAL_DOUBLE(&maxX, maximum_extent.get().maxx());
        args[2] = maxX;

        zval maxY;
        ZVAL_DOUBLE(&maxY, maximum_extent.get().maxy());
        args[3] = maxY;

        if (call_user_function(
            NULL,
            return_value,
            &ctor,
            &dummy_return_value,
            4,
            args TSRMLS_CC) == FAILURE
        ) {
            php_mapnik_throw_exception("Creating Box2D return value failed");
        }

        zval_ptr_dtor(&ctor);
        zval_ptr_dtor(&dummy_return_value);
        efree(args);
    }
}

// Class Method: Mapnik\Map::setMaximumExtent

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_setMaximumExtent, 0, 0, 1)
    ZEND_ARG_OBJ_INFO(0, box2d, Mapnik\\Box2D, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Map, setMaximumExtent)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    zval* box2d_zval;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_OBJECT_OF_CLASS(box2d_zval, php_mapnik_box2d_ce)
    ZEND_PARSE_PARAMETERS_END();

    php_mapnik_box2d_object *box2d_obj = Z_PHP_MAPNIK_BOX2D_P(box2d_zval);
    obj->map->set_maximum_extent(*box2d_obj->box2d);
}

// Class Method: Mapnik\Map::resetMaximumExtent

PHP_METHOD(Map, resetMaximumExtent)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    obj->map->reset_maximum_extent();
}

// Class Method: Mapnik\Map::getCurrentExtent

PHP_METHOD(Map, getCurrentExtent)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    mapnik::box2d<double> current_extent = obj->map->get_current_extent();

    // Return Box2D object
    object_init_ex(return_value, php_mapnik_box2d_ce);
    zval ctor;
    ZVAL_STRING(&ctor, "__construct");
    zval dummy_return_value;
    zval * args = NULL;
    args = (zval*)safe_emalloc(sizeof(zval), 4, 0);

    zval minX;
    ZVAL_DOUBLE(&minX, current_extent.minx());
    args[0] = minX;

    zval minY;
    ZVAL_DOUBLE(&minY, current_extent.miny());
    args[1] = minY;

    zval maxX;
    ZVAL_DOUBLE(&maxX, current_extent.maxx());
    args[2] = maxX;

    zval maxY;
    ZVAL_DOUBLE(&maxY, current_extent.maxy());
    args[3] = maxY;

    if (call_user_function(
        NULL,
        return_value,
        &ctor,
        &dummy_return_value,
        4,
        args TSRMLS_CC) == FAILURE
    ) {
        php_mapnik_throw_exception("Creating Box2D return value failed");
    }

    zval_ptr_dtor(&ctor);
    zval_ptr_dtor(&dummy_return_value);
    efree(args);
}

// Class Method: Mapnik\Map::getBufferSize

PHP_METHOD(Map, getBufferSize)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    RETURN_LONG(obj->map->buffer_size());
}

// Class Method: Mapnik\Map::setBufferSize

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_setBufferSize, 0, 0, 1)
    ZEND_ARG_INFO(0, bufferSize)
ZEND_END_ARG_INFO()

PHP_METHOD(Map, setBufferSize)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    zend_long buffer_size;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_LONG(buffer_size)
    ZEND_PARSE_PARAMETERS_END();

    obj->map->set_buffer_size(buffer_size);
}

// Class Method: Mapnik\Map::getBufferedExtent

PHP_METHOD(Map, getBufferedExtent)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    mapnik::box2d<double> buffered_extent = obj->map->get_buffered_extent();

    // Return Box2D object
    object_init_ex(return_value, php_mapnik_box2d_ce);
    zval ctor;
    ZVAL_STRING(&ctor, "__construct");
    zval dummy_return_value;
    zval * args = NULL;
    args = (zval*)safe_emalloc(sizeof(zval), 4, 0);

    zval minX;
    ZVAL_DOUBLE(&minX, buffered_extent.minx());
    args[0] = minX;

    zval minY;
    ZVAL_DOUBLE(&minY, buffered_extent.miny());
    args[1] = minY;

    zval maxX;
    ZVAL_DOUBLE(&maxX, buffered_extent.maxx());
    args[2] = maxX;

    zval maxY;
    ZVAL_DOUBLE(&maxY, buffered_extent.maxy());
    args[3] = maxY;

    if (call_user_function(
        NULL,
        return_value,
        &ctor,
        &dummy_return_value,
        4,
        args TSRMLS_CC) == FAILURE
    ) {
        php_mapnik_throw_exception("Creating Box2D return value failed");
    }

    zval_ptr_dtor(&ctor);
    zval_ptr_dtor(&dummy_return_value);
    efree(args);
}

// Class Method: Mapnik\Map::getScale

PHP_METHOD(Map, getScale)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    RETURN_DOUBLE(obj->map->scale());
}

// Class Method: Mapnik\Map::getScaleDenominator

PHP_METHOD(Map, getScaleDenominator)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    RETURN_DOUBLE(obj->map->scale_denominator());
}

// Class Method: Mapnik\Map::countLayers

PHP_METHOD(Map, countLayers)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    RETURN_LONG(obj->map->layer_count());
}

// Class Method: Mapnik\Map::removeStyle

PHP_METHOD(Map, removeStyle)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    zend_string *style;

    if (::zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "S", &style) == FAILURE) {
        php_mapnik_throw_exception("Wrong arguments passed to \\Mapnik\\Map::removeStyle");
        return;
    }

    obj->map->remove_style(style->val);
}

// Class Method: Mapnik\Map::removeAll

PHP_METHOD(Map, removeAll)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    obj->map->remove_all();
}

// Class Method: Mapnik\Map::getAspectFixMode

PHP_METHOD(Map, getAspectFixMode)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    RETURN_LONG(obj->map->get_aspect_fix_mode());
}

// Class Method: Mapnik\Map::setAspectFixMode

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_setAspectFixMode, 0, 0, 1)
    ZEND_ARG_INFO(0, aspectFixMode)
ZEND_END_ARG_INFO()

PHP_METHOD(Map, setAspectFixMode)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    zend_long aspect_fix_mode;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_LONG(aspect_fix_mode)
    ZEND_PARSE_PARAMETERS_END();

    if (!(aspect_fix_mode >= mapnik::Map::GROW_BBOX && aspect_fix_mode <= mapnik::Map::RESPECT)) {
        php_mapnik_throw_exception("Invalid aspect fix mode.");
        return;
    }

    obj->map->set_aspect_fix_mode(mapnik::Map::aspect_fix_mode(aspect_fix_mode));
}

// Register methods

zend_function_entry php_mapnik_map_methods[] = {
    PHP_ME(Map, __construct, argInfo_map_construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
    PHP_ME(Map, loadXmlFile, argInfo_map_loadXmlFile, ZEND_ACC_PUBLIC)
    PHP_ME(Map, loadXmlString, argInfo_map_loadXmlString, ZEND_ACC_PUBLIC)
    PHP_ME(Map, zoom, argInfo_map_zoom, ZEND_ACC_PUBLIC)
    PHP_ME(Map, zoomAll, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, pan, argInfo_map_pan, ZEND_ACC_PUBLIC)
    PHP_ME(Map, panAndZoom, argInfo_map_panAndZoom, ZEND_ACC_PUBLIC)
    PHP_ME(Map, zoomToBox, argInfo_map_zoomToBox, ZEND_ACC_PUBLIC)
    PHP_ME(Map, registerFonts, argInfo_map_registerFonts, ZEND_ACC_PUBLIC)
    PHP_ME(Map, getBasePath, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, setBasePath, argInfo_map_setBasePath, ZEND_ACC_PUBLIC)
    PHP_ME(Map, getWidth, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, setWidth, argInfo_map_setWidth, ZEND_ACC_PUBLIC)
    PHP_ME(Map, getHeight, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, setHeight, argInfo_map_setHeight, ZEND_ACC_PUBLIC)
    PHP_ME(Map, resize, argInfo_map_resize, ZEND_ACC_PUBLIC)
    PHP_ME(Map, getSrs, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, setSrs, argInfo_map_setSrs, ZEND_ACC_PUBLIC)
    PHP_ME(Map, getBackgroundImage, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, setBackgroundImage, argInfo_map_setBackgroundImage, ZEND_ACC_PUBLIC)
    PHP_ME(Map, getBackgroundImageOpacity, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, setBackgroundImageOpacity, argInfo_map_setBackgroundImageOpacity, ZEND_ACC_PUBLIC)
    PHP_ME(Map, getMaximumExtent, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, setMaximumExtent, argInfo_map_setMaximumExtent, ZEND_ACC_PUBLIC)
    PHP_ME(Map, resetMaximumExtent, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, getCurrentExtent, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, getBufferSize, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, setBufferSize, argInfo_map_setBufferSize, ZEND_ACC_PUBLIC)
    PHP_ME(Map, getBufferedExtent, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, getScale, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, getScaleDenominator, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, countLayers, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, removeStyle, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, removeAll, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, getAspectFixMode, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, setAspectFixMode, argInfo_map_setAspectFixMode, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

// Extension class startup

void php_mapnik_map_startup(INIT_FUNC_ARGS)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Mapnik", "Map", php_mapnik_map_methods);

    php_mapnik_map_ce = zend_register_internal_class(&ce TSRMLS_CC);
    php_mapnik_map_ce->create_object = php_mapnik_map_new;

    zend_declare_class_constant_long(
        php_mapnik_map_ce,
        "GROW_BBOX",
        sizeof("GROW_BBOX") - 1,
        mapnik::Map::GROW_BBOX TSRMLS_CC
    );

    zend_declare_class_constant_long(
        php_mapnik_map_ce,
        "GROW_CANVAS",
        sizeof("GROW_CANVAS") - 1,
        mapnik::Map::GROW_CANVAS TSRMLS_CC
    );

    zend_declare_class_constant_long(
        php_mapnik_map_ce,
        "SHRINK_BBOX",
        sizeof("SHRINK_BBOX") - 1,
        mapnik::Map::SHRINK_BBOX TSRMLS_CC
    );

    zend_declare_class_constant_long(
        php_mapnik_map_ce,
        "SHRINK_CANVAS",
        sizeof("SHRINK_CANVAS") - 1,
        mapnik::Map::SHRINK_CANVAS TSRMLS_CC
    );

    zend_declare_class_constant_long(
        php_mapnik_map_ce,
        "ADJUST_BBOX_WIDTH",
        sizeof("ADJUST_BBOX_WIDTH") - 1,
        mapnik::Map::ADJUST_BBOX_WIDTH TSRMLS_CC
    );

    zend_declare_class_constant_long(
        php_mapnik_map_ce,
        "ADJUST_BBOX_HEIGHT",
        sizeof("ADJUST_BBOX_HEIGHT") - 1,
        mapnik::Map::ADJUST_BBOX_HEIGHT TSRMLS_CC
    );

    zend_declare_class_constant_long(
        php_mapnik_map_ce,
        "ADJUST_CANVAS_WIDTH",
        sizeof("ADJUST_CANVAS_WIDTH") - 1,
        mapnik::Map::ADJUST_CANVAS_WIDTH TSRMLS_CC
    );

    zend_declare_class_constant_long(
        php_mapnik_map_ce,
        "ADJUST_CANVAS_HEIGHT",
        sizeof("ADJUST_CANVAS_HEIGHT") - 1,
        mapnik::Map::ADJUST_CANVAS_HEIGHT TSRMLS_CC
    );

    zend_declare_class_constant_long(
        php_mapnik_map_ce,
        "RESPECT",
        sizeof("RESPECT") - 1,
        mapnik::Map::RESPECT TSRMLS_CC
    );
    
    memcpy(&php_mapnik_map_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    php_mapnik_map_object_handlers.offset = XtOffsetOf(struct php_mapnik_map_object, std);
    php_mapnik_map_object_handlers.free_obj = &php_mapnik_map_free_storage;
    php_mapnik_map_object_handlers.clone_obj = NULL;
}
