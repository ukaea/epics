#pragma once

#include <vector>

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
    EnumPressureSource m_PressureSourceInput;
    std::vector<double> m_AveragedADCOutput;
    EnumAvgMode m_AveragingModeInput;
	
public:
	SAverageData()
	{
		m_ValidScanDataIn = false;
		m_BuffersAveraged = 0;
		m_AvgTotalPres = 0;
		m_InstTotalPres = 0;
		m_PressureSourceInput = EnumPressureSource_None;
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
	EnumPressureSource PressureSourceInput() { return m_PressureSourceInput; }
	EnumAvgMode AveragingModeInput() { return m_AveragingModeInput; }

	void SetValues(bool validScanDataIn, int buffersAveraged, double avgTotalPres, double instTotalPres, 
					EnumPressureSource pressureSourceInput, EnumAvgMode avgMode)
	{
		m_ValidScanDataIn = validScanDataIn;
		m_BuffersAveraged = buffersAveraged;
		m_AvgTotalPres = avgTotalPres;
		m_InstTotalPres = instTotalPres;
		m_PressureSourceInput = pressureSourceInput;
		m_AveragingModeInput = avgMode;
	}
};