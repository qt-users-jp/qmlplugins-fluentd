%modules = ( # path to module name map
    "QtFluentd" => "$basedir/src/fluentd",
);

%moduleheaders = ( # restrict the module headers to those found in relative path
);

%classnames = (
    "qfluentd.h" => "QFluentd"
);
%dependencies = (
    "qtbase" => "refs/heads/dev",
);
