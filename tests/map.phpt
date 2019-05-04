--TEST--
\Mapnik\Map
--SKIPIF--
<?php use \Mapnik\Exception;

if (!extension_loaded("mapnik")) print "skip"; ?>
--FILE--
<?php

declare(strict_types=1);

require_once('util/test_case.php');

class MapTest extends MapnikTestCase
{
    private $basePath;

    /**
     * @var \Mapnik\Map
     */
    private $exampleMap;

    public function setup()
    {
        $configOutput = [];
        exec('mapnik-config --input-plugins', $configOutput);
        \Mapnik\DatasourceCache::registerDatasources($configOutput[0]);

        $this->basePath = realpath(__DIR__ . '/data');

        $this->exampleMap = new \Mapnik\Map(640, 480);
        $this->exampleMap->loadXmlFile($this->basePath . "/world.xml", false, $this->basePath);
        $this->exampleMap->zoomAll();
    }

    public function testConstructor()
    {
        try {
            $map = new \Mapnik\Map('foo', 'bar');
        } catch (\Throwable $e) {
            assert(
                $e instanceof TypeError,
                'Instantiating \Mapnik\Map with bad arguments did not throw TypeError.'
            );
        }

        $map = new \Mapnik\Map(640, 480);
        assert($map instanceof \Mapnik\Map, 'Instantiating \Mapnik\Map failed.');
    }

    public function testLoadXmlString()
    {
        $this->exampleMap = new \Mapnik\Map(640, 480);

        $xml = file_get_contents($this->basePath . '/world.xml');
        assert($this->exampleMap->loadXmlString($xml, false, $this->basePath) === null, 'Loading XML string failed.');
        assert($this->exampleMap->countLayers() === 1, 'Map did not have layer after loadXmlString.');

        $xml = file_get_contents($this->basePath . '/world_bad_srs.xml');
        try {
            $this->exampleMap->loadXmlString($xml);
        } catch (\Exception $e) {
            assert($e instanceof \Mapnik\Exception, 'Loading XML with bad projection did not throw Mapnik exception.');
        }
    }

    public function testLoadXmlFile()
    {
        $this->exampleMap = new \Mapnik\Map(640, 480);

        assert(
            $this->exampleMap->loadXmlFile($this->basePath . "/world.xml", false, $this->basePath) === null,
            'Loading XML file failed.'
        );
        assert($this->exampleMap->countLayers() === 1, 'Map did not have layer after loadXmlFile.');

        try {
            $this->exampleMap->loadXmlFile($this->basePath . "/world-not-there.xml", false, $this->basePath);
        } catch (\Exception $e) {
            assert($e instanceof \Mapnik\Exception, 'Loading non-existent XML file did not throw Mapnik exception.');
        }
    }

    public function testZoom()
    {
        $box = new \Mapnik\Box2D(-140, -90, -120, -70);
        $this->exampleMap->setAspectFixMode(\Mapnik\Map::RESPECT);
        $this->exampleMap->zoomToBox($box);

        $this->exampleMap->zoom(3);

        $extent = $this->exampleMap->getCurrentExtent();
        assert(
            floor($extent->minX()) === -160.0 && floor($extent->minY()) === -110.0 && floor($extent->maxX()) === -100.0 && floor($extent->maxY()) === -50.0,
            'Map->zoom() failed.'
        );
    }

    public function testZoomAll()
    {
        $box = new \Mapnik\Box2D(-140, -90, -120, -70);
        $this->exampleMap->setAspectFixMode(\Mapnik\Map::RESPECT);
        $this->exampleMap->setMaximumExtent($box);
        $this->exampleMap->zoomAll();
        $extent = $this->exampleMap->getCurrentExtent();

        assert(
            $extent->minX() === -140.0 && $extent->minY() === -90.0 && $extent->maxX() === -120.0 && $extent->maxY() === -70.0,
            'Map->zoomAll() failed.'
        );
    }

