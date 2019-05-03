#include <string>
#include <sstream>
#include <iostream>
#include "php.h"
#include "exception.h"
#include "datasource_cache.h"
#include <boost/foreach.hpp>
#include <mapnik/datasource_cache.hpp>

zend_class_entry *php_mapnik_datasource_cache_ce;

// Class Method: Mapnik\DatasourceCache::registerDatasources

ZEND_BEGIN_ARG_INFO_EX(argInfo_datasourceCache_registerDatasources, 0, 0, 1)
    ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

PHP_METHOD(DatasourceCache, registerDatasources)
{
    zend_string *path;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_STR(path)
    ZEND_PARSE_PARAMETERS_END();

    std::string path_str(path->val, path->len);
    mapnik::datasource_cache::instance().register_datasources(path_str);

    RETURN_TRUE;
}

// Class Method: Mapnik\DatasourceCache::getPluginNames

PHP_METHOD(DatasourceCache, getPluginNames)
{
    std::vector<std::string> all_plugin_names = mapnik::datasource_cache::instance().plugin_names();
    array_init(return_value);

    BOOST_FOREACH (std::string plugin, all_plugin_names) {
        add_next_index_string(return_value, plugin.c_str());
    }
}

// Register methods

zend_function_entry php_mapnik_datasource_cache_methods[] = {
    PHP_ME(DatasourceCache, registerDatasources, argInfo_datasourceCache_registerDatasources, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(DatasourceCache, getPluginNames, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    { NULL, NULL, NULL }
};

// Extension class startup

void php_mapnik_datasource_cache_startup(INIT_FUNC_ARGS)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Mapnik", "DatasourceCache", php_mapnik_datasource_cache_methods);
    php_mapnik_datasource_cache_ce = zend_register_internal_class(&ce TSRMLS_CC);
}
