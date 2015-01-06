//////////////////////////////////////////////////////////////////////////////////////////////////////
//																									//
//	Module:																							//
//		USSPacket.h																					//
//																									//
//	Description:																					//
//		Declares the USSPacket union and USSPacketStruct struct.									//
//		The structure is used by both the 'real' IOC and the simulator.								//
//		Defines the content of the packet.															//
//		The union is used because the content is accessed both as individual structured fields,		//
//		and as a byte array.																		//
//																									//
//	Author:  Peter Heesterman (Tessella plc). Date: 05 Jan 2015.									//
//																									//
//	LeyboldTurbo is distributed subject to a Software License Agreement								//
//	found in file LICENSE that is included with this distribution.									//
//																									//
//////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef USSPacketStruct_H
#define USSPacketStruct_H

#include <epicsTypes.h>
#include <osiSock.h>

static const size_t USSPacketSize = 24;

#pragma pack(push, 1)

union USSPacket
{
	struct USSPacketStruct
	{
		void SetDefault() {
			m_STX = 2;			// Start byte 2
			m_LGE = 22;			// LGE Length of the payload data block in bytes (bytes 3 to 22) + 2: 22
			m_ADR = m_Reserved = m_IND = m_BCC = 0;
			m_PKE = m_PZD1 = m_PZD2 = m_PZD3 = m_PZD4 = m_PZD5 = m_PZD6 = 0;
			m_PWE = 0;
		}
		epicsUInt8  m_STX;		// Start byte 2
		epicsUInt8  m_LGE;		// LGE Length of the payload data block in bytes (bytes 3 to 22) + 2: 22
		epicsUInt8  m_ADR;		// Frequency converter address RS232: 0. RS485: 0...15
		epicsUInt16 m_PKE;		// Parameter number and type of access Value (s. 2.1)
		epicsUInt8  m_Reserved;	// 0
		epicsUInt8	m_IND;		// Parameter index Value (s. 2.1)
		epicsUInt32 m_PWE;		// Parameter value Value
		epicsUInt16	m_PZD1;		// Status and control bits Value (see 2.2)
		epicsUInt16	m_PZD2;		// Current stator frequency (= P3) Value (Hz)
		epicsUInt16 m_PZD3;		// Current frequency converter temperature (= P11) Value (°C)
		epicsUInt16 m_PZD4;		// Current motor current (= P5) Value (0.1 A)
		epicsUInt16 m_PZD5;		// Current pump temperature (= P127) Value (°C)
		epicsUInt16 m_PZD6;		// Current intermediate circuit voltage (=P4) Value (0.1 V)
		epicsUInt8	m_BCC;		// Recursive calculation:
		void HToN() {
			// In the case of word data (16 or 32 bits long) the high byte is transferred first (Motorola standard).
			m_PKE = htons(m_PKE);
			m_PWE = htonl(m_PWE);
			m_PZD1 = htons(m_PZD1);
			m_PZD2 = htons(m_PZD2);
			m_PZD3 = htons(m_PZD3);
			m_PZD4 = htons(m_PZD4);
			m_PZD5 = htons(m_PZD5);
			m_PZD6 = htons(m_PZD6);
		}
		void NToH() {
			// In the case of word data (16 or 32 bits long) the high byte is transferred first (Motorola standard).
			m_PKE = ntohs(m_PKE);
			m_PWE = ntohl(m_PWE);
			m_PZD1 = ntohs(m_PZD1);
			m_PZD2 = ntohs(m_PZD2);
			m_PZD3 = ntohs(m_PZD3);
			m_PZD4 = ntohs(m_PZD4);
			m_PZD5 = ntohs(m_PZD5);
			m_PZD6 = ntohs(m_PZD6);
		}
	};
	USSPacket() {
		m_USSPacketStruct.SetDefault();
		GenerateChecksum();
	}
	USSPacketStruct m_USSPacketStruct;
	epicsUInt8 m_Bytes[USSPacketSize];
	void GenerateChecksum() {
		m_USSPacketStruct.m_BCC = Checksum();
	}
	bool ValidateChecksum() const {
		return (m_USSPacketStruct.m_BCC == Checksum());
	}
private:
	epicsUInt8 Checksum() const {
		// Recursive calculation:
		//	Checksum (I = 0) = byte (I = 0)
		//	Checksum (i) = checksum (i-1) XOR byte (i);
		//	i from 1 to 22, i = byte No.
		epicsUInt8 Checksum = 0;
		for (size_t i = 1; i < 22; i++)
			Checksum = Checksum ^ m_Bytes[i];
		return Checksum;
	}
};

#pragma pack(pop)

#endif //USSPacketStruct_H
