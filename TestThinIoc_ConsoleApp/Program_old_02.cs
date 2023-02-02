//
// Program.cs
//

#if false

using System.Runtime.InteropServices;

namespace TestThinIoc_ConsoleApp
{

  //
  // The 'thin_ioc' project builds the dll to x64\Debug_DLL.
  // This dll (together with the necessary support dll's) has to be
  // manually copied to the console app project directory, alongside Program.cs.
  //
  // These DLL's need to be configured as 'Copy to output directory'.
  // so that they'll get copied to the same directory as our EXE.
  //
  //   thin_ioc.dll
  //   ca.dll
  //   com.dll
  //   dbCore.dll
  //   dbRecStd.dll
  //
  // ALTERNATIVELY (and much more conventiently) call 
  //   SetDllDirectory
  // to configure the P/Invoke search path to look for DLL's in the
  // standard place (eg x64/Debug_DLL) that they have been built to.
  //

  // To enable single-stepping into the DLL code :
  // - copy the .pdb file alongside the .dll
  //   [ note that this doesn't have to be copied to the output directory ]
  // - in the C# project properties, check 'enable native debugging'
  // https://stackoverflow.com/questions/21996641/how-to-step-into-p-invoked-c-code
  //

  //
  // To build and run this mess :
  //   COMPILE thin-ioc.cpp
  //   BUILD the thin_ioc project
  //   Copy thin_ioc.* from x64/Debug_DLL to /TestThinIoc_ConsoleApp
  //   Rebuild the TestThinIoc_ConsoleApp project, so that the DLL's get copied to the output directory
  //   Run the TestThinIoc_ConsoleApp
  //

  //
  // Default Marshaling for Arrays - .NET Framework | Microsoft Docs
  // https://docs.microsoft.com/en-us/dotnet/framework/interop/default-marshaling-for-arrays
  // Platform Invoke (P/Invoke) | Microsoft Docs
  // https://docs.microsoft.com/en-us/dotnet/standard/native-interop/pinvoke
  //

  //
  // Exporting C Functions for Use in C or C++ Language Executables
  // https://docs.microsoft.com/en-us/cpp/build/exporting-c-functions-for-use-in-c-or-cpp-language-executables?view=msvc-170
  // 
  // DLL Export Viewer
  // https://www.nirsoft.net/utils/dll_export_viewer.html
  // 
  // Native DLL export functions viewer
  // https://newbedev.com/is-there-any-native-dll-export-functions-viewer
  //
  // Exporting from a DLL Using __declspec(dllexport)
  // https://docs.microsoft.com/en-us/cpp/build/exporting-from-a-dll-using-declspec-dllexport?view=msvc-170
  //
  // A Journey Through a P/Invoke Call
  // https://www.jacksondunstan.com/articles/5120
  //

  //
  // How to Debug a Release Build
  // https://docs.microsoft.com/en-us/cpp/build/how-to-debug-a-release-build?view=msvc-170
  //

  //
  // How can I specify a [DllImport] path at runtime ? *****************
  // https://stackoverflow.com/questions/8836093/how-can-i-specify-a-dllimport-path-at-runtime
  // It's the native Win32 DLL loading rules that govern things, regardless of whether you're using
  // the handy managed wrappers (the P/Invoke marshaller just calls LoadLibrary). Those rules are enumerated
  // in great detail here :
  // https://docs.microsoft.com/en-gb/windows/win32/dlls/dynamic-link-library-search-order?redirectedfrom=MSDN
  // The default search order will start looking in the directory from which your application was loaded.
  // If you place the DLL there during the install, it will be found.
  // If you need to force the application to look in a different directory for the DLL,
  // you can modify the default search path using the SetDllDirectory function from Kernel32.
  // https://docs.microsoft.com/en-gb/windows/win32/api/winbase/nf-winbase-setdlldirectorya
  //
  // Register PDB File Location in Visual Studio
  //   Tools -> Options -> Debugging -> Symbols
  //   Add path to PDB files
  //

  public class Program
  {

