
window.projectVersion = 'master';

(function(root) {

    var bhIndex = null;
    var rootPath = '';
    var treeHtml = '        <ul>                <li data-name="namespace:Mapnik" class="opened">                    <div style="padding-left:0px" class="hd">                        <span class="glyphicon glyphicon-play"></span><a href="Mapnik.html">Mapnik</a>                    </div>                    <div class="bd">                                <ul>                <li data-name="class:Mapnik_AggRenderer" class="opened">                    <div style="padding-left:26px" class="hd leaf">                        <a href="Mapnik/AggRenderer.html">AggRenderer</a>                    </div>                </li>                            <li data-name="class:Mapnik_Box2D" class="opened">                    <div style="padding-left:26px" class="hd leaf">                        <a href="Mapnik/Box2D.html">Box2D</a>                    </div>                </li>                            <li data-name="class:Mapnik_DatasourceCache" class="opened">                    <div style="padding-left:26px" class="hd leaf">                        <a href="Mapnik/DatasourceCache.html">DatasourceCache</a>                    </div>                </li>                            <li data-name="class:Mapnik_Exception" class="opened">                    <div style="padding-left:26px" class="hd leaf">                        <a href="Mapnik/Exception.html">Exception</a>                    </div>                </li>                            <li data-name="class:Mapnik_Image" class="opened">                    <div style="padding-left:26px" class="hd leaf">                        <a href="Mapnik/Image.html">Image</a>                    </div>                </li>                            <li data-name="class:Mapnik_Map" class="opened">                    <div style="padding-left:26px" class="hd leaf">                        <a href="Mapnik/Map.html">Map</a>                    </div>                </li>                            <li data-name="class:Mapnik_ProjTransform" class="opened">                    <div style="padding-left:26px" class="hd leaf">                        <a href="Mapnik/ProjTransform.html">ProjTransform</a>                    </div>                </li>                            <li data-name="class:Mapnik_Projection" class="opened">                    <div style="padding-left:26px" class="hd leaf">                        <a href="Mapnik/Projection.html">Projection</a>                    </div>                </li>                </ul></div>                </li>                </ul>';

    var searchTypeClasses = {
        'Namespace': 'label-default',
        'Class': 'label-info',
        'Interface': 'label-primary',
        'Trait': 'label-success',
        'Method': 'label-danger',
        '_': 'label-warning'
    };

    var searchIndex = [
                    
            {"type": "Namespace", "link": "Mapnik.html", "name": "Mapnik", "doc": "Namespace Mapnik"},
            
            {"type": "Class", "fromName": "Mapnik", "fromLink": "Mapnik.html", "link": "Mapnik/AggRenderer.html", "name": "Mapnik\\AggRenderer", "doc": "&quot;&quot;"},
                                                        {"type": "Method", "fromName": "Mapnik\\AggRenderer", "fromLink": "Mapnik/AggRenderer.html", "link": "Mapnik/AggRenderer.html#method___construct", "name": "Mapnik\\AggRenderer::__construct", "doc": "&quot;AggRenderer constructor.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\AggRenderer", "fromLink": "Mapnik/AggRenderer.html", "link": "Mapnik/AggRenderer.html#method_apply", "name": "Mapnik\\AggRenderer::apply", "doc": "&quot;Render\/paint image from Map configuration.&quot;"},
            
            {"type": "Class", "fromName": "Mapnik", "fromLink": "Mapnik.html", "link": "Mapnik/Box2D.html", "name": "Mapnik\\Box2D", "doc": "&quot;&quot;"},
                                                        {"type": "Method", "fromName": "Mapnik\\Box2D", "fromLink": "Mapnik/Box2D.html", "link": "Mapnik/Box2D.html#method___construct", "name": "Mapnik\\Box2D::__construct", "doc": "&quot;Box2D constructor.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Box2D", "fromLink": "Mapnik/Box2D.html", "link": "Mapnik/Box2D.html#method_minX", "name": "Mapnik\\Box2D::minX", "doc": "&quot;Get east boundary of envelope.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Box2D", "fromLink": "Mapnik/Box2D.html", "link": "Mapnik/Box2D.html#method_minY", "name": "Mapnik\\Box2D::minY", "doc": "&quot;Get north boundary of envelope.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Box2D", "fromLink": "Mapnik/Box2D.html", "link": "Mapnik/Box2D.html#method_maxX", "name": "Mapnik\\Box2D::maxX", "doc": "&quot;Get west boundary of envelope.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Box2D", "fromLink": "Mapnik/Box2D.html", "link": "Mapnik/Box2D.html#method_maxY", "name": "Mapnik\\Box2D::maxY", "doc": "&quot;Get south boundary of envelope.&quot;"},
            
            {"type": "Class", "fromName": "Mapnik", "fromLink": "Mapnik.html", "link": "Mapnik/DatasourceCache.html", "name": "Mapnik\\DatasourceCache", "doc": "&quot;&quot;"},
                                                        {"type": "Method", "fromName": "Mapnik\\DatasourceCache", "fromLink": "Mapnik/DatasourceCache.html", "link": "Mapnik/DatasourceCache.html#method_registerDatasources", "name": "Mapnik\\DatasourceCache::registerDatasources", "doc": "&quot;Register datasource input plugins.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\DatasourceCache", "fromLink": "Mapnik/DatasourceCache.html", "link": "Mapnik/DatasourceCache.html#method_getPluginNames", "name": "Mapnik\\DatasourceCache::getPluginNames", "doc": "&quot;Get array of registered input plugins.&quot;"},
            
            {"type": "Class", "fromName": "Mapnik", "fromLink": "Mapnik.html", "link": "Mapnik/Exception.html", "name": "Mapnik\\Exception", "doc": "&quot;&quot;"},
                    
            {"type": "Class", "fromName": "Mapnik", "fromLink": "Mapnik.html", "link": "Mapnik/Image.html", "name": "Mapnik\\Image", "doc": "&quot;&quot;"},
                                                        {"type": "Method", "fromName": "Mapnik\\Image", "fromLink": "Mapnik/Image.html", "link": "Mapnik/Image.html#method___construct", "name": "Mapnik\\Image::__construct", "doc": "&quot;Image constructor.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Image", "fromLink": "Mapnik/Image.html", "link": "Mapnik/Image.html#method_saveToFile", "name": "Mapnik\\Image::saveToFile", "doc": "&quot;Save image to a file.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Image", "fromLink": "Mapnik/Image.html", "link": "Mapnik/Image.html#method_saveToString", "name": "Mapnik\\Image::saveToString", "doc": "&quot;Save image to a string.&quot;"},
            
            {"type": "Class", "fromName": "Mapnik", "fromLink": "Mapnik.html", "link": "Mapnik/Map.html", "name": "Mapnik\\Map", "doc": "&quot;&quot;"},
                                                        {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method___construct", "name": "Mapnik\\Map::__construct", "doc": "&quot;Map constructor.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_loadXmlString", "name": "Mapnik\\Map::loadXmlString", "doc": "&quot;Load map XML from string.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_loadXmlFile", "name": "Mapnik\\Map::loadXmlFile", "doc": "&quot;Load XML map file.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_zoom", "name": "Mapnik\\Map::zoom", "doc": "&quot;Zoom in or out by a given factor. Positive number zooms in, negative number zooms out.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_zoomAll", "name": "Mapnik\\Map::zoomAll", "doc": "&quot;Set the geographical extent of the map to the combined extents of all active layers.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_pan", "name": "Mapnik\\Map::pan", "doc": "&quot;Set the Map center to x,y location in the coordinates of the pixmap or map surface.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_panAndZoom", "name": "Mapnik\\Map::panAndZoom", "doc": "&quot;Set Map center to x,y location and zoom in or out.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_zoomToBox", "name": "Mapnik\\Map::zoomToBox", "doc": "&quot;Set the geographical extent of the map to Mapnik Box2D envelope.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_registerFonts", "name": "Mapnik\\Map::registerFonts", "doc": "&quot;Set path to search for fonts used in map.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_getBasePath", "name": "Mapnik\\Map::getBasePath", "doc": "&quot;Get base path used for loading map assets.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_setBasePath", "name": "Mapnik\\Map::setBasePath", "doc": "&quot;Set base path used for loading map assets.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_getWidth", "name": "Mapnik\\Map::getWidth", "doc": "&quot;Get canvas width.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_setWidth", "name": "Mapnik\\Map::setWidth", "doc": "&quot;Set canvas width.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_getHeight", "name": "Mapnik\\Map::getHeight", "doc": "&quot;Get canvas height.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_setHeight", "name": "Mapnik\\Map::setHeight", "doc": "&quot;Set canvas height.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_resize", "name": "Mapnik\\Map::resize", "doc": "&quot;Resize the canvas to given height and width.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_getSrs", "name": "Mapnik\\Map::getSrs", "doc": "&quot;Get spatial reference system.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_setSrs", "name": "Mapnik\\Map::setSrs", "doc": "&quot;Set spatial reference system.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_getBackgroundImage", "name": "Mapnik\\Map::getBackgroundImage", "doc": "&quot;Get path to background image.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_setBackgroundImage", "name": "Mapnik\\Map::setBackgroundImage", "doc": "&quot;Set path to background image.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_getBackgroundImageOpacity", "name": "Mapnik\\Map::getBackgroundImageOpacity", "doc": "&quot;Get background image opacity. 0 is transparent, 1 is opaque.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_setBackgroundImageOpacity", "name": "Mapnik\\Map::setBackgroundImageOpacity", "doc": "&quot;Set background image opacity. 0 is transparent, 1 is opaque.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_getMaximumExtent", "name": "Mapnik\\Map::getMaximumExtent", "doc": "&quot;Get maximum extent of projected map.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_setMaximumExtent", "name": "Mapnik\\Map::setMaximumExtent", "doc": "&quot;Set maximum extent of projected map.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_resetMaximumExtent", "name": "Mapnik\\Map::resetMaximumExtent", "doc": "&quot;Reset maximum extent.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_getCurrentExtent", "name": "Mapnik\\Map::getCurrentExtent", "doc": "&quot;Get current extent of projected map.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_getBufferSize", "name": "Mapnik\\Map::getBufferSize", "doc": "&quot;Get map bounds buffer size.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_setBufferSize", "name": "Mapnik\\Map::setBufferSize", "doc": "&quot;Set map bounds buffer. Used by placement detector to help avoid cutting labels.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_getBufferedExtent", "name": "Mapnik\\Map::getBufferedExtent", "doc": "&quot;Get extent of map with buffer applied.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_getScale", "name": "Mapnik\\Map::getScale", "doc": "&quot;Get map scale.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_getScaleDenominator", "name": "Mapnik\\Map::getScaleDenominator", "doc": "&quot;Get scale denominator.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_countLayers", "name": "Mapnik\\Map::countLayers", "doc": "&quot;Count number of map layers.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_removeStyle", "name": "Mapnik\\Map::removeStyle", "doc": "&quot;Remove a style.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_removeAll", "name": "Mapnik\\Map::removeAll", "doc": "&quot;Remove all styles and layers.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_getAspectFixMode", "name": "Mapnik\\Map::getAspectFixMode", "doc": "&quot;Get aspect fix mode.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\Map", "fromLink": "Mapnik/Map.html", "link": "Mapnik/Map.html#method_setAspectFixMode", "name": "Mapnik\\Map::setAspectFixMode", "doc": "&quot;Set aspect fix mode. See Map constants for valid values.&quot;"},
            
            {"type": "Class", "fromName": "Mapnik", "fromLink": "Mapnik.html", "link": "Mapnik/ProjTransform.html", "name": "Mapnik\\ProjTransform", "doc": "&quot;Re-project Box2D objects.&quot;"},
                                                        {"type": "Method", "fromName": "Mapnik\\ProjTransform", "fromLink": "Mapnik/ProjTransform.html", "link": "Mapnik/ProjTransform.html#method___construct", "name": "Mapnik\\ProjTransform::__construct", "doc": "&quot;ProjTransform constructor.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\ProjTransform", "fromLink": "Mapnik/ProjTransform.html", "link": "Mapnik/ProjTransform.html#method_forward", "name": "Mapnik\\ProjTransform::forward", "doc": "&quot;Transform box from source to destination projection.&quot;"},
                    {"type": "Method", "fromName": "Mapnik\\ProjTransform", "fromLink": "Mapnik/ProjTransform.html", "link": "Mapnik/ProjTransform.html#method_backward", "name": "Mapnik\\ProjTransform::backward", "doc": "&quot;Transform box from destination to source projection.&quot;"},
            
            {"type": "Class", "fromName": "Mapnik", "fromLink": "Mapnik.html", "link": "Mapnik/Projection.html", "name": "Mapnik\\Projection", "doc": "&quot;Represents a spatial reference system. Used in conjunction with ProjTransform to project bounding boxes.&quot;"},
                                                        {"type": "Method", "fromName": "Mapnik\\Projection", "fromLink": "Mapnik/Projection.html", "link": "Mapnik/Projection.html#method___construct", "name": "Mapnik\\Projection::__construct", "doc": "&quot;Projection constructor.&quot;"},
            
            
                                        // Fix trailing commas in the index
        {}
    ];

    /** Tokenizes strings by namespaces and functions */
    function tokenizer(term) {
        if (!term) {
            return [];
        }

        var tokens = [term];
        var meth = term.indexOf('::');

        // Split tokens into methods if "::" is found.
        if (meth > -1) {
            tokens.push(term.substr(meth + 2));
            term = term.substr(0, meth - 2);
        }

        // Split by namespace or fake namespace.
        if (term.indexOf('\\') > -1) {
            tokens = tokens.concat(term.split('\\'));
        } else if (term.indexOf('_') > 0) {
            tokens = tokens.concat(term.split('_'));
        }

        // Merge in splitting the string by case and return
        tokens = tokens.concat(term.match(/(([A-Z]?[^A-Z]*)|([a-z]?[^a-z]*))/g).slice(0,-1));

        return tokens;
    };

    root.Sami = {
        /**
         * Cleans the provided term. If no term is provided, then one is
         * grabbed from the query string "search" parameter.
         */
        cleanSearchTerm: function(term) {
            // Grab from the query string
            if (typeof term === 'undefined') {
                var name = 'search';
                var regex = new RegExp("[\\?&]" + name + "=([^&#]*)");
                var results = regex.exec(location.search);
                if (results === null) {
                    return null;
                }
                term = decodeURIComponent(results[1].replace(/\+/g, " "));
            }

            return term.replace(/<(?:.|\n)*?>/gm, '');
        },

        /** Searches through the index for a given term */
        search: function(term) {
            // Create a new search index if needed
            if (!bhIndex) {
                bhIndex = new Bloodhound({
                    limit: 500,
                    local: searchIndex,
                    datumTokenizer: function (d) {
                        return tokenizer(d.name);
                    },
                    queryTokenizer: Bloodhound.tokenizers.whitespace
                });
                bhIndex.initialize();
            }

            results = [];
            bhIndex.get(term, function(matches) {
                results = matches;
            });

            if (!rootPath) {
                return results;
            }

            // Fix the element links based on the current page depth.
            return $.map(results, function(ele) {
                if (ele.link.indexOf('..') > -1) {
                    return ele;
                }
                ele.link = rootPath + ele.link;
                if (ele.fromLink) {
                    ele.fromLink = rootPath + ele.fromLink;
                }
                return ele;
            });
        },

        /** Get a search class for a specific type */
        getSearchClass: function(type) {
            return searchTypeClasses[type] || searchTypeClasses['_'];
        },

        /** Add the left-nav tree to the site */
        injectApiTree: function(ele) {
            ele.html(treeHtml);
        }
    };

    $(function() {
        // Modify the HTML to work correctly based on the current depth
        rootPath = $('body').attr('data-root-path');
        treeHtml = treeHtml.replace(/href="/g, 'href="' + rootPath);
        Sami.injectApiTree($('#api-tree'));
    });

    return root.Sami;
})(window);

$(function() {

    // Enable the version switcher
    $('#version-switcher').change(function() {
        window.location = $(this).val()
    });

    
        // Toggle left-nav divs on click
        $('#api-tree .hd span').click(function() {
            $(this).parent().parent().toggleClass('opened');
        });

        // Expand the parent namespaces of the current page.
        var expected = $('body').attr('data-name');

        if (expected) {
            // Open the currently selected node and its parents.
            var container = $('#api-tree');
            var node = $('#api-tree li[data-name="' + expected + '"]');
            // Node might not be found when simulating namespaces
            if (node.length > 0) {
                node.addClass('active').addClass('opened');
                node.parents('li').addClass('opened');
                var scrollPos = node.offset().top - container.offset().top + container.scrollTop();
                // Position the item nearer to the top of the screen.
                scrollPos -= 200;
                container.scrollTop(scrollPos);
            }
        }

    
    
        var form = $('#search-form .typeahead');
        form.typeahead({
            hint: true,
            highlight: true,
            minLength: 1
        }, {
            name: 'search',
            displayKey: 'name',
            source: function (q, cb) {
                cb(Sami.search(q));
            }
        });

        // The selection is direct-linked when the user selects a suggestion.
        form.on('typeahead:selected', function(e, suggestion) {
            window.location = suggestion.link;
        });

        // The form is submitted when the user hits enter.
        form.keypress(function (e) {
            if (e.which == 13) {
                $('#search-form').submit();
                return true;
            }
        });

    
});


