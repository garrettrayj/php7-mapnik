<?php

namespace Mapnik;

/**
 * Class Projection
 *
 * Represents a spatial reference system. Used in conjunction with ProjTransform to project bounding boxes.
 *
 * @package Mapnik
 * @see https://github.com/mapnik/mapnik/blob/master/include/mapnik/projection.hpp C++ projection include
 */
class Projection
{
    /**
     * Projection constructor.
     *
     * @param string $params proj.4 projection string
     * @see http://proj4.org proj.4 documentation
     * @see http://spatialreference.org Spatial reference system repository
     */
    public function __construct(string $params) {}
}