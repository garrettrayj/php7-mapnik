#!/usr/bin/env bash

export PHP=`which php`
export TEST_PHP_EXECUTABLE=$PHP

$PHP run-tests.php -q -m --show-diff
find . -name *.mem | xargs cat
