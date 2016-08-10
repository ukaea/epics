#include "GaugeDAQ.h"

#include <asynOctetSyncIO.h>
#include <epicsGuard.h>
#include <epicsTime.h>

#ifdef _DEBUG
// Infinite timeout, convenient for debugging.
const double TimeOut = -1;
#else
const double TimeOut = 5;
#endif

const size_t GaugeDAQ::MaxNrSegments = 30;
size_t GaugeDAQ::SegmentSizes[GaugeDAQ::MaxNrSegments] = {
//	#H0535,#H039c,#H02c2,#H023b,#H01e0,#H019e,#H016c,#H0144,#H0125,#H010a,#H00f5,#H00e3,#H00d2,#H00c5,#H00b8,#H00ae,#H00a5,#H009c,#H0094,#H008d,#H0087,#H0081,#H007b,#H0077,#H0073,#H006e,#H006a,#H0066,#H0064,#H0060
	1333,     924,   706,   571,   480,   414,   364,   324,   293,   266,   245,   227,   210,   197,   184,   174,   165,   156,   148,   141,   135,   129,   123,   119,   115,   110,   106,   102,   100,    96
};

GaugeDAQ::GaugeDAQ(asynUser* IOUser)
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	m_IOUser = IOUser;
	m_lastScanNumber = 0;
	m_RawData.resize(1);

	m_AvgMode = Accumulator;
	m_numAverages = 1; 
	m_lastScanNumber = 0;
	m_GaugeState = EnumGaugeState_OFF;
	m_lowerRange = 0;
	m_upperRange = 0;
	m_EmissionCurrent = 0;

	connect();
}

void GaugeDAQ::connect()
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	ThrowException(pasynOctetSyncIO->setInputEos(m_IOUser, "", 0), __FUNCTION__, "setInputEos");
	Flush();
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
	SetGaugeState(EnumGaugeState_OFF);
	m_GaugeState = GetGaugeState();
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
	GetScanRange();
	write("INST MSP;:FORM:ALL 0,1");
	GrabScanData();
	SetGaugeState(EnumGaugeState_SCAN);
	m_GaugeState = GetGaugeState();
}

bool GaugeDAQ::GetExtendedRangeCapabilities() const
{
	// TODO
	return false;
}

EnumGaugeState GaugeDAQ::GetGaugeState() const
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	write("INST MSP");
	std::string OUTPResponse, INITResponse;
	size_t OUTPValue = 0, INITValue = 0;
	writeRead("OUTP?", OUTPResponse);
	FromString(OUTPResponse, OUTPValue);
	writeRead("INIT:CONT?", INITResponse);
	FromString(INITResponse, INITValue);
	if ((OUTPValue == 1) && (INITValue == 0))
		return EnumGaugeState_STANDBY;
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

void GaugeDAQ::DataAnalysisSetAvgMode(EnumAvgMode AvgMode)
{
	while (m_AvgMode == Running_Avg)
		m_RawData.resize(1);
	m_AvgMode = AvgMode;
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

void GaugeDAQ::SetElectrometerGainSetpoint(double value)
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	// TODO
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

void GaugeDAQ::GetScanRange()
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	std::string AMUResponse;
	writeRead("CONF:AMU?", AMUResponse);
	// +9.28392E-01,+1.47596E+02
	size_t CommaPos = FindMarkerPos(AMUResponse, 0, ",");
	std::string lowerRange = AMUResponse.substr(0, CommaPos-1);
	FromString(lowerRange, m_lowerRange);
	std::string upperRange = AMUResponse.substr(CommaPos+1);
	FromString(upperRange, m_upperRange);
}

void GaugeDAQ::SetScanRange(double lowerRange, double upperRange)
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	std::string ErrResponse;
	writeRead("CONF:AMU " + ToString(lowerRange) + "," + ToString(upperRange), ErrResponse);
	GetScanRange();
}

