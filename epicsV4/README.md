# EPICS V4 Release 4.6.0 C++ Bundle Read-Me

This README provides a guide to building the EPICS V4 C++ modules from this
packaged release.

Authors:
 * David Hickin, Diamond
 * Andrew Johnson, Argonne National Laboratory

## Prerequisites

The EPICS V4 C++ Release Bundle is provided as source code, and must be built
for the specific set of target architectures needed.

The bundle requires recent versions of following software:

1. EPICS Base (3.14.12.x or 3.15.x) already built
2. Standard development tools as required for EPICS Base
3. Doxygen (optional, for generating documentation)

The pvAccess for Python package, pvaPy, requires:

1. Python development header files/libraries (v2.6.6 or later)
2. Boost (v1.41 or later) with Boost.Python
3. Boost.NumPy (optional, for use with NumPy)
4. Sphinx (optional, for generating documentation)


## Building EPICS V4

This bundle automates the steps of configuring and building the individual
sub-modules that make up the C++ implementation of EPICS V4. The modules
themselves rely on the EPICS Base build system. Detailed documentation of the
EPICS Base build facility can be found in the [EPICS Application Developers
Guide](http://www.aps.anl.gov/epics/base/R3-15/4-docs/AppDevGuide/node5.html).

Simple builds of the bundle can be achieved on most operating systems with a
single command, specifying the path to EPICS Base as a parameter:

    make EPICS_BASE=/path/to/epics/base

This will create configuration files for each module and build them in order. If
you have a computer with multiple CPUs you can make use of GNUmake's ability to
compile different parts in parallel on different CPUs by adding the `-j` flag;
on Windows you should use `-j8` for example on an 8-CPU box since GNUmake's load
detection doesn't work very well. With EPICS Base 3.14.12.x on Windows the
parallel build rules don't work properly though so `-j` should not be used.

It is only necessary to give the `EPICS_BASE=...` argument the first time you run `make`; for any subsequent commands you can omit that as it will be saved in a file `RELEASE.<host-arch>.Common` that gets created on the first run.

Some additional make commands that may be useful are:

* `make host` - This only builds for the host operating system, avoiding any cross-compiled architectures that the EPICS Base installation might be configured for. See the Manual Configuration section below for an alternative to using this target though.
* `make doxygen` - Run the `doxygen` tool on all sub-modules that use this to generate reference documents. Excludes the pvaPy and exampleCPP modules.
* `make runtests` - This executes all of the self-test programs that are delivered in the EPICS V4 C++ modules.
* `make clean` - Delete intermediate files which are not required after the build has finished for all architectures built on this host. This may be useful on systems where disk space is tight.
* `make distclean` - Delete all generated files for all architectures, including installed headers, binaries and library files. This also removes the file that saves the location of EPICS Base, so after running this the next build must specify the `EPICS_BASE=...` argument again.


### Building the Python API (pvaPy)

This bundle includes a copy of the pvaPy python module, but the build commands above will not compile it (pvaPy cannot be built on Microsoft Windows at all). To configure and build pvaPy after the above build has completed:

    make python

If you only want to build EPICS V4 for the python API, you can avoid compiling some of the modules in the first step above by just running

    make EPICS_BASE=/path/to/epics/base python

If you use the Python library NumPy and have the Boost.NumPy package installed, you can build pvaPy with NumPy support by adding the path to that package on either of the above command-lines:

    make python BOOST_NUMPY=/path/to/boost/numpy

or

    make EPICS_BASE=/path/to/epics/base BOOST_NUMPY=/path/to/boost/numpy python

After building pvaPy you can also generate the related reference documentation if you have the Sphinx Python documentation generator installed:

    make sphinx


### Manually Configuring Target Architectures

In many cases the automated configuration steps described above will be
sufficient to build the standard C++ modules. However in some circumstances a
manual configuration step may be needed first to control which embedded
cross-compiled target architectures get built. If your copy of EPICS Base is
configured to also compile for unsupported VxWorks targets (i.e. any version
earlier than VxWorks 6.8) you will have to exclude those targets from the build.
To do that, create a file called `CONFIG_SITE.local` in the top-level bundle
directory that overrides the `CROSS_COMPILER_TARGET_ARCHS` variable as
appropriate.

For example

    # CONFIG_SITE.local
    CROSS_COMPILER_TARGET_ARCHS = vxWorks-ppc32

explicitly configures the build for only the vxWorks-ppc32 target architecture (in addition to the host architecture), while

    # CONFIG_SITE.local
    CROSS_COMPILER_TARGET_ARCHS = \
        $(filter-out vxWorks-68040,$(CROSS_COMPILER_TARGET_ARCHS))

removes just the vxWorks-68040 cross-target from the set provided by EPICS Base.


## Further Information

For information about the individual modules, consult the documentation that comes with each one. In particular these files

* README.md
* documentation/RELEASE_NOTES.md
* Other files in the documentation directory

Additional information about EPICS V4 can be found on the
[EPICS V4 Website](http://epics-pvdata.sourceforge.net/)
and its
[Documentation page](http://epics-pvdata.sourceforge.net/literature.html)
