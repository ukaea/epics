#include "GaugeDAQ.h"

#include <asynOctetSyncIO.h>
#include <epicsGuard.h>

#ifdef _DEBUG
// Infinite timeout, convenient for debugging.
const double TimeOut = -1;
#else
const double TimeOut = 5;
#endif

GaugeDAQ::GaugeDAQ(asynUser* IOUser)
{
	m_IOUser = IOUser;
	static const size_t NrSegments = 30;
	static const int SegmentSizes[NrSegments] = {
		1333, 924, 706, 571, 480, 414, 364, 324, 293, 266, 245, 227, 210, 197, 184, 174, 165, 156, 148, 141, 135, 129, 123, 119, 110, 106, 102, 100, 96, 90
	};

	int Accumulation = 0;
	m_SegmentBoundaries.assign(NrSegments+1, SegmentBoundary());
	for(size_t Segment = 0; Segment < m_SegmentBoundaries.size(); Segment++)
	{
		m_SegmentBoundaries[Segment].m_RawPoint = Accumulation;
		Accumulation += SegmentSizes[Segment];
	}
	m_lastScanNumber = 0;
	connect();
}

void GaugeDAQ::connect()
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	ThrowException(pasynOctetSyncIO->flush(m_IOUser), __FUNCTION__, "flush");
	ThrowException(pasynOctetSyncIO->setOutputEos(m_IOUser, "\r", 1), __FUNCTION__, "setOutputEos");
	ThrowException(pasynOctetSyncIO->setInputEos(m_IOUser, "\r", 1), __FUNCTION__, "setInputEos");
	write("INST MSP");
	std::string ErrResponse;
	writeRead("SYST:ERR:ALL?", ErrResponse);
	std::string ReportResponse;
	writeRead("TEST:REPORT?", ReportResponse);
	epicsTime Time = epicsTime::getCurrent();
	char Date[11], TimeOfDay[9]; 
    Time.strftime ( Date, sizeof(Date), "%Y,%m,%d");
    Time.strftime ( TimeOfDay, sizeof(TimeOfDay), "%H,%M,%S");

	write("SYST:DATE " + std::string(Date));
	write("SYST:TIME " + std::string(TimeOfDay));
	write("STAT:OPER:CONT:NTR 32767");
	m_GaugeState = GetGaugeState();
	SetGaugeState(EnumGaugeState_OFF);
	write("INST FIL");
	write("SOUR:MODE ADJ");
	writeRead("SYST:ERR:ALL?", ErrResponse);
	GetLogicalInstrumentMinMaxVoltage(FILAMENTBIAS);
	GetLogicalInstrumentMinMaxVoltage(REPELLERBIAS);
	GetLogicalInstrumentMinMaxVoltage(ENTRYPLATE);
	GetLogicalInstrumentMinMaxVoltage(PRESSUREPLATE);
	GetLogicalInstrumentMinMaxVoltage(CUPS);
	GetLogicalInstrumentMinMaxVoltage(TRANSITION);
	GetLogicalInstrumentMinMaxVoltage(EXITPLATE);
	GetLogicalInstrumentMinMaxVoltage(EMSHIELD);
	GetLogicalInstrumentMinMaxVoltage(EMBIAS);
	GetLogicalInstrumentMinMaxVoltage(RFAMP);
	std::string AMUResponse;
	writeRead("CONF:AMU?", AMUResponse);
	// +9.28392E-01,+1.47596E+02
	size_t CommaPos = FindMarkerPos(AMUResponse, 0, ",");
	std::string lowerRange = AMUResponse.substr(0, CommaPos-1);
	FromString(lowerRange, m_lowerRange);
	std::string upperRange = AMUResponse.substr(CommaPos+1);
	FromString(upperRange, m_upperRange);
}

