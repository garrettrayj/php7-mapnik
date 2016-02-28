#include "php.h"

#include "php_mapnik.h"
#include "exception.h"
#include "box2d.h"
#include "map.h"
#include "image.h"
#include "agg_renderer.h"
#include "datasource_cache.h"

PHP_MINIT_FUNCTION(mapnik)
{
    php_mapnik_exception_startup(INIT_FUNC_ARGS_PASSTHRU);
    php_mapnik_box2d_startup(INIT_FUNC_ARGS_PASSTHRU);
    php_mapnik_map_startup(INIT_FUNC_ARGS_PASSTHRU);
    php_mapnik_image_startup(INIT_FUNC_ARGS_PASSTHRU);
    php_mapnik_agg_renderer_startup(INIT_FUNC_ARGS_PASSTHRU);
    php_mapnik_datasource_cache_startup(INIT_FUNC_ARGS_PASSTHRU);

    return SUCCESS;
}

// http://php.net/manual/en/internals2.structure.modstruct.php
zend_module_entry mapnik_module_entry = {
    STANDARD_MODULE_HEADER,
    PHP_MAPNIK_EXTENSION_NAME, // name
    NULL,                      // functions
    PHP_MINIT(mapnik),         // module_startup_func
    NULL,                      // module_shutdown_func
    NULL,                      // request_startup_func
    NULL,                      // request_shutdown_func
    NULL,                      // info_func
    PHP_MAPNIK_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_MAPNIK
ZEND_GET_MODULE(mapnik)
#endif