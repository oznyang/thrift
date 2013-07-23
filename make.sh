#!/bin/sh

./bootstrap.sh
./configure CFLAGS='-O2' CXXFLAGS='-O2' --without-erlang --without-csharp --without-python --without-java --without-php --without-php_extension --without-boost --without-d --without-c_glib --without-ruby --without-haskell --without-go --without-qt4 --without-perl
make -j8
make install
