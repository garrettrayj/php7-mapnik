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
        try {
            $projTransform = new \Mapnik\ProjTransform('foo', 'bar');
        } catch (\Exception $e) {

        }
        assert(
            $e instanceof \Mapnik\Exception,
            'Instantiating \Mapnik\ProjTransform with bad arguments did not throw exception.'
        );

        $projTransform = new \Mapnik\ProjTransform(
            new \Mapnik\Projection("+init=epsg:4326"),
            new \Mapnik\Projection("+init=epsg:3857")
        );
        assert($projTransform instanceof \Mapnik\ProjTransform, 'Instantiating \Mapnik\ProjTransform failed.');
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
            $bounds === "-20037508.342789,-20037508.342789,20037508.342789,20037508.342789",
            'ProjTransform->forward() failed'
        );
    }

    public function testForwardProj()
    {
        $projTransform = new \Mapnik\ProjTransform(
            new \Mapnik\Projection("+init=epsg:4326"),
            new \Mapnik\Projection("+init=epsg:2267")
        );

        $box2d = new \Mapnik\Box2D(-102, 35.500, -93, 36);

        $forwardBox = $projTransform->forward($box2d);

        assert(
            floor($forwardBox->minX()) === 778149.0 && floor($forwardBox->minY()) === 206534.0 &&
            floor($forwardBox->maxX()) === 3456201.0 && floor($forwardBox->maxY()) === 402092.0,
            'ProjTransform->forward() with uncommon EPSG code failed'
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
            $bounds === "-180,-85.051128779807,180,85.051128779807",
            'ProjTransform->backward() failed'
        );
    }

    public function testBackwardProj()
    {
        $projTransform = new \Mapnik\ProjTransform(
            new \Mapnik\Projection("+init=epsg:4326"),
            new \Mapnik\Projection("+init=epsg:2267")
        );

        $box2d = new \Mapnik\Box2D(778149.57465738,206534.77597693,3456201.2327298,402092.40888926);

        $backwardBox = $projTransform->backward($box2d);

        assert(
            floor($backwardBox->minX()) === -103.0 && floor($backwardBox->minY()) === 35.0 &&
            floor($backwardBox->maxX()) === -93.0 && floor($backwardBox->maxY()) === 36.0,
            'ProjTransform->backward() with uncommon EPSG code failed'
        );
    }
}

new ProjTransformTest();

?>
--EXPECT--