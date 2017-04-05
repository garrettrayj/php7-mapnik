--TEST--
\Mapnik\AggRenderer
--SKIPIF--
<?php if (!extension_loaded("mapnik")) print "skip"; ?>
--FILE--
<?php

require_once('util/test_case.php');

class AggRendererTest extends MapnikTestCase
{
    private $basePath;

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
        $image = new \Mapnik\Image(640, 480);
        $renderer = new \Mapnik\AggRenderer($this->exampleMap, $image);

        assert('$renderer instanceof \Mapnik\AggRenderer', 'Instantiating \Mapnik\AggRenderer failed.');
    }

    public function testApply()
    {
        $image = new \Mapnik\Image(640, 480);
        $renderer = new \Mapnik\AggRenderer($this->exampleMap, $image);

        assert('$renderer->apply() === true', 'AggRenderer->apply() failed.');
    }
}

new AggRendererTest();

?>
--EXPECT--