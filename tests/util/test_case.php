<?php

class MapnikTestCase
{
    public function __construct()
    {
        $methods = get_class_methods($this);

        foreach($methods as $method) {
            if (substr($method, 0, 4) === 'test') {
                if (method_exists($this, 'setup')) $this->setup();
                $this->{$method}();
            }
        }
    }
}