<?php

namespace Mapnik;

/**
 * Re-project Box2D objects.
 *
 * @package Mapnik
 * @see https://github.com/mapnik/mapnik/blob/master/include/mapnik/proj_transform.hpp C++ proj_transform include
 */
class ProjTransform
{
    /**
     * ProjTransform constructor.
     *
     * @param Projection $source Source projection
     * @param Projection $destination Destination projection
     */
    public function __construct(Projection $source, Projection $destination) {}

    /**
     * Transform box from source to destination projection.
     *
     * @param Box2D $box
     * @return Box2D
     */
    public function forward(Box2D $box): Box2D {}

    /**
     * Transform box from destination to source projection.
     *
     * @param Box2D $box
     * @return Box2D
     */
    public function backward(Box2D $box): Box2D {}
}