void GaugeDAQ::GetIDENtifyValues(std::string const& IDENtify)
{
	// (DIF (VERSion 1999.0) IDENtify (DATE 2016,07,25 TIME 13,55,09.737 UUT (ID "835A0405"  DESign "0A,03.001.01301"))
	size_t IDPos = FindMarkerPos(IDENtify, 0, "ID \"");
	size_t QuotePos = FindMarkerPos(IDENtify, IDPos, "\"");
	m_HeaderData.m_MID = mbstowcs(IDENtify.substr(IDPos, QuotePos-IDPos-1));

	size_t DESignPos = FindMarkerPos(IDENtify, IDPos, "DESign \"");

	size_t CommaPos = FindMarkerPos(IDENtify, DESignPos, ",");
	QuotePos = FindMarkerPos(IDENtify, CommaPos, "\"");
	m_HeaderData.m_HardwareRevision = mbstowcs(IDENtify.substr(DESignPos, CommaPos-DESignPos-1));
	m_HeaderData.m_FirmwareRevision = mbstowcs(IDENtify.substr(CommaPos, QuotePos-CommaPos-1));
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

void GaugeDAQ::GetSEGMentValues(std::string const&  MEASurementSEGMentValues)
{
	// MEASurement=SEGMent (VALues #H0535,#H039c,#H02c2,#H023b,#H01e0,#H019e,#H016c,#H0144,#H0125,#H010a,#H00f5,#H00e3,#H00d2,#H00c5,#H00b8,#H00ae,#H00a5,#H009c,#H0094,#H008d,#H0087,#H0081,#H007b,#H0077,#H0073,#H006e,#H006a,#H0066,#H0064,#H0060)
	size_t CommaPos = 0;
	for(size_t Segment = 0; Segment < GaugeDAQ::MaxNrSegments; Segment++)
	{
		size_t HashHPos = MEASurementSEGMentValues.find("#H", CommaPos);
		CommaPos = MEASurementSEGMentValues.find(',', HashHPos);
		std::string SegmentValue = MEASurementSEGMentValues.substr(HashHPos+2, CommaPos-HashHPos-2);
		FromHexString(SegmentValue, SegmentSizes[Segment]);
	}
}

size_t GaugeDAQ::GetDATAAMUValues(std::string const&  DATAAMUValues)
{
	size_t SpacePos = FindMarkerPos(DATAAMUValues, 0, " "); 
	std::string CVALueStr = DATAAMUValues.substr(0, SpacePos-1);
	FromString(CVALueStr, m_HeaderData.m_MassAxisCalibrationFactor);
	m_HeaderData.m_MassAxisCalibrationFactor /= 1000;

	size_t BAMuPos = FindMarkerPos(DATAAMUValues, SpacePos, "BAMu ");
	SpacePos = FindMarkerPos(DATAAMUValues, BAMuPos, " "); 
	std::string BAMuStr = DATAAMUValues.substr(BAMuPos, SpacePos-BAMuPos-1);
	FromString(BAMuStr, m_lowerRange);
	size_t EAMuPos = FindMarkerPos(DATAAMUValues, SpacePos, "EAMu ");
	SpacePos = FindMarkerPos(DATAAMUValues, EAMuPos, " "); 
	std::string EAMuStr = DATAAMUValues.substr(EAMuPos, SpacePos-EAMuPos-1);
	FromString(EAMuStr, m_upperRange);

	size_t NSAMplesPos = FindMarkerPos(DATAAMUValues, SpacePos, "NSAMples #H");
	SpacePos = FindMarkerPos(DATAAMUValues, NSAMplesPos, " ");
	std::string DataSamplesStr = DATAAMUValues.substr(NSAMplesPos, SpacePos-NSAMplesPos-1);
	size_t DataSamples;
	FromHexString(DataSamplesStr, DataSamples);
	return DataSamples;
}

void GaugeDAQ::GrabScanData()
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	std::string FirstHeader;
	write("fetc?");
	ThrowException(pasynOctetSyncIO->setInputEos(m_IOUser, "", 0), __FUNCTION__, "setInputEos");
	readTill(FirstHeader, "CURVe", 0);
	readTill(FirstHeader, "VALues #", 7);

	// (DIF (VERSion 1999.0) IDENtify (DATE 2016,07,25 TIME 13,55,05.321 UUT (ID "835A0405"  DESign "0A,03.001.01301")
	size_t IDENtifyPos = FindMarkerPos(FirstHeader, 0, "DIF (VERSion 1999.0) IDENtify (");
	size_t CloseBracketPos = FindMarkerPos(FirstHeader, IDENtifyPos, "))");
	std::string IDENtify = FirstHeader.substr(IDENtifyPos, CloseBracketPos-IDENtifyPos-2);
	GetIDENtifyValues(IDENtify);

	// (MEASurement=TSETtings (VALues -5.29913E+01,+7.50000E-05,+3.09385E+01,+2.83090E+00,+1.30018E+02,+7.50945E+01,+2.69785E+01,-6.85262E+02,+1.21151E+02,+4.50000E-01,+1.23000E+02,-8.58475E+02,+1.90069E-08,+9.99999E-10))
	size_t MEASurementTSETtingsPos = FindMarkerPos(FirstHeader, CloseBracketPos, "DATA (MEASurement=TSETtings (VALues ");
	CloseBracketPos = FindMarkerPos(FirstHeader, MEASurementTSETtingsPos, "))");
	std::string MEASurementTSETtingsValues = FirstHeader.substr(MEASurementTSETtingsPos, CloseBracketPos-MEASurementTSETtingsPos-2);
	GetTSETingsValues(MEASurementTSETtingsValues);

	if (m_GaugeState == EnumGaugeState_OFF)
	{
		// DATA (CVALue +6.13000E+05 BAMu +9.28392E-01 EAMu +1.22370E+02 BSEGment #H01 ESEGment #H12 NSAMples #H00001c41 
		size_t DataCVALuePos = FindMarkerPos(FirstHeader, CloseBracketPos, "DATA (CVALue ");
		size_t MEASurementSEGMentPos = FindMarkerPos(FirstHeader, DataCVALuePos, "MEASurement=SEGMent (VALues ");

		std::string DataAMUValues = FirstHeader.substr(DataCVALuePos, MEASurementSEGMentPos-DataCVALuePos-strlen("MEASurement=SEGMent (VALues ")-1);
		size_t DataSamples = GetDATAAMUValues(DataAMUValues);

		// MEASurement=SEGMent (VALues #H0535,#H039c,#H02c2,#H023b,#H01e0,#H019e,#H016c,#H0144,#H0125,#H010a,#H00f5,#H00e3,#H00d2,#H00c5,#H00b8,#H00ae,#H00a5,#H009c,#H0094,#H008d,#H0087,#H0081,#H007b,#H0077,#H0073,#H006e,#H006a,#H0066,#H0064,#H0060)
		CloseBracketPos = FindMarkerPos(FirstHeader, MEASurementSEGMentPos, ")");
		std::string MEASurementSEGMentValues = FirstHeader.substr(MEASurementSEGMentPos, CloseBracketPos-MEASurementSEGMentPos-1);
		GetSEGMentValues(MEASurementSEGMentValues);

		size_t  DataValuesPos = FindMarkerPos(FirstHeader, MEASurementSEGMentPos, "VALues #5");
		std::string DataVALuesStr = FirstHeader.substr(DataValuesPos);
		size_t DataValues;
		FromString(DataVALuesStr, DataValues);
		_ASSERT(DataValues==DataSamples*sizeof(epicsFloat32));
		m_ScanVector.resize(DataSamples);
		read(m_ScanVector);
		FILE* CSVFile=fopen("ScanVector.csv", "wt");
		for (size_t Sample=0;Sample<m_ScanVector.size();Sample++)
			fprintf(CSVFile, "%f\n", m_ScanVector[Sample]);
		fclose(CSVFile);
		_ASSERT(fabs(m_ScanVector.front()-m_lowerRange) < 0.001);
		_ASSERT(fabs(m_ScanVector.back()-m_upperRange) < 0.01);
	}
	else
	{
		std::string SecondHeader;
		size_t DIMNoisePos = FindMarkerPos(FirstHeader, CloseBracketPos, "DIMension=NOISe");
		size_t NSAMplesPos = FindMarkerPos(FirstHeader, DIMNoisePos, "NSAMples #H");
		size_t SpacePos = FindMarkerPos(FirstHeader, NSAMplesPos, " ");
		std::string NoiseSamplesStr = FirstHeader.substr(NSAMplesPos, SpacePos-NSAMplesPos-1);
		size_t NoiseSamples;
		FromHexString(NoiseSamplesStr, NoiseSamples);

		// DIMension=NOISe (TYPE EXPLicit UNITs "COUNt") DATA (NSAMples #H0000017e BLINe +0.00000E+00 RMSNoise +0.00000E+00 CURVe (DATE 2016,08,02 TIME 09,23,10.659 VALues #3764   
		size_t  NoiseValuesPos = FindMarkerPos(FirstHeader, DIMNoisePos, "VALues #3");
		std::string NoiseVALuesStr = FirstHeader.substr(NoiseValuesPos);
		size_t NoiseValues;
		FromString(NoiseVALuesStr, NoiseValues);
		_ASSERT(NoiseValues==NoiseSamples*sizeof(epicsUInt16));

		m_NoiseData.resize(NoiseSamples);
		read(m_NoiseData);

		// DIMension=COUNt (TYPE EXPLicit UNITs "COUNt") 
		//		DATA (TSOurce "IMMediate" OTCounter #H00000000 BOCounter #H00001d26 CVALue +6.13000E+05 BAMu +9.28392E-01 EAMu +1.22370E+02 BSEGment #H01 ESEGment #H12 NSAMples #H00001c41 BLINe +0.00000E+00 RMSNoise +0.00000E+00
		//			CURVe (DATE 2016,08,01 TIME 15,38,19.539 VALues #514466 
		readTill(SecondHeader, "DIMension=COUNt (TYPE EXPLicit UNITs \"COUNt\") DATA (", 0);
		readTill(SecondHeader, "VALues #5", 6);
		size_t DimensionCOUNtPos = FindMarkerPos(SecondHeader, 0, "DIMension=COUNt");

		size_t DataCVALuePos = FindMarkerPos(SecondHeader, DimensionCOUNtPos, "CVALue ");
		size_t BSEGmentPos = FindMarkerPos(SecondHeader, DataCVALuePos, " CURVe (");
		std::string DataAMUValues = SecondHeader.substr(DataCVALuePos, BSEGmentPos-DataCVALuePos-strlen(" CURVe ("));
		size_t DataSamples = GetDATAAMUValues(DataAMUValues);

		size_t  DataValuesPos = FindMarkerPos(SecondHeader, DimensionCOUNtPos, "VALues #5");
		std::string DataVALuesStr = SecondHeader.substr(DataValuesPos);
		size_t DataValues;
		FromString(DataVALuesStr, DataValues);
		_ASSERT(DataValues==DataSamples*sizeof(epicsUInt16));
		while (m_AvgMode == Running_Avg)
		{
			while (m_RawData.size() >= m_numAverages)
				m_RawData.pop_front();
			if (m_RawData.size() < m_numAverages)
				m_RawData.push_back(std::vector<epicsUInt16>(DataSamples));
		}
		m_RawData.back().resize(DataSamples);
		read(m_RawData.back());
	}


	std::string DiscardedTerminator;
	readTill(DiscardedTerminator, " )))", 0);
	_ASSERT(DiscardedTerminator==" )))");
	ThrowException(pasynOctetSyncIO->setInputEos(m_IOUser, "\r", 1), __FUNCTION__, "setInputEos");
#ifdef _DEBUG
//	size_t ExtraData = CheckExtraData(IOUser);
//	_ASSERT(ExtraData == 0);
#endif
	m_lastScanNumber++;
}

