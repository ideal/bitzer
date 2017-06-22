# Bitzer

[![Build Status]][Travis CI]

Bitzer is an application that can be used to run and manage a number of processes on UNIX-like operating systems. It's much like [supervisord](http://supervisord.org/) and [supervise](http://cr.yp.to/daemontools/supervise.html).

What Bitzer do is quite simple currently. When Bitzer starts, it starts a number of processes based on your config, and then going to sleep. At sometime when one of the processes exits due to some reason, it will restart the process automatically.

Authors
=======

* Shang Yuanchun

Installation
============

You need cmake 2.6 or higher to build Bitzer from source. Further more, you also need flex (2.5.4 or higher), and bison (2.5 or higher).

```shell
mkdir build && cd build
```

```shell
cmake .. -DCMAKE_INSTALL_PREFIX=/tmp
make && make install
```

In that prefix directory, there is a `bitzer.conf` under `conf` directory, the format is described as below.

Configuration
=============

In `bitzer.conf`, you describe the processes that need to run. Here is an example:

```
task {
    name siesta;
    path /bin/sleep;
    args 10;
    dir  /tmp;
}
```

`name` and `path` must be provided. `args` is the arguments passed to the process. If `dir` is specified, it will be the process's working directory.

Running
=======

Just run `bitzer` if its path is in PATH, or type in the absolute path of bitzer.

The name
=======

[`Bitzer`](http://shaunthesheep.wikia.com/wiki/Bitzer) is a sheepdog in `Shaun the Sheep`.

[Build Status]: https://travis-ci.org/ideal/bitzer.svg?branch=master
[Travis CI]:    https://travis-ci.org/ideal/bitzer
