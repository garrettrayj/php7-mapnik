--TEST--
\Mapnik\AggRenderer
--SKIPIF--
<?php if (!extension_loaded("mapnik")) print "skip"; ?>
--FILE--
<?php

// Setup
$configOutput = [];
exec('mapnik-config --input-plugins', $configOutput);
\Mapnik\DatasourceCache::registerDatasources($configOutput[0]);
$map = new \Mapnik\Map(640, 480);
$basePath = realpath(dirname(__FILE__) . '/data');
$map->loadXmlFile($basePath . '/world.xml', false, $basePath);
$map->zoomAll();
$image = new \Mapnik\Image(640, 480);

// Assert AggRenderer instantiation
$renderer = new \Mapnik\AggRenderer($map, $image);
print ($renderer instanceof \Mapnik\AggRenderer);

// Assert apply() successful
print $renderer->apply();

?>
--EXPECT--
11