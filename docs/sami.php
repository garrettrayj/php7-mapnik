<?php

use Sami\RemoteRepository\GitHubRemoteRepository;

return new Sami\Sami(__DIR__ . '/prototypes', [
    'theme' => 'default',
    'title' => 'PHP7 Mapnik API',
    'build_dir' => __DIR__ . '/api',
    'cache_dir' => __DIR__ . '/cache',
    'remote_repository' => new GitHubRemoteRepository('garrettrayj/php7-mapnik', ''),
    'default_opened_level' => 2
]);