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
    php_mapnik_map_object *obj = NULL;
    mapnik::Map* map;

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
            RETURN_FALSE;
        }
    } catch (const std::exception & ex) {
        php_mapnik_throw_exception(ex.what());
        RETURN_FALSE;
    } catch ( ... ) {
        php_mapnik_throw_exception("Unknown exception during Mapnik map instantiation.");
        RETURN_FALSE;
    }

    obj = Z_PHP_MAPNIK_MAP_P(getThis());
    obj->map = map;
}

PHP_METHOD(Map, loadXmlString)
{
    php_mapnik_map_object *obj;
    mapnik::Map *map;

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
        RETURN_FALSE;
    }

    try {
        std::string xml_str(xml->val, xml->len);
        std::string base_path_str(base_path->val, base_path->len);
        obj = Z_PHP_MAPNIK_MAP_P(getThis());
        map = obj->map;
        mapnik::load_map_string(*map, xml_str, strict, base_path_str);
    } catch (const mapnik::proj_init_error & ex) {
        php_mapnik_throw_exception(ex.what());
        RETURN_FALSE;
    } catch (const std::runtime_error & ex) {
        php_mapnik_throw_exception(ex.what());
        RETURN_FALSE;
    } catch (const std::exception & ex) {
        php_mapnik_throw_exception(ex.what());
        RETURN_FALSE;
    } catch (...) {
        php_mapnik_throw_exception("Unknown exception thrown while loading XML string.");
        RETURN_FALSE;
    }
}

PHP_METHOD(Map, loadXmlFile)
{
    php_mapnik_map_object *obj;
    mapnik::Map *map;

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
        RETURN_FALSE;
    }

    try {
        std::string filename_str(filename->val, filename->len);
        std::string base_path_str(base_path->val, base_path->len);
        obj = Z_PHP_MAPNIK_MAP_P(getThis());
        map = obj->map;
        mapnik::load_map(*map, filename_str, strict, base_path_str);
    } catch (const mapnik::proj_init_error & ex) {
        php_mapnik_throw_exception(ex.what());
        RETURN_FALSE;
    } catch (const std::runtime_error & ex) {
        php_mapnik_throw_exception(ex.what());
        RETURN_FALSE;
    } catch (const std::exception & ex) {
        php_mapnik_throw_exception(ex.what());
        RETURN_FALSE;
    } catch (...) {
        php_mapnik_throw_exception("Unknown exception thrown while loading XML file.");
        RETURN_FALSE;
    }

    RETURN_TRUE;
}

PHP_METHOD(Map, zoom)
{
    php_mapnik_map_object *obj;

    double zoom;

    if (::zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &zoom) == FAILURE) {
        php_mapnik_throw_exception("Wrong arguments passed to \\Mapnik\\Map::zoom");
        RETURN_FALSE;
    }

    obj = Z_PHP_MAPNIK_MAP_P(getThis());
    obj->map->zoom(zoom);

    RETURN_TRUE;
}

PHP_METHOD(Map, zoomAll)
{
    php_mapnik_map_object *obj;

    obj = Z_PHP_MAPNIK_MAP_P(getThis());
    obj->map->zoom_all();

    RETURN_TRUE;
}

PHP_METHOD(Map, pan)
{
    php_mapnik_map_object *obj;

    long x, y;

    if (::zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &x, &y) == FAILURE) {
        php_mapnik_throw_exception("Wrong arguments passed to \\Mapnik\\Map::pan");
        RETURN_FALSE;
    }

    obj = Z_PHP_MAPNIK_MAP_P(getThis());
    obj->map->pan(x, y);

    RETURN_TRUE;
}

PHP_METHOD(Map, panAndZoom)
{
    php_mapnik_map_object *obj;

    long x, y;
    double zoom;

    if (::zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lld", &x, &y, &zoom) == FAILURE) {
        php_mapnik_throw_exception("Wrong arguments passed to \\Mapnik\\Map::panAndZoom");
        RETURN_FALSE;
    }

    obj = Z_PHP_MAPNIK_MAP_P(getThis());
    obj->map->pan_and_zoom(x, y, zoom);

    RETURN_TRUE;
}

PHP_METHOD(Map, zoomToBox)
{
    php_mapnik_map_object *map_obj;
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
        RETURN_FALSE;
    }

    box2d_obj = Z_PHP_MAPNIK_BOX2D_P(box2d_zval);
    box2d = box2d_obj->box2d;

    map_obj = Z_PHP_MAPNIK_MAP_P(getThis());
    map_obj->map->zoom_to_box(*box2d_obj->box2d);

    RETURN_TRUE;
}

PHP_METHOD(Map, registerFonts)
{
    php_mapnik_map_object *obj;
    mapnik::Map *map;
    zend_string *path;
    bool fonts_registered = false;

    if (::zend_parse_parameters_ex(
        ZEND_PARSE_PARAMS_QUIET,
        ZEND_NUM_ARGS() TSRMLS_CC,
        "S",
        &path) == FAILURE
    ) {
        php_mapnik_throw_exception("Wrong arguments passed to \\Mapnik\\Map::registerFonts");
        RETURN_FALSE;
    }

    try {
        std::string path_str(path->val, path->len);
        obj = Z_PHP_MAPNIK_MAP_P(getThis());
        fonts_registered = obj->map->register_fonts(path_str);
    } catch (const std::exception & ex) {
        php_mapnik_throw_exception(ex.what());
        RETURN_FALSE;
    } catch (...) {
        php_mapnik_throw_exception("Unknown exception thrown while registering font path.");
        RETURN_FALSE;
    }

    if (fonts_registered) {
        RETURN_TRUE;
    }

    RETURN_FALSE;
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
    {NULL, NULL, NULL}
};

// Extension class startup

void php_mapnik_map_startup(INIT_FUNC_ARGS)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Mapnik", "Map", php_mapnik_map_methods);
    php_mapnik_map_ce = zend_register_internal_class(&ce TSRMLS_CC);
    php_mapnik_map_ce->create_object = php_mapnik_map_new;
    
    memcpy(&php_mapnik_map_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    php_mapnik_map_object_handlers.offset = XtOffsetOf(struct php_mapnik_map_object, std);
    php_mapnik_map_object_handlers.free_obj = &php_mapnik_map_free_storage;
    php_mapnik_map_object_handlers.clone_obj = NULL;
}