--TEST--
Header Graphic
--SKIPIF--
<?php if (!extension_loaded("mapnik")) print "skip"; ?>
--FILE--
<?php

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
        $map->loadXmlFile($basePath . '/readme_header.xml', false, $basePath);

        $box = new \Mapnik\Box2D(-134, -25, 174, 67);
        $map->zoomToBox($box);

        $image = new \Mapnik\Image(1280, 320);
        $renderer = new \Mapnik\AggRenderer($map, $image);
        $renderer->apply();

        $imageFile = realpath(__DIR__ . '/../docs/assets') . '/readme_header.png';
        $image->saveToFile($imageFile, 'png8');

        assert(file_exists($imageFile) === true, 'Header graphic was not generated.');
    }

    public function testSocialPreviewImage()
    {
        $pluginConfigOutput = [];
        exec('mapnik-config --input-plugins', $pluginConfigOutput);
        \Mapnik\DatasourceCache::registerDatasources($pluginConfigOutput[0]);

        $map = new \Mapnik\Map(1280, 640);

        $fontConfigOutput = [];
        exec('mapnik-config --fonts', $fontConfigOutput);
        $map->registerFonts($fontConfigOutput[0]);

        $basePath = realpath(__DIR__ . '/data');
        $map->loadXmlFile($basePath . '/social_preview.xml', false, $basePath);

        $box = new \Mapnik\Box2D(-134, -25, 174, 67);
        $map->zoomToBox($box);

        $image = new \Mapnik\Image(1280, 640);
        $renderer = new \Mapnik\AggRenderer($map, $image);
        $renderer->apply();

        $imageFile = realpath(__DIR__ . '/../docs/assets') . '/social_preview.png';
        $image->saveToFile($imageFile, 'png8');

        assert(file_exists($imageFile) === true, 'Social preview graphic was not generated.');
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
        $map->loadXmlFile($basePath . '/documentation_index.xml', false, $basePath);

        $box = new \Mapnik\Box2D(-180, -70, 180, 85);
        $map->zoomToBox($box);

        $image = new \Mapnik\Image(4500, 2000);
        $renderer = new \Mapnik\AggRenderer($map, $image);
        $renderer->apply();

        $imageFile = realpath(__DIR__ . '/../docs/assets') . '/documentation_index.png';
        $image->saveToFile($imageFile, 'png8');

        assert(file_exists($imageFile) === true, 'Documentation index background was not generated.');
    }
}

new DocumentationAssetsTest();

?>
--EXPECT--
