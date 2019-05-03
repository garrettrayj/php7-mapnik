#include "php_mapnik.h"
#include "src/exception.h"
#include "src/box2d.h"
#include "src/map.h"
#include "src/image.h"
#include "src/agg_renderer.h"
#include "src/datasource_cache.h"
#include "src/projection.h"
#include "src/proj_transform.h"

PHP_MINIT_FUNCTION(mapnik)
{
    init_agg_renderer(INIT_FUNC_ARGS_PASSTHRU);
    init_box2d(INIT_FUNC_ARGS_PASSTHRU);
    init_datasource_cache(INIT_FUNC_ARGS_PASSTHRU);
    init_exception(INIT_FUNC_ARGS_PASSTHRU);
    init_image(INIT_FUNC_ARGS_PASSTHRU);
    init_map(INIT_FUNC_ARGS_PASSTHRU);
    init_proj_transform(INIT_FUNC_ARGS_PASSTHRU);
    init_projection(INIT_FUNC_ARGS_PASSTHRU);

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
extern "C" {
    ZEND_GET_MODULE(mapnik)
}
#endif
