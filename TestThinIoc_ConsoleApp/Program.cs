//
// Program.cs
//

using System.Runtime.InteropServices;

namespace TestThinIoc_ConsoleApp
{

  //
  // The 'thin_ioc' project builds the dll to x64\Debug_DLL.
  // This dll has to be manually copied to the console app project directory,
  // alongside this program (together with the support dll's).
  // These all get copied to the same directory as our EXE.
  //
  // To enable single-stepping into the DLL code :
  // - copy the .pdb file alongside the .dll
  //   [ note that this doesn't have to be copied to the output directory ]
  // - in the C# project properties, check 'enable native debugging'
  // https://stackoverflow.com/questions/21996641/how-to-step-into-p-invoked-c-code
  //

  public class Program
  {

    public static void Main ( string[] args ) 
    {

      System.Console.WriteLine("Invoking 'thin_ioc'") ;

      {
        // Just testing that we can invoke DLL functions
        int x = thin_ioc_func() ;
        string version = ca_version() ;
        int sum = thin_ioc_add(1,2) ;
        int product = thin_ioc_mul(2,3) ;
        int div = thin_ioc_div(12,3) ;
      }

      {
        var dbDescriptors = new DbDescriptor[]{
          new DbDescriptor("db_A","macros_for_A"),
          new DbDescriptor("db_B","macros_for_B"),
          new DbDescriptor("db_C")
        } ;
        int x = thin_ioc_start(
          "pathToDbdFile",
          dbDescriptors,
          dbDescriptors.Length,
          "pathToCmdFile"
        ) ;
      }

      System.Threading.Tasks.Task.Run(
        () => {
          int started = thin_ioc_start_xx(
            nSecs : 60
          ) ;
        }
      ) ;

      System.Console.WriteLine("Waiting ...") ;
      System.Console.ReadLine() ;
    }

    // In VS2022 command prompt :
    // > dumpbin /EXPORTS mydll.dll

    private const string THIN_IOC_DLL_path = (
      // @"C:\Users\steve\source\repos\epics.dotnet\x64\thin_ioc" 
      "thin_ioc" 
    ) ;

    [System.Runtime.InteropServices.DllImport(THIN_IOC_DLL_path)]
    static extern short thin_ioc_add ( short a, short b ) ;

    [System.Runtime.InteropServices.DllImport(THIN_IOC_DLL_path)]
    static extern short thin_ioc_mul ( short a, short b ) ;

    [System.Runtime.InteropServices.DllImport(THIN_IOC_DLL_path)]
    static extern short thin_ioc_div ( short a, short b ) ;

    [System.Runtime.InteropServices.DllImport(THIN_IOC_DLL_path)]
    static extern short thin_ioc_func ( ) ;

    [System.Runtime.InteropServices.DllImport(THIN_IOC_DLL_path)]
    static extern short thin_ioc_start_xx ( short nSecs = 60 ) ;

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
      [MarshalAs(UnmanagedType.LPStr)] string?        pathToDbdFile,
      [In]                             DbDescriptor[] dbDescriptors,
      int                                             nDbDescriptors,
      [MarshalAs(UnmanagedType.LPStr)] string?        pathToCmdFile
    ) ;

    [System.Runtime.InteropServices.DllImport(THIN_IOC_DLL_path)]
    static extern short thin_ioc_is_running ( ) ;

    [System.Runtime.InteropServices.DllImport(THIN_IOC_DLL_path)]
    static extern short thin_ioc_request_stop ( ) ;

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
