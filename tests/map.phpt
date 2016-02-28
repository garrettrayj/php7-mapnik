--TEST--
\Mapnik\Map
--SKIPIF--
<?php if (!extension_loaded("mapnik")) print "skip"; ?>
--FILE--
<?php

// Setup
$basePath = realpath(dirname(__FILE__) . '/data');
$configOutput = [];
exec('mapnik-config --input-plugins', $configOutput);
print \Mapnik\DatasourceCache::registerDatasources($configOutput[0]);

// Assert map instantiation
$map = new \Mapnik\Map(640, 480);
print ($map instanceof \Mapnik\Map);

// Assert loading XML from string succeeds
$xml = file_get_contents($basePath . '/world.xml');
print $map->loadXmlString($xml, false, $basePath);

// Assert loading XML from a file
print $map->loadXmlFile($basePath . '/world.xml', false, $basePath);

// Assert zoom
print $map->zoom(4);

// Assert zoomAll
print $map->zoomAll();

// Assert pan
print $map->pan(20, 20);

// Assert panAndZoom
print $map->panAndZoom(20, 20, 4);

// Assert zoomToBox
$box = new \Mapnik\Box2D(-180, -90, 180, 90);
print $map->zoomToBox($box);

?>
--EXPECT--
11111111