# Sylmruby

Sylmruby is a plugin for Sylpheed.

It implements simple interface to control Sylpheed
by ruby scripts.

# How to use

Sylmruby adds functionality to hook plugin interface.

# How to install

Clone sylpheed-mruby repository and build from scratch.

```sh
% svn checkout svn://sylpheed.sraoss.jp/sylpheed/trunk sylpheed
% cd sylpheed
% ./autogen.sh
% ./configure
% make
% cd plugin
% git clone https://github.com/kenhys/sylpheed-mruby.git sylmruby
% cd sylmruby
% git submodule init
% git submodule update
% ./autogen.sh
% ./configure --with-sylpheed-build-dir=`pwd`/../../
% make
% make copy
```

# How to use

# TODO

