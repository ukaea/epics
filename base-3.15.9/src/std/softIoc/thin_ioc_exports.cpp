//
// thin_ioc_exports.cpp
//

// For testing access via P/Invoke ...

// #define epicsExportSharedSymbols


// epicsShareAPI

extern "C" {

  __declspec(dllexport) int thin_ioc_add ( int a, int b ) ;

  int thin_ioc_add ( int a, int b )
  {
    return a + b ;
  }

  __declspec(dllexport) int thin_ioc_func ( )
  {
    return 123 ;
  }

}

extern "C" __declspec(dllexport) 
int thin_ioc_mul ( int a, int b )
{
  return a * b ;
}

