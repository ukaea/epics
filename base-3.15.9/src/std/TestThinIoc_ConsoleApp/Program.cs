//
// Program.cs
//

using System.Collections.Generic ;
using System.Linq;
using System.Reflection.Metadata;
using System.Runtime.InteropServices ;
using System.Threading.Channels;

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
  // ALTERNATIVELY (and much more conveniently) call 
  //   SetDllDirectory
  // to configure the P/Invoke search path to look for DLL's in the
  // standard place (eg x64/Debug_DLL) that they have been built to.
  //

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

      // We can make a good guess at the DLL location 
      // by working back from the directory holding this source code ...

      string pathToDirectoryHoldingThisProjectCode = System.IO.Path.GetDirectoryName(
        GetFullPathToThisSourceFile()
      )! ;
      string pathToSolutionDirectory = System.IO.Path.GetFullPath(
        path     : @"..\..\..\..\",
        basePath : pathToDirectoryHoldingThisProjectCode
      ) ;
      string pathToDllDirectory = (
        #if DEBUG
          pathToSolutionDirectory + "x64\\Debug_DLL"
        #else
          pathToSolutionDirectory + "x64\\Release_DLL"
        #endif
      ) ;

      // EDIT THIS TO EXPLICITLY SPECIFY THE DIRECTORY CONTAINING THE THINIOC DLL'S :

      bool ok = SetDllDirectory(
        pathToDllDirectory
        // "C:\\Users\\ktn98257\\source\\repos\\epics.dotnet\\x64\\Debug_DLL"
        // "C:\\Users\\steve\\source\\repos\\epics.dotnet\\x64\\Debug_DLL"
      ) ;

      try
      {

        System.Console.WriteLine("Invoking 'thin_ioc' with 'xx.db'") ;

        // The db file we'll load is called 'xx.db' (as it defines various PV's
        // that all have the prefix 'xx:') and it's located in the same directory
        // as this 'Program.cs' file.

        string pathToDbFileToLoad = GetFullPathToThisSourceFile().Replace(
          "Program.cs",
          "xx.db"
        ) ;

        int thin_ioc_version = thin_ioc_get_version() ;

        {
          // OLD CODE - just testing that we can invoke DLL functions
          // int x = thin_ioc_func() ;
          // int sum = thin_ioc_add(1,2) ;
          // int product = thin_ioc_mul(2,3) ;
          // string caVersion = ca_version() ;
        }

        // Hmm, we can really only invoke 'thin_ioc_start' once,
        // because it allocates lots of memory etc for the db's and so on,
        // which don't get released unless you make a complicated sequence
        // of API calls.

        System.Threading.ManualResetEvent stopThinIoc_event = new(false) ;

        var task_ignored = System.Threading.Tasks.Task.Run(
          () => {
            System.Console.WriteLine("ThinIoc thread is starting") ;
            ApiCallResult result = thin_ioc_initialise() ;
            if ( result != ApiCallResult.SUCCESS )
            {
              System.Console.WriteLine($"thin_ioc_initialise failed : {result}") ;
              return ;
            }
            result = thin_ioc_load_db_file(
              pathToDbFileToLoad
            ) ;
            if ( result != ApiCallResult.SUCCESS )
            {
              System.Console.WriteLine($"thin_ioc_start failed : {result}") ;
              return ;
            }
            result = thin_ioc_start() ;
            if ( result is ApiCallResult.SUCCESS )
            {
              System.Console.WriteLine($"thin_ioc_start succeeded") ;
              foreach ( string pvName in thin_ioc_get_pv_names() )
              {
                System.Console.WriteLine($"  PV : {pvName}") ;
              }
              System.Console.WriteLine("Waiting for 'stopThinIoc_event'") ;
              stopThinIoc_event.WaitOne() ;
              System.Console.WriteLine("'stopThinIoc_event' has been signalled") ;
              thin_ioc_call_atExits() ;
              System.Console.WriteLine("ThinIoc thread is terminating") ;
            }
            else
            {
              System.Console.WriteLine($"thin_ioc_start failed : {result}") ;
              System.Console.WriteLine("Waiting for ENTER ...") ;
              System.Console.ReadLine() ;
              return ;
            }
          }
        ) ;

        while ( true )
        {
          System.Console.WriteLine("Enter 'x' to issue 'stop' request ...") ;
          string? line = System.Console.ReadLine() ;
          if ( line?.StartsWith("x") == true )
          {
            System.Console.WriteLine("Signalling 'stopThinIoc_event") ;
            stopThinIoc_event.Set() ;
            break ;
          }
        }

      }
      catch ( System.Exception x )
      {
        System.Console.WriteLine(
          x.Message
        ) ;
      }

      System.Console.WriteLine("Waiting for ENTER ...") ;
      System.Console.ReadLine() ;

    }

    private static unsafe IReadOnlyList<string> thin_ioc_get_pv_names ( )
    {
      string commaSeparatedPvNames = Marshal.PtrToStringAnsi(
        (nint) thin_ioc_get_pv_names() 
      )! ;
      return commaSeparatedPvNames.Split(',') ;
      [System.Runtime.InteropServices.DllImport(THIN_IOC_DLL_path)]
      static extern unsafe byte * thin_ioc_get_pv_names ( ) ;
    }

    // private static unsafe bool CanGetPvNames_old_01 ( out IEnumerable<string> pvNames )
    // {
    //   int nBufferBytes = 64 ;
    //   while ( nBufferBytes <= 4096 ) 
    //   {
    //     byte[] buffer = new byte[nBufferBytes] ;
    //     fixed ( byte * pBuffer = buffer ) 
    //     {
    //       int status = thin_ioc_dbl_old_01(pBuffer,nBufferBytes) ;
    //       if ( status == 0 )
    //       {
    //         // string commaSeparatedPvNames = System.Text.Encoding.ASCII.GetString(buffer) ;
    //         string commaSeparatedPvNames = Marshal.PtrToStringAnsi(
    //           (nint) pBuffer
    //         )! ;
    //         pvNames = commaSeparatedPvNames.Split(',') ;
    //         return true ;
    //       }
    //       else if ( status == 1 ) 
    //       {
    //         // Try again with an increased buffer size
    //         nBufferBytes += nBufferBytes ; 
    //       }
    //     }
    //   }
    //   pvNames = Enumerable.Empty<string>() ;
    //   return false ;
    // }

    // In VS2022 command prompt :
    // > dumpbin /EXPORTS mydll.dll

    private const string THIN_IOC_DLL_path = "thin_ioc" ;

    [System.Runtime.InteropServices.DllImport("Kernel32")]
    // https://docs.microsoft.com/en-gb/windows/win32/api/winbase/nf-winbase-setdlldirectorya
    static extern bool SetDllDirectory ( [In] [Optional] [MarshalAs(UnmanagedType.LPStr)] string? pathToSearch ) ;

    public enum ApiCallResult : short {
      SUCCESS                   = 0,
      FAILED_TO_LOAD_DBD_FILE   = 1,
      FAILED_TO_REGISTER_DRIVER = 2,
      FAILED_TO_LOAD_DB_FILE    = 3,
      DBD_NOT_LOADED            = 4,
      INIT_FAILED               = 5,
    }

    [System.Runtime.InteropServices.DllImport(THIN_IOC_DLL_path)]
    static extern short thin_ioc_get_version ( ) ;

    [System.Runtime.InteropServices.DllImport(THIN_IOC_DLL_path)]
    static extern ApiCallResult thin_ioc_load_db_file ( 
      [In] [MarshalAs(UnmanagedType.LPStr)] string  pathTodbFile, 
      [In] [MarshalAs(UnmanagedType.LPStr)] string? macros = null
    ) ;

    [System.Runtime.InteropServices.DllImport(THIN_IOC_DLL_path)]
    static extern ApiCallResult thin_ioc_start ( ) ;

    [System.Runtime.InteropServices.DllImport(THIN_IOC_DLL_path)]
    static extern ApiCallResult thin_ioc_initialise ( int dbdOption = 0 ) ;

    [System.Runtime.InteropServices.DllImport(THIN_IOC_DLL_path)]
    static extern void thin_ioc_call_atExits ( ) ;

    // [System.Runtime.InteropServices.DllImport(THIN_IOC_DLL_path)]
    // static extern unsafe int thin_ioc_dbl_old_01 ( byte * resultBuffer, int nBytesAllocated ) ;

    // This helper function returns the path to the directory
    // that contains this 'Program.cs' file.

    private static string GetFullPathToThisSourceFile ( 
      [System.Runtime.CompilerServices.CallerFilePath] string? pathUtilitiesSourceCodePath = null 
    ) => pathUtilitiesSourceCodePath! ;

  }

}