size_t GaugeDAQ::FindRawPt(size_t& RawPt, double ScaledPt) const
{
	while ((RawPt+1 < m_ScanVector.size()) && (m_ScanVector[RawPt+1] < ScaledPt))
		RawPt++;
	while ((RawPt > 0) && (m_ScanVector[RawPt] > ScaledPt))
		RawPt--;
	return RawPt;
}

float GaugeDAQ::AverageNoise() const
{
	float AverageNoise = 0;
	for (size_t Sample = 0; Sample < m_NoiseData.size(); Sample++)
		AverageNoise += m_NoiseData[Sample];
	AverageNoise /= m_NoiseData.size();
	return AverageNoise;
}

size_t GaugeDAQ::FindMarkerPos(std::string const& HeaderData, size_t Offset, const char* Marker) const
{
	size_t MarkerPos = HeaderData.find(Marker, Offset);
	if (MarkerPos != std::string::npos)
		MarkerPos += strlen(Marker);
	return MarkerPos;
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
	epicsTime StartTime = epicsTime::getCurrent ();
	while (eomReason != ASYN_EOM_EOS)
	{
		asynStatus Status = pasynOctetSyncIO->read(m_IOUser, Buf, sizeof(Buf), TimeOut, &nBytesIn, &eomReason);
		if (Status == asynTimeout)
		{
			if ((TimeOut != -1) && (epicsTime::getCurrent() - StartTime > TimeOut))
				ThrowException(Status, __FUNCTION__, "read");
		}
		else
			ThrowException(Status, __FUNCTION__, "read");
		for(size_t Byte = 0; Byte < nBytesIn; Byte++)
			ReadPacket.push_back(Buf[Byte]);
	}
	_ASSERT(eomReason == ASYN_EOM_EOS);
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
	epicsTime StartTime = epicsTime::getCurrent ();
	// NB, *don't* pass pasynUser to this function - it has the wrong type and will cause an access violation.
	while ((ReadPacket.size() < Termination.size()) ||
		   (ReadPacket.substr(ReadPacket.size()-Termination.size(), Termination.size()) != Termination))
	{
		asynStatus Status = pasynOctetSyncIO->read(m_IOUser, &Buf, sizeof(Buf), TimeOut, &nBytesIn, &eomReason);
		if (Status == asynTimeout)
		{
			if ((TimeOut != -1) && (epicsTime::getCurrent() - StartTime > TimeOut))
				ThrowException(Status, __FUNCTION__, "read");
		}
		else
			ThrowException(Status, __FUNCTION__, "read");
		if (nBytesIn != 0)
			ReadPacket += Buf;
	}
	for(int Additional = 0; Additional < AdditionalChars; Additional++)
	{
		ThrowException(pasynOctetSyncIO->read(m_IOUser, &Buf, sizeof(Buf), TimeOut, &nBytesIn, &eomReason), __FUNCTION__, "read");
		ReadPacket += Buf;
	}
}

