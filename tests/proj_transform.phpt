--TEST--
\Mapnik\ProjTransform
--SKIPIF--
<?php if (!extension_loaded("mapnik")) print "skip"; ?>
--FILE--
<?php

// Assert instantiation
$wgs84 = new \Mapnik\Projection("+init=epsg:4326");
$webMercator = new \Mapnik\Projection("+init=epsg:3857");

$projTransform = new \Mapnik\ProjTransform($wgs84, $webMercator);
print ($projTransform instanceof \Mapnik\ProjTransform) . "\n";

// Assert forward() works
$box2d = new \Mapnik\Box2D(-180, -90, 180, 90);
$webMercatorBox = $projTransform->forward($box2d);
print implode(
    ',',
    [$webMercatorBox->minX(), $webMercatorBox->minY(), $webMercatorBox->maxX(), $webMercatorBox->maxY()]
);
print "\n";

// Assert backward() works
$latLngBox = $projTransform->backward($webMercatorBox);
print implode(
    ',',
    [$latLngBox->minX(), $latLngBox->minY(), $latLngBox->maxX(), $latLngBox->maxY()]
);

?>
--EXPECT--
1
-20037508.342789,-20037508.342789,20037508.342789,20037508.342789
-180,-85.051128779807,180,85.051128779807
