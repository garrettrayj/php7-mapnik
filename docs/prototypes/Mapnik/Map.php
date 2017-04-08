<?php

namespace Mapnik;

/**
 * @package Mapnik
 * @see https://github.com/mapnik/mapnik/blob/master/include/mapnik/map.hpp C++ map include
 * @see https://github.com/mapnik/mapnik/wiki/mapnik.Map C++ map function reference
 */
class Map
{
    /**
     * Grow the width or height of the specified geo bbox to fill the map size. Default behaviour.
     *
     * @var int
     */
    const GROW_BBOX = 0;

    /**
     * Grow the width or height of the map to accommodate the specified geo bbox.
     *
     * @var int
     */
    const GROW_CANVAS = 1;

    /**
     * Shrink the width or height of the specified geo bounding box to fill the map size.
     *
     * @var int
     */
    const SHRINK_BBOX = 2;

    /**
     * Shrink the width or height of the map to accommodate the specified geo bounding box.
     *
     * @var int
     */
    const SHRINK_CANVAS = 3;

    /**
     * Adjust the width of the specified geo bounding box, leave height and map size unchanged.
     *
     * @var int
     */
    const ADJUST_BBOX_WIDTH = 4;

    /**
     * Adjust the height of the specified geo bounding box, leave width and map size unchanged.
     *
     * @var int
     */
    const ADJUST_BBOX_HEIGHT = 5;

    /**
     * Adjust the width of the map, leave height and geo bounding box unchanged.
     *
     * @var int
     */
    const ADJUST_CANVAS_WIDTH = 6;

    /**
     * Adjust the height of the map, leave width and geo bounding box unchanged.
     *
     * @var int
     */
    const ADJUST_CANVAS_HEIGHT = 7;

    /**
     * Do nothing, leave map and geo bounding box unchanged.
     *
     * @var int
     */
    const RESPECT = 8;

    /**
     * Map constructor.
     *
     * @param int $width Map canvas width
     * @param int $height Map canvas height
     * @param string $srs Spatial reference system
     */
    public function __construct(
        int $width = 400,
        int $height = 400,
        string $srs = '+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs'
    ) {}

    /**
     * Load map XML from string.
     *
     * @param string $xml Mapnik XML
     * @see https://github.com/mapnik/mapnik/wiki/XMLConfigReference Mapnik XML config reference
     */
    public function loadXmlString(string $xml) {}

    /**
     * Load XML map file.
     *
     * @param string $file Path to Mapnik XML file
     * @param bool $strict Enable or disable strict map parsing
     * @param string $basePath Path to directory with map assets
     * @see https://github.com/mapnik/mapnik/wiki/XMLConfigReference Mapnik XML config reference
     */
    public function loadXmlFile(string $file, bool $strict, string $basePath) {}

    /**
     * Zoom in or out by a given factor. Positive number zooms in, negative number zooms out.
     *
     * @param float $zoom Zoom factor
     */
    public function zoom(float $zoom) {}

    /**
     * Set the geographical extent of the map to the combined extents of all active layers.
     */
    public function zoomAll() {}

    /**
     * Set the Map center to x,y location in the coordinates of the pixmap or map surface.
     *
     * @param int $x Horizontal coordinate
     * @param int $y Vertical coordinate
     */
    public function pan(int $x, int $y) {}

    /**
     * Set Map center to x,y location and zoom in or out.
     *
     * @param int $x Horizontal coordinate
     * @param int $y Vertical coordinate
     * @param float $zoom Zoom factor
     */
    public function panAndZoom(int $x, int $y, float $zoom) {}

    /**
     * Set the geographical extent of the map to Mapnik Box2D envelope.
     *
     * @param Box2D $box Bounding box
     * @see \Mapnik\Box2D Bounding box object
     */
    public function zoomToBox(Box2D $box) {}

    /**
     * Set path to search for fonts used in map.
     *
     * @param string $path Fonts path
     */
    public function registerFonts(string $path) {}

    /**
     * Get base path used for loading map assets.
     *
     * @return string Base path
     */
    public function getBasePath(): string {}

    /**
     * Set base path used for loading map assets.
     *
     * @param string $path Base path
     */
    public function setBasePath(string $path) {}

    /**
     * Get canvas width.
     *
     * @return int Map width
     */
    public function getWidth(): int {}

    /**
     * Set canvas width.
     *
     * @param int $width Map width
     */
    public function setWidth(int $width) {}

    /**
     * Get canvas height.
     *
     * @return int Map height
     */
    public function getHeight(): int {}

