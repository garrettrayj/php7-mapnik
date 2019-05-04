#include "exception.h"
#include "box2d.h"
#include "map.h"
#include <mapnik/box2d.hpp>
#include <mapnik/load_map.hpp>
#include <mapnik/map.hpp>
#include <mapnik/projection.hpp>

zend_class_entry *map_ce;
zend_object_handlers map_object_handlers;

// Class methods

PHP_METHOD(Map, __construct)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
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
            map = new mapnik::Map(width, height, ZSTR_VAL(srs));
        } else {
            throw_mapnik_exception("Wrong arguments passed to \\Mapnik\\Map::__construct");
        }
    } catch (const std::exception & ex) {
        throw_mapnik_exception(ex.what());
    } catch ( ... ) {
        throw_mapnik_exception("Unknown exception during Mapnik map instantiation.");
    }

    obj->map = map;
}

PHP_METHOD(Map, loadXmlString)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());

    zend_string *xml, *base_path;
    zend_bool strict = false;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 3)
        Z_PARAM_STR(xml)
        Z_PARAM_OPTIONAL
        Z_PARAM_BOOL(strict)
        Z_PARAM_STR(base_path)
    ZEND_PARSE_PARAMETERS_END();

    try {
        std::string xml_str(ZSTR_VAL(xml), ZSTR_LEN(xml));
        std::string base_path_str(ZSTR_VAL(base_path), ZSTR_LEN(base_path));
        mapnik::load_map_string(*obj->map, xml_str, strict, base_path_str);
    } catch (const mapnik::proj_init_error & ex) {
        throw_mapnik_exception(ex.what());
    } catch (const std::runtime_error & ex) {
        throw_mapnik_exception(ex.what());
    } catch (const std::exception & ex) {
        throw_mapnik_exception(ex.what());
    } catch (...) {
        throw_mapnik_exception("Unknown exception thrown while loading XML string.");
    }
}

PHP_METHOD(Map, loadXmlFile)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());

    zend_string *filename, *base_path;
    zend_bool strict = false;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 3)
        Z_PARAM_STR(filename)
        Z_PARAM_OPTIONAL
        Z_PARAM_BOOL(strict)
        Z_PARAM_STR(base_path)
    ZEND_PARSE_PARAMETERS_END();

    try {
        std::string filename_str(ZSTR_VAL(filename), ZSTR_LEN(filename));
        std::string base_path_str(ZSTR_VAL(base_path), ZSTR_LEN(base_path));
        mapnik::load_map(*obj->map, filename_str, strict, base_path_str);
    } catch (const mapnik::proj_init_error & ex) {
        throw_mapnik_exception(ex.what());
    } catch (const std::runtime_error & ex) {
        throw_mapnik_exception(ex.what());
    } catch (const std::exception & ex) {
        throw_mapnik_exception(ex.what());
    } catch (...) {
        throw_mapnik_exception("Unknown exception thrown while loading XML file.");
    }
}

PHP_METHOD(Map, zoom)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    double zoom;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_DOUBLE(zoom)
    ZEND_PARSE_PARAMETERS_END();

    obj->map->zoom(zoom);
}

PHP_METHOD(Map, zoomAll)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());

    obj->map->zoom_all();
}

PHP_METHOD(Map, pan)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    zend_long x, y;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 2, 2)
        Z_PARAM_LONG(x)
        Z_PARAM_LONG(y)
    ZEND_PARSE_PARAMETERS_END();

    obj->map->pan(x, y);
}

PHP_METHOD(Map, panAndZoom)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    zend_long x, y;
    double zoom;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 3, 3)
        Z_PARAM_LONG(x)
        Z_PARAM_LONG(y)
        Z_PARAM_DOUBLE(zoom)
    ZEND_PARSE_PARAMETERS_END();

    obj->map->pan_and_zoom(x, y, zoom);
}

PHP_METHOD(Map, zoomToBox)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    box2d_object *box2d_obj;
    zval* box2d_zval;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_OBJECT_OF_CLASS(box2d_zval, box2d_ce)
    ZEND_PARSE_PARAMETERS_END();

    box2d_obj = Z_PHP_MAPNIK_BOX2D_P(box2d_zval);
    obj->map->zoom_to_box(*box2d_obj->box2d);
}

PHP_METHOD(Map, registerFonts)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    zend_string *path;
    bool fonts_registered = false;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_STR(path)
    ZEND_PARSE_PARAMETERS_END();

    try {
        std::string path_str(ZSTR_VAL(path), ZSTR_LEN(path));
        fonts_registered = obj->map->register_fonts(path_str);
    } catch (const std::exception & ex) {
        throw_mapnik_exception(ex.what());
        return;
    } catch (...) {
        throw_mapnik_exception("Unknown exception thrown while registering fonts path.");
        return;
    }

    if (fonts_registered) {
        RETURN_TRUE;
    }

    RETURN_FALSE;
}

PHP_METHOD(Map, getBasePath)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());

    RETURN_STRING(obj->map->base_path().c_str());
}

