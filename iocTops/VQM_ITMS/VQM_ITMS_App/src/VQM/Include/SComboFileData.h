#pragma once

#include "IServiceWrapper.h"
#include <vector>

/*
Maps to the SComboFileGasData class.
*/
struct SComboFileGasData
{
private:
	STRING_TYPE m_gasName;
    std::vector<double> m_fittedAmuValues;
    std::vector<double> m_residualAmuValues;
    std::vector<double> m_fittedGasesValues;
    std::vector<double> m_residualsValues;

public:
    SComboFileGasData()
    {
    }

    ~SComboFileGasData()
    {
    }

public:
	//Getters
	STRING_TYPE GasName()
	{
		return m_gasName;
	}
	std::vector<double>& FittedAmuValues()
	{
		return m_fittedAmuValues;
	}
	std::vector<double>& ResidualAmuValues()
	{
		return m_residualAmuValues;
	}
	std::vector<double>& FittedGases()
	{
		return m_fittedGasesValues;
	}
	std::vector<double>& Residuals()
	{
		return m_residualsValues;
	}
	void SetValues(STRING_TYPE gasName)
	{
		m_gasName = gasName;
	}
};

/*
Maps to the data in the combo files used for gas fitting.
*/
struct SComboFileData
{
private:
	std::vector<SComboFileGasData> m_List;
	
public:
	SComboFileData()
	{
	}

	~SComboFileData()
	{
	}

public:
	std::vector<SComboFileGasData>& List() { return m_List; }
};