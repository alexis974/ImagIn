# ImagIn

**ImagIn** is a free and open-source software designed for both beginner and intermediate photographers. The purpose of this software is to combine both calibration and retouch tools in one same software.

This project is made by four second year students at EPITA.

* [Building](#building-hammer)
  + [GUI](#gui)
  + [CLI](#cli)
* [Requirements](#requirements)
* [Wiki](#wiki-book)
* [Website](#website)
* [Team](#team)

## Building :hammer:

**ImagIn** development has reached its end unexpectedly due to the covid-19 crisis. However some members of the *Silvergrain* team are still going to work on it. Therefore be sure to run the following command to be sure to have the latest stable version before using.

```bash
git pull
```

### GUI

The software is made to be run with the GUI.

```bash
make all
./imagin.out
```

### CLI

The CLI is meant only to test the modules using samples choosed by the developers. Follow these steps to download them:

```bash
cd src/tools
./samples -h
```

Once the images have been downloaded, run this:

```bash
make cli
./imagin_cli.out
```

## Requirements

Please make sure you have the following libraries intalled in order to be able to compile the projet:

| Library       | Version   |
|:-------------:|:---------:|
| gtk+-3.0      | 3.0       |
| libjpeg       | 8.2.2     |
| libpng        | 16.37.0   |
| libtiff-4     | 5.5.0     |
| libxml-2.0    | 2.9.10    |
| gexiv2        | 2.0.0     |

## Wiki :book:

There is no wiki yet because of the unexpected end of the project. If you have any questions do not hesitate to ask them on the [Github page](https://github.com/alexis974/ImagIn/issues).


## Website

The [website](https://turodoras.github.io/ImagIn_website "ImagIn website") contains a small description of the project. It was supposed to have the wiki, some release notes and screenshots but the development stopped before it was done.

## Team

**ImagIn** is the creation of the *Silvergrain* team:

* Thomas Berlioz
* Alexis Boissière
* Alexis Boyadjian
* Alexandre Fresnais