    /**
     * Set canvas height.
     *
     * @param int $height Map height
     */
    public function setHeight(int $height) {}

    /**
     * Resize the canvas to given height and width.
     *
     * @param int $width Map width
     * @param int $height Map height
     */
    public function resize(int $width, int $height) {}

    /**
     * Get spatial reference system.
     *
     * @return string proj.4 projection
     * @see http://proj4.org proj.4 documentation
     */
    public function getSrs(): string {}

    /**
     * Set spatial reference system.
     *
     * @param string $srs proj.4 projection
     * @see http://proj4.org proj.4 documentation
     * @see http://spatialreference.org Spatial reference system repository
     */
    public function setSrs(string $srs) {}

    /**
     * Get path to background image.
     *
     * @return string Image path
     */
    public function getBackgroundImage(): string {}

    /**
     * Set path to background image.
     *
     * @param string $file Image path
     */
    public function setBackgroundImage(string $file) {}

    /**
     * Get background image opacity. 0 is transparent, 1 is opaque.
     *
     * @return float Opacity
     */
    public function getBackgroundImageOpacity(): float {}

    /**
     * Set background image opacity. 0 is transparent, 1 is opaque.
     *
     * @param float $opacity
     */
    public function setBackgroundImageOpacity(float $opacity) {}

    /**
     * Get maximum extent of projected map.
     *
     * @return Box2D Maximum extent
     * @see https://github.com/mapnik/mapnik/wiki/BoundsClipping Mapnik bounds clipping documentation
     */
    public function getMaximumExtent(): Box2D {}

    /**
     * Set maximum extent of projected map.
     *
     * @param Box2D $box Maximum extent
     * @see https://github.com/mapnik/mapnik/wiki/BoundsClipping Mapnik bounds clipping documentation
     */
    public function setMaximumExtent(Box2D $box) {}

    /**
     * Reset maximum extent.
     */
    public function resetMaximumExtent() {}

    /**
     * Get current extent of projected map.
     *
     * @return Box2D Current extent
     */
    public function getCurrentExtent(): Box2D {}

    /**
     * Get map bounds buffer size.
     *
     * @return int Buffer pixels
     */
    public function getBufferSize(): int {}

    /**
     * Set map bounds buffer. Used by placement detector to help avoid cutting labels.
     *
     * @param int $bufferSize Buffer pixels
     */
    public function setBufferSize(int $bufferSize) {}

    /**
     * Get extent of map with buffer applied.
     *
     * @return Box2D Buffered extent
     */
    public function getBufferedExtent(): Box2D {}

    /**
     * Get map scale.
     *
     * @return float
     * @see https://github.com/mapnik/mapnik/wiki/ScaleAndPpi Mapnik scale and scale denominators documentation
     */
    public function getScale(): float {}

    /**
     * Get scale denominator.
     *
     * @return float
     * @see https://github.com/mapnik/mapnik/wiki/ScaleAndPpi Mapnik scale and scale denominators documentation
     */
    public function getScaleDenominator(): float {}

    /**
     * Count number of map layers.
     *
     * @return int
     */
    public function countLayers(): int {}

    /**
     * Remove a style.
     *
     * @param string $styleName Style name
     */
    public function removeStyle(string $styleName) {}

    /**
     * Remove all styles and layers.
     */
    public function removeAll() {}

    /**
     * Get aspect fix mode.
     *
     * @return int
     * @see https://github.com/mapnik/mapnik/wiki/Aspect-Fix-Mode Mapnik aspect fix mode documentation
     */
    public function getAspectFixMode(): int {}

    /**
     * Set aspect fix mode. See Map constants for valid values.
     *
     * @param int $aspectFixMode
     * @see https://github.com/mapnik/mapnik/wiki/Aspect-Fix-Mode Mapnik aspect fix mode documentation
     * @see \Mapnik\Map::GROW_BBOX
     * @see \Mapnik\Map::GROW_CANVAS
     * @see \Mapnik\Map::SHRINK_BBOX
     * @see \Mapnik\Map::SHRINK_CANVAS
     * @see \Mapnik\Map::ADJUST_BBOX_WIDTH
     * @see \Mapnik\Map::ADJUST_BBOX_HEIGHT
     * @see \Mapnik\Map::ADJUST_CANVAS_WIDTH
     * @see \Mapnik\Map::ADJUST_CANVAS_HEIGHT
     * @see \Mapnik\Map::RESPECT
     */
    public function setAspectFixMode(int $aspectFixMode) {}
}