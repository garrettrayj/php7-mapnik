PHP_ARG_WITH(mapnik, for Mapnik support,
[  --with-mapnik[=FILE]       Include Mapnik support. Optionally, provide a path mapnik-config.])

dnl Check whether the extension is enabled at all
if test "$PHP_MAPNIK" != "no"; then

    dnl Setup for C++
    AC_LANG_CPLUSPLUS
    PHP_REQUIRE_CXX()
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
        MAPNIK_INCLUDES=`$MAPNIK_PATH --includes --dep-includes`
        MAPNIK_LIBS=`$MAPNIK_PATH --libs --dep-libs`
        MAPNIK_FLAGS=`$MAPNIK_PATH --defines --cxxflags`

        CPPFLAGS="$CPPFLAGS $MAPNIK_FLAGS"

        dnl Add the necessary include dirs
        PHP_EVAL_INCLINE($MAPNIK_INCLUDES)

        dnl Add the necessary libraries and library dirs
        PHP_EVAL_LIBLINE($MAPNIK_LIBS, MAPNIK_SHARED_LIBADD)
    else
        dnl No usable mapnik-config, bail
        AC_MSG_RESULT([mapnik-config not found])
        AC_MSG_ERROR([Please check your Mapnik installation.])
    fi

    dnl Finally, tell the build system about the extension and what files are needed

    PHP_SUBST(MAPNIK_SHARED_LIBADD)
    PHP_NEW_EXTENSION(
        mapnik,
        php_mapnik.cc \
        src/exception.cc \
        src/box2d.cc \
        src/map.cc \
        src/image.cc \
        src/agg_renderer.cc \
        src/datasource_cache.cc \
        src/projection.cc \
        src/proj_transform.cc,
        $ext_shared,
        $CPPFLAGS
    )

fi
