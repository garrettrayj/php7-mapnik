<?php

namespace Mapnik;

/**
 * Class Box2D
 *
 * @package Mapnik
 * @see https://github.com/mapnik/mapnik/blob/master/include/mapnik/geometry/box2d.hpp C++ box2d include
 */
class Box2D
{
    /**
     * Box2D constructor.
     *
     * @param float $minX East
     * @param float $minY North
     * @param float $maxX West
     * @param float $maxY South
     */
    public function __construct(float $minX, float $minY, float $maxX, float $maxY) {}

    /**
     * Get east boundary of envelope.
     *
     * @return float East
     */
    public function minX(): float {}

    /**
     * Get north boundary of envelope.
     *
     * @return float North
     */
    public function minY(): float {}

    /**
     * Get west boundary of envelope.
     *
     * @return float West
     */
    public function maxX(): float {}

    /**
     * Get south boundary of envelope.
     *
     * @return float South
     */
    public function maxY(): float {}
}