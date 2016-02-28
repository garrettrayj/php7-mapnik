--TEST--
\Mapnik\Box2D
--SKIPIF--
<?php if (!extension_loaded("mapnik")) print "skip"; ?>
--FILE--
<?php

// Assert Box2D instantiation
$box2d = new \Mapnik\Box2D(-180, -90, 180, 90);
print ($box2d instanceof \Mapnik\Box2D);

?>
--EXPECT--
1