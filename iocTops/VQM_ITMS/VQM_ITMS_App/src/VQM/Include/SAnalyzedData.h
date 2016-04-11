#pragma once

#include "IServiceWrapper.h"
#include <vector>

/*
Analyzed data structure
*/
struct SAnalyzedData
{
private:
    std::vector<double> m_FittedGases;
    std::vector<double> m_Residuals;
    std::vector<double> m_MassAxis;
    std::vector<double> m_DenoisedRawData;
    std::vector<double> m_PartialPressure;
    std::vector<double> m_PeakGoodnessOfFit;
    std::vector<double> m_PeakFWHM;
    std::vector<double> m_PeakResolution;
    std::vector<double> m_PeakAmplitude;
    std::vector<double> m_PeakCenter;
    double m_RMSScaledNoise;
	double m_TotalArea;
    double m_HighMassArea;
    double m_NoiseBaseline;

public:
	SAnalyzedData()
	{
		m_RMSScaledNoise = 0;
		m_TotalArea = 0;
		m_HighMassArea = 0;
		m_NoiseBaseline = 0;
	}

	~SAnalyzedData()
	{
	}

public:
	std::vector<double>& FittedGases() { return m_FittedGases; }
	std::vector<double>& Residuals() { return m_Residuals; }
	std::vector<double>& MassAxis() { return m_MassAxis; }
	std::vector<double>& DenoisedRawData() { return m_DenoisedRawData; }
	std::vector<double>& PeakArea() { return m_PartialPressure; }
	std::vector<double>& PeakGoodnessOfFit() { return m_PeakGoodnessOfFit; }
	std::vector<double>& PeakFWHM() { return m_PeakFWHM; }
	std::vector<double>& PeakResolution() { return m_PeakResolution; }
	std::vector<double>& PeakAmplitude() { return m_PeakAmplitude; }
	std::vector<double>& PeakCenter() { return m_PeakCenter; }
	double RMSScaledNoise()	{ return m_RMSScaledNoise; }
	double TotalArea()	{ return m_TotalArea; }
	double HighMassArea()	{ return m_HighMassArea; }
	double NoiseBaseline() { return m_NoiseBaseline; }

	void SetValues(double RMSScaledNoise, double totalArea, double highMassArea, double noiseBaseline)
	{
		m_RMSScaledNoise = RMSScaledNoise;
		m_TotalArea = totalArea;
		m_HighMassArea = highMassArea;
		m_NoiseBaseline = noiseBaseline;
	}
};