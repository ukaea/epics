//
// Program.cs
//

namespace TestThinIoc_ConsoleApp
{

  public class Program
  {

    public static void Main ( string[] args ) 
    {

      System.Console.WriteLine("Invoking 'thin_ioc'") ;

      int x = thin_ioc_func() ;

      string version = ca_version() ;

      int sum = thin_ioc_add(1,2) ;

      int product = thin_ioc_mul(2,3) ;

      int div = thin_ioc_div(12,3) ;

      System.Threading.Tasks.Task.Run(
        () => {
          int started = thin_ioc_start_xx() ;
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
