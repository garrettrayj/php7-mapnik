--TEST--
\Mapnik\ProjTransform
--SKIPIF--
<?php if (!extension_loaded("mapnik")) print "skip"; ?>
--FILE--
<?php

require_once('util/test_case.php');

class ProjectionTest extends MapnikTestCase
{
    public function testConstructor()
    {
        try {
            $projection = new \Mapnik\Projection("foo bar");
        } catch (\Exception $e) {

        }
        assert(
            $e instanceof \Mapnik\Exception,
            'Instantiating \Mapnik\Projection with bad parameters did not throw exception.'
        );

        $projection = new \Mapnik\Projection("+init=epsg:4326");
        assert($projection instanceof \Mapnik\Projection, 'Instantiating \Mapnik\Projection failed.');

        $projection = new \Mapnik\Projection("+init=epsg:32638");
        assert($projection instanceof \Mapnik\Projection, 'Instantiating \Mapnik\Projection failed.');
    }
}

new ProjectionTest();

?>
--EXPECT--