# ImagIn
ImagIn is a free and open-source software designed to be used for beginner and intermediate photographers. The purpose of the software is to combine both calibration and retouch tools in the same place.
This project is made by four second year students at EPITA


Building
--------

Since ImagIn is a software that is currently in active developement, if you cloned the repository a couple of days ago, please run the following command to be sure to have the latest stable version.

```bash
git pull
```

### GUI way

```bash
make all
./imagin.out

```

### CLI way

The cli way is meant only to test a lot of module using samples choosed by the developers. To download them, follow these steps :

```bash
cd src/tools
./samples -h
```

Once the images have been downloaded, run this :

```bash
make cli
./imagin_cli.out
```


Wiki
----

The wiki will arrive shortly. We thank you in advance for your patience


Authors
-------
* Thomas Berlioz
* Alexis Boissière
* Alexis Boyadjian
* Alexandre Fresnais
