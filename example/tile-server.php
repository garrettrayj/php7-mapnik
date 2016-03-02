<?php

// Quick procedural tile server to demonstrate PHP 7 Mapnik

namespace PHP7MapnikExample;

const RAD_TO_DEG = 180 / M_PI;

$x = isset($_GET['x']) ? $_GET['x'] : null;
$y = isset($_GET['y']) ? $_GET['y'] : null;
$zoom = isset($_GET['z']) ? $_GET['z'] : null;

if ($x === null || $y === null || $zoom === null) {
    throw new \Exception('Missing X, Y, or Z query parameter.');
}

$size = 256;
$levels = 18;

$Bc = [];
$Cc = [];
$zc = [];
$Ac = [];

$levelSize = $size;
for ($d = 0; $d <= $levels; $d++) {
    $Bc[] = $levelSize / 360;
    $Cc[] = $levelSize / (2 * M_PI);
    $zc[] = $levelSize / 2;
    $Ac[] = $levelSize;
    $levelSize *= 2;
}

/**
 * Convert screen pixel value to lat lng
 *
 * @param array $px
 * @param int $zoom
 * @return array
 */
function pxToLatLng(array $px, $zoom) {
    global $Bc, $Cc, $zc;
    $zoomDenominator = $zc[$zoom];
    $g = ($px[1] - $zoomDenominator) / (-$Cc[$zoom]);
    $lat = ($px[0] - $zoomDenominator) / $Bc[$zoom];
    $lng = RAD_TO_DEG * (2 * atan(exp($g)) - 0.5 * M_PI);
    return [$lat, $lng];
}

// Find tile boundary

$lowerLeft = [$x * $size, ($y + 1) * $size];
$upperRight = [($x + 1) * $size, $y * $size];

$lowerLeftLatLng = pxToLatLng($lowerLeft, $zoom);
$upperRightLatLng = pxToLatLng($upperRight, $zoom);

$source = new \Mapnik\Projection('+init=epsg:4326');
$destination = new \Mapnik\Projection('+init=epsg:3857');
$transform = new \Mapnik\ProjTransform($source, $destination);
$boundingBox = new \Mapnik\Box2D(
    $lowerLeftLatLng[0],
    $lowerLeftLatLng[1],
    $upperRightLatLng[0],
    $upperRightLatLng[1]
);

$tileBoundingBox = $transform->forward($boundingBox);

// Render

$pluginConfigOutput = [];
exec('mapnik-config --input-plugins', $pluginConfigOutput);
\Mapnik\DatasourceCache::registerDatasources($pluginConfigOutput[0]);

$map = new \Mapnik\Map(256, 256, '+init=epsg:3857');

$fontConfigOutput = [];
exec('mapnik-config --fonts', $fontConfigOutput);
$map->registerFonts($fontConfigOutput[0]);

$exampleXmlPath = realpath(dirname(__FILE__)) . '/example.xml';
$basePath = realpath(dirname(__FILE__) . '/../tests/data');

$map->loadXmlFile($exampleXmlPath, false, $basePath);
$map->zoomToBox($tileBoundingBox);

$image = new \Mapnik\Image(256, 256);
$renderer = new \Mapnik\AggRenderer($map, $image);
$renderer->apply();

$renderedImage = $image->saveToString('png');

// Response

header("Content-Type: image/png");
header("Content-Length: " . strlen($renderedImage));

print $renderedImage;

exit();