    public static void Main ( string[] args ) 
    {

      System.Console.WriteLine("Invoking 'thin_ioc'") ;

      // If the function fails, the return value is zero.
      // To get extended error information, call GetLastError.
      bool ok = SetDllDirectory("C:\\Users\\steve\\source\\repos\\epics.dotnet\\x64\\Release_DLL") ;

      {
        // Just testing that we can invoke DLL functions
        int x = thin_ioc_func() ;
        int sum = thin_ioc_add(1,2) ;
        int product = thin_ioc_mul(2,3) ;
        string version = ca_version() ;
      }

      // Hmm, we can really only invoke 'thin_ioc_start' once,
      // because it allocates lots of memory etc for the db's and so on,
      // whch don't get released unless you call a complicated sequence.

      {
        var dbDescriptors = new DbDescriptor[]{
          new DbDescriptor("C:\\tmp\\xx.db"),
          //new DbDescriptor("db_B","macros_for_B"),
          //new DbDescriptor("db_C")
        } ;
        thin_ioc_set_is_running() ;
        System.Threading.Tasks.Task.Run(
          () => {
            int x = thin_ioc_start(
              dbDescriptors,
              dbDescriptors.Length
              // "pathToCmdFile",
              // "pathToDbdFile"
            ) ;
          }
        ) ;
      }

      while ( thin_ioc_is_running() != 0 )
      {
        System.Console.WriteLine("Waiting ...") ;
        string? line = System.Console.ReadLine() ;
        if ( line?.StartsWith("x") == true )
        {
          thin_ioc_request_stop() ;
        }
      }
      System.Console.WriteLine("Stopped ...") ;
      System.Console.ReadLine() ;

    }

    // In VS2022 command prompt :
    // > dumpbin /EXPORTS mydll.dll

    private const string THIN_IOC_DLL_path = (
      // @"C:\Users\steve\source\repos\epics.dotnet\x64\thin_ioc" 
      "thin_ioc" 
    ) ;

    [System.Runtime.InteropServices.DllImport("Kernel32")]
    // https://docs.microsoft.com/en-gb/windows/win32/api/winbase/nf-winbase-setdlldirectorya
    static extern bool SetDllDirectory ( [In] [Optional] [MarshalAs(UnmanagedType.LPStr)] string? pathToSearch ) ;

    [System.Runtime.InteropServices.DllImport(THIN_IOC_DLL_path)]
    static extern short thin_ioc_add ( short a, short b ) ;

    [System.Runtime.InteropServices.DllImport(THIN_IOC_DLL_path)]
    static extern short thin_ioc_mul ( short a, short b ) ;

    [System.Runtime.InteropServices.DllImport(THIN_IOC_DLL_path)]
    static extern short thin_ioc_div ( short a, short b ) ;

    [System.Runtime.InteropServices.DllImport(THIN_IOC_DLL_path)]
    static extern short thin_ioc_func ( ) ;

    struct DbDescriptor 
    {
      [MarshalAs(UnmanagedType.LPStr)] public string  PathToDbFile ;
      [MarshalAs(UnmanagedType.LPStr)] public string? Macros ;
      public DbDescriptor (
        string  pathToDbFile,
        string? macros = null
      ) {
        PathToDbFile = pathToDbFile ;
        Macros       = macros ; 
      }
    }

    [System.Runtime.InteropServices.DllImport(THIN_IOC_DLL_path)]
    static extern int thin_ioc_start ( 
      [In] DbDescriptor[]                           dbDescriptors,
      [In] int                                      nDbDescriptors,
      [In] [MarshalAs(UnmanagedType.LPStr)] string? pathToCmdFile   = null,
      [In] [MarshalAs(UnmanagedType.LPStr)] string? pathToDbdFile   = null
    ) ;

    [System.Runtime.InteropServices.DllImport(THIN_IOC_DLL_path)]
    static extern int thin_ioc_is_running ( ) ;

    [System.Runtime.InteropServices.DllImport(THIN_IOC_DLL_path)]
    static extern void thin_ioc_set_is_running ( ) ;

    [System.Runtime.InteropServices.DllImport(THIN_IOC_DLL_path)]
    static extern void thin_ioc_request_stop ( ) ;

    [System.Runtime.InteropServices.DllImport(THIN_IOC_DLL_path)]
    static extern void thin_ioc_call_atExits ( ) ;

    public static string ca_version ( )
    {
      return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(
        ca_version()
      )! ;
      [System.Runtime.InteropServices.DllImport("ca")]
      // https://epics.anl.gov/base/R3-15/9-docs/CAref.html#ca_version
      static extern System.IntPtr ca_version ( ) ;
    }

  }

}

#endif