EnumGaugeState GaugeDAQ::GetGaugeState() const
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	write("INST MSP");
	std::string OUTPResponse, INITResponse;
	int OUTPValue = 0, INITValue = 0;
	writeRead("OUTP?", OUTPResponse);
	FromString(OUTPResponse, OUTPValue);
	writeRead("INIT:CONT?", INITResponse);
	FromString(INITResponse, INITValue);
	if ((OUTPValue == 0) && (INITValue == 0))
		return EnumGaugeState_OFF;
	return EnumGaugeState_SCAN;
}

void GaugeDAQ::SetGaugeState(EnumGaugeState gaugeState)
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	write("INST MSP");
	switch (gaugeState)
	{
	case EnumGaugeState_OFF :
		write ("OUTP OFF");
		break;
	case EnumGaugeState_STANDBY :
		write ("SOUR:MODE STANDBY");
		write ("OUTP ON");
		break;
	case EnumGaugeState_ON :
		write ("OUTP ON");
		break;
	case EnumGaugeState_SCAN :
		write ("OUTP ON");
		write ("FORM:ALL 1,0");
		write ("INIT:CONT ON");
		break;
	default: _ASSERT(false);
	}
}

void GaugeDAQ::GetLogicalInstrumentMinMaxVoltage(EnumLogicalInstruments logicalInstrumentEnum)
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	double Min, Max;
	write(EnumToText(logicalInstrumentEnum));
	std::string MaxResponse, MinResponse;
	writeRead("SOUR:VOLT? MAX", MaxResponse);
	FromString(MaxResponse, Max);
	writeRead("SOUR:VOLT? MIN", MinResponse);
	FromString(MinResponse, Min);
	m_InstrumentVoltages[logicalInstrumentEnum].m_Min = Min;
	m_InstrumentVoltages[logicalInstrumentEnum].m_Max = Max;
}

void GaugeDAQ::SetFilamentEmissionCurrentSetpoint(double value)
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	write("INST FIL");
	write("SOUR CURR " + ToString(value));
}

std::string GaugeDAQ::EnumToText(EnumLogicalInstruments logicalInstrumentEnum) const
{
	switch(logicalInstrumentEnum)
	{
		case FILAMENT : // duplicate?
		case FILAMENTBIAS : return "INST FIL"; break;
		case REPELLERBIAS : return "INST REP"; break;
		case ENTRYPLATE : return "INST ENTR"; break;
		case PRESSUREPLATE : return "INST PPL"; break;
		case CUPS : return "INST CUPS"; break;
		case TRANSITION : return "INST TPL"; break;
		case EXITPLATE : return "INST EXIT"; break;
		case EMSHIELD : return "INST EMSH"; break;
		case EMBIAS : return "INST EMUL"; break;
		case RFAMP : return "INST DDS"; break;
		default : _ASSERT(false);
	}
	return "";
}

double GaugeDAQ::GetLogicalInstrumentCurrentVoltage(EnumLogicalInstruments logicalInstrumentEnum) const
{
	double value = 0;
	switch (logicalInstrumentEnum)
	{
		case FILAMENT:		value = m_HeaderData.FilamentPower(); break;
		case FILAMENTBIAS:	value = m_HeaderData.FilamentBiasVoltage(); break;
		case REPELLERBIAS:	value = m_HeaderData.RepellerVoltage(); break;
		case ENTRYPLATE:	value = m_HeaderData.EntryPlateVoltage(); break;
		case PRESSUREPLATE:	value = m_HeaderData.PressurePlateVoltage(); break;
		case CUPS:			value = m_HeaderData.CupsVoltage(); break;
		case TRANSITION:	value = m_HeaderData.TransitionVoltage(); break;
		case EXITPLATE:		value = m_HeaderData.ExitPlateVoltage(); break;
		case EMSHIELD:		value = m_HeaderData.ElectronMultiplierShieldVoltage(); break;
		case EMBIAS:		value = m_HeaderData.ElectronMultiplierVoltage(); break;
		case RFAMP:			value = m_HeaderData.DDSAmplitude(); break;
		default: _ASSERT(false);
	}
	return value;
}

