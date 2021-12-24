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

      string version = ca_version() ;

      int sum = thin_ioc_add(1,2) ;

    }


    private const string THIN_IOC_DLL_path = (
      // @"C:\Users\steve\source\repos\epics.dotnet\x64\thin_ioc" 
      "thin_ioc" 
    ) ;

    [System.Runtime.InteropServices.DllImport(THIN_IOC_DLL_path)]
    static extern short thin_ioc_add ( short a, short b ) ;

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
