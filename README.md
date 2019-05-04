PHP7 Mapnik
===========

<img src="docs/assets/readme_header_image.png?raw=true" alt="PHP7 Mapnik" title="Generated by PHP7 Mapnik" width="640" height="160">

Introduction
------------

This project is an extension for PHP 7 that enables geospatial rendering with [Mapnik](http://mapnik.org/).
Create tile generation scripts, dynamic tile services, or static maps with Mapnik XML and PHP.

* [API Documentation](http://garrettrayj.github.io/php7-mapnik/api/)
* [Mapnik on GitHub](https://github.com/mapnik/mapnik)
* [Mapnik XML Config Reference](https://github.com/mapnik/mapnik/wiki/XMLConfigReference)

[![Build Status](https://travis-ci.org/garrettrayj/php7-mapnik.svg?branch=master)](https://travis-ci.org/garrettrayj/php7-mapnik)


Requirements
------------

* PHP >= 7.0
   * Available through `brew intall php` on MacOS with Homebrew.
   * Available through `sudo apt-get install php-dev php-gd` on Ubuntu 16.04 Xenial.
* Mapnik 3.0.x
    * Available through `brew install mapnik` on MacOS with Homebrew.
    * Available through `sudo apt-get install libmapnik-dev` on Ubuntu 16.04 Xenial.

Installation
------------

    git clone https://github.com/garrettrayj/php7-mapnik.git && cd php7-mapnik
    phpize
    ./configure --with-mapnik
    make test
    make install
    echo "extension=mapnik.so" > /etc/php.d/mapnik.ini

Notes: 
- The PHP configuration directory may differ from above. Run `php -i | grep "Scan this dir"` to find your system's location.
- The ICU library path may need to be exported before building on MacOS. `export LDFLAGS="-L/usr/local/opt/icu4c/lib"`

Tile Server Example
-------------------

Run the example...

    ./example/run.sh

...then visit [http://localhost:8000/](http://localhost:8000/)

Usage
-----

```php
<?php

// Register datasource plugins
// Use `mapnik-config --input-plugins` to get input plugin directory
\Mapnik\DatasourceCache::registerDatasources('/usr/local/lib/mapnik/input');

// Create map
$map = new \Mapnik\Map(640, 480);

// Register fonts
// Use `mapnik-config --fonts` to get Mapnik fonts directory
$map->registerFonts('/usr/local/lib/mapnik/fonts');

// Load Mapnik XML
$map->loadXmlFile('my_awesome_map.xml', false, $basePath);

// Situate map content within canvas
$map->zoomAll();

// Create image
$image = new \Mapnik\Image(640, 480);

// Render
$renderer = new \Mapnik\AggRenderer($map, $image);
$renderer->apply();

// Save PNG image file
$image->saveToFile('my_awesome_map.png');
```

See the [API Documentation](http://garrettrayj.github.io/php7-mapnik/api/) for a complete list of
available objects and methods.

Dev Container
-------------

The included Dockerfile builds an image with all the dependencies need for extension development and testing, including testing for memory leaks.

    docker build -t php7-mapnik-dev .
    docker run -it --mount type=bind,source=$(pwd),target=/opt/php7-mapnik php7-mapnik-dev:latest
