<?php

namespace Mapnik;

/**
 * @package Mapnik
 */
class Image
{
    /**
     * Image constructor.
     *
     * @param int $width Image width
     * @param int $height Image height
     */
    public function __construct(int $width, int $height) {}

    /**
     * Save image to a file.
     *
     * @param string $file File path
     * @param string $format Format
     * @see https://github.com/mapnik/mapnik/wiki/Image-IO Mapnik image formats documentation
     */
    public function saveToFile(string $file, string $format = 'png') {}

    /**
     * Save image to a string.
     *
     * @param string $format
     * @return string
     * @see https://github.com/mapnik/mapnik/wiki/Image-IO Mapnik image formats documentation
     */
    public function saveToString(string $format = 'png'): string {}
}