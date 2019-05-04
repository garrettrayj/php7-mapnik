#!/usr/bin/env bash

# Hijack PHP_TEST_SHARED_EXTENSIONS variable to inject run-tests.php valgrind options.
make PHP_TEST_SHARED_EXTENSIONS='-m --show-diff ` if test "x$(PHP_MODULES)" != "x"; then for i in $(PHP_MODULES)""; do . $$i; $(top_srcdir)/build/shtool echo -n -- " -d extension=$$dlname"; done; fi; if test "x$(PHP_ZEND_EX)" != "x"; then for i in $(PHP_ZEND_EX)""; do . $$i; $(top_srcdir)/build/shtool echo -n -- " -d $(ZEND_EXT_TYPE)=$(top_builddir)/modules/$$dlname"; done; fi`' test