PHP_METHOD(Map, setBasePath)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    zend_string *path;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_STR(path)
    ZEND_PARSE_PARAMETERS_END();

    obj->map->set_base_path(ZSTR_VAL(path));
}

PHP_METHOD(Map, getWidth)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());

    RETURN_LONG(obj->map->width());
}

PHP_METHOD(Map, setWidth)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    zend_long width;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_LONG(width)
    ZEND_PARSE_PARAMETERS_END();

    obj->map->set_width(width);
}

PHP_METHOD(Map, getHeight)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());

    RETURN_LONG(obj->map->height());
}

PHP_METHOD(Map, setHeight)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    zend_long height;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_LONG(height)
    ZEND_PARSE_PARAMETERS_END();

    obj->map->set_height(height);
}

PHP_METHOD(Map, resize)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    zend_long width, height;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 2, 2)
        Z_PARAM_LONG(width)
        Z_PARAM_LONG(height)
    ZEND_PARSE_PARAMETERS_END();

    obj->map->resize(width, height);
}

PHP_METHOD(Map, getSrs)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());

    RETURN_STRING(obj->map->srs().c_str());
}

PHP_METHOD(Map, setSrs)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    zend_string *srs;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_STR(srs)
    ZEND_PARSE_PARAMETERS_END();

    obj->map->set_srs(ZSTR_VAL(srs));
}

PHP_METHOD(Map, getBackgroundImage)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    boost::optional<std::string> background_image = obj->map->background_image();

    if (background_image) {
        RETURN_STRING(background_image.get().c_str());
    }
}

PHP_METHOD(Map, setBackgroundImage)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    zend_string *image_filename;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_STR(image_filename)
    ZEND_PARSE_PARAMETERS_END();

    obj->map->set_background_image(ZSTR_VAL(image_filename));
}

PHP_METHOD(Map, getBackgroundImageOpacity)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());

    RETURN_DOUBLE(obj->map->background_image_opacity());
}

PHP_METHOD(Map, setBackgroundImageOpacity)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    double opacity;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_DOUBLE(opacity)
    ZEND_PARSE_PARAMETERS_END();

    obj->map->set_background_image_opacity(opacity);
}

PHP_METHOD(Map, getMaximumExtent)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    boost::optional<mapnik::box2d<double>> maximum_extent = obj->map->maximum_extent();

    if (maximum_extent) {
        zval box2d_zval = construct_box2d_zval(
            maximum_extent.get().minx(),
            maximum_extent.get().miny(),
            maximum_extent.get().maxx(),
            maximum_extent.get().maxy()
        );

        RETURN_ZVAL(&box2d_zval, false, false);
    }
}

PHP_METHOD(Map, setMaximumExtent)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    zval* box2d_zval;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_OBJECT_OF_CLASS(box2d_zval, box2d_ce)
    ZEND_PARSE_PARAMETERS_END();

    box2d_object *box2d_obj = Z_PHP_MAPNIK_BOX2D_P(box2d_zval);
    obj->map->set_maximum_extent(*box2d_obj->box2d);
}

PHP_METHOD(Map, resetMaximumExtent)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    obj->map->reset_maximum_extent();
}

PHP_METHOD(Map, getCurrentExtent)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    mapnik::box2d<double> current_extent = obj->map->get_current_extent();

    zval box2d_zval = construct_box2d_zval(
        current_extent.minx(),
        current_extent.miny(),
        current_extent.maxx(),
        current_extent.maxy()
    );

    RETURN_ZVAL(&box2d_zval, false, false);
}

PHP_METHOD(Map, getBufferSize)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    RETURN_LONG(obj->map->buffer_size());
}

PHP_METHOD(Map, setBufferSize)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    zend_long buffer_size;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_LONG(buffer_size)
    ZEND_PARSE_PARAMETERS_END();

    obj->map->set_buffer_size(buffer_size);
}

PHP_METHOD(Map, getBufferedExtent)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    mapnik::box2d<double> buffered_extent = obj->map->get_buffered_extent();

    zval box2d_zval = construct_box2d_zval(
        buffered_extent.minx(),
        buffered_extent.miny(),
        buffered_extent.maxx(),
        buffered_extent.maxy()
    );

    RETURN_ZVAL(&box2d_zval, false, false);
}

PHP_METHOD(Map, getScale)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    RETURN_DOUBLE(obj->map->scale());
}

PHP_METHOD(Map, getScaleDenominator)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    RETURN_DOUBLE(obj->map->scale_denominator());
}

PHP_METHOD(Map, countLayers)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    RETURN_LONG(obj->map->layer_count());
}

PHP_METHOD(Map, removeStyle)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    zend_string *style;

    if (::zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "S", &style) == FAILURE) {
        throw_mapnik_exception("Wrong arguments passed to \\Mapnik\\Map::removeStyle");
        return;
    }

    obj->map->remove_style(ZSTR_VAL(style));
}

PHP_METHOD(Map, removeAll)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    obj->map->remove_all();
}

PHP_METHOD(Map, getAspectFixMode)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    RETURN_LONG(obj->map->get_aspect_fix_mode());
}

