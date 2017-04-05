--TEST--
\Mapnik\ProjTransform
--SKIPIF--
<?php if (!extension_loaded("mapnik")) print "skip"; ?>
--FILE--
<?php

require_once('util/test_case.php');

class ProjTransformTest extends MapnikTestCase
{
    public function testConstructor()
    {
        $projTransform = new \Mapnik\ProjTransform(
            new \Mapnik\Projection("+init=epsg:4326"),
            new \Mapnik\Projection("+init=epsg:3857")
        );
        assert('$projTransform instanceof \Mapnik\ProjTransform', 'Instantiating \Mapnik\ProjTransform failed.');
    }

    public function testForward()
    {
        $projTransform = new \Mapnik\ProjTransform(
            new \Mapnik\Projection("+init=epsg:4326"),
            new \Mapnik\Projection("+init=epsg:3857")
        );

        $box2d = new \Mapnik\Box2D(-180, -90, 180, 90);
        $webMercatorBox = $projTransform->forward($box2d);

        $bounds = implode(
            ',',
            [$webMercatorBox->minX(), $webMercatorBox->minY(), $webMercatorBox->maxX(), $webMercatorBox->maxY()]
        );

        assert(
            '$bounds === "-20037508.342789,-20037508.342789,20037508.342789,20037508.342789"',
            'ProjTransform->forward() failed'
        );
    }

    public function testBackward()
    {
        $projTransform = new \Mapnik\ProjTransform(
            new \Mapnik\Projection("+init=epsg:4326"),
            new \Mapnik\Projection("+init=epsg:3857")
        );

        $box2d = new \Mapnik\Box2D(-180, -90, 180, 90);
        $webMercatorBox = $projTransform->forward($box2d);
        $latLngBox = $projTransform->backward($webMercatorBox);

        $bounds = implode(',', [$latLngBox->minX(), $latLngBox->minY(), $latLngBox->maxX(), $latLngBox->maxY()]);

        assert(
            '$bounds === "-180,-85.051128779807,180,85.051128779807"',
            'ProjTransform->forward() failed'
        );
    }
}

new ProjTransformTest();

?>
--EXPECT--