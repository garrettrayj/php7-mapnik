<?php

namespace Mapnik;

/**
 * @package Mapnik
 * @see https://github.com/mapnik/mapnik/blob/master/include/mapnik/datasource_cache.hpp C++ datasource_cache include
 */
class DatasourceCache
{
    /**
     * Register datasource input plugins.
     *
     * @param string $path Path to input plugins directory
     */
    public static function registerDatasources(string $path) {}

    /**
     * Get array of registered input plugins.
     *
     * @return array
     */
    public static function getPluginNames(): array {}
}