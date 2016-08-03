#ifndef IHEADERDATA_H_INCLUDED
#define IHEADERDATA_H_INCLUDED

#include <string>

struct IHeaderData
{
	friend class GaugeDAQ;
public:
	double EmissionCurrent() const {
		return m_EmissionCurrent;
	}
	double FilamentBiasVoltage() const {
		return m_FilamentBiasVoltage;
	}
    double FilamentPower() const {
		return m_FilamentPower;
	}
	double RepellerVoltage() const {
		return m_RepellerVoltage;
	}
	double EntryPlateVoltage() const {
		return m_EntryPlateVoltage;
	}
	double PressurePlateVoltage() const {
		return m_PressurePlateVoltage;
	}
	double CupsVoltage() const {
		return m_CupsVoltage;
	}
	double TransitionVoltage() const {
		return m_TransitionVoltage;
	}
	double ExitPlateVoltage() const {
		return m_ExitPlateVoltage;
	}
	double ElectronMultiplierShieldVoltage() const {
		return m_ElectronMultiplierShieldVoltage;
	}
	double ElectronMultiplierVoltage() const {
		return m_ElectronMultiplierVoltage;
	}
	double DDSAmplitude() const {
		return m_DDSAmplitude;
	}
	double ElectronMultiplierElectrometerGain() const {
		return m_ElectronMultiplierElectrometerGain;
	}
	double MassAxisCalibrationFactor() const {
		return m_MassAxisCalibrationFactor;
	}
	std::wstring FirmwareRevision() const {
		return m_FirmwareRevision;
	}
	std::wstring HardwareRevision() const {
		return m_HardwareRevision;
	}
	std::wstring MID() {
		return m_MID;
	}
private:
	double m_EmissionCurrent;
	double m_FilamentBiasVoltage;
	double m_FilamentPower;
	double m_RepellerVoltage;
	double m_EntryPlateVoltage;
	double m_PressurePlateVoltage;
	double m_CupsVoltage;
	double m_TransitionVoltage;
	double m_ExitPlateVoltage;
	double m_ElectronMultiplierShieldVoltage;
	double m_ElectronMultiplierVoltage;
	double m_DDSAmplitude;
	double m_ElectronMultiplierElectrometerGain;
	double m_MassAxisCalibrationFactor;
	std::wstring m_FirmwareRevision;
	std::wstring m_HardwareRevision;
	std::wstring m_MID;
};

#endif //IHEADERDATA_H_INCLUDE