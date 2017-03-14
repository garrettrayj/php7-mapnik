PHP_ARG_WITH(mapnik, for Mapnik support,
[  --with-mapnik[=FILE]       Include Mapnik support. Optionally, provide a path mapnik-config.])

dnl Check whether the extension is enabled at all
if test "$PHP_MAPNIK" != "no"; then

    dnl Setup for C++
    PHP_REQUIRE_CXX()
    AC_LANG_CPLUSPLUS
    PHP_ADD_LIBRARY(stdc++, 1, MAPNIK_SHARED_LIBADD)
    CPPFLAGS="$CPPFLAGS $INCLUDES -DHAVE_MAPNIK -std=c++11 -Wno-deprecated-register"

    dnl Check for mapnik-config. First try any path that was given to us, then look in $PATH
    AC_MSG_CHECKING([for mapnik-config])
    MAPNIK_CONFIG="mapnik-config"
    if test "$PHP_MAPNIK" != "yes"; then
        MAPNIK_PATH=$PHP_MAPNIK
    else
        MAPNIK_PATH=`$php_shtool path $MAPNIK_CONFIG`
    fi

    dnl If a usable mapnik-config was found, use it

    if test -f "$MAPNIK_PATH" && test -x "$MAPNIK_PATH" && $MAPNIK_PATH --version > /dev/null 2>&1; then
        AC_MSG_RESULT([$MAPNIK_PATH])
        MAPNIK_LIB_NAME=`$MAPNIK_PATH --lib-name`
        MAPNIK_INCLUDES=`$MAPNIK_PATH --includes`
        MAPNIK_LIBS=`$MAPNIK_PATH --libs`
        MAPNIK_FLAGS=`$MAPNIK_PATH --all-flags`

        ICU4C_FLAGS='pkg-config --cflags icu-uc icu-io'

        CPP_FLAGS="$CPP_FLAGS $MAPNIK_FLAGS $ICU4C_FLAGS"

        dnl Add the necessary include dirs
        PHP_EVAL_INCLINE($MAPNIK_INCLUDES)
        PHP_EVAL_INCLINE($ICU4C_FLAGS)

        dnl Add the necessary libraries and library dirs
        PHP_EVAL_LIBLINE($MAPNIK_LIBS, MAPNIK_SHARED_LIBADD)
    else
        dnl No usable mapnik-config, bail
        AC_MSG_RESULT([mapnik-config not found])
        AC_MSG_ERROR([Please check your Mapnik installation.])
    fi

    dnl Finally, tell the build system about the extension and what files are needed
    PHP_NEW_EXTENSION(
        mapnik,
        mapnik.cc exception.cc box2d.cc map.cc image.cc agg_renderer.cc datasource_cache.cc projection.cc proj_transform.cc,
        $ext_shared,
        $CPP_FLAGS
    )
    PHP_SUBST(MAPNIK_SHARED_LIBADD)
fi