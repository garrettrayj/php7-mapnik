--TEST--
\Mapnik\DatasourceCache
--SKIPIF--
<?php if (!extension_loaded("mapnik")) print "skip"; ?>
--FILE--
<?php

// Assert no plugins registered at start
$plugins = \Mapnik\DatasourceCache::getPluginNames();
print (count($plugins) === 0);

// Assert successful registration of datasource plugins
$configOutput = [];
exec('mapnik-config --input-plugins', $configOutput);
print \Mapnik\DatasourceCache::registerDatasources($configOutput[0]);

// Assert that we have plugins now
$plugins = \Mapnik\DatasourceCache::getPluginNames();
print (count($plugins) > 0);

?>
--EXPECT--
111