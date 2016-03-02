--TEST--
\Mapnik\ProjTransform
--SKIPIF--
<?php if (!extension_loaded("mapnik")) print "skip"; ?>
--FILE--
<?php

// Assert bad parameters throw exception
try {
    $projection = new \Mapnik\Projection("bad dog");
} catch (\Exception $e) {
    print ($e instanceof \Mapnik\Exception);
}

// Assert correct instantiation
$projection = new \Mapnik\Projection("+init=epsg:4326");
print ($projection instanceof \Mapnik\Projection);

?>
--EXPECT--
11