void GaugeDAQ::SetLogicalInstrumentVoltageSetpoint(EnumLogicalInstruments logicalInstrumentEnum, double& value)
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	write(EnumToText(logicalInstrumentEnum));
	write("SOUR:VOLT");
}

void GaugeDAQ::SetScanRange(double lowerRange, double upperRange)
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	write("CONF:AMU " + ToString(lowerRange) + "," + ToString(upperRange));
}

void GaugeDAQ::GetTSETingsValues(std::string const&  TSETingsValues)
{
	// enum EnumLogicalInstruments { FILAMENT = 0, FILAMENTBIAS, REPELLERBIAS, ENTRYPLATE, PRESSUREPLATE, CUPS, TRANSITION, EXITPLATE, EMSHIELD, EMBIAS, RFAMP };

	//								  RepellerBias  Emission    FilamentBias FilamentPower EntryPlate  PressurePlate Cups        Transition   ExitPlate     RFAmp       EMShield     EMBias      ElectrometerGain  no idea
	// (MEASurement=TSETtings (VALues -5.29913E+01,+7.50000E-05,+3.09385E+01,+2.83090E+00,+1.30018E+02,+7.50945E+01,+2.69785E+01,-6.85262E+02,+1.21151E+02,+4.50000E-01,+1.23000E+02,-8.58475E+02,+1.90069E-08,+9.99999E-10))
	size_t FirstCommaPos = 0, SecondCommaPos = FindMarkerPos(TSETingsValues, FirstCommaPos, ",");
	std::string RepellerBias = TSETingsValues.substr(FirstCommaPos, SecondCommaPos-FirstCommaPos-1);
	FromString(RepellerBias, m_HeaderData.m_RepellerVoltage);

	FirstCommaPos = SecondCommaPos; SecondCommaPos = FindMarkerPos(TSETingsValues, FirstCommaPos+1, ",");
	std::string EmissionCurrent = TSETingsValues.substr(FirstCommaPos, SecondCommaPos-FirstCommaPos-1);
	FromString(EmissionCurrent, m_HeaderData.m_EmissionCurrent);

	FirstCommaPos = SecondCommaPos; SecondCommaPos = FindMarkerPos(TSETingsValues, FirstCommaPos+1, ",");
	std::string FilamentBias = TSETingsValues.substr(FirstCommaPos, SecondCommaPos-FirstCommaPos-1);
	FromString(FilamentBias, m_HeaderData.m_FilamentBiasVoltage);

	// No idea (+2.83090E+00)
	FirstCommaPos = SecondCommaPos; SecondCommaPos = FindMarkerPos(TSETingsValues, FirstCommaPos+1, ",");
	std::string FilamentPower = TSETingsValues.substr(FirstCommaPos, SecondCommaPos-FirstCommaPos-1);
	FromString(FilamentPower, m_HeaderData.m_FilamentPower);

	FirstCommaPos = SecondCommaPos; SecondCommaPos = FindMarkerPos(TSETingsValues, FirstCommaPos+1, ",");
	std::string EntryPlate = TSETingsValues.substr(FirstCommaPos, SecondCommaPos-FirstCommaPos-1);
	FromString(EntryPlate, m_HeaderData.m_EntryPlateVoltage);

	FirstCommaPos = SecondCommaPos; SecondCommaPos = FindMarkerPos(TSETingsValues, FirstCommaPos+1, ",");
	std::string PressurePlate = TSETingsValues.substr(FirstCommaPos, SecondCommaPos-FirstCommaPos-1);
	FromString(PressurePlate, m_HeaderData.m_PressurePlateVoltage);

	FirstCommaPos = SecondCommaPos; SecondCommaPos = FindMarkerPos(TSETingsValues, FirstCommaPos+1, ",");
	std::string Cups = TSETingsValues.substr(FirstCommaPos, SecondCommaPos-FirstCommaPos-1);
	FromString(Cups, m_HeaderData.m_CupsVoltage);

	FirstCommaPos = SecondCommaPos; SecondCommaPos = FindMarkerPos(TSETingsValues, FirstCommaPos+1, ",");
	std::string Transition = TSETingsValues.substr(FirstCommaPos, SecondCommaPos-FirstCommaPos-1);
	FromString(Transition, m_HeaderData.m_TransitionVoltage);

	FirstCommaPos = SecondCommaPos; SecondCommaPos = FindMarkerPos(TSETingsValues, FirstCommaPos+1, ",");
	std::string ExitPlate = TSETingsValues.substr(FirstCommaPos, SecondCommaPos-FirstCommaPos-1);
	FromString(ExitPlate, m_HeaderData.m_ExitPlateVoltage);

	FirstCommaPos = SecondCommaPos; SecondCommaPos = FindMarkerPos(TSETingsValues, FirstCommaPos+1, ",");
	std::string RFAmp = TSETingsValues.substr(FirstCommaPos, SecondCommaPos-FirstCommaPos-1);
	FromString(RFAmp, m_HeaderData.m_DDSAmplitude);

	FirstCommaPos = SecondCommaPos; SecondCommaPos = FindMarkerPos(TSETingsValues, FirstCommaPos+1, ",");
	std::string EMShield = TSETingsValues.substr(FirstCommaPos, SecondCommaPos-FirstCommaPos-1);
	FromString(EMShield, m_HeaderData.m_ElectronMultiplierShieldVoltage);

	FirstCommaPos = SecondCommaPos; SecondCommaPos = FindMarkerPos(TSETingsValues, FirstCommaPos+1, ",");
	std::string EMBias = TSETingsValues.substr(FirstCommaPos, SecondCommaPos-FirstCommaPos-1);
	FromString(EMBias, m_HeaderData.m_ElectronMultiplierVoltage);

	FirstCommaPos = SecondCommaPos; SecondCommaPos = FindMarkerPos(TSETingsValues, FirstCommaPos+1, ",");
	std::string ElectrometetrGain = TSETingsValues.substr(FirstCommaPos, SecondCommaPos-FirstCommaPos-1);
	FromString(ElectrometetrGain, m_HeaderData.m_ElectronMultiplierElectrometerGain);
}

