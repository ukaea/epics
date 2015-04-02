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
//		It is templated because there are two different packet sizes in use:						//
//			The rear port of the MAG.DRIVE Digital controller uses a 16-byte packet (NoOfPZD == 2).	//
//			The front port of the MAG.DRIVE Digital controller uses a 24-byte packet (NoOfPZD == 6).//
//			Older controllers use a 24-byte packet (NoOfPZD == 6).									//
//																									//
//	Author:  Peter Heesterman (Tessella plc). Date: 05 Jan 2015.									//
//	Written for CCFE (Culham Centre for Fusion Energy).												//
//																									//
//	LeyboldTurbo is distributed subject to a Software License Agreement								//
//	found in file LICENSE that is included with this distribution.									//
//																									//
//////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef USSPacketStruct_H
#define USSPacketStruct_H

#include <epicsTypes.h>
#include <osiSock.h>

#pragma pack(push, 1)

template<size_t NoOfPZD> union USSPacket
{
	static const size_t USSPacketSize = NoOfPZD * sizeof(epicsUInt16) + 12;

	template<size_t NoOfPZD> struct USSPacketStruct
	{
		void SetDefault(bool Running = true, int Parameter = 0) {
			m_STX = 2;			// Start byte 2
			m_ADR = 0;
			m_PKE = 0;
			if (Parameter != 0) 
				m_PKE = ((1 << 12) | Parameter);
			m_IND = 0;
			m_PWE = 0;
			m_LGE = USSPacketSize-2;			// TYPE 1: 4 / 2 words (12 Bytes) LGE = 14 Bytes
			m_BCC = 0;
			m_PZD[0] = (Running) ? (1 << 0) : 0;// Pump is normally running.
			for(size_t PZD = 1; PZD < NoOfPZD; PZD++)
				m_PZD[PZD] = 0;
		}
		epicsUInt8  m_STX;		// Start byte 2
		epicsUInt8  m_LGE;		// TYPE 1: 4 / 2 words (12 Bytes) LGE = 14 Bytes
		epicsUInt8  m_ADR;		// Frequency converter address RS232: 0. RS485: 0...15
		epicsUInt16 m_PKE;		// Parameter number and type of access Value (s. 2.1)
		epicsUInt16	m_IND;		// Parameter index Value (s. 2.1)
		epicsUInt32 m_PWE;		// Parameter value Value
		epicsUInt16	m_PZD[NoOfPZD];		// Status and control bits Value (see 2.2)
		epicsUInt8	m_BCC;		// Recursive calculation:
		void HToN() {
			// In the case of word data (16 or 32 bits long) the high byte is transferred first (Motorola standard).
			m_PKE = htons(m_PKE);
			m_IND = htons(m_IND);
			m_PWE = htonl(m_PWE);
			for(size_t PZD = 0; PZD < NoOfPZD; PZD++)
				m_PZD[PZD] = htons(m_PZD[PZD]);
		}
		void NToH() {
			// In the case of word data (16 or 32 bits long) the high byte is transferred first (Motorola standard).
			m_PKE = ntohs(m_PKE);
			m_IND = ntohs(m_IND);
			m_PWE = ntohl(m_PWE);
			for(size_t PZD = 0; PZD < NoOfPZD; PZD++)
				m_PZD[PZD] = ntohs(m_PZD[PZD]);
		}
	};
	USSPacket(bool Running = true, int Parameter = 0) {
		m_USSPacketStruct.SetDefault(Running, Parameter);
		GenerateChecksum();
	}
	USSPacketStruct<NoOfPZD> m_USSPacketStruct;
	epicsUInt8 m_Bytes[USSPacketSize];
	void GenerateChecksum() {
		m_USSPacketStruct.m_BCC = Checksum();
	}
	bool ValidateChecksum() const {
		return (m_USSPacketStruct.m_BCC == Checksum());
	}
private:
	epicsUInt8 Checksum() const {
		//	Recursive calculation:
		//	Checksum (I = 0) = byte (I = 0)
		//	Checksum (i) = checksum (i-1) XOR byte (i);
		//	i from 1 to 22, i = byte No.
		epicsUInt8 Checksum = 0;
		for (size_t i = 0; i < USSPacketSize-1; i++)
			Checksum = Checksum ^ m_Bytes[i];
		return Checksum;
	}
};

#pragma pack(pop)

#endif //USSPacketStruct_H
