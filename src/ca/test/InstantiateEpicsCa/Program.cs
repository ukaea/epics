using System;

namespace InstantiateEpicsCa
{
  class Program
  {
    static void Main(string[] args)
    {
      Console.WriteLine($"Channel access is {read_ca_version()}");
    }

    public static string read_ca_version()
    {
      return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(
      ca_version()
    )!;

      [System.Runtime.InteropServices.DllImport("ca")]
      static extern System.IntPtr ca_version();
    }
  }
}