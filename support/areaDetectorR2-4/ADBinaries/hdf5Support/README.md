The files in this directory come from:


*.h, header files HDF 1.10.0-patch1 source distribution. These files were located in the hdf5/include directory
after installing the package on Linux.  The exception is H5FDWindows.h came from src/ directory in the
distribution. 

The Windows binaries were built using cmake (3.4) and the Visual Studio 2010 compiler, using instructions
from the HDF Group website. The build has been configured as a Release build - but including debug files
as these may make it easier to debug an IOC which is linked to the HDF5 libraries.

The szip and zlib compression libraries were built from sources as part of the HDF5 library build.

This version of ADBinaries include both a static and a shared (dynamic/DLL) build of the HDF5 libraries. The
library names do not conflict so both the libraries of both builds are installed into lib/<T_A>/. The include
files (H5pubconf.h) has been modified to support both builds. The H5pubconf.h file was copied from the
static/shared build (as it is generated at cmake configuration time) into os/WIN32/H5pubconf_[32|64]_[static|
shared].h as it contain build-specific #defines. If using the dynamic/shared build the user
must define the either one of the pre-processor variables H5_BUILT_AS_DYNAMIC_LIB or H5_BUILD_AS_STATIC_LIB. 

Shared 64-bit build for Windows
-------------------------------

The HDF5 library build has been configured in the thread-safe mode which places a global lock around
all public HDF5 function calls. This makes it safe to use in multiple instances of HDF5 plugins.

The commands to configure and build the libraries were:

    cmake.exe -G "Visual Studio 10 Win64" -DCMAKE_BUILD_TYPE:STRING=RelWithDebInfo 
        -DBUILD_SHARED_LIBS:BOOL=ON 
        -DCMAKE_SKIP_RPATH:BOOL=ON 
        -DHDF5_ALLOW_EXTERNAL_SUPPORT:STRING=TGZ 
        -DHDF5_ENABLE_SZIP_SUPPORT:BOOL=ON 
        -DHDF5_ENABLE_THREADSAFE:BOOL=ON 
        -DHDF5_ENABLE_Z_LIB_SUPPORT:BOOL=ON 
        -DHDF5_ENABLE_SZIP_ENCODING:BOOL=ON 
        -DSZIP_USE_EXTERNAL:BOOL=ON 
        -DZLIB_USE_EXTERNAL:BOOL=ON  
        -DHDF5_ALLOW_EXTERNAL_SUPPORT:STRING=TGZ 
        -DZLIB_TGZ_NAME:STRING="ZLib.tar.gz" 
        -DSZIP_TGZ_NAME:STRING="SZip.tar.gz" 
        -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON
        -DHDF5_BUILD_CPP_LIB:BOOL=OFF 
        -DHDF5_BUILD_HL_LIB:BOOL=OFF 
        -DHDF5_BUILD_EXAMPLES:BOOL=OFF 
        -DHDF5_BUILD_TOOLS:BOOL=OFF 
        -DBUILD_TESTING:BOOL=OFF 
        -DBUILD_STATIC_EXECS:BOOL=OFF         
        ..
    
    set VERBOSE=1
    
    cmake.exe --build . --config RelWithDebInfo


Static 64-bit build for Windows
-------------------------------

Threadsafe option is not compatible with the static build. Thus when the HDF5 library is a static build 
the plugins which use the HDF5 library can not be used in multiple instances of the plugin.

The MSVC compiler flags /MD is set by default by cmake - however this conflicts with the 
areaDetector build configuration which uses /MT. Thus all instances of /MD are replaced with
/MT in the build configuration CMAKE_C[XX]_FLAGS_<config> (see below). The explanation of the
relevant flags can be found on [MSDN](http://msdn.microsoft.com/en-us/library/2kzt1wy3%28v=vs.100%29.aspx)

The following commands were used to compile and build the 64-bit libraries:
 
    cmake.exe -G "Visual Studio 10 Win64" -DCMAKE_BUILD_TYPE:STRING=RelWithDebInfo 
        -DBUILD_SHARED_LIBS:BOOL=OFF 
        -DCMAKE_SKIP_RPATH:BOOL=ON 
        -DHDF5_ALLOW_EXTERNAL_SUPPORT:STRING=TGZ 
        -DHDF5_ENABLE_SZIP_SUPPORT:BOOL=ON 
        -DHDF5_ENABLE_Z_LIB_SUPPORT:BOOL=ON 
        -DHDF5_ENABLE_SZIP_ENCODING:BOOL=ON 
        -DSZIP_USE_EXTERNAL:BOOL=ON 
        -DZLIB_USE_EXTERNAL:BOOL=ON  
        -DHDF5_ALLOW_EXTERNAL_SUPPORT:STRING=TGZ 
        -DZLIB_TGZ_NAME:STRING="ZLib.tar.gz" 
        -DSZIP_TGZ_NAME:STRING="SZip.tar.gz" 
        -DCMAKE_CXX_FLAGS_DEBUG:STRING="/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1"
        -DCMAKE_CXX_FLAGS_MINSIZEREL:STRING="/MT /O1 /Ob1 /D NDEBUG"
        -DCMAKE_CXX_FLAGS_RELEASE:STRING="/MT /O2 /Ob2 /D NDEBUG"
        -DCMAKE_CXX_FLAGS_RELWITHDEBINFO:STRING="/MT /Zi /O2 /Ob1 /D NDEBUG"
        -DCMAKE_C_FLAGS_DEBUG:STRING="/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1"
        -DCMAKE_C_FLAGS_MINSIZEREL:STRING="/MT /O1 /Ob1 /D NDEBUG"
        -DCMAKE_C_FLAGS_RELEASE:STRING="/MT /O2 /Ob2 /D NDEBUG"
        -DCMAKE_C_FLAGS_RELWITHDEBINFO:STRING="/MT /Zi /O2 /Ob1 /D NDEBUG"
        -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON
        -DHDF5_BUILD_CPP_LIB:BOOL=OFF 
        -DHDF5_BUILD_HL_LIB:BOOL=OFF 
        -DHDF5_BUILD_EXAMPLES:BOOL=OFF 
        -DHDF5_BUILD_TOOLS:BOOL=OFF 
        -DBUILD_TESTING:BOOL=OFF 
        -DBUILD_STATIC_EXECS:BOOL=OFF
        ..
    
    set VERBOSE=1
    
    cmake.exe --build . --config RelWithDebInfo


32 bit build configuration
--------------------------

The 32-bit binaries were built with the same cmake configuration options as for the 64-bit build
as described above; only replacing the first option:

    cmake.exe -G "Visual Studio 10 Win64"

With:

     cmake.exe -G "Visual Studio 10"



