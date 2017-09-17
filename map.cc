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

// Class Methods

PHP_METHOD(Map, __construct)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    mapnik::Map* map = NULL;

    long width, height;
    zend_string *srs;

    try {
        if (ZEND_NUM_ARGS() == 0) {
            map = new mapnik::Map();
        } else if (::zend_parse_parameters_ex(
            ZEND_PARSE_PARAMS_QUIET,
            ZEND_NUM_ARGS() TSRMLS_CC,
            "ll",
            &width,
            &height) == SUCCESS
        ) {
            map = new mapnik::Map(width, height);
        } else if (::zend_parse_parameters_ex(
            ZEND_PARSE_PARAMS_QUIET,
            ZEND_NUM_ARGS() TSRMLS_CC,
            "llS",
            &width,
            &height,
            &srs) == SUCCESS
        ) {
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

PHP_METHOD(Map, loadXmlString)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());

    zend_string *xml, *base_path;
    bool strict = false;

    if (::zend_parse_parameters_ex(
        ZEND_PARSE_PARAMS_QUIET,
        ZEND_NUM_ARGS() TSRMLS_CC,
        "SbS",
        &xml,
        &strict,
        &base_path) == FAILURE
    ) {
        php_mapnik_throw_exception("Wrong arguments passed to \\Mapnik\\Map::loadXmlString");
        return;
    }

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

PHP_METHOD(Map, loadXmlFile)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());

    zend_string *filename, *base_path;
    bool strict = false;

    if (::zend_parse_parameters_ex(
        ZEND_PARSE_PARAMS_QUIET,
        ZEND_NUM_ARGS() TSRMLS_CC,
        "SbS",
        &filename,
        &strict,
        &base_path) == FAILURE
    ) {
        php_mapnik_throw_exception("Wrong arguments passed to \\Mapnik\\Map::loadXmlFile");
    }

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

PHP_METHOD(Map, zoom)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    double zoom;

    if (::zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &zoom) == FAILURE) {
        php_mapnik_throw_exception("Wrong arguments passed to \\Mapnik\\Map::zoom");
        return;
    }

    obj->map->zoom(zoom);
}

PHP_METHOD(Map, zoomAll)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());

    obj->map->zoom_all();
}

PHP_METHOD(Map, pan)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    long x, y;

    if (::zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &x, &y) == FAILURE) {
        php_mapnik_throw_exception("Wrong arguments passed to \\Mapnik\\Map::pan");
        return;
    }

    obj->map->pan(x, y);
}

PHP_METHOD(Map, panAndZoom)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    long x, y;
    double zoom;

    if (::zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lld", &x, &y, &zoom) == FAILURE) {
        php_mapnik_throw_exception("Wrong arguments passed to \\Mapnik\\Map::panAndZoom");
        return;
    }

    obj->map->pan_and_zoom(x, y, zoom);
}

PHP_METHOD(Map, zoomToBox)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    php_mapnik_box2d_object *box2d_obj;
    mapnik::box2d<double> *box2d;

    zval* box2d_zval;

    if (::zend_parse_parameters_ex(
        ZEND_PARSE_PARAMS_QUIET,
        ZEND_NUM_ARGS() TSRMLS_CC,
        "O",
        &box2d_zval,
        php_mapnik_box2d_ce) == FAILURE
    ) {
        php_mapnik_throw_exception("Wrong arguments passed to \\Mapnik\\Map::zoomToBox");
        return;
    }

    box2d_obj = Z_PHP_MAPNIK_BOX2D_P(box2d_zval);
    box2d = box2d_obj->box2d;

    obj->map->zoom_to_box(*box2d_obj->box2d);
}

PHP_METHOD(Map, registerFonts)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    zend_string *path;
    bool fonts_registered = false;

    if (::zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "S", &path) == FAILURE) {
        php_mapnik_throw_exception("Wrong arguments passed to \\Mapnik\\Map::registerFonts");
    }

    try {
        std::string path_str(path->val, path->len);
        fonts_registered = obj->map->register_fonts(path_str);
    } catch (const std::exception & ex) {
        php_mapnik_throw_exception(ex.what());
        return;
    } catch (...) {
        php_mapnik_throw_exception("Unknown exception thrown while registering font path.");
        return;
    }

    if (fonts_registered) {
        RETURN_TRUE;
    }

    RETURN_FALSE;
}

PHP_METHOD(Map, getBasePath)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());

    RETURN_STRING(obj->map->base_path().c_str());
}

PHP_METHOD(Map, setBasePath)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    zend_string *path;

    if (::zend_parse_parameters_ex(
        ZEND_PARSE_PARAMS_QUIET,
        ZEND_NUM_ARGS() TSRMLS_CC,
        "S",
        &path) == FAILURE
    ) {
        php_mapnik_throw_exception("Wrong arguments passed to \\Mapnik\\Map::setBaseBath");
        return;
    }

    obj->map->set_base_path(path->val);
}

PHP_METHOD(Map, getWidth)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());

    RETURN_LONG(obj->map->width());
}

PHP_METHOD(Map, setWidth)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    long width;

    if (::zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "l", &width) == FAILURE) {
        php_mapnik_throw_exception("Wrong argument passed to \\Mapnik\\Map::setWidth");
        return;
    }

    obj->map->set_width(width);
}

PHP_METHOD(Map, getHeight)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());

    RETURN_LONG(obj->map->height());
}

PHP_METHOD(Map, setHeight)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    long height;

    if (::zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "l", &height) == FAILURE) {
        php_mapnik_throw_exception("Wrong argument passed to \\Mapnik\\Map::setHeight");
        return;
    }

    obj->map->set_height(height);
}


