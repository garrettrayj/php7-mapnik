--TEST--
Header Graphic
--SKIPIF--
<?php if (!extension_loaded("mapnik")) print "skip"; ?>
--FILE--
<?php

// Just a fun test to give the project a header graphic...

require_once('util/test_case.php');

class HeaderGraphicTest extends MapnikTestCase
{
    public function testMakeHeaderGraphic()
    {
        $pluginConfigOutput = [];
        exec('mapnik-config --input-plugins', $pluginConfigOutput);
        \Mapnik\DatasourceCache::registerDatasources($pluginConfigOutput[0]);

        // Travis build machine does not have CSV plugin due to incompatible Boost version.
        // Just return early if CSV isn't available.
        if (!in_array('csv', \Mapnik\DatasourceCache::getPluginNames())) return;

        $map = new \Mapnik\Map(1280, 320);

        $fontConfigOutput = [];
        exec('mapnik-config --fonts', $fontConfigOutput);
        $map->registerFonts($fontConfigOutput[0]);

        $basePath = realpath(__DIR__ . '/data');
        $map->loadXmlFile($basePath . '/header_graphic.xml', false, $basePath);

        $box = new \Mapnik\Box2D(-134, -25, 174, 67);
        $map->zoomToBox($box);

        $image = new \Mapnik\Image(1280, 320);
        $renderer = new \Mapnik\AggRenderer($map, $image);
        $renderer->apply();

        $imageFile = realpath(__DIR__ . '/../') . '/header_graphic.png';
        $image->saveToFile($imageFile);

        assert('file_exists($imageFile) === true', 'Header graphic does not exist.');
    }
}

new HeaderGraphicTest();

?>
--EXPECT--