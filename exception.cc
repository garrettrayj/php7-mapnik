#include "php.h"
#include "exception.h"
#include <zend_exceptions.h>

zend_class_entry *php_mapnik_exception_ce;

void php_mapnik_exception_startup(INIT_FUNC_ARGS)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Mapnik", "Exception", NULL);
    php_mapnik_exception_ce = zend_register_internal_class_ex(
        &ce,
        (zend_class_entry*) zend_exception_get_default(TSRMLS_C) TSRMLS_CC
    );
}

void php_mapnik_throw_exception_with_message(const char *message, int code TSRMLS_DC)
{
    zend_throw_exception(php_mapnik_exception_ce, (char*)message, code TSRMLS_CC);
}