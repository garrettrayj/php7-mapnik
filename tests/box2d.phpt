--TEST--
\Mapnik\Box2D
--SKIPIF--
<?php if (!extension_loaded("mapnik")) print "skip"; ?>
--FILE--
<?php

require_once('util/test_case.php');

class Box2DTest extends MapnikTestCase
{
    public function testConstructor()
    {
        try {
            $box2d = new \Mapnik\Box2D('foo');
        } catch (\Throwable $e) {
            assert(
                $e instanceof \ArgumentCountError,
                'Instantiating \Mapnik\Box2D with single argument did not throw exception.'
            );
        }

        $box2d = new \Mapnik\Box2D(-180, -90, 180, 90);
        assert($box2d instanceof \Mapnik\Box2D, 'Instantiating \Mapnik\Box2D failed.');
    }

    public function testMinX()
    {
        $box2d = new \Mapnik\Box2D(-180, -90, 180, 90);
        assert($box2d->minX() === -180.0, 'Box2D->minX() failed.');
    }

    public function testMinY()
    {
        $box2d = new \Mapnik\Box2D(-180, -90, 180, 90);
        assert($box2d->minY() === -90.0, 'Box2D->minY() failed.');
    }

    public function testMaxX()
    {
        $box2d = new \Mapnik\Box2D(-180, -90, 180, 90);
        assert($box2d->maxX() === 180.0, 'Box2D->maxX() failed.');
    }

    public function testMaxY()
    {
        $box2d = new \Mapnik\Box2D(-180, -90, 180, 90);
        assert($box2d->maxY() === 90.0, 'Box2D->maxY() failed.');
    }
}

new Box2DTest();

?>
--EXPECT--
