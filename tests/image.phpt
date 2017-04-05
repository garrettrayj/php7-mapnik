--TEST--
\Mapnik\Image
--EXTENSIONS--
gd
--SKIPIF--
<?php if (!extension_loaded("mapnik")) print "skip"; ?>
--FILE--
<?php

require_once('util/test_case.php');

class ImageTest extends MapnikTestCase
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
        assert('$image instanceof \Mapnik\Image', '\Mapnik\Image instantiation failed.');
    }

    public function testSaveToFile()
    {
        $image = new \Mapnik\Image(640, 480);
        $renderer = new \Mapnik\AggRenderer($this->exampleMap, $image);
        $renderer->apply();

        $imageFile = sys_get_temp_dir() . '/image-test.png';
        $image->saveToFile($imageFile);
        $phpImageResource = imagecreatefrompng($imageFile);

        assert('$phpImageResource != false', 'Image->saveToFile() failed.');
        unlink($imageFile);
    }

    public function testSaveToFileWithFormat()
    {
        $image = new \Mapnik\Image(640, 480);
        $renderer = new \Mapnik\AggRenderer($this->exampleMap, $image);
        $renderer->apply();

        $imageFile = sys_get_temp_dir() . '/image-test';
        $image->saveToFile($imageFile, 'jpeg70');
        $phpImageResource = imagecreatefromjpeg($imageFile);

        assert('$phpImageResource != false', 'Image->saveToFile() with format failed.');
        unlink($imageFile);
    }

    public function testSaveToString()
    {
        $image = new \Mapnik\Image(640, 480);
        $renderer = new \Mapnik\AggRenderer($this->exampleMap, $image);
        $renderer->apply();

        $output = $image->saveToString();
        $phpImageResource = imagecreatefromstring($output);
        assert('$phpImageResource != false', 'Image->saveToString() failed.');
    }

    public function testSaveToStringWithFormat()
    {
        $image = new \Mapnik\Image(640, 480);
        $renderer = new \Mapnik\AggRenderer($this->exampleMap, $image);
        $renderer->apply();

        $output = $image->saveToString('jpeg');
        $phpImageResource = imagecreatefromstring($output);
        assert('$phpImageResource != false', 'Image->saveToString() with format failed.');
    }
}

new ImageTest();

?>
--EXPECT--