    public function testPan()
    {
        $box = new \Mapnik\Box2D(-140, -90, -120, -70);
        $this->exampleMap->setAspectFixMode(\Mapnik\Map::RESPECT);
        $this->exampleMap->zoomToBox($box);

        $this->exampleMap->pan(-10, 10);

        $extent = $this->exampleMap->getCurrentExtent();
        assert(
            floor($extent->minX()) === -151.0 && floor($extent->minY()) === -83.0 && floor($extent->maxX()) === -131.0 && floor($extent->maxY()) === -63.0,
            'Map->pan() failed.'
        );
    }

    public function testPanAndZoom()
    {
        $box = new \Mapnik\Box2D(-140, -90, -120, -70);
        $this->exampleMap->setAspectFixMode(\Mapnik\Map::RESPECT);
        $this->exampleMap->zoomToBox($box);

        $this->exampleMap->panAndZoom(-10, 10, 2);

        $extent = $this->exampleMap->getCurrentExtent();

        assert(
            floor($extent->minX()) === -161.0 && floor($extent->minY()) === -93.0 && floor($extent->maxX()) === -121.0 && floor($extent->maxY()) === -53.0,
            'Map->panAndZoom() failed.'
        );
    }

    public function testZoomToBox()
    {
        $box = new \Mapnik\Box2D(-140, -90, -120, -70);
        $this->exampleMap->setAspectFixMode(\Mapnik\Map::RESPECT);
        $this->exampleMap->zoomToBox($box);

        $extent = $this->exampleMap->getCurrentExtent();
        assert(
            $extent->minX() === -140.0 && $extent->minY() === -90.0 && $extent->maxX() === -120.0 && $extent->maxY() === -70.0,
            'Map->zoomToBox() failed.'
        );
    }

    public function testGetBasePath()
    {
        $basePath = $this->exampleMap->getBasePath();
        assert($basePath === __DIR__ . "/data", "Map->getBaseBath() returned wrong path.");
    }

    public function testSetBasePath()
    {
        $this->exampleMap->setBasePath(__DIR__);
        $basePath = $this->exampleMap->getBasePath();
        assert($basePath === __DIR__, "Map->setBasePath() failed.");
    }


    public function testGetWidth()
    {
        assert($this->exampleMap->getWidth() === 640, 'Map->getWidth() returned incorrect value.');
    }

    public function testSetWidth()
    {
        $this->exampleMap->setWidth(1024);
        assert($this->exampleMap->getWidth() === 1024, 'Map->setWidth() failed');

        try {
            $this->exampleMap->setWidth('foo');
        } catch (\Throwable $e) {
            assert(
                $e instanceof TypeError,
                'Calling Map->setWidth() with bad argument did not throw Mapnik exception.'
            );
        }
    }

    public function testGetHeight()
    {
        assert($this->exampleMap->getHeight() === 480, 'Map->getHeight() returned incorrect value.');
    }

    public function testSetHeight()
    {
        $this->exampleMap->setHeight(768);
        assert($this->exampleMap->getHeight() === 768, 'Map->setHeight() failed');

        try {
            $this->exampleMap->setHeight('foo');
        } catch (\Throwable $e) {
            assert(
                $e instanceof TypeError,
                'Calling Map->setHeight() with bad argument did not throw Mapnik exception.'
            );
        }
    }

    public function testResize()
    {
        $this->exampleMap->resize(1440, 720);
        assert($this->exampleMap->getWidth() === 1440, 'Incorrect width after resize');
        assert($this->exampleMap->getHeight() === 720, 'Incorrect height after resize');

        try {
            $this->exampleMap->resize('foo', 'bar');
        } catch (\Throwable $e) {
            assert(
                $e instanceof TypeError,
                'Calling Map->resize() with bad arguments did not throw Mapnik exception.'
            );
        }
    }

    public function testGetSrs()
    {
        assert(
            $this->exampleMap->getSrs() === "+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs",
            'Map->getSrs() returned invalid SRS'
        );
    }

