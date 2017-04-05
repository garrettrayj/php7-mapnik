--TEST--
\Mapnik\DatasourceCache
--SKIPIF--
<?php if (!extension_loaded("mapnik")) print "skip"; ?>
--FILE--
<?php

require_once('util/test_case.php');

class DatasourceCacheTest extends MapnikTestCase
{
    public function testGetPluginNamesEmpty()
    {
        $plugins = \Mapnik\DatasourceCache::getPluginNames();
        assert('count($plugins) === 0', 'DatasourceCache cache plugins not empty even though register was not called.');
    }

    public function testRegisterDatasources()
    {
        $configOutput = [];
        exec('mapnik-config --input-plugins', $configOutput);
        \Mapnik\DatasourceCache::registerDatasources($configOutput[0]);
        $plugins = \Mapnik\DatasourceCache::getPluginNames();

        assert('count($plugins) > 0', 'No plugins after registering directory with DatasourceCache.');
    }
}

new DatasourceCacheTest();

?>
--EXPECT--