bool GaugeDAQ::GrabScanData()
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	std::string FirstHeader;
	write("fetc?");
	ThrowException(pasynOctetSyncIO->setInputEos(m_IOUser, "", 0), __FUNCTION__, "setInputEos");
	readTill(FirstHeader, "CURVe", 0);
	readTill(FirstHeader, "VALues #3", 6);

	size_t IDPos = FindMarkerPos(FirstHeader, 0, "(DIF (VERSion 1999.0) IDENtify", "(ID \"");
	size_t QuotePos = FindMarkerPos(FirstHeader, IDPos, "\"");
	m_HeaderData.m_MID = mbstowcs(FirstHeader.substr(IDPos, QuotePos-IDPos));

	size_t DESignPos = FindMarkerPos(FirstHeader, QuotePos, "DESign \"");

	// (DIF (VERSion 1999.0) IDENtify (DATE 2016,07,25 TIME 13,55,09.737 UUT (ID "835A0405"  DESign "0A,03.001.01301"))
	size_t CommaPos = FindMarkerPos(FirstHeader, DESignPos, ",");
	QuotePos = FindMarkerPos(FirstHeader, CommaPos, "\"");
	m_HeaderData.m_HardwareRevision = mbstowcs(FirstHeader.substr(DESignPos, CommaPos-DESignPos-1));
	m_HeaderData.m_FirmwareRevision = mbstowcs(FirstHeader.substr(CommaPos, QuotePos-CommaPos-1));

	// enum EnumLogicalInstruments { FILAMENT = 0, FILAMENTBIAS, REPELLERBIAS, ENTRYPLATE, PRESSUREPLATE, CUPS, TRANSITION, EXITPLATE, EMSHIELD, EMBIAS, RFAMP };
	//								  RepellerBias  Emission    FilamentBias   No idea     EntryPlate   PressurePlate Cups        Transition   ExitPlate     RFAmp       EMShield     no idea     no idea      no idea
	// (MEASurement=TSETtings (VALues -5.29913E+01,+7.50000E-05,+3.09385E+01,+2.83090E+00,+1.30018E+02,+7.50945E+01,+2.69785E+01,-6.85262E+02,+1.21151E+02,+4.50000E-01,+1.23000E+02,-8.58475E+02,+1.90069E-08,+9.99999E-10))
	size_t VALuesPos = FindMarkerPos(FirstHeader, QuotePos, "DATA (MEASurement=TSETtings (VALues ");
	size_t CloseBracketPos = FindMarkerPos(FirstHeader, VALuesPos, "))");
	std::string TSETingsValues = FirstHeader.substr(VALuesPos, CloseBracketPos-VALuesPos-2);
	GetTSETingsValues(TSETingsValues);

	size_t DIMNoisePos = FindMarkerPos(FirstHeader, CloseBracketPos, "DIMension=NOISe", "NSAMples #H");
	size_t SpacePos = FindMarkerPos(FirstHeader, DIMNoisePos, " ");
	std::string NoiseSamplesStr = FirstHeader.substr(DIMNoisePos, SpacePos-DIMNoisePos-1);
	int NoiseSamples;
	FromHexString(NoiseSamplesStr, NoiseSamples);

	// DIMension=NOISe (TYPE EXPLicit UNITs "COUNt") DATA (NSAMples #H0000017e BLINe +0.00000E+00 RMSNoise +0.00000E+00 CURVe (DATE 2016,08,02 TIME 09,23,10.659 VALues #3764   
	size_t  NoiseValuesPos = FindMarkerPos(FirstHeader, DIMNoisePos, "VALues #3");
	std::string NoiseVALuesStr = FirstHeader.substr(NoiseValuesPos);
	int NoiseValues;
	FromString(NoiseVALuesStr, NoiseValues);
	_ASSERT(NoiseValues==NoiseSamples*sizeof(epicsUInt16));

	m_NoisePacket.resize(NoiseSamples);
	read(m_NoisePacket);

	std::string SecondHeader;
	readTill(SecondHeader, "DIMension=COUNt (TYPE EXPLicit UNITs \"COUNt\") DATA (", 0);
	readTill(SecondHeader, "VALues #5", 6);

	// DIMension=COUNt (TYPE EXPLicit UNITs "COUNt") 
	//		DATA (TSOurce "IMMediate" OTCounter #H00000000 BOCounter #H00001d26 CVALue +6.13000E+05 BAMu +9.28392E-01 EAMu +1.22370E+02 BSEGment #H01 ESEGment #H12 NSAMples #H00001c41 BLINe +0.00000E+00 RMSNoise +0.00000E+00
	//			CURVe (DATE 2016,08,01 TIME 15,38,19.539 VALues #514466 

	size_t DIMDataPos = FindMarkerPos(SecondHeader, 0, "DIMension=COUNt");
	size_t CVALuePos = FindMarkerPos(SecondHeader, DIMDataPos, "CVALue ");
	SpacePos = FindMarkerPos(SecondHeader, CVALuePos, " "); 
	std::string CVALueStr = SecondHeader.substr(CVALuePos, SpacePos-CVALuePos-1);
	FromString(CVALueStr, m_HeaderData.m_MassAxisCalibrationFactor);
	m_HeaderData.m_MassAxisCalibrationFactor /= 1000;

	size_t NSAMplesPos = FindMarkerPos(SecondHeader, DIMDataPos, "NSAMples #H");
	SpacePos = FindMarkerPos(SecondHeader, NSAMplesPos, " ");
	std::string DataSamplesStr = SecondHeader.substr(NSAMplesPos, SpacePos-NSAMplesPos-1);
	int DataSamples;
	FromHexString(DataSamplesStr, DataSamples);

	size_t  DataValuesPos = FindMarkerPos(SecondHeader, DIMDataPos, "VALues #5");
	std::string DataVALuesStr = SecondHeader.substr(DataValuesPos);
	int DataValues;
	FromString(DataVALuesStr, DataValues);
	_ASSERT(DataValues==DataSamples*sizeof(epicsUInt16));

	m_DataPacket.resize(DataSamples);
	read(m_DataPacket);

	std::string DiscardedTerminator;
	readTill(DiscardedTerminator, " )))", 0);
	_ASSERT(DiscardedTerminator==" )))");
	for(size_t Segment = 0; Segment < m_SegmentBoundaries.size(); Segment++)
		m_SegmentBoundaries[Segment].m_ScaledPoint = m_lowerRange + Segment * (m_upperRange - m_lowerRange) / (m_SegmentBoundaries.size()-1);
	m_lastScanNumber++;
	return true;
}

