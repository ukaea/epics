#pragma once

#include <vector>

////////////////////////////////////////////////////////////////////////////////
// #defines

//#ifdef _UNICODE
//.Net service returns unicode characters
#define STRING_TYPE std::wstring
#define CHAR_TYPE wchar_t

////////////////////////////////////////////////////////////////////////////////

/*
Average data structure
*/
struct SAverageData
{
private:
	bool m_ValidScanDataIn;
	int m_BuffersAveraged;
	double m_AvgTotalPres;
	double m_InstTotalPres;
	STRING_TYPE m_PressureSourceInput;  //
	//EnumPressureSource m_PressureSourceInput;
	std::vector<double> m_AveragedADCOutput;
	EnumAvgMode m_AveragingModeInput;
	
public:
	SAverageData()
	{
		m_ValidScanDataIn = false;
		m_BuffersAveraged = 0;
		m_AvgTotalPres = 0;
		m_InstTotalPres = 0;
		STRING_TYPE m_PressureSourceInput = (L"None");//EnumPressureSource_None;  //Pressure source string from Viewer Application list 
		m_AveragingModeInput = Off;
	}

	~SAverageData()
	{
	}

public:
	std::vector<double>& AveragedADCOutput() { return m_AveragedADCOutput; }
	bool ValidScanDataIn() { return m_ValidScanDataIn; }
	int BuffersAverage() { return m_BuffersAveraged; }
	double AvgTotalPres() { return m_AvgTotalPres; }
	double InstTotalPres() { return m_InstTotalPres; }
	//EnumPressureSource PressureSourceInput() { return m_PressureSourceInput; }
	STRING_TYPE PressureSourceInput() { return m_PressureSourceInput; }  //ryn
	EnumAvgMode AveragingModeInput() { return m_AveragingModeInput; }

	void SetValues(bool validScanDataIn, int buffersAveraged, double avgTotalPres, double instTotalPres, 
					STRING_TYPE pressureSourceInput, EnumAvgMode avgMode)  
	{
		m_ValidScanDataIn = validScanDataIn;
		m_BuffersAveraged = buffersAveraged;
		m_AvgTotalPres = avgTotalPres;
		m_InstTotalPres = instTotalPres;
		m_PressureSourceInput = pressureSourceInput;
		m_AveragingModeInput = avgMode;
	}
};