    public function testSetSrs()
    {
        $this->exampleMap->setSrs('EPSG:3857');
        assert($this->exampleMap->getSrs() === "EPSG:3857", 'Map->setSrs() failed');
    }

    public function testGetBackgroundImage()
    {
        assert($this->exampleMap->getBackgroundImage() === null, 'Map->getBackgroundImage() failed.');
    }

    public function testSetBackgroundImage()
    {
        $this->exampleMap->setBackgroundImage('foo');
        assert($this->exampleMap->getBackgroundImage() === "foo", 'Map->getBackgroundImage() failed.');
    }

    public function testGetBackgroundImageOpacity()
    {
        assert($this->exampleMap->getBackgroundImageOpacity() === 1.0, 'Map->getBackgroundImageOpacity() failed.');
    }

    public function testSetGetBackgroundImageOpacity()
    {
        $this->exampleMap->setBackgroundImageOpacity(0.5);
        assert(
            $this->exampleMap->getBackgroundImageOpacity() === 0.5,
            'Map->setBackgroundImageOpacity() failed with float.'
        );

        $this->exampleMap->setBackgroundImageOpacity(1);
        assert(
            $this->exampleMap->getBackgroundImageOpacity() === 1.0,
            'Map->setBackgroundImageOpacity() failed with integer.'
        );
    }

    public function testGetMaximumExtent()
    {
        assert($this->exampleMap->getMaximumExtent() === null, 'Map->getMaximumExtent() failed.');
    }

    public function testSetMaximumExtent()
    {
        $box = new \Mapnik\Box2D(-140, -90, -120, -70);
        $this->exampleMap->setMaximumExtent($box);
        $extent = $this->exampleMap->getMaximumExtent();
        assert(
            $extent->minX() === -140.0 && $extent->minY() === -90.0 &&
            $extent->maxX() === -120.0 && $extent->maxY() === -70.0
            ,
            'Map->setMaximumExtent() failed.'
        );
    }

    public function testResetMaximumExtent()
    {
        $box = new \Mapnik\Box2D(-140, -90, -120, -70);
        $this->exampleMap->setMaximumExtent($box);
        $this->exampleMap->resetMaximumExtent();
        $extent = $this->exampleMap->getMaximumExtent();
        assert(
            $extent === null,
            'Map->resetMaximumExtent() failed.'
        );
    }

    public function testGetCurrentExtent()
    {
        $extent = $this->exampleMap->getCurrentExtent();
        assert(
            floor($extent->minX()) === -180.0 && floor($extent->minY()) === -139.0 && floor($extent->maxX()) === 180.0 && floor($extent->maxY()) === 131.0,
            'Map->getCurrentExtent() failed.'
        );
    }

    public function testGetBufferSize()
    {
        $buffer = $this->exampleMap->getBufferSize();
        assert($buffer === 0, 'Map->getBufferSize() failed.');
    }

    public function testSetBufferSize()
    {
        $this->exampleMap->setBufferSize(10);
        assert($this->exampleMap->getBufferSize() === 10, 'Map->setBufferSize() failed.');
    }

    public function testGetBufferedExtent()
    {
        $extent = $this->exampleMap->getBufferedExtent();
        assert(
            floor($extent->minX()) === -180.0 && floor($extent->minY()) === -139.0 && floor($extent->maxX()) === 180.0 && floor($extent->maxY()) === 131.0,
            'Map->getBufferedExtent() failed.'
        );

        $this->exampleMap->setBufferSize(10);
        $extent = $this->exampleMap->getBufferedExtent();

        assert(
            floor($extent->minX()) === -186.0 && floor($extent->minY()) === -144.0 && floor($extent->maxX()) === 185.0 && floor($extent->maxY()) === 137.0,
            'Map->getBufferedExtent() failed.'
        );
    }

    public function testGetScale()
    {
        assert((float) sprintf("%.2f", $this->exampleMap->getScale()) === 0.56, 'Map->getScale() failed.');
    }

