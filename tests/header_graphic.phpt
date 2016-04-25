--TEST--
Header Graphic
--SKIPIF--
<?php if (!extension_loaded("mapnik")) print "skip"; ?>
--FILE--
<?php

// Just a fun test to give the project a header graphic...

$pluginConfigOutput = [];
exec('mapnik-config --input-plugins', $pluginConfigOutput);
\Mapnik\DatasourceCache::registerDatasources($pluginConfigOutput[0]);

$map = new \Mapnik\Map(1280, 320);

$fontConfigOutput = [];
exec('mapnik-config --fonts', $fontConfigOutput);
$map->registerFonts($fontConfigOutput[0]);

$basePath = realpath(dirname(__FILE__) . '/data');
$map->loadXmlFile($basePath . '/header_graphic.xml', false, $basePath);

$box = new \Mapnik\Box2D(-134, -25, 174, 67);
$map->zoomToBox($box);

$image = new \Mapnik\Image(1280, 320);
$renderer = new \Mapnik\AggRenderer($map, $image);
$renderer->apply();

$imageFile = realpath(dirname(__FILE__) . '/../') . '/header_graphic.png';
$image->saveToFile($imageFile);
print file_exists($imageFile);

?>
--EXPECT--
1
