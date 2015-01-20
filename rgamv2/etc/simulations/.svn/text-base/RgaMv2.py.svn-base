from pkg_resources import require
require('dls_serial_sim')
from dls_serial_sim import serial_device

# An MV2 RGA simulation
class RgaMv2(serial_device):

    def __init__(self, name='none', tcpPort=None, ui=None):
        '''Create an MV2 RGA simulator.'''
        self.name = name
        serial_device.__init__(self, ui=ui)
        self.InTerminator = "\r\n"
        self.OutTerminator = "\r\r"
        if tcpPort is not None:
            self.start_ip(tcpPort)
        self.userApplication = ''
        self.userVersion = ''
        self.barchartName = ''
        self.barchartStartMass = 1
        self.barchartEndMass = 50
        self.barchartFilterMode = 'PeakCenter'
        self.barchartAccuracy = 5
        self.barchartEGainIndex = 0
        self.barchartSourceIndex = 0
        self.barchartDetectorIndex = 0
        self.scanMeasurement = ''
        self.schedule(self.process, 1)
        self.diagnostic("Created MV2 RGA simulator %s\n" % name)

    def reply(self, command):
        '''Handle commands.  Lots still TODO'''
        result = None
        tokens = command.split()
        if len(tokens) >= 1 and tokens[0] == "Release":
            result = 'Release OK\r\n\r\n'
        elif len(tokens) >= 1 and tokens[0] == "Sensors":
            result = 'Sensors OK\r\nState SerialNumber Name\r\nReady LM70-00197021 \"Chamber A\"\r\n\r\n'
        elif len(tokens) >= 2 and tokens[0] == "Select":
            result = 'Select OK\r\nSerialNumber LM70-00197021\r\nState Ready\r\n\r\n'
        elif len(tokens) >= 1 and tokens[0] == "FilamentInfo":
            result = 'FilamentInfo OK\r\n'\
                'SummaryState OFF\r\n'\
                'ActiveFilament 2\r\n'\
                'ExternalTripEnable No\r\n'\
                'ExternalTripMode Trip\r\n'\
                'EmissionTripEnable Yes\r\n'\
                'MaxOnTime 900\r\n'\
                'OnTimeRemaining 0\r\n'\
                'Trip None\r\n'\
                'Drive Off\r\n'\
                'EmissionTripState OK\r\n'\
                'ExternalTripState OK\r\n'\
                'RVCTripState OK\r\n\r\n'
        elif len(tokens) >= 1 and tokens[0] == "MultiplierInfo":
            result = 'MultiplierInfo OK\r\n'\
                'InhibitWhenFilamentOff Yes\r\n'\
                'InhibitWhenRVCHeaterOn Yes\r\n'\
                'MultiplierOn No\r\n'\
                'LockedByFilament Yes\r\n'\
                'LockedByRVC No\r\n'\
                'LockedBySoftware No\r\n\r\n'
        elif len(tokens) >= 3 and tokens[0] == "Control":
            self.userApplication = tokens[1]
            self.userVersion = tokens[2]
            result = 'Control OK\r\nSerialNumber LM70-00197021\r\n\r\n'
        elif len(tokens) >= 3 and tokens[0] == "AnalogInputInterval":
            result = 'AnalogInputInterval OK\r\n\r\n'
        elif len(tokens) >= 3 and tokens[0] == "AnalogInputEnable":
            result = 'AnalogInputEnable OK\r\n\r\n'
        elif len(tokens) >= 1 and tokens[0] == "MeasurementRemoveAll":
            result = 'MeasurementRemoveAll OK\r\n\r\n'
        elif len(tokens) >= 1 and tokens[0] == "SensorState":
            result = 'SensorState OK\r\n' \
                'State InUse\r\n'\
                'UserApplication %s\r\n'\
                'UserVersion %s\r\n'\
                'UserAddress 127.0.0.1\r\n\r\n' % (self.userApplication, self.userVersion)
        elif len(tokens) >= 1 and tokens[0] == "Info":
            result = 'Info OK\r\n' \
                'SerialNumber LM70-00197021\r\n' \
                'Name \"Chamber A\"\r\n' \
                'State Ready\r\n' \
                'UserApplication %s\r\n' \
                'UserVersion %s\r\n' \
                'UserAddress 127.0.0.1\r\n' \
                'ProductID 70 MicroVision+\r\n' \
                'RFConfiguration 0 \"Smart Head\"\r\n' \
                'DetectorType 0 Faraday\r\n' \
                'SEMSupply 3000 3.0kV\r\n' \
                'ExternalHardware 0 None\r\n' \
                'TotalPressureGauge 0 \"Not Fitted\"\r\n' \
                'FilamentType 0 Tungsten\r\n' \
                'ControlUnitUse 4 \"Standard RGA\"\r\n' \
                'SensorType 1 \"Standard Open Source\"\r\n' \
                'InletType 1 None\r\n' \
                'Version V3.70\r\n' \
                'NumEGains 3\r\n' \
                'NumDigitalPorts 2\r\n' \
                'NumAnalogInputs 4\r\n' \
                'NumAnalogOutputs 1\r\n' \
                'NumSourceSettings 6\r\n' \
                'NumInlets 1\r\n' \
                'MaxMass 200\r\n' \
                'ActiveFilament 1\r\n' \
                'FullScaleADCAmps 0.000002\r\n' \
                'FullScaleADCCount 8388608\r\n' \
                'PeakResolution 32\r\n' \
                'ConfigurableIonSource Yes\r\n' \
                'RolloverCompensation No\r\n\r\n' % (self.userApplication, self.userVersion)
        elif len(tokens) >= 1 and tokens[0] == 'ScanStop':
            result = 'ScanStop OK\r\n\r\n'
        elif len(tokens) >= 9 and tokens[0] == 'AddBarchart':
            self.barchartName = tokens[1]
            self.barchartStartMass = int(tokens[2])
            self.barchartEndMass = int(tokens[3])
            self.barchartFilterMode = tokens[4]
            self.barchartAccuracy = int(tokens[5])
            self.barchartEGainIndex = int(tokens[6])
            self.barchartSourceIndex = int(tokens[7])
            self.barchartDetectorIndex = int(tokens[8])
            result = 'AddBarchart OK\r\n' + \
                ('Name %s\r\n' % self.barchartName) + \
                ('StartMass %s\r\n' % self.barchartStartMass) + \
                ('EndMass %s\r\n' % self.barchartEndMass) + \
                ('FilterMode %s\r\n' % self.barchartFilterMode) + \
                ('Accuracy %s\r\n' % self.barchartAccuracy) + \
                ('EGainIndex %s\r\n' % self.barchartEGainIndex) + \
                ('SourceIndex %s\r\n' % self.barchartSourceIndex) + \
                ('DetectorIndex %s\r\n\r\n' % self.barchartDetectorIndex)
        elif len(tokens) >= 2 and tokens[0] == 'ScanAdd':
            self.scanMeasurement = tokens[1]
            result = 'ScanAdd OK\r\nMeasurement %s\r\n' % self.scanMeasurement
        elif len(tokens) >= 2 and tokens[0] == 'ScanStart':
            self.numberOfScans = int(tokens[1])
            result = 'ScanStart OK\r\n\r\n'
        elif len(tokens) >= 2 and tokens[0] == 'ScanResume':
            self.numberOfScans = int(tokens[1])
            result = 'ScanResume OK\r\n\r\n'
        text = "%s==>%s" % (repr(command), repr(result))
        self.diagnostic(text, 1)
        return result

    def process(self):
        '''Perform the background processing'''
        pass

    