PHP_METHOD(Map, setAspectFixMode)
{
    map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    zend_long aspect_fix_mode;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_LONG(aspect_fix_mode)
    ZEND_PARSE_PARAMETERS_END();

    if (!(aspect_fix_mode >= mapnik::Map::GROW_BBOX && aspect_fix_mode <= mapnik::Map::RESPECT)) {
        throw_mapnik_exception("Invalid aspect fix mode.");
        return;
    }

    obj->map->set_aspect_fix_mode(mapnik::Map::aspect_fix_mode(aspect_fix_mode));
}

// Reflection info

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_construct, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, width)
    ZEND_ARG_INFO(0, height)
    ZEND_ARG_INFO(0, srs)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_loadXmlString, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, xml)
    ZEND_ARG_INFO(0, strict)
    ZEND_ARG_INFO(0, basePath)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_loadXmlFile, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, filename)
    ZEND_ARG_INFO(0, strict)
    ZEND_ARG_INFO(0, basePath)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_zoom, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, zoom)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_pan, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 2)
    ZEND_ARG_INFO(0, x)
    ZEND_ARG_INFO(0, y)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_panAndZoom, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 3)
    ZEND_ARG_INFO(0, x)
    ZEND_ARG_INFO(0, y)
    ZEND_ARG_INFO(0, zoom)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_zoomToBox, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_OBJ_INFO(0, box2d, Mapnik\\Box2D, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_registerFonts, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_setBasePath, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_setWidth, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, width)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_setHeight, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, height)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_resize, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 2)
    ZEND_ARG_INFO(0, width)
    ZEND_ARG_INFO(0, height)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_setSrs, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, srs)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_setBackgroundImage, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_setBackgroundImageOpacity, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, opacity)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_setMaximumExtent, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_OBJ_INFO(0, box2d, Mapnik\\Box2D, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_setBufferSize, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, bufferSize)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(argInfo_map_setAspectFixMode, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, aspectFixMode)
ZEND_END_ARG_INFO()

// Register methods

zend_function_entry map_methods[] = {
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

// Internal object handling

void free_map(zend_object *object TSRMLS_DC)
{
    map_object *obj;
    obj = fetch_map_object(object);
    delete obj->map;
    zend_object_std_dtor(object TSRMLS_DC);
}

zend_object * create_map(zend_class_entry *ce TSRMLS_DC)
{
    map_object *intern;
    intern = (map_object*) ecalloc(1, sizeof(map_object) + zend_object_properties_size(ce));

    zend_object_std_init(&intern->std, ce TSRMLS_CC);
    object_properties_init(&intern->std, ce);

    intern->std.handlers = &map_object_handlers;

    return &intern->std;
}

// Extension class startup

void init_map(INIT_FUNC_ARGS)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Mapnik", "Map", map_methods);

    map_ce = zend_register_internal_class(&ce TSRMLS_CC);
    map_ce->create_object = create_map;

    zend_declare_class_constant_long(map_ce, "GROW_BBOX", sizeof("GROW_BBOX") - 1, mapnik::Map::GROW_BBOX TSRMLS_CC);
    zend_declare_class_constant_long(map_ce, "GROW_CANVAS", sizeof("GROW_CANVAS") - 1, mapnik::Map::GROW_CANVAS TSRMLS_CC);
    zend_declare_class_constant_long(map_ce, "SHRINK_BBOX", sizeof("SHRINK_BBOX") - 1, mapnik::Map::SHRINK_BBOX TSRMLS_CC);
    zend_declare_class_constant_long(map_ce, "SHRINK_CANVAS", sizeof("SHRINK_CANVAS") - 1, mapnik::Map::SHRINK_CANVAS TSRMLS_CC);
    zend_declare_class_constant_long(map_ce, "ADJUST_BBOX_WIDTH", sizeof("ADJUST_BBOX_WIDTH") - 1, mapnik::Map::ADJUST_BBOX_WIDTH TSRMLS_CC);
    zend_declare_class_constant_long(map_ce, "ADJUST_BBOX_HEIGHT", sizeof("ADJUST_BBOX_HEIGHT") - 1, mapnik::Map::ADJUST_BBOX_HEIGHT TSRMLS_CC);
    zend_declare_class_constant_long(map_ce, "ADJUST_CANVAS_WIDTH", sizeof("ADJUST_CANVAS_WIDTH") - 1, mapnik::Map::ADJUST_CANVAS_WIDTH TSRMLS_CC);
    zend_declare_class_constant_long(map_ce, "ADJUST_CANVAS_HEIGHT", sizeof("ADJUST_CANVAS_HEIGHT") - 1, mapnik::Map::ADJUST_CANVAS_HEIGHT TSRMLS_CC);
    zend_declare_class_constant_long(map_ce, "RESPECT", sizeof("RESPECT") - 1, mapnik::Map::RESPECT TSRMLS_CC);
    
    memcpy(&map_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    map_object_handlers.offset = XtOffsetOf(struct map_object, std);
    map_object_handlers.free_obj = &free_map;
    map_object_handlers.clone_obj = NULL;
}