    public function testGetScaleDenominator()
    {
        assert(
            (float) sprintf("%.2f", $this->exampleMap->getScaleDenominator()) === 223632905.61,
            'Map->getScaleDenominator() failed.'
        );
    }

    public function testCountLayers()
    {
        assert($this->exampleMap->countLayers() === 1, 'Map->countLayers() failed.');
    }

    public function testRemoveStyle()
    {
        assert($this->exampleMap->removeStyle("world_style") === null, 'Map->removeStyle() failed.');
    }

    public function testRemoveAll()
    {
        $this->exampleMap->removeAll();
        assert($this->exampleMap->countLayers() === 0, 'Map->removeAll() failed.');
    }

    public function testGetAspectFixMode()
    {
        assert($this->exampleMap->getAspectFixMode() === \Mapnik\Map::GROW_BBOX, 'Map->getAspectFixMode() failed.');
    }

    public function testSetAspectFixMode()
    {
        $this->exampleMap->setAspectFixMode(\Mapnik\Map::GROW_CANVAS);
        assert(
            $this->exampleMap->getAspectFixMode() === \Mapnik\Map::GROW_CANVAS,
            'Map->setAspectFixMode() GROW_CANVAS failed.'
        );

        $this->exampleMap->setAspectFixMode(\Mapnik\Map::SHRINK_BBOX);
        assert(
            $this->exampleMap->getAspectFixMode() === \Mapnik\Map::SHRINK_BBOX,
            'Map->setAspectFixMode() SHRINK_BBOX failed.'
        );

        $this->exampleMap->setAspectFixMode(\Mapnik\Map::SHRINK_CANVAS);
        assert(
            $this->exampleMap->getAspectFixMode() === \Mapnik\Map::SHRINK_CANVAS,
            'Map->setAspectFixMode() SHRINK_CANVAS failed.'
        );

        $this->exampleMap->setAspectFixMode(\Mapnik\Map::ADJUST_BBOX_WIDTH);
        assert(
            $this->exampleMap->getAspectFixMode() === \Mapnik\Map::ADJUST_BBOX_WIDTH,
            'Map->setAspectFixMode() ADJUST_BBOX_WIDTH failed.'
        );

        $this->exampleMap->setAspectFixMode(\Mapnik\Map::ADJUST_BBOX_HEIGHT);
        assert(
            $this->exampleMap->getAspectFixMode() === \Mapnik\Map::ADJUST_BBOX_HEIGHT,
            'Map->setAspectFixMode() ADJUST_BBOX_HEIGHT failed.'
        );

        $this->exampleMap->setAspectFixMode(\Mapnik\Map::ADJUST_CANVAS_WIDTH);
        assert(
            $this->exampleMap->getAspectFixMode() === \Mapnik\Map::ADJUST_CANVAS_WIDTH,
            'Map->setAspectFixMode() ADJUST_CANVAS_WIDTH failed.'
        );

        $this->exampleMap->setAspectFixMode(\Mapnik\Map::ADJUST_CANVAS_HEIGHT);
        assert(
            $this->exampleMap->getAspectFixMode() === \Mapnik\Map::ADJUST_CANVAS_HEIGHT,
            'Map->setAspectFixMode() ADJUST_CANVAS_HEIGHT failed.'
        );

        $this->exampleMap->setAspectFixMode(\Mapnik\Map::RESPECT);
        assert(
            $this->exampleMap->getAspectFixMode() === \Mapnik\Map::RESPECT,
            'Map->setAspectFixMode() RESPECT failed.'
        );

        $this->exampleMap->setAspectFixMode(\Mapnik\Map::GROW_BBOX);
        assert(
            $this->exampleMap->getAspectFixMode() === \Mapnik\Map::GROW_BBOX,
            'Map->setAspectFixMode() GROW_BBOX failed.'
        );

        try {
            $this->exampleMap->setAspectFixMode(100);
        } catch (\Exception $e) {
            assert($e instanceof \Mapnik\Exception, 'Setting invalid aspect fix mode did not throw exception.');
        }
    }
}

new MapTest();

?>
--EXPECT--