int GaugeDAQ::FindRawPt(size_t& Segment, size_t ScaledPt) const
{
	while((Segment > 0) && (m_SegmentBoundaries[Segment].m_ScaledPoint > ScaledPt))
		Segment--;
	while((Segment < m_SegmentBoundaries.size()-1) && (m_SegmentBoundaries[Segment+1].m_ScaledPoint <= ScaledPt))
		Segment++;
	int RawPt = m_SegmentBoundaries[Segment].m_RawPoint;
	if (Segment < m_SegmentBoundaries.size()-1)
	{
		double ScaledRange = m_SegmentBoundaries[Segment+1].m_ScaledPoint - m_SegmentBoundaries[Segment].m_ScaledPoint;
		size_t RawRange = m_SegmentBoundaries[Segment+1].m_RawPoint - m_SegmentBoundaries[Segment].m_RawPoint;
		RawPt += size_t(RawRange * (ScaledPt - m_SegmentBoundaries[Segment].m_ScaledPoint) / ScaledRange);
	}
	return RawPt;
}

float GaugeDAQ::AverageNoise() const
{
	float AverageNoise = 0;
	for (size_t Sample = 0; Sample < m_NoisePacket.size(); Sample++)
		AverageNoise += m_NoisePacket[Sample];
	AverageNoise /= m_NoisePacket.size();
	return AverageNoise;
}

