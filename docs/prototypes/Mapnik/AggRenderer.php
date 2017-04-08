<?php

namespace Mapnik;

/**
 * @package Mapnik
 * @see https://github.com/mapnik/mapnik/blob/master/include/mapnik/agg_renderer.hpp C++ agg_renderer include
 * @see https://github.com/mapnik/mapnik/wiki/MapnikRenderers Mapnik renderers documentation
 */
class AggRenderer
{
    /**
     * AggRenderer constructor.
     *
     * @param Map $map Map object
     * @param Image $image Image object
     */
    public function __construct(Map $map, Image $image) {}

    /**
     * Render/paint image from Map configuration.
     */
    public function apply() {}
}