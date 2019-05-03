#include <string>
#include <sstream>
#include <iostream>
#include "php.h"
#include "exception.h"
#include "datasource_cache.h"
#include <boost/foreach.hpp>
#include <mapnik/datasource_cache.hpp>

zend_class_entry *datasource_cache_ce;

// Class methods

PHP_METHOD(DatasourceCache, registerDatasources)
{
    zend_string *path;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_STR(path)
    ZEND_PARSE_PARAMETERS_END();

    std::string path_str(ZSTR_VAL(path), ZSTR_LEN(path));
    mapnik::datasource_cache::instance().register_datasources(path_str);

    RETURN_TRUE;
}

PHP_METHOD(DatasourceCache, getPluginNames)
{
    std::vector<std::string> all_plugin_names = mapnik::datasource_cache::instance().plugin_names();
    array_init(return_value);

    BOOST_FOREACH (std::string plugin, all_plugin_names) {
        add_next_index_string(return_value, plugin.c_str());
    }
}

// Reflection info

ZEND_BEGIN_ARG_INFO_EX(argInfo_datasourceCache_registerDatasources, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

// Register methods

zend_function_entry datasource_cache_methods[] = {
    PHP_ME(DatasourceCache, registerDatasources, argInfo_datasourceCache_registerDatasources, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
    PHP_ME(DatasourceCache, getPluginNames, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
    { NULL, NULL, NULL }
};

// Extension class initializer

void init_datasource_cache(INIT_FUNC_ARGS)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Mapnik", "DatasourceCache", datasource_cache_methods);
    datasource_cache_ce = zend_register_internal_class(&ce TSRMLS_CC);
}