size_t GaugeDAQ::FindMarkerPos(std::string const& HeaderData, size_t Offset, const char* FirstMarker) const
{
	size_t FirstMarkerPos = HeaderData.find(FirstMarker, Offset);
	if (FirstMarkerPos != std::string::npos)
		FirstMarkerPos += strlen(FirstMarker);
	return FirstMarkerPos;
}

size_t GaugeDAQ::FindMarkerPos(std::string const& HeaderData, size_t Offset, const char* FirstMarker, const char* SecondMarker) const
{
	size_t FirstMarkerPos = FindMarkerPos(HeaderData, Offset, FirstMarker);
	size_t SecondMarkerPos = std::string::npos;
	if (FirstMarkerPos != std::string::npos)
	{
		FirstMarkerPos += strlen(FirstMarker);
		SecondMarkerPos = HeaderData.find(SecondMarker, FirstMarkerPos);
		if (SecondMarkerPos != std::string::npos)
			SecondMarkerPos += strlen(SecondMarker);
	}
	return SecondMarkerPos;
}

void GaugeDAQ::write(std::string const& WritePacket) const
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	size_t nBytesOut;
	// NB, *don't* pass pasynUser to this function - it has the wrong type and will cause an access violation.
	ThrowException(pasynOctetSyncIO->write(m_IOUser, WritePacket.c_str(), WritePacket.size(), TimeOut, &nBytesOut), __FUNCTION__, "write");
}

