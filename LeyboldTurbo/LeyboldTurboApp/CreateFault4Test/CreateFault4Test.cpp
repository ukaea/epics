//////////////////////////////////////////////////////////////////////////////////////////////////////
//																									//
//	Module:																							//
//		CreateFault4Test.cpp																		//
//																									//
//	Description:																					//
//		In order to test the 'RESET' functionality, how to put the controller in to a fault state	//
//		without risking damage to the hardware? Tricky...											//
//																									//
//		From Oliver Wasser (Oerlikon KN):															//
//		You can create a run-up time failure.														//
//		You have to switch parameter 32 to “0” and after that  you have to start the pump.			//
//																									//
//		The value of this parameter was 1200s before I changed anything.							//
//		(So as to put it back afterwards.)															//
//																									//
//		This code is for testing only; it allows to program the said parameter.						//
//																									//
//	LeyboldTurbo is distributed subject to a Software License Agreement								//
//	found in file LICENSE that is included with this distribution.									//
//																									//
//////////////////////////////////////////////////////////////////////////////////////////////////////

#include "..\src\USSPacket.h"
#include "..\src\LeyboldBase.h"

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

// Grrr, preprocessor...
#ifdef printf
#  undef printf
#endif /* printf */

int main(int argc, const char* argv[])
{
	if (argc < 2)
	{
		printf("Usage: COM<n> <new param32 value>\n");
		return -1;
	}

	int NewValue = -1;
	if (argc == 3)
		NewValue = atoi(argv[2]);

	// From https://msdn.microsoft.com/en-gb/library/windows/desktop/aa363201%28v=vs.85%29.aspx

	DCB dcb;
	HANDLE hCom;
	BOOL fSuccess;
	hCom = CreateFile(argv[1],  GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL );
	if (hCom == INVALID_HANDLE_VALUE)
	{
		//  Handle the error.
		printf ("CreateFile failed with error %lu.\n", GetLastError());
		return -1;
	}

	//  Initialize the DCB structure.
	SecureZeroMemory(&dcb, sizeof(DCB));
	dcb.DCBlength = sizeof(DCB);

	//  Build on the current configuration by first retrieving all current
	//  settings.
	fSuccess = GetCommState(hCom, &dcb);

	if (!fSuccess) 
	{
		//  Handle the error.
		printf ("GetCommState failed with error %lu.\n", GetLastError());
		CloseHandle(hCom);
		return (2);
	}

	//  Fill in some DCB values and set the com state: 
	//  19,200 bps, 8 data bits, even parity, and 1 stop bit.
	dcb.BaudRate = CBR_9600;	  //  baud rate
	dcb.ByteSize = 8;             //  data size, xmit and rcv
	dcb.Parity   = EVENPARITY;    //  parity bit
	dcb.StopBits = ONESTOPBIT;    //  stop bit

	fSuccess = SetCommState(hCom, &dcb);

	if (!fSuccess) 
	{
		//  Handle the error.
		printf ("SetCommState failed with error %lu.\n", GetLastError());
		CloseHandle(hCom);
		return (3);
	}

	//  Get the comm config again.
	fSuccess = GetCommState(hCom, &dcb);

	if (!fSuccess) 
	{
		//  Handle the error.
		printf ("GetCommState failed with error %lu.\n", GetLastError());
		CloseHandle(hCom);
		return (2);
	}

	COMMTIMEOUTS timeouts;

	timeouts.ReadIntervalTimeout = 1000;
	timeouts.ReadTotalTimeoutMultiplier = 1;
	timeouts.ReadTotalTimeoutConstant = 1000;
	timeouts.WriteTotalTimeoutMultiplier = 1;
	timeouts.WriteTotalTimeoutConstant = 1000;
	if (!SetCommTimeouts(hCom, &timeouts))
	{
		printf("setting timeouts failed\n");
		CloseHandle(hCom);
		return (2);
	}

	bool Running = true;
	USSPacket<CLeyboldBase::NoOfPZD2> USSWritePacket(Running, 32), USSReadPacket;

	if (NewValue != -1)
	{
		USSWritePacket.m_USSPacketStruct.m_PKE |= (2 << 12);
		USSWritePacket.m_USSPacketStruct.m_PWE = NewValue;
		USSWritePacket.GenerateChecksum();
	}
	USSWritePacket.m_USSPacketStruct.HToN();
	DWORD NumberOfBytesWritten, NumberOfBytesRead;
	if (!WriteFile(hCom, USSWritePacket.m_Bytes, USSWritePacket.m_USSPacketStruct.USSPacketSize, &NumberOfBytesWritten, NULL))
	{
		printf ("WriteFile failed with error %lu.\n", GetLastError());
		CloseHandle(hCom);
		return (3);
	}
	if (NumberOfBytesWritten != USSWritePacket.m_USSPacketStruct.USSPacketSize)
	{
		printf ("WriteFile did not write any data %lu.\n", GetLastError());
		CloseHandle(hCom);
		return (4);
	}

	if (!ReadFile(hCom,  USSReadPacket.m_Bytes, USSReadPacket.m_USSPacketStruct.USSPacketSize, &NumberOfBytesRead, NULL))
	{
		printf ("ReadFile failed with error %lu.\n", GetLastError());
		CloseHandle(hCom);
		return (5);
	}
	if (NumberOfBytesRead != USSReadPacket.m_USSPacketStruct.USSPacketSize)
	{
		printf ("ReadFile did not recieve any data %lu.\n", GetLastError());
		CloseHandle(hCom);
		return (6);
	}
	USSReadPacket.m_USSPacketStruct.NToH();
	if (!USSReadPacket.ValidateChecksum())
		printf("Checksum validation failed\n");

	printf("Current value of parameter 32 is %d\n", USSReadPacket.m_USSPacketStruct.m_PWE);

	CloseHandle(hCom);
	return 0;
}

