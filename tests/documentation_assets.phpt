--TEST--
Header Graphic
--SKIPIF--
<?php if (!extension_loaded("mapnik")) print "skip"; ?>
--FILE--
<?php

// Just a fun test to give the project a header graphic...

declare(strict_types=1);

require_once('util/test_case.php');

class DocumentationAssetsTest extends MapnikTestCase
{
    public function testHeaderGraphic()
    {
        $pluginConfigOutput = [];
        exec('mapnik-config --input-plugins', $pluginConfigOutput);
        \Mapnik\DatasourceCache::registerDatasources($pluginConfigOutput[0]);

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

        $imageFile = realpath(__DIR__ . '/../docs/assets') . '/header_graphic.png';
        $image->saveToFile($imageFile, 'png8');

        assert(file_exists($imageFile) === true, 'Header graphic does not exist.');
    }

    public function testDocsIndexBackground()
    {
        $pluginConfigOutput = [];
        exec('mapnik-config --input-plugins', $pluginConfigOutput);
        \Mapnik\DatasourceCache::registerDatasources($pluginConfigOutput[0]);

        $map = new \Mapnik\Map(4500, 2000);

        $fontConfigOutput = [];
        exec('mapnik-config --fonts', $fontConfigOutput);
        $map->registerFonts($fontConfigOutput[0]);

        $basePath = realpath(__DIR__ . '/data');
        $map->loadXmlFile($basePath . '/docs_index_background.xml', false, $basePath);

        $box = new \Mapnik\Box2D(-180, -70, 180, 85);
        $map->zoomToBox($box);

        $image = new \Mapnik\Image(4500, 2000);
        $renderer = new \Mapnik\AggRenderer($map, $image);
        $renderer->apply();

        $imageFile = realpath(__DIR__ . '/../docs/assets') . '/docs_index_background.png';
        $image->saveToFile($imageFile, 'png8');

        assert(file_exists($imageFile) === true, 'Docs index background does not exist.');
    }
}

new DocumentationAssetsTest();

?>
--EXPECT--