template<class T> void GaugeDAQ::read(std::vector<T>& ReadPacket) const
{
	epicsGuard < epicsMutex > guard (m_Mutex);
	size_t nTotalBytes = 0;
	int eomReason = ASYN_EOM_CNT;
	asynStatus Status = asynTimeout;
	epicsTime StartTime = epicsTime::getCurrent ();
	while (nTotalBytes < ReadPacket.size()*sizeof(T))
	{
		size_t nBytesIn;
		Status = pasynOctetSyncIO->read(m_IOUser, reinterpret_cast<char*>(&(ReadPacket[0]))+nTotalBytes, ReadPacket.size()*sizeof(T)-nTotalBytes, 0.1, &nBytesIn, &eomReason);
		nTotalBytes += nBytesIn; 
		if (Status == asynTimeout)
		{
			if ((TimeOut != -1) && (epicsTime::getCurrent() - StartTime > TimeOut) && (nTotalBytes < ReadPacket.size()*sizeof(T)))
				ThrowException(Status, __FUNCTION__, "read");
			write("fetc?");
		}
		else
			ThrowException(Status, __FUNCTION__, "read");
	}
	_ASSERT(eomReason == ASYN_EOM_CNT);
}

int GaugeDAQ::Flush()
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
	while (nBytesIn > 0);
	pasynOctetSyncIO->flush(m_IOUser);
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