PHP_METHOD(Map, resize)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    long width, height;

    if (::zend_parse_parameters_ex(
        ZEND_PARSE_PARAMS_QUIET,
        ZEND_NUM_ARGS() TSRMLS_CC,
        "ll",
        &width,
        &height) == FAILURE
    ) {
        php_mapnik_throw_exception("Wrong argument passed to \\Mapnik\\Map::resize");
        return;
    }

    obj->map->resize(width, height);
}

PHP_METHOD(Map, getSrs)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());

    RETURN_STRING(obj->map->srs().c_str());
}

PHP_METHOD(Map, setSrs)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    zend_string *srs;

    if (::zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "S", &srs) == FAILURE) {
        php_mapnik_throw_exception("Wrong arguments passed to \\Mapnik\\Map::setSrs");
        return;
    }

    obj->map->set_srs(srs->val);
}

PHP_METHOD(Map, getBackgroundImage)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    boost::optional<std::string> background_image = obj->map->background_image();

    if (background_image) {
        RETURN_STRING(background_image.get().c_str());
    }
}

PHP_METHOD(Map, setBackgroundImage)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    zend_string *image_filename;

    if (::zend_parse_parameters_ex(
        ZEND_PARSE_PARAMS_QUIET,
        ZEND_NUM_ARGS() TSRMLS_CC,
        "S",
        &image_filename) == FAILURE
    ) {
        php_mapnik_throw_exception("Wrong arguments passed to \\Mapnik\\Map::setBackgroundImage");
        return;
    }

    obj->map->set_background_image(image_filename->val);
}


PHP_METHOD(Map, getBackgroundImageOpacity)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());

    RETURN_DOUBLE(obj->map->background_image_opacity());
}

PHP_METHOD(Map, setBackgroundImageOpacity)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    double opacity;

    if (::zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &opacity) == FAILURE) {
        php_mapnik_throw_exception("Wrong arguments passed to \\Mapnik\\Map::setBackgroundImageOpacity");
        return;
    }

    obj->map->set_background_image_opacity(opacity);
}

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

PHP_METHOD(Map, setMaximumExtent)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    zval* box2d_zval;

    if (::zend_parse_parameters_ex(
        ZEND_PARSE_PARAMS_QUIET,
        ZEND_NUM_ARGS() TSRMLS_CC,
        "O",
        &box2d_zval,
        php_mapnik_box2d_ce) == FAILURE
    ) {
        php_mapnik_throw_exception("Wrong arguments passed to \\Mapnik\\Map::setMaximumExtent");
        return;
    }

    php_mapnik_box2d_object *box2d_obj = Z_PHP_MAPNIK_BOX2D_P(box2d_zval);
    obj->map->set_maximum_extent(*box2d_obj->box2d);
}

PHP_METHOD(Map, resetMaximumExtent)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    obj->map->reset_maximum_extent();
}

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

PHP_METHOD(Map, getBufferSize)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    RETURN_LONG(obj->map->buffer_size());
}

PHP_METHOD(Map, setBufferSize)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    long buffer_size;

    if (::zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "l", &buffer_size) == FAILURE) {
        php_mapnik_throw_exception("Wrong arguments passed to \\Mapnik\\Map::setBufferSize");
        return;
    }

    obj->map->set_buffer_size(buffer_size);
}

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

PHP_METHOD(Map, getScale)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    RETURN_DOUBLE(obj->map->scale());
}

PHP_METHOD(Map, getScaleDenominator)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    RETURN_DOUBLE(obj->map->scale_denominator());
}

PHP_METHOD(Map, countLayers)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    RETURN_LONG(obj->map->layer_count());
}

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

PHP_METHOD(Map, removeAll)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    obj->map->remove_all();
}

PHP_METHOD(Map, getAspectFixMode)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    RETURN_LONG(obj->map->get_aspect_fix_mode());
}

PHP_METHOD(Map, setAspectFixMode)
{
    php_mapnik_map_object *obj = Z_PHP_MAPNIK_MAP_P(getThis());
    long aspect_fix_mode;

    if (::zend_parse_parameters_ex(
        ZEND_PARSE_PARAMS_QUIET,
        ZEND_NUM_ARGS() TSRMLS_CC,
        "l",
        &aspect_fix_mode) == FAILURE
    ) {
        php_mapnik_throw_exception("Wrong arguments passed to \\Mapnik\\Map::setAspectFixMode");
        return;
    }

    if (!(aspect_fix_mode >= mapnik::Map::GROW_BBOX && aspect_fix_mode <= mapnik::Map::RESPECT)) {
        php_mapnik_throw_exception("Invalid aspect fix mode.");
        return;
    }

    obj->map->set_aspect_fix_mode(mapnik::Map::aspect_fix_mode(aspect_fix_mode));
}

// Register methods

zend_function_entry php_mapnik_map_methods[] = {
    PHP_ME(Map, __construct, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, loadXmlFile, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, loadXmlString, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, zoom, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, zoomAll, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, pan, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, panAndZoom, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, zoomToBox, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, registerFonts, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, getBasePath, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, setBasePath, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, getWidth, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, setWidth, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, getHeight, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, setHeight, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, resize, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, getSrs, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, setSrs, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, getBackgroundImage, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, setBackgroundImage, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, getBackgroundImageOpacity, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, setBackgroundImageOpacity, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, getMaximumExtent, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, setMaximumExtent, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, resetMaximumExtent, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, getCurrentExtent, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, getBufferSize, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, setBufferSize, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, getBufferedExtent, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, getScale, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, getScaleDenominator, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, countLayers, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, removeStyle, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, removeAll, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, getAspectFixMode, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Map, setAspectFixMode, NULL, ZEND_ACC_PUBLIC)
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