void GaugeDAQ::read(std::string& ReadPacket) const
{
	int eomReason = ASYN_EOM_CNT;
	char Buf[1024];
	size_t nBytesIn;
	while (eomReason == ASYN_EOM_CNT)
	{
		ThrowException(pasynOctetSyncIO->read(m_IOUser, Buf, sizeof(Buf), TimeOut, &nBytesIn, &eomReason), __FUNCTION__, "read");
		for(size_t Byte = 0; Byte < nBytesIn; Byte++)
			ReadPacket.push_back(Buf[Byte]);
	}
}

void GaugeDAQ::writeRead(std::string const& WritePacket, std::string& ReadPacket) const
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	// NB, *don't* pass pasynUser to this function - it has the wrong type and will cause an access violation.
	write(WritePacket);
	read(ReadPacket);
}

void GaugeDAQ::readTill(std::string& ReadPacket, std::string const& Termination, int AdditionalChars) const
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	size_t nBytesIn;
	int eomReason = ASYN_EOM_CNT;
	char Buf;
	// NB, *don't* pass pasynUser to this function - it has the wrong type and will cause an access violation.
	while ((ReadPacket.size() < Termination.size()) ||
		   (ReadPacket.substr(ReadPacket.size()-Termination.size(), Termination.size()) != Termination))
	{
		ThrowException(pasynOctetSyncIO->read(m_IOUser, &Buf, sizeof(Buf), TimeOut, &nBytesIn, &eomReason), __FUNCTION__, "read");
		ReadPacket += Buf;
	}
	for(int Additional = 0; Additional < AdditionalChars; Additional++)
	{
		ThrowException(pasynOctetSyncIO->read(m_IOUser, &Buf, sizeof(Buf), TimeOut, &nBytesIn, &eomReason), __FUNCTION__, "read");
		ReadPacket += Buf;
	}
}

void GaugeDAQ::read(std::vector<epicsUInt16>& ReadPacket) const
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	size_t nBytesIn;
	int eomReason = ASYN_EOM_CNT;
	// NB, *don't* pass pasynUser to this function - it has the wrong type and will cause an access violation.
	ThrowException(pasynOctetSyncIO->read(m_IOUser, reinterpret_cast<char*>(&(ReadPacket[0])), ReadPacket.size()*sizeof(epicsUInt16), TimeOut, &nBytesIn, &eomReason), __FUNCTION__, "read");
	_ASSERT(eomReason == ASYN_EOM_CNT);
}

int GaugeDAQ::CheckExtraData()
{
	size_t ExtraData = 0;
	size_t nBytesIn;
	int eomReason = ASYN_EOM_CNT;
	char Buf;
	do
	{
		asynStatus Status = pasynOctetSyncIO->read(m_IOUser, &Buf, sizeof(Buf), 0.01, &nBytesIn, &eomReason);
		ExtraData += nBytesIn;
	}
	while (eomReason == ASYN_EOM_CNT);
	_ASSERT(ExtraData == 0);
	return ExtraData;
}

GaugeDAQ::CException::CException(asynUser* AsynUser, asynStatus Status, const char* functionName, std::string const& what) :
	std::runtime_error(what) 
{
	std::string message = "%s:%s ERROR: " + what + "%s\n";
	asynPrint(AsynUser, ASYN_TRACE_ERROR, message.c_str(), __FILE__, functionName, AsynUser->errorMessage);
	m_Status = Status;
}

void GaugeDAQ::ThrowException(asynStatus Status, const char* Function, std::string const& what) const
{
	if (Status == asynSuccess)
		return;
	throw CException(m_IOUser, Status, Function, what);
}
