/*============================================================================*/
/*                 National Instruments / Data Acquisition                    */
/*----------------------------------------------------------------------------*/
/*    Copyright (c) National Instruments 2004.  All Rights Reserved.          */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Title:       nidaqmxbase.h                                                 */
/* Purpose:     Include file for NI-DAQmxBase library support.                */
/*                                                                            */
/*============================================================================*/

#ifndef ___nidaqmxbase_h___
#define ___nidaqmxbase_h___

#ifdef __cplusplus
    extern "C" {
#endif

#define __CFUNC         __cdecl
#define __CFUNC_C       __cdecl
#define __CFUNCPTRVAR   __cdecl
#if defined WIN32
   #ifndef DllExport
      #define DllExport     __declspec(dllexport)
   #endif
#else
   #ifndef DllExport
      #define DllExport
   #endif
   #ifndef __cdecl
      #define __cdecl
   #endif
#endif

#ifndef _NI_int8_DEFINED_
#define _NI_int8_DEFINED_
   typedef signed char        int8;
#endif
#ifndef _NI_uInt8_DEFINED_
#define _NI_uInt8_DEFINED_
   typedef unsigned char      uInt8;
#endif
#ifndef _NI_int16_DEFINED_
#define _NI_int16_DEFINED_
   typedef signed short       int16;
#endif
#ifndef _NI_uInt16_DEFINED_
#define _NI_uInt16_DEFINED_
   typedef unsigned short     uInt16;
#endif
#ifndef _NI_int32_DEFINED_
#define _NI_int32_DEFINED_
   typedef signed long        int32;
#endif
#ifndef _NI_uInt32_DEFINED_
#define _NI_uInt32_DEFINED_
   typedef unsigned long      uInt32;
#endif
#ifndef _NI_float32_DEFINED_
#define _NI_float32_DEFINED_
   typedef float              float32;
#endif
#ifndef _NI_float64_DEFINED_
#define _NI_float64_DEFINED_
   typedef double             float64;
#endif
#ifndef _NI_int64_DEFINED_
#define _NI_int64_DEFINED_
   #if defined WIN32
      typedef __int64         int64;
   #else
      typedef long long       int64;
   #endif
#endif
#ifndef _NI_uInt64_DEFINED_
#define _NI_uInt64_DEFINED_
   #if defined WIN32
      typedef unsigned __int64    uInt64;
   #else
      typedef unsigned long long  uInt64;
   #endif
#endif

typedef uInt32             bool32;
typedef uInt32             *TaskHandle;

#ifndef TRUE
 #define TRUE            (1L)
#endif
#ifndef FALSE
 #define FALSE           (0L)
#endif
#ifndef NULL
 #define NULL            (0L)
#endif

/******************************************************************************
 *** NI-DAQmx Attributes ******************************************************
 ******************************************************************************/

#define DAQmx_SelfCal_Supported                                          0x1860 
#define DAQmx_SelfCal_LastTemp                                           0x1864 
#define DAQmx_ExtCal_RecommendedInterval                                 0x1868 
#define DAQmx_ExtCal_LastTemp                                            0x1867 
#define DAQmx_Cal_UserDefinedInfo                                        0x1861 
#define DAQmx_Cal_UserDefinedInfo_MaxSize                                0x191C 

#define DAQmx_ChanType                                                   0x187F 
#define DAQmx_PhysicalChanName                                           0x18F5 
#define DAQmx_ChanDescr                                                  0x1926 
#define DAQmx_AI_Max                                                     0x17DD 
#define DAQmx_AI_Min                                                     0x17DE 
#define DAQmx_AI_CustomScaleName                                         0x17E0 
#define DAQmx_AI_MeasType                                                0x0695 
#define DAQmx_AI_Voltage_Units                                           0x1094 
#define DAQmx_AI_Temp_Units                                              0x1033 
#define DAQmx_AI_Thrmcpl_Type                                            0x1050 
#define DAQmx_AI_Thrmcpl_CJCSrc                                          0x1035 
#define DAQmx_AI_Thrmcpl_CJCVal                                          0x1036 
#define DAQmx_AI_Thrmcpl_CJCChan                                         0x1034 
#define DAQmx_AI_RTD_Type                                                0x1032 
#define DAQmx_AI_RTD_R0                                                  0x1030 
#define DAQmx_AI_RTD_A                                                   0x1010 
#define DAQmx_AI_RTD_B                                                   0x1011 
#define DAQmx_AI_RTD_C                                                   0x1013 
#define DAQmx_AI_Thrmstr_A                                               0x18C9 
#define DAQmx_AI_Thrmstr_B                                               0x18CB 
#define DAQmx_AI_Thrmstr_C                                               0x18CA 
#define DAQmx_AI_Thrmstr_R1                                              0x1061 
#define DAQmx_AI_ForceReadFromChan                                       0x18F8 
#define DAQmx_AI_Current_Units                                           0x0701 
#define DAQmx_AI_Strain_Units                                            0x0981 
#define DAQmx_AI_StrainGage_GageFactor                                   0x0994 
#define DAQmx_AI_StrainGage_PoissonRatio                                 0x0998 
#define DAQmx_AI_StrainGage_Cfg                                          0x0982 
#define DAQmx_AI_Resistance_Units                                        0x0955 
#define DAQmx_AI_Freq_Units                                              0x0806 
#define DAQmx_AI_Freq_ThreshVoltage                                      0x0815 
#define DAQmx_AI_Freq_Hyst                                               0x0814 
#define DAQmx_AI_LVDT_Units                                              0x0910 
#define DAQmx_AI_LVDT_Sensitivity                                        0x0939 
#define DAQmx_AI_LVDT_SensitivityUnits                                   0x219A 
#define DAQmx_AI_RVDT_Units                                              0x0877 
#define DAQmx_AI_RVDT_Sensitivity                                        0x0903 
#define DAQmx_AI_RVDT_SensitivityUnits                                   0x219B 
#define DAQmx_AI_Accel_Units                                             0x0673 
#define DAQmx_AI_Accel_Sensitivity                                       0x0692 
#define DAQmx_AI_Accel_SensitivityUnits                                  0x219C 
#define DAQmx_AI_Coupling                                                0x0064 
#define DAQmx_AI_Impedance                                               0x0062 
#define DAQmx_AI_TermCfg                                                 0x1097 
#define DAQmx_AI_ResistanceCfg                                           0x1881 
#define DAQmx_AI_LeadWireResistance                                      0x17EE 
#define DAQmx_AI_Bridge_Cfg                                              0x0087 
#define DAQmx_AI_Bridge_NomResistance                                    0x17EC 
#define DAQmx_AI_Bridge_InitialVoltage                                   0x17ED 
#define DAQmx_AI_Bridge_ShuntCal_Enable                                  0x0094 
#define DAQmx_AI_Bridge_ShuntCal_Select                                  0x21D5 
#define DAQmx_AI_Bridge_ShuntCal_GainAdjust                              0x193F 
#define DAQmx_AI_Bridge_Balance_CoarsePot                                0x17F1 
#define DAQmx_AI_Bridge_Balance_FinePot                                  0x18F4 
#define DAQmx_AI_CurrentShunt_Loc                                        0x17F2 
#define DAQmx_AI_CurrentShunt_Resistance                                 0x17F3 
#define DAQmx_AI_Excit_Src                                               0x17F4 
#define DAQmx_AI_Excit_Val                                               0x17F5 
#define DAQmx_AI_Excit_UseForScaling                                     0x17FC 
#define DAQmx_AI_Excit_UseMultiplexed                                    0x2180 
#define DAQmx_AI_Excit_ActualVal                                         0x1883 
#define DAQmx_AI_Excit_DCorAC                                            0x17FB 
#define DAQmx_AI_Excit_VoltageOrCurrent                                  0x17F6 
#define DAQmx_AI_ACExcit_Freq                                            0x0101 
#define DAQmx_AI_ACExcit_SyncEnable                                      0x0102 
#define DAQmx_AI_ACExcit_WireMode                                        0x18CD 
#define DAQmx_AI_Atten                                                   0x1801 
#define DAQmx_AI_Lowpass_Enable                                          0x1802 
#define DAQmx_AI_Lowpass_CutoffFreq                                      0x1803 
#define DAQmx_AI_Lowpass_SwitchCap_ClkSrc                                0x1884 
#define DAQmx_AI_Lowpass_SwitchCap_ExtClkFreq                            0x1885 
#define DAQmx_AI_Lowpass_SwitchCap_ExtClkDiv                             0x1886 
#define DAQmx_AI_Lowpass_SwitchCap_OutClkDiv                             0x1887 
#define DAQmx_AI_ResolutionUnits                                         0x1764 
#define DAQmx_AI_Resolution                                              0x1765 
#define DAQmx_AI_Dither_Enable                                           0x0068 
#define DAQmx_AI_Rng_High                                                0x1815 
#define DAQmx_AI_Rng_Low                                                 0x1816 
#define DAQmx_AI_Gain                                                    0x1818 
#define DAQmx_AI_SampAndHold_Enable                                      0x181A 
#define DAQmx_AI_AutoZeroMode                                            0x1760 
#define DAQmx_AI_DataXferMech                                            0x1821 
#define DAQmx_AI_DataXferReqCond                                         0x188B 
#define DAQmx_AI_MemMapEnable                                            0x188C 
#define DAQmx_AI_DevScalingCoeff                                         0x1930 
#define DAQmx_AO_Max                                                     0x1186 
#define DAQmx_AO_Min                                                     0x1187 
#define DAQmx_AO_CustomScaleName                                         0x1188 
#define DAQmx_AO_OutputType                                              0x1108 
#define DAQmx_AO_Voltage_Units                                           0x1184 
#define DAQmx_AO_Current_Units                                           0x1109 
#define DAQmx_AO_OutputImpedance                                         0x1490 
#define DAQmx_AO_LoadImpedance                                           0x0121 
#define DAQmx_AO_ResolutionUnits                                         0x182B 
#define DAQmx_AO_Resolution                                              0x182C 
#define DAQmx_AO_DAC_Rng_High                                            0x182E 
#define DAQmx_AO_DAC_Rng_Low                                             0x182D 
#define DAQmx_AO_DAC_Ref_ConnToGnd                                       0x0130 
#define DAQmx_AO_DAC_Ref_AllowConnToGnd                                  0x1830 
#define DAQmx_AO_DAC_Ref_Src                                             0x0132 
#define DAQmx_AO_DAC_Ref_Val                                             0x1832 
#define DAQmx_AO_ReglitchEnable                                          0x0133 
#define DAQmx_AO_UseOnlyOnBrdMem                                         0x183A 
#define DAQmx_AO_DataXferMech                                            0x0134 
#define DAQmx_AO_DataXferReqCond                                         0x183C 
#define DAQmx_AO_MemMapEnable                                            0x188F 
#define DAQmx_AO_DevScalingCoeff                                         0x1931 
#define DAQmx_DI_InvertLines                                             0x0793 
#define DAQmx_DI_NumLines                                                0x2178 
#define DAQmx_DI_DigFltr_Enable                                          0x21D6 
#define DAQmx_DI_DigFltr_MinPulseWidth                                   0x21D7 
#define DAQmx_DO_InvertLines                                             0x1133 
#define DAQmx_DO_NumLines                                                0x2179 
#define DAQmx_DO_Tristate                                                0x18F3 
#define DAQmx_CI_Max                                                     0x189C 
#define DAQmx_CI_Min                                                     0x189D 
#define DAQmx_CI_CustomScaleName                                         0x189E 
#define DAQmx_CI_MeasType                                                0x18A0 
#define DAQmx_CI_Freq_Units                                              0x18A1 
#define DAQmx_CI_Freq_Term                                               0x18A2 
#define DAQmx_CI_Freq_StartingEdge                                       0x0799 
#define DAQmx_CI_Freq_MeasMeth                                           0x0144 
#define DAQmx_CI_Freq_MeasTime                                           0x0145 
#define DAQmx_CI_Freq_Div                                                0x0147 
#define DAQmx_CI_Period_Units                                            0x18A3 
#define DAQmx_CI_Period_Term                                             0x18A4 
#define DAQmx_CI_Period_StartingEdge                                     0x0852 
#define DAQmx_CI_Period_MeasMeth                                         0x192C 
#define DAQmx_CI_Period_MeasTime                                         0x192D 
#define DAQmx_CI_Period_Div                                              0x192E 
#define DAQmx_CI_CountEdges_Term                                         0x18C7 
#define DAQmx_CI_CountEdges_Dir                                          0x0696 
#define DAQmx_CI_CountEdges_DirTerm                                      0x21E1 
#define DAQmx_CI_CountEdges_InitialCnt                                   0x0698 
#define DAQmx_CI_CountEdges_ActiveEdge                                   0x0697 
#define DAQmx_CI_AngEncoder_Units                                        0x18A6 
#define DAQmx_CI_AngEncoder_PulsesPerRev                                 0x0875 
#define DAQmx_CI_AngEncoder_InitialAngle                                 0x0881 
#define DAQmx_CI_LinEncoder_Units                                        0x18A9 
#define DAQmx_CI_LinEncoder_DistPerPulse                                 0x0911 
#define DAQmx_CI_LinEncoder_InitialPos                                   0x0915 
#define DAQmx_CI_Encoder_DecodingType                                    0x21E6 
#define DAQmx_CI_Encoder_AInputTerm                                      0x219D 
#define DAQmx_CI_Encoder_BInputTerm                                      0x219E 
#define DAQmx_CI_Encoder_ZInputTerm                                      0x219F 
#define DAQmx_CI_Encoder_ZIndexEnable                                    0x0890 
#define DAQmx_CI_Encoder_ZIndexVal                                       0x0888 
#define DAQmx_CI_Encoder_ZIndexPhase                                     0x0889 
#define DAQmx_CI_PulseWidth_Units                                        0x0823 
#define DAQmx_CI_PulseWidth_Term                                         0x18AA 
#define DAQmx_CI_PulseWidth_StartingEdge                                 0x0825 
#define DAQmx_CI_TwoEdgeSep_Units                                        0x18AC 
#define DAQmx_CI_TwoEdgeSep_FirstTerm                                    0x18AD 
#define DAQmx_CI_TwoEdgeSep_FirstEdge                                    0x0833 
#define DAQmx_CI_TwoEdgeSep_SecondTerm                                   0x18AE 
#define DAQmx_CI_TwoEdgeSep_SecondEdge                                   0x0834 
#define DAQmx_CI_SemiPeriod_Units                                        0x18AF 
#define DAQmx_CI_SemiPeriod_Term                                         0x18B0 
#define DAQmx_CI_CtrTimebaseSrc                                          0x0143 
#define DAQmx_CI_CtrTimebaseRate                                         0x18B2 
#define DAQmx_CI_CtrTimebaseActiveEdge                                   0x0142 
#define DAQmx_CI_Count                                                   0x0148 
#define DAQmx_CI_OutputState                                             0x0149 
#define DAQmx_CI_TCReached                                               0x0150 
#define DAQmx_CI_CtrTimebaseMasterTimebaseDiv                            0x18B3 
#define DAQmx_CI_DataXferMech                                            0x0200 
#define DAQmx_CI_NumPossiblyInvalidSamps                                 0x193C 
#define DAQmx_CI_DupCountPrevent                                         0x21AC 
#define DAQmx_CO_OutputType                                              0x18B5 
#define DAQmx_CO_Pulse_IdleState                                         0x1170 
#define DAQmx_CO_Pulse_Term                                              0x18E1 
#define DAQmx_CO_Pulse_Time_Units                                        0x18D6 
#define DAQmx_CO_Pulse_HighTime                                          0x18BA 
#define DAQmx_CO_Pulse_LowTime                                           0x18BB 
#define DAQmx_CO_Pulse_Time_InitialDelay                                 0x18BC 
#define DAQmx_CO_Pulse_DutyCyc                                           0x1176 
#define DAQmx_CO_Pulse_Freq_Units                                        0x18D5 
#define DAQmx_CO_Pulse_Freq                                              0x1178 
#define DAQmx_CO_Pulse_Freq_InitialDelay                                 0x0299 
#define DAQmx_CO_Pulse_HighTicks                                         0x1169 
#define DAQmx_CO_Pulse_LowTicks                                          0x1171 
#define DAQmx_CO_Pulse_Ticks_InitialDelay                                0x0298 
#define DAQmx_CO_CtrTimebaseSrc                                          0x0339 
#define DAQmx_CO_CtrTimebaseRate                                         0x18C2 
#define DAQmx_CO_CtrTimebaseActiveEdge                                   0x0341 
#define DAQmx_CO_Count                                                   0x0293 
#define DAQmx_CO_OutputState                                             0x0294 
#define DAQmx_CO_AutoIncrCnt                                             0x0295 
#define DAQmx_CO_CtrTimebaseMasterTimebaseDiv                            0x18C3 
#define DAQmx_CO_PulseDone                                               0x190E 

#define DAQmx_Exported_AIConvClk_OutputTerm                              0x1687 
#define DAQmx_Exported_AIConvClk_Pulse_Polarity                          0x1688 
#define DAQmx_Exported_20MHzTimebase_OutputTerm                          0x1657 
#define DAQmx_Exported_SampClk_OutputBehavior                            0x186B 
#define DAQmx_Exported_SampClk_OutputTerm                                0x1663 
#define DAQmx_Exported_AdvTrig_OutputTerm                                0x1645 
#define DAQmx_Exported_AdvTrig_Pulse_Polarity                            0x1646 
#define DAQmx_Exported_AdvTrig_Pulse_WidthUnits                          0x1647 
#define DAQmx_Exported_AdvTrig_Pulse_Width                               0x1648 
#define DAQmx_Exported_RefTrig_OutputTerm                                0x0590 
#define DAQmx_Exported_StartTrig_OutputTerm                              0x0584 
#define DAQmx_Exported_AdvCmpltEvent_OutputTerm                          0x1651 
#define DAQmx_Exported_AdvCmpltEvent_Delay                               0x1757 
#define DAQmx_Exported_AdvCmpltEvent_Pulse_Polarity                      0x1652 
#define DAQmx_Exported_AdvCmpltEvent_Pulse_Width                         0x1654 
#define DAQmx_Exported_AIHoldCmpltEvent_OutputTerm                       0x18ED 
#define DAQmx_Exported_AIHoldCmpltEvent_PulsePolarity                    0x18EE 
#define DAQmx_Exported_ChangeDetectEvent_OutputTerm                      0x2197 
#define DAQmx_Exported_CtrOutEvent_OutputTerm                            0x1717 
#define DAQmx_Exported_CtrOutEvent_OutputBehavior                        0x174F 
#define DAQmx_Exported_CtrOutEvent_Pulse_Polarity                        0x1718 
#define DAQmx_Exported_CtrOutEvent_Toggle_IdleState                      0x186A 
#define DAQmx_Exported_WatchdogExpiredEvent_OutputTerm                   0x21AA 

#define DAQmx_Dev_ProductType                                            0x0631 
#define DAQmx_Dev_SerialNum                                              0x0632 

#define DAQmx_Read_RelativeTo                                            0x190A 
#define DAQmx_Read_Offset                                                0x190B 
#define DAQmx_Read_ChannelsToRead                                        0x1823 
#define DAQmx_Read_ReadAllAvailSamp                                      0x1215 
#define DAQmx_Read_AutoStart                                             0x1826 
#define DAQmx_Read_OverWrite                                             0x1211 
#define DAQmx_Read_CurrReadPos                                           0x1221 
#define DAQmx_Read_AvailSampPerChan                                      0x1223 
#define DAQmx_Read_TotalSampPerChanAcquired                              0x192A 
#define DAQmx_Read_ChangeDetect_HasOverflowed                            0x2194 
#define DAQmx_Read_RawDataWidth                                          0x217A 
#define DAQmx_Read_NumChans                                              0x217B 
#define DAQmx_Read_DigitalLines_BytesPerChan                             0x217C 

#define DAQmx_SwitchChan_Usage                                           0x18E4 
#define DAQmx_SwitchChan_MaxACCarryCurrent                               0x0648 
#define DAQmx_SwitchChan_MaxACSwitchCurrent                              0x0646 
#define DAQmx_SwitchChan_MaxACCarryPwr                                   0x0642 
#define DAQmx_SwitchChan_MaxACSwitchPwr                                  0x0644 
#define DAQmx_SwitchChan_MaxDCCarryCurrent                               0x0647 
#define DAQmx_SwitchChan_MaxDCSwitchCurrent                              0x0645 
#define DAQmx_SwitchChan_MaxDCCarryPwr                                   0x0643 
#define DAQmx_SwitchChan_MaxDCSwitchPwr                                  0x0649 
#define DAQmx_SwitchChan_MaxACVoltage                                    0x0651 
#define DAQmx_SwitchChan_MaxDCVoltage                                    0x0650 
#define DAQmx_SwitchChan_WireMode                                        0x18E5 
#define DAQmx_SwitchChan_Bandwidth                                       0x0640 
#define DAQmx_SwitchChan_Impedance                                       0x0641 

#define DAQmx_SwitchDev_SettlingTime                                     0x1244 
#define DAQmx_SwitchDev_AutoConnAnlgBus                                  0x17DA 
#define DAQmx_SwitchDev_Settled                                          0x1243 
#define DAQmx_SwitchDev_RelayList                                        0x17DC 
#define DAQmx_SwitchDev_NumRelays                                        0x18E6 
#define DAQmx_SwitchDev_SwitchChanList                                   0x18E7 
#define DAQmx_SwitchDev_NumSwitchChans                                   0x18E8 
#define DAQmx_SwitchDev_NumRows                                          0x18E9 
#define DAQmx_SwitchDev_NumColumns                                       0x18EA 
#define DAQmx_SwitchDev_Topology                                         0x193D 

#define DAQmx_SwitchScan_BreakMode                                       0x1247 
#define DAQmx_SwitchScan_RepeatMode                                      0x1248 
#define DAQmx_SwitchScan_WaitingForAdv                                   0x17D9 

#define DAQmx_Scale_Descr                                                0x1226 
#define DAQmx_Scale_ScaledUnits                                          0x191B 
#define DAQmx_Scale_PreScaledUnits                                       0x18F7 
#define DAQmx_Scale_Type                                                 0x1929 
#define DAQmx_Scale_Lin_Slope                                            0x1227 
#define DAQmx_Scale_Lin_YIntercept                                       0x1228 
#define DAQmx_Scale_Map_ScaledMax                                        0x1229 
#define DAQmx_Scale_Map_PreScaledMax                                     0x1231 
#define DAQmx_Scale_Map_ScaledMin                                        0x1230 
#define DAQmx_Scale_Map_PreScaledMin                                     0x1232 
#define DAQmx_Scale_Poly_ForwardCoeff                                    0x1234 
#define DAQmx_Scale_Poly_ReverseCoeff                                    0x1235 
#define DAQmx_Scale_Table_ScaledVals                                     0x1236 
#define DAQmx_Scale_Table_PreScaledVals                                  0x1237 

#define DAQmx_Sys_GlobalChans                                            0x1265 
#define DAQmx_Sys_Scales                                                 0x1266 
#define DAQmx_Sys_Tasks                                                  0x1267 
#define DAQmx_Sys_DevNames                                               0x193B 
#define DAQmx_Sys_NIDAQMajorVersion                                      0x1272 
#define DAQmx_Sys_NIDAQMinorVersion                                      0x1923 

#define DAQmx_Task_Name                                                  0x1276 
#define DAQmx_Task_Channels                                              0x1273 
#define DAQmx_Task_NumChans                                              0x2181 
#define DAQmx_Task_Complete                                              0x1274 

#define DAQmx_SampQuant_SampMode                                         0x1300 
#define DAQmx_SampQuant_SampPerChan                                      0x1310 
#define DAQmx_SampTimingType                                             0x1347 
#define DAQmx_SampClk_Rate                                               0x1344 
#define DAQmx_SampClk_Src                                                0x1852 
#define DAQmx_SampClk_ActiveEdge                                         0x1301 
#define DAQmx_SampClk_TimebaseDiv                                        0x18EB 
#define DAQmx_SampClk_Timebase_Rate                                      0x1303 
#define DAQmx_SampClk_Timebase_Src                                       0x1308 
#define DAQmx_SampClk_Timebase_ActiveEdge                                0x18EC 
#define DAQmx_SampClk_Timebase_MasterTimebaseDiv                         0x1305 
#define DAQmx_ChangeDetect_DI_RisingEdgePhysicalChans                    0x2195 
#define DAQmx_ChangeDetect_DI_FallingEdgePhysicalChans                   0x2196 
#define DAQmx_OnDemand_SimultaneousAOEnable                              0x21A0 
#define DAQmx_AIConv_Rate                                                0x1848 
#define DAQmx_AIConv_Src                                                 0x1502 
#define DAQmx_AIConv_ActiveEdge                                          0x1853 
#define DAQmx_AIConv_TimebaseDiv                                         0x1335 
#define DAQmx_AIConv_Timebase_Src                                        0x1339 
#define DAQmx_MasterTimebase_Rate                                        0x1495 
#define DAQmx_MasterTimebase_Src                                         0x1343 
#define DAQmx_DelayFromSampClk_DelayUnits                                0x1304 
#define DAQmx_DelayFromSampClk_Delay                                     0x1317 

#define DAQmx_StartTrig_Type                                             0x1393 
#define DAQmx_DigEdge_StartTrig_Src                                      0x1407 
#define DAQmx_DigEdge_StartTrig_Edge                                     0x1404 
#define DAQmx_AnlgEdge_StartTrig_Src                                     0x1398 
#define DAQmx_AnlgEdge_StartTrig_Slope                                   0x1397 
#define DAQmx_AnlgEdge_StartTrig_Lvl                                     0x1396 
#define DAQmx_AnlgEdge_StartTrig_Hyst                                    0x1395 
#define DAQmx_AnlgWin_StartTrig_Src                                      0x1400 
#define DAQmx_AnlgWin_StartTrig_When                                     0x1401 
#define DAQmx_AnlgWin_StartTrig_Top                                      0x1403 
#define DAQmx_AnlgWin_StartTrig_Btm                                      0x1402 
#define DAQmx_StartTrig_Delay                                            0x1856 
#define DAQmx_StartTrig_DelayUnits                                       0x18C8 
#define DAQmx_StartTrig_Retriggerable                                    0x190F 
#define DAQmx_RefTrig_Type                                               0x1419 
#define DAQmx_RefTrig_PretrigSamples                                     0x1445 
#define DAQmx_DigEdge_RefTrig_Src                                        0x1434 
#define DAQmx_DigEdge_RefTrig_Edge                                       0x1430 
#define DAQmx_AnlgEdge_RefTrig_Src                                       0x1424 
#define DAQmx_AnlgEdge_RefTrig_Slope                                     0x1423 
#define DAQmx_AnlgEdge_RefTrig_Lvl                                       0x1422 
#define DAQmx_AnlgEdge_RefTrig_Hyst                                      0x1421 
#define DAQmx_AnlgWin_RefTrig_Src                                        0x1426 
#define DAQmx_AnlgWin_RefTrig_When                                       0x1427 
#define DAQmx_AnlgWin_RefTrig_Top                                        0x1429 
#define DAQmx_AnlgWin_RefTrig_Btm                                        0x1428 
#define DAQmx_AdvTrig_Type                                               0x1365 
#define DAQmx_DigEdge_AdvTrig_Src                                        0x1362 
#define DAQmx_DigEdge_AdvTrig_Edge                                       0x1360 
#define DAQmx_PauseTrig_Type                                             0x1366 
#define DAQmx_AnlgLvl_PauseTrig_Src                                      0x1370 
#define DAQmx_AnlgLvl_PauseTrig_When                                     0x1371 
#define DAQmx_AnlgLvl_PauseTrig_Lvl                                      0x1369 
#define DAQmx_AnlgLvl_PauseTrig_Hyst                                     0x1368 
#define DAQmx_AnlgWin_PauseTrig_Src                                      0x1373 
#define DAQmx_AnlgWin_PauseTrig_When                                     0x1374 
#define DAQmx_AnlgWin_PauseTrig_Top                                      0x1376 
#define DAQmx_AnlgWin_PauseTrig_Btm                                      0x1375 
#define DAQmx_DigLvl_PauseTrig_Src                                       0x1379 
#define DAQmx_DigLvl_PauseTrig_When                                      0x1380 
#define DAQmx_ArmStartTrig_Type                                          0x1414 
#define DAQmx_DigEdge_ArmStartTrig_Src                                   0x1417 
#define DAQmx_DigEdge_ArmStartTrig_Edge                                  0x1415 

#define DAQmx_Watchdog_Timeout                                           0x21A9 
#define DAQmx_WatchdogExpirTrig_Type                                     0x21A3 
#define DAQmx_DigEdge_WatchdogExpirTrig_Src                              0x21A4 
#define DAQmx_DigEdge_WatchdogExpirTrig_Edge                             0x21A5 
#define DAQmx_Watchdog_DO_ExpirState                                     0x21A7 
#define DAQmx_Watchdog_HasExpired                                        0x21A8 

#define DAQmx_Write_RelativeTo                                           0x190C 
#define DAQmx_Write_Offset                                               0x190D 
#define DAQmx_Write_RegenMode                                            0x1453 
#define DAQmx_Write_CurrWritePos                                         0x1458 
#define DAQmx_Write_SpaceAvail                                           0x1460 
#define DAQmx_Write_TotalSampPerChanGenerated                            0x192B 
#define DAQmx_Write_RawDataWidth                                         0x217D 
#define DAQmx_Write_NumChans                                             0x217E 
#define DAQmx_Write_DigitalLines_BytesPerChan                            0x217F 


/******************************************************************************
 *** NI-DAQmx Values **********************************************************
 ******************************************************************************/

/******************************************************/
/***    Non-Attribute Function Parameter Values     ***/
/******************************************************/

#define DAQmx_Val_Task_Start                                              0   
#define DAQmx_Val_Task_Stop                                               1   
#define DAQmx_Val_Task_Verify                                             2   
#define DAQmx_Val_Task_Commit                                             3   
#define DAQmx_Val_Task_Reserve                                            4   
#define DAQmx_Val_Task_Unreserve                                          5   
#define DAQmx_Val_Task_Abort                                              6   

#define DAQmx_Val_ResetTimer                                              0   
#define DAQmx_Val_ClearExpiration                                         1   

#define DAQmx_Val_ChanPerLine                                             0   
#define DAQmx_Val_ChanForAllLines                                         1   

#define DAQmx_Val_GroupByChannel                                          0   
#define DAQmx_Val_GroupByScanNumber                                       1   

#define DAQmx_Val_DoNotInvertPolarity                                     0   
#define DAQmx_Val_InvertPolarity                                          1   

#define DAQmx_Val_Action_Commit                                           0   
#define DAQmx_Val_Action_Cancel                                           1   

#define DAQmx_Val_AdvanceTrigger                                          12488 

#define DAQmx_Val_AIConvertClock                                          12484 
#define DAQmx_Val_20MHzTimebaseClock                                      12486 
#define DAQmx_Val_SampleClock                                             12487 
#define DAQmx_Val_AdvanceTrigger                                          12488 
#define DAQmx_Val_ReferenceTrigger                                        12490 
#define DAQmx_Val_StartTrigger                                            12491 
#define DAQmx_Val_AdvCmpltEvent                                           12492 
#define DAQmx_Val_AIHoldCmpltEvent                                        12493 
#define DAQmx_Val_CounterOutputEvent                                      12494 
#define DAQmx_Val_ChangeDetectionEvent                                    12511 
#define DAQmx_Val_WDTExpiredEvent                                         12512 

#define DAQmx_Val_Rising                                                  10280 
#define DAQmx_Val_Falling                                                 10171 

#define DAQmx_Val_PathStatus_Available                                    10431 
#define DAQmx_Val_PathStatus_AlreadyExists                                10432 
#define DAQmx_Val_PathStatus_Unsupported                                  10433 
#define DAQmx_Val_PathStatus_ChannelInUse                                 10434 
#define DAQmx_Val_PathStatus_SourceChannelConflict                        10435 
#define DAQmx_Val_PathStatus_ChannelReservedForRouting                    10436 

#define DAQmx_Val_DegC                                                    10143 
#define DAQmx_Val_DegF                                                    10144 
#define DAQmx_Val_Kelvins                                                 10325 
#define DAQmx_Val_DegR                                                    10145 

#define DAQmx_Val_High                                                    10192 
#define DAQmx_Val_Low                                                     10214 
#define DAQmx_Val_Tristate                                                10310 

#define DAQmx_Val_Open                                                    10437 
#define DAQmx_Val_Closed                                                  10438 

#define DAQmx_Val_Cfg_Default                                             -1 

#define DAQmx_Val_WaitInfinitely                                          -1.0

#define DAQmx_Val_Auto                                                    -1

/******************************************************/
/***              Attribute Values                  ***/
/******************************************************/

#define DAQmx_Val_4Wire                                                       4 
#define DAQmx_Val_5Wire                                                       5 

#define DAQmx_Val_Voltage                                                 10322 
#define DAQmx_Val_Current                                                 10134 
#define DAQmx_Val_Voltage_CustomWithExcitation                            10323 
#define DAQmx_Val_Freq_Voltage                                            10181 
#define DAQmx_Val_Resistance                                              10278 
#define DAQmx_Val_Temp_TC                                                 10303 
#define DAQmx_Val_Temp_Thrmstr                                            10302 
#define DAQmx_Val_Temp_RTD                                                10301 
#define DAQmx_Val_Temp_BuiltInSensor                                      10311 
#define DAQmx_Val_Strain_Gage                                             10300 
#define DAQmx_Val_Position_LVDT                                           10352 
#define DAQmx_Val_Position_RVDT                                           10353 
#define DAQmx_Val_Accelerometer                                           10356 

#define DAQmx_Val_Voltage                                                 10322 
#define DAQmx_Val_Current                                                 10134 

#define DAQmx_Val_mVoltsPerG                                              12509 
#define DAQmx_Val_VoltsPerG                                               12510 

#define DAQmx_Val_AccelUnit_g                                             10186 
#define DAQmx_Val_FromCustomScale                                         10065 

#define DAQmx_Val_FiniteSamps                                             10178 
#define DAQmx_Val_ContSamps                                               10123 

#define DAQmx_Val_AboveLvl                                                10093 
#define DAQmx_Val_BelowLvl                                                10107 

#define DAQmx_Val_Degrees                                                 10146 
#define DAQmx_Val_Radians                                                 10273 
#define DAQmx_Val_FromCustomScale                                         10065 

#define DAQmx_Val_Degrees                                                 10146 
#define DAQmx_Val_Radians                                                 10273 
#define DAQmx_Val_Ticks                                                   10304 
#define DAQmx_Val_FromCustomScale                                         10065 

#define DAQmx_Val_None                                                    10230 
#define DAQmx_Val_Once                                                    10244 

#define DAQmx_Val_NoAction                                                10227 
#define DAQmx_Val_BreakBeforeMake                                         10110 

#define DAQmx_Val_FullBridge                                              10182 
#define DAQmx_Val_HalfBridge                                              10187 
#define DAQmx_Val_QuarterBridge                                           10270 
#define DAQmx_Val_NoBridge                                                10228 

#define DAQmx_Val_CountEdges                                              10125 
#define DAQmx_Val_Freq                                                    10179 
#define DAQmx_Val_Period                                                  10256 
#define DAQmx_Val_PulseWidth                                              10359 
#define DAQmx_Val_SemiPeriod                                              10289 
#define DAQmx_Val_Position_AngEncoder                                     10360 
#define DAQmx_Val_Position_LinEncoder                                     10361 
#define DAQmx_Val_TwoEdgeSep                                              10267 

#define DAQmx_Val_BuiltIn                                                 10200 
#define DAQmx_Val_ConstVal                                                10116 
#define DAQmx_Val_Chan                                                    10113 

#define DAQmx_Val_Pulse_Time                                              10269 
#define DAQmx_Val_Pulse_Freq                                              10119 
#define DAQmx_Val_Pulse_Ticks                                             10268 

#define DAQmx_Val_AI                                                      10100 
#define DAQmx_Val_AO                                                      10102 
#define DAQmx_Val_DI                                                      10151 
#define DAQmx_Val_DO                                                      10153 
#define DAQmx_Val_CI                                                      10131 
#define DAQmx_Val_CO                                                      10132 

#define DAQmx_Val_CountUp                                                 10128 
#define DAQmx_Val_CountDown                                               10124 
#define DAQmx_Val_ExtControlled                                           10326 

#define DAQmx_Val_LowFreq1Ctr                                             10105 
#define DAQmx_Val_HighFreq2Ctr                                            10157 
#define DAQmx_Val_LargeRng2Ctr                                            10205 

#define DAQmx_Val_AC                                                      10045 
#define DAQmx_Val_DC                                                      10050 
#define DAQmx_Val_GND                                                     10066 

#define DAQmx_Val_Internal                                                10200 
#define DAQmx_Val_External                                                10167 

#define DAQmx_Val_Amps                                                    10342 
#define DAQmx_Val_FromCustomScale                                         10065 

#define DAQmx_Val_DMA                                                     10054 
#define DAQmx_Val_Interrupts                                              10204 
#define DAQmx_Val_ProgrammedIO                                            10264 

#define DAQmx_Val_High                                                    10192 
#define DAQmx_Val_Low                                                     10214 
#define DAQmx_Val_Tristate                                                10310 
#define DAQmx_Val_NoChange                                                10160 

#define DAQmx_Val_SampClkPeriods                                          10286 
#define DAQmx_Val_Seconds                                                 10364 
#define DAQmx_Val_Ticks                                                   10304 

#define DAQmx_Val_Seconds                                                 10364 
#define DAQmx_Val_Ticks                                                   10304 

#define DAQmx_Val_Seconds                                                 10364 

#define DAQmx_Val_Rising                                                  10280 
#define DAQmx_Val_Falling                                                 10171 

#define DAQmx_Val_X1                                                      10090 
#define DAQmx_Val_X2                                                      10091 
#define DAQmx_Val_X4                                                      10092 
#define DAQmx_Val_TwoPulseCounting                                        10313 

#define DAQmx_Val_AHighBHigh                                              10040 
#define DAQmx_Val_AHighBLow                                               10041 
#define DAQmx_Val_ALowBHigh                                               10042 
#define DAQmx_Val_ALowBLow                                                10043 

#define DAQmx_Val_DC                                                      10050 
#define DAQmx_Val_AC                                                      10045 

#define DAQmx_Val_Internal                                                10200 
#define DAQmx_Val_External                                                10167 
#define DAQmx_Val_None                                                    10230 

#define DAQmx_Val_Voltage                                                 10322 
#define DAQmx_Val_Current                                                 10134 

#define DAQmx_Val_Pulse                                                   10265 
#define DAQmx_Val_Toggle                                                  10307 

#define DAQmx_Val_Pulse                                                   10265 
#define DAQmx_Val_Lvl                                                     10210 

#define DAQmx_Val_Hz                                                      10373 
#define DAQmx_Val_FromCustomScale                                         10065 

#define DAQmx_Val_Hz                                                      10373 

#define DAQmx_Val_Hz                                                      10373 
#define DAQmx_Val_Ticks                                                   10304 
#define DAQmx_Val_FromCustomScale                                         10065 


#define DAQmx_Val_OnBrdMemMoreThanHalfFull                                10237 
#define DAQmx_Val_OnBrdMemNotEmpty                                        10241 

#define DAQmx_Val_RSE                                                     10083 
#define DAQmx_Val_NRSE                                                    10078 
#define DAQmx_Val_Diff                                                    10106 

#define DAQmx_Val_mVoltsPerVoltPerMillimeter                              12506 
#define DAQmx_Val_mVoltsPerVoltPerMilliInch                               12505 

#define DAQmx_Val_Meters                                                  10219 
#define DAQmx_Val_Inches                                                  10379 
#define DAQmx_Val_FromCustomScale                                         10065 

#define DAQmx_Val_Meters                                                  10219 
#define DAQmx_Val_Inches                                                  10379 
#define DAQmx_Val_Ticks                                                   10304 
#define DAQmx_Val_FromCustomScale                                         10065 

#define DAQmx_Val_High                                                    10192 
#define DAQmx_Val_Low                                                     10214 

#define DAQmx_Val_SameAsSampTimebase                                      10284 
#define DAQmx_Val_SameAsMasterTimebase                                    10282 

#define DAQmx_Val_OnBrdMemEmpty                                           10235 
#define DAQmx_Val_OnBrdMemHalfFullOrLess                                  10239 
#define DAQmx_Val_OnBrdMemNotFull                                         10242 

#define DAQmx_Val_OverwriteUnreadSamps                                    10252 
#define DAQmx_Val_DoNotOverwriteUnreadSamps                               10159 

#define DAQmx_Val_ActiveHigh                                              10095 
#define DAQmx_Val_ActiveLow                                               10096 

#define DAQmx_Val_Pt3750                                                  12481 
#define DAQmx_Val_Pt3851                                                  10071 
#define DAQmx_Val_Pt3911                                                  12482 
#define DAQmx_Val_Pt3916                                                  10069 
#define DAQmx_Val_Pt3920                                                  10053 
#define DAQmx_Val_Pt3928                                                  12483 
#define DAQmx_Val_Custom                                                  10137 

#define DAQmx_Val_mVoltsPerVoltPerDegree                                  12507 
#define DAQmx_Val_mVoltsPerVoltPerRadian                                  12508 

#define DAQmx_Val_FirstSample                                             10424 
#define DAQmx_Val_CurrReadPos                                             10425 
#define DAQmx_Val_RefTrig                                                 10426 
#define DAQmx_Val_FirstPretrigSamp                                        10427 
#define DAQmx_Val_MostRecentSamp                                          10428 


#define DAQmx_Val_AllowRegen                                              10097 
#define DAQmx_Val_DoNotAllowRegen                                         10158 

#define DAQmx_Val_2Wire                                                       2 
#define DAQmx_Val_3Wire                                                       3 
#define DAQmx_Val_4Wire                                                       4 

#define DAQmx_Val_Ohms                                                    10384 
#define DAQmx_Val_FromCustomScale                                         10065 

#define DAQmx_Val_Bits                                                    10109 

#define DAQmx_Val_SampClk                                                 10388 
#define DAQmx_Val_Handshake                                               10389 
#define DAQmx_Val_Implicit                                                10451 
#define DAQmx_Val_OnDemand                                                10390 
#define DAQmx_Val_ChangeDetection                                         12504 

#define DAQmx_Val_Linear                                                  10447 
#define DAQmx_Val_MapRanges                                               10448 
#define DAQmx_Val_Polynomial                                              10449 
#define DAQmx_Val_Table                                                   10450 

#define DAQmx_Val_A                                                       12513 
#define DAQmx_Val_B                                                       12514 
#define DAQmx_Val_AandB                                                   12515 

#define DAQmx_Val_RisingSlope                                             10280 
#define DAQmx_Val_FallingSlope                                            10171 

#define DAQmx_Val_Internal                                                10200 
#define DAQmx_Val_External                                                10167 

#define DAQmx_Val_FullBridgeI                                             10183 
#define DAQmx_Val_FullBridgeII                                            10184 
#define DAQmx_Val_FullBridgeIII                                           10185 
#define DAQmx_Val_HalfBridgeI                                             10188 
#define DAQmx_Val_HalfBridgeII                                            10189 
#define DAQmx_Val_QuarterBridgeI                                          10271 
#define DAQmx_Val_QuarterBridgeII                                         10272 

#define DAQmx_Val_Strain                                                  10299 
#define DAQmx_Val_FromCustomScale                                         10065 

#define DAQmx_Val_Finite                                                  10172 
#define DAQmx_Val_Cont                                                    10117 

#define DAQmx_Val_Source                                                  10439 
#define DAQmx_Val_Load                                                    10440 
#define DAQmx_Val_ReservedForRouting                                      10441 

#define DAQmx_Val_DegC                                                    10143 
#define DAQmx_Val_DegF                                                    10144 
#define DAQmx_Val_Kelvins                                                 10325 
#define DAQmx_Val_DegR                                                    10145 
#define DAQmx_Val_FromCustomScale                                         10065 

#define DAQmx_Val_J_Type_TC                                               10072 
#define DAQmx_Val_K_Type_TC                                               10073 
#define DAQmx_Val_N_Type_TC                                               10077 
#define DAQmx_Val_R_Type_TC                                               10082 
#define DAQmx_Val_S_Type_TC                                               10085 
#define DAQmx_Val_T_Type_TC                                               10086 
#define DAQmx_Val_B_Type_TC                                               10047 
#define DAQmx_Val_E_Type_TC                                               10055 

#define DAQmx_Val_Seconds                                                 10364 

#define DAQmx_Val_Seconds                                                 10364 
#define DAQmx_Val_Ticks                                                   10304 
#define DAQmx_Val_FromCustomScale                                         10065 

#define DAQmx_Val_AnlgEdge                                                10099 
#define DAQmx_Val_DigEdge                                                 10150 
#define DAQmx_Val_AnlgWin                                                 10103 
#define DAQmx_Val_None                                                    10230 

#define DAQmx_Val_DigEdge                                                 10150 
#define DAQmx_Val_None                                                    10230 

#define DAQmx_Val_DigEdge                                                 10150 
#define DAQmx_Val_Software                                                10292 
#define DAQmx_Val_None                                                    10230 

#define DAQmx_Val_AnlgLvl                                                 10101 
#define DAQmx_Val_AnlgWin                                                 10103 
#define DAQmx_Val_DigLvl                                                  10152 
#define DAQmx_Val_None                                                    10230 

#define DAQmx_Val_AnlgEdge                                                10099 
#define DAQmx_Val_DigEdge                                                 10150 
#define DAQmx_Val_AnlgWin                                                 10103 
#define DAQmx_Val_None                                                    10230 

#define DAQmx_Val_Volts                                                   10348 
#define DAQmx_Val_Amps                                                    10342 
#define DAQmx_Val_DegF                                                    10144 
#define DAQmx_Val_DegC                                                    10143 
#define DAQmx_Val_DegR                                                    10145 
#define DAQmx_Val_Kelvins                                                 10325 
#define DAQmx_Val_Strain                                                  10299 
#define DAQmx_Val_Ohms                                                    10384 
#define DAQmx_Val_Hz                                                      10373 
#define DAQmx_Val_Seconds                                                 10364 
#define DAQmx_Val_Meters                                                  10219 
#define DAQmx_Val_Inches                                                  10379 
#define DAQmx_Val_Degrees                                                 10146 
#define DAQmx_Val_Radians                                                 10273 
#define DAQmx_Val_g                                                       10186 

#define DAQmx_Val_Volts                                                   10348 
#define DAQmx_Val_FromCustomScale                                         10065 

#define DAQmx_Val_Volts                                                   10348 
#define DAQmx_Val_FromCustomScale                                         10065 

#define DAQmx_Val_EnteringWin                                             10163 
#define DAQmx_Val_LeavingWin                                              10208 

#define DAQmx_Val_InsideWin                                               10199 
#define DAQmx_Val_OutsideWin                                              10251 

#define DAQmx_Val_FirstSample                                             10424 
#define DAQmx_Val_CurrWritePos                                            10430 


/******************************************************************************
 *** NI-DAQBasemx Function Declarations ***************************************
 ******************************************************************************/

/******************************************************/
/***         Task Configuration/Control             ***/
/******************************************************/


int32 DllExport __CFUNC     DAQmxBaseLoadTask            (const char taskName[], TaskHandle *taskHandle);
int32 DllExport __CFUNC     DAQmxBaseCreateTask          (const char taskName[], TaskHandle *taskHandle);

int32 DllExport __CFUNC     DAQmxBaseStartTask           (TaskHandle taskHandle);
int32 DllExport __CFUNC     DAQmxBaseStopTask            (TaskHandle taskHandle);

int32 DllExport __CFUNC     DAQmxBaseClearTask           (TaskHandle taskHandle);

int32 DllExport __CFUNC     DAQmxBaseIsTaskDone          (TaskHandle taskHandle, bool32 *isTaskDone);





/******************************************************/
/***        Channel Configuration/Creation          ***/
/******************************************************/


int32 DllExport __CFUNC     DAQmxBaseCreateAIVoltageChan          (TaskHandle taskHandle, const char physicalChannel[], const char nameToAssignToChannel[], int32 terminalConfig, float64 minVal, float64 maxVal, int32 units, const char customScaleName[]);
int32 DllExport __CFUNC     DAQmxBaseCreateAIThrmcplChan          (TaskHandle taskHandle, const char physicalChannel[], const char nameToAssignToChannel[], float64 minVal, float64 maxVal, int32 units, int32 thermocoupleType, int32 cjcSource, float64 cjcVal, const char cjcChannel[]);

int32 DllExport __CFUNC     DAQmxBaseCreateAOVoltageChan          (TaskHandle taskHandle, const char physicalChannel[], const char nameToAssignToChannel[], float64 minVal, float64 maxVal, int32 units, const char customScaleName[]);

int32 DllExport __CFUNC     DAQmxBaseCreateDIChan                 (TaskHandle taskHandle, const char lines[], const char nameToAssignToLines[], int32 lineGrouping);

int32 DllExport __CFUNC     DAQmxBaseCreateDOChan                 (TaskHandle taskHandle, const char lines[], const char nameToAssignToLines[], int32 lineGrouping);

int32 DllExport __CFUNC     DAQmxBaseCreateCIPeriodChan           (TaskHandle taskHandle, const char counter[], const char nameToAssignToChannel[], float64 minVal, float64 maxVal, int32 units, int32 edge, int32 measMethod, float64 measTime, uInt32 divisor, const char customScaleName[]);
int32 DllExport __CFUNC     DAQmxBaseCreateCICountEdgesChan       (TaskHandle taskHandle, const char counter[], const char nameToAssignToChannel[], int32 edge, uInt32 initialCount, int32 countDirection);
int32 DllExport __CFUNC     DAQmxBaseCreateCIPulseWidthChan       (TaskHandle taskHandle, const char counter[], const char nameToAssignToChannel[], float64 minVal, float64 maxVal, int32 units, int32 startingEdge, const char customScaleName[]);

int32 DllExport __CFUNC     DAQmxBaseCreateCOPulseChanFreq        (TaskHandle taskHandle, const char counter[], const char nameToAssignToChannel[], int32 units, int32 idleState, float64 initialDelay, float64 freq, float64 dutyCycle);



/******************************************************/
/***                    Timing                      ***/
/******************************************************/


int32 DllExport __CFUNC     DAQmxBaseCfgSampClkTiming          (TaskHandle taskHandle, const char source[], float64 rate, int32 activeEdge, int32 sampleMode, uInt64 sampsPerChan);
int32 DllExport __CFUNC     DAQmxBaseCfgImplicitTiming         (TaskHandle taskHandle, int32 sampleMode, uInt64 sampsPerChan);



/******************************************************/
/***                  Triggering                    ***/
/******************************************************/


int32 DllExport __CFUNC     DAQmxBaseDisableStartTrig      (TaskHandle taskHandle);
int32 DllExport __CFUNC     DAQmxBaseCfgDigEdgeStartTrig   (TaskHandle taskHandle, const char triggerSource[], int32 triggerEdge);
int32 DllExport __CFUNC     DAQmxBaseCfgAnlgEdgeStartTrig  (TaskHandle taskHandle, const char triggerSource[], int32 triggerSlope, float64 triggerLevel);

int32 DllExport __CFUNC     DAQmxBaseDisableRefTrig        (TaskHandle taskHandle);
int32 DllExport __CFUNC     DAQmxBaseCfgDigEdgeRefTrig     (TaskHandle taskHandle, const char triggerSource[], int32 triggerEdge, uInt32 pretriggerSamples);
int32 DllExport __CFUNC     DAQmxBaseCfgAnlgEdgeRefTrig    (TaskHandle taskHandle, const char triggerSource[], int32 triggerSlope, float64 triggerLevel, uInt32 pretriggerSamples);





/******************************************************/
/***                 Read Data                      ***/
/******************************************************/


int32 DllExport __CFUNC     DAQmxBaseReadAnalogF64         (TaskHandle taskHandle, int32 numSampsPerChan, float64 timeout, bool32 fillMode, float64 readArray[], uInt32 arraySizeInSamps, int32 *sampsPerChanRead, bool32 *reserved);

int32 DllExport __CFUNC     DAQmxBaseReadBinaryI16         (TaskHandle taskHandle, int32 numSampsPerChan, float64 timeout, bool32 fillMode, int16 readArray[], uInt32 arraySizeInSamps, int32 *sampsPerChanRead, bool32 *reserved);


int32 DllExport __CFUNC     DAQmxReadBinaryI32             (TaskHandle taskHandle, int32 numSampsPerChan, float64 timeout, bool32 fillMode, int32 readArray[], uInt32 arraySizeInSamps, int32 *sampsPerChanRead, bool32 *reserved);


int32 DllExport __CFUNC     DAQmxBaseReadDigitalU8         (TaskHandle taskHandle, int32 numSampsPerChan, float64 timeout, bool32 fillMode, uInt8 readArray[], uInt32 arraySizeInSamps, int32 *sampsPerChanRead, bool32 *reserved);
int32 DllExport __CFUNC     DAQmxBaseReadDigitalU32        (TaskHandle taskHandle, int32 numSampsPerChan, float64 timeout, bool32 fillMode, uInt32 readArray[], uInt32 arraySizeInSamps, int32 *sampsPerChanRead, bool32 *reserved);
int32 DllExport __CFUNC     DAQmxBaseReadDigitalScalarU32  (TaskHandle taskHandle, float64 timeout, uInt32 *value, bool32 *reserved);

int32 DllExport __CFUNC     DAQmxBaseReadCounterF64        (TaskHandle taskHandle, int32 numSampsPerChan, float64 timeout, float64 readArray[], uInt32 arraySizeInSamps, int32 *sampsPerChanRead, bool32 *reserved);
int32 DllExport __CFUNC     DAQmxBaseReadCounterU32        (TaskHandle taskHandle, int32 numSampsPerChan, float64 timeout, uInt32 readArray[], uInt32 arraySizeInSamps, int32 *sampsPerChanRead, bool32 *reserved);
int32 DllExport __CFUNC     DAQmxBaseReadCounterScalarF64  (TaskHandle taskHandle, float64 timeout, float64 *value, bool32 *reserved);
int32 DllExport __CFUNC     DAQmxBaseReadCounterScalarU32  (TaskHandle taskHandle, float64 timeout, uInt32 *value, bool32 *reserved);





/******************************************************/
/***                 Write Data                     ***/
/******************************************************/


int32 DllExport __CFUNC     DAQmxBaseWriteAnalogF64          (TaskHandle taskHandle, int32 numSampsPerChan, bool32 autoStart, float64 timeout, bool32 dataLayout, float64 writeArray[], int32 *sampsPerChanWritten, bool32 *reserved);


int32 DllExport __CFUNC     DAQmxBaseWriteDigitalU8          (TaskHandle taskHandle, int32 numSampsPerChan, bool32 autoStart, float64 timeout, bool32 dataLayout, uInt8 writeArray[], int32 *sampsPerChanWritten, bool32 *reserved);
int32 DllExport __CFUNC     DAQmxBaseWriteDigitalU32         (TaskHandle taskHandle, int32 numSampsPerChan, bool32 autoStart, float64 timeout, bool32 dataLayout, uInt32 writeArray[], int32 *sampsPerChanWritten, bool32 *reserved);
int32 DllExport __CFUNC     DAQmxBaseWriteDigitalScalarU32   (TaskHandle taskHandle, bool32 autoStart, float64 timeout, uInt32 value, bool32 *reserved);




/******************************************************/
/***               Events & Signals                 ***/
/******************************************************/





/******************************************************/
/***              Scale Configurations              ***/
/******************************************************/





/******************************************************/
/***             Buffer Configurations              ***/
/******************************************************/


int32 DllExport __CFUNC     DAQmxBaseCfgInputBuffer   (TaskHandle taskHandle, uInt32 numSampsPerChan);


/******************************************************/
/***                Switch Functions                ***/
/******************************************************/




#define DAQmx_Val_Switch_Topology_1127_1_Wire_64x1_Mux         "1127/1-Wire 64x1 Mux"           
#define DAQmx_Val_Switch_Topology_1127_2_Wire_32x1_Mux         "1127/2-Wire 32x1 Mux"           
#define DAQmx_Val_Switch_Topology_1127_2_Wire_4x8_Matrix       "1127/2-Wire 4x8 Matrix"         
#define DAQmx_Val_Switch_Topology_1127_4_Wire_16x1_Mux         "1127/4-Wire 16x1 Mux"           
#define DAQmx_Val_Switch_Topology_1127_Independent             "1127/Independent"               
#define DAQmx_Val_Switch_Topology_1128_1_Wire_64x1_Mux         "1128/1-Wire 64x1 Mux"           
#define DAQmx_Val_Switch_Topology_1128_2_Wire_32x1_Mux         "1128/2-Wire 32x1 Mux"           
#define DAQmx_Val_Switch_Topology_1128_2_Wire_4x8_Matrix       "1128/2-Wire 4x8 Matrix"         
#define DAQmx_Val_Switch_Topology_1128_4_Wire_16x1_Mux         "1128/4-Wire 16x1 Mux"           
#define DAQmx_Val_Switch_Topology_1128_Independent             "1128/Independent"               
#define DAQmx_Val_Switch_Topology_1129_2_Wire_16x16_Matrix     "1129/2-Wire 16x16 Matrix"       
#define DAQmx_Val_Switch_Topology_1129_2_Wire_8x32_Matrix      "1129/2-Wire 8x32 Matrix"        
#define DAQmx_Val_Switch_Topology_1129_2_Wire_4x64_Matrix      "1129/2-Wire 4x64 Matrix"        
#define DAQmx_Val_Switch_Topology_1129_2_Wire_Dual_8x16_Matrix "1129/2-Wire Dual 8x16 Matrix"   
#define DAQmx_Val_Switch_Topology_1129_2_Wire_Dual_4x32_Matrix "1129/2-Wire Dual 4x32 Matrix"   
#define DAQmx_Val_Switch_Topology_1129_2_Wire_Quad_4x16_Matrix "1129/2-Wire Quad 4x16 Matrix"   
#define DAQmx_Val_Switch_Topology_1130_1_Wire_256x1_Mux        "1130/1-Wire 256x1 Mux"          
#define DAQmx_Val_Switch_Topology_1130_2_Wire_128x1_Mux        "1130/2-Wire 128x1 Mux"          
#define DAQmx_Val_Switch_Topology_1130_4_Wire_64x1_Mux         "1130/4-Wire 64x1 Mux"           
#define DAQmx_Val_Switch_Topology_1130_1_Wire_4x64_Matrix      "1130/1-Wire 4x64 Matrix"        
#define DAQmx_Val_Switch_Topology_1130_1_Wire_8x32_Matrix      "1130/1-Wire 8x32 Matrix"        
#define DAQmx_Val_Switch_Topology_1130_2_Wire_4x32_Matrix      "1130/2-Wire 4x32 Matrix"        
#define DAQmx_Val_Switch_Topology_1130_Independent             "1130/Independent"               
#define DAQmx_Val_Switch_Topology_1160_16_SPDT                 "1160/16-SPDT"                   
#define DAQmx_Val_Switch_Topology_1161_8_SPDT                  "1161/8-SPDT"                    
#define DAQmx_Val_Switch_Topology_1163R_Octal_4x1_Mux          "1163R/Octal 4x1 Mux"            
#define DAQmx_Val_Switch_Topology_1166_32_SPDT                 "1166/32-SPDT"                   
#define DAQmx_Val_Switch_Topology_1167_Independent             "1167/Independent"               
#define DAQmx_Val_Switch_Topology_1190_Quad_4x1_Mux            "1190/Quad 4x1 Mux"              
#define DAQmx_Val_Switch_Topology_1191_Quad_4x1_Mux            "1191/Quad 4x1 Mux"              
#define DAQmx_Val_Switch_Topology_1192_8_SPDT                  "1192/8-SPDT"                    
#define DAQmx_Val_Switch_Topology_1193_32x1_Mux                "1193/32x1 Mux"                  
#define DAQmx_Val_Switch_Topology_1193_Dual_16x1_Mux           "1193/Dual 16x1 Mux"             
#define DAQmx_Val_Switch_Topology_1193_Quad_8x1_Mux            "1193/Quad 8x1 Mux"              
#define DAQmx_Val_Switch_Topology_1193_16x1_Terminated_Mux     "1193/16x1 Terminated Mux"       
#define DAQmx_Val_Switch_Topology_1193_Dual_8x1_Terminated_Mux "1193/Dual 8x1 Terminated Mux"   
#define DAQmx_Val_Switch_Topology_1193_Quad_4x1_Terminated_Mux "1193/Quad 4x1 Terminated Mux"   
#define DAQmx_Val_Switch_Topology_1193_Independent             "1193/Independent"               
#define DAQmx_Val_Switch_Topology_2529_2_Wire_8x16_Matrix      "2529/2-Wire 8x16 Matrix"        
#define DAQmx_Val_Switch_Topology_2529_2_Wire_4x32_Matrix      "2529/2-Wire 4x32 Matrix"        
#define DAQmx_Val_Switch_Topology_2529_2_Wire_Dual_4x16_Matrix "2529/2-Wire Dual 4x16 Matrix"   
#define DAQmx_Val_Switch_Topology_2530_1_Wire_128x1_Mux        "2530/1-Wire 128x1 Mux"          
#define DAQmx_Val_Switch_Topology_2530_2_Wire_64x1_Mux         "2530/2-Wire 64x1 Mux"           
#define DAQmx_Val_Switch_Topology_2530_4_Wire_32x1_Mux         "2530/4-Wire 32x1 Mux"           
#define DAQmx_Val_Switch_Topology_2530_1_Wire_4x32_Matrix      "2530/1-Wire 4x32 Matrix"        
#define DAQmx_Val_Switch_Topology_2530_1_Wire_8x16_Matrix      "2530/1-Wire 8x16 Matrix"        
#define DAQmx_Val_Switch_Topology_2530_2_Wire_4x16_Matrix      "2530/2-Wire 4x16 Matrix"        
#define DAQmx_Val_Switch_Topology_2530_Independent             "2530/Independent"               
#define DAQmx_Val_Switch_Topology_2566_16_SPDT                 "2566/16-SPDT"                   
#define DAQmx_Val_Switch_Topology_2567_Independent             "2567/Independent"               
#define DAQmx_Val_Switch_Topology_2570_40_SPDT                 "2570/40-SPDT"                   
#define DAQmx_Val_Switch_Topology_2593_16x1_Mux                "2593/16x1 Mux"                  
#define DAQmx_Val_Switch_Topology_2593_Dual_8x1_Mux            "2593/Dual 8x1 Mux"              
#define DAQmx_Val_Switch_Topology_2593_8x1_Terminated_Mux      "2593/8x1 Terminated Mux"        
#define DAQmx_Val_Switch_Topology_2593_Dual_4x1_Terminated_Mux "2593/Dual 4x1 Terminated Mux"   
#define DAQmx_Val_Switch_Topology_2593_Independent             "2593/Independent"               










/******************************************************/
/***                Signal Routing                  ***/
/******************************************************/




/******************************************************/
/***                Device Control                  ***/
/******************************************************/


int32 DllExport __CFUNC     DAQmxBaseResetDevice              (const char deviceName[]);


/******************************************************/
/***              Watchdog Timer                    ***/
/******************************************************/





/******************************************************/
/***                 Calibration                    ***/
/******************************************************/







/******************************************************/
/***              System Configuration              ***/
/******************************************************/


/******************************************************/
/***                 Error Handling                 ***/
/******************************************************/


int32 DllExport __CFUNC     DAQmxBaseGetExtendedErrorInfo (char errorString[], uInt32 bufferSize);


/******************************************************************************
 *** NI-DAQmxBase Specific Attribute Get/Set/Reset Function Declarations **********
 ******************************************************************************/




int32 DllExport __CFUNC DAQmxBaseGetDevSerialNum(const char device[], uInt32 *data);













/******************************************************************************
 *** NI-DAQmx Error Codes *****************************************************
 ******************************************************************************/

#define DAQmxSuccess                                  (0)

#define DAQmxFailed(error)                            ((error)!=0)

#define DAQmxErrorInvalidInstallation                                          (-200683)
#define DAQmxErrorRefTrigMasterSessionUnavailable                              (-200682)
#define DAQmxErrorRouteFailedBecauseWatchdogExpired                            (-200681)
#define DAQmxErrorDeviceShutDownDueToHighTemp                                  (-200680)
#define DAQmxErrorNoMemMapWhenHWTimedSinglePoint                               (-200679)
#define DAQmxErrorWriteFailedBecauseWatchdogExpired                            (-200678)
#define DAQmxErrorDifftInternalAIInputSrcs                                     (-200677)
#define DAQmxErrorDifftAIInputSrcInOneChanGroup                                (-200676)
#define DAQmxErrorInternalAIInputSrcInMultChanGroups                           (-200675)
#define DAQmxErrorSwitchOpFailedDueToPrevError                                 (-200674)
#define DAQmxErrorWroteMultiSampsUsingSingleSampWrite                          (-200673)
#define DAQmxErrorMismatchedInputArraySizes                                    (-200672)
#define DAQmxErrorCantExceedRelayDriveLimit                                    (-200671)
#define DAQmxErrorDACRngLowNotEqualToMinusRefVal                               (-200670)
#define DAQmxErrorCantAllowConnectDACToGnd                                     (-200669)
#define DAQmxErrorWatchdogTimeoutOutOfRangeAndNotSpecialVal                    (-200668)
#define DAQmxErrorNoWatchdogOutputOnPortReservedForInput                       (-200667)
#define DAQmxErrorNoInputOnPortCfgdForWatchdogOutput                           (-200666)
#define DAQmxErrorWatchdogExpirationStateNotEqualForLinesInPort                (-200665)
#define DAQmxErrorCannotPerformOpWhenTaskNotReserved                           (-200664)
#define DAQmxErrorPowerupStateNotSupported                                     (-200663)
#define DAQmxErrorWatchdogTimerNotSupported                                    (-200662)
#define DAQmxErrorOpNotSupportedWhenRefClkSrcNone                              (-200661)
#define DAQmxErrorSampClkRateUnavailable                                       (-200660)
#define DAQmxErrorPrptyGetSpecdSingleActiveChanFailedDueToDifftVals            (-200659)
#define DAQmxErrorPrptyGetImpliedActiveChanFailedDueToDifftVals                (-200658)
#define DAQmxErrorPrptyGetSpecdActiveChanFailedDueToDifftVals                  (-200657)
#define DAQmxErrorNoRegenWhenUsingBrdMem                                       (-200656)
#define DAQmxErrorNonbufferedReadMoreThanSampsPerChan                          (-200655)
#define DAQmxErrorWatchdogExpirationTristateNotSpecdForEntirePort              (-200654)
#define DAQmxErrorPowerupTristateNotSpecdForEntirePort                         (-200653)
#define DAQmxErrorPowerupStateNotSpecdForEntirePort                            (-200652)
#define DAQmxErrorCantSetWatchdogExpirationOnDigInChan                         (-200651)
#define DAQmxErrorCantSetPowerupStateOnDigInChan                               (-200650)
#define DAQmxErrorPhysChanNotInTask                                            (-200649)
#define DAQmxErrorPhysChanDevNotInTask                                         (-200648)
#define DAQmxErrorDigInputNotSupported                                         (-200647)
#define DAQmxErrorDigFilterIntervalNotEqualForLines                            (-200646)
#define DAQmxErrorDigFilterIntervalAlreadyCfgd                                 (-200645)
#define DAQmxErrorCantResetExpiredWatchdog                                     (-200644)
#define DAQmxErrorActiveChanTooManyLinesSpecdWhenGettingPrpty                  (-200643)
#define DAQmxErrorActiveChanNotSpecdWhenGetting1LinePrpty                      (-200642)
#define DAQmxErrorDigPrptyCannotBeSetPerLine                                   (-200641)
#define DAQmxErrorSendAdvCmpltAfterWaitForTrigInScanlist                       (-200640)
#define DAQmxErrorDisconnectionRequiredInScanlist                              (-200639)
#define DAQmxErrorTwoWaitForTrigsAfterConnectionInScanlist                     (-200638)
#define DAQmxErrorActionSeparatorRequiredAfterBreakingConnectionInScanlist     (-200637)
#define DAQmxErrorConnectionInScanlistMustWaitForTrig                          (-200636)
#define DAQmxErrorActionNotSupportedTaskNotWatchdog                            (-200635)
#define DAQmxErrorWfmNameSameAsScriptName                                      (-200634)
#define DAQmxErrorScriptNameSameAsWfmName                                      (-200633)
#define DAQmxErrorDSFStopClock                                                 (-200632)
#define DAQmxErrorDSFReadyForStartClock                                        (-200631)
#define DAQmxErrorWriteOffsetNotMultOfIncr                                     (-200630)
#define DAQmxErrorDifferentPrptyValsNotSupportedOnDev                          (-200629)
#define DAQmxErrorRefAndPauseTrigConfigured                                    (-200628)
#define DAQmxErrorFailedToEnableHighSpeedInputClock                            (-200627)
#define DAQmxErrorEmptyPhysChanInPowerUpStatesArray                            (-200626)
#define DAQmxErrorActivePhysChanTooManyLinesSpecdWhenGettingPrpty              (-200625)
#define DAQmxErrorActivePhysChanNotSpecdWhenGetting1LinePrpty                  (-200624)
#define DAQmxErrorPXIDevTempCausedShutDown                                     (-200623)
#define DAQmxErrorInvalidNumSampsToWrite                                       (-200622)
#define DAQmxErrorOutputFIFOUnderflow2                                         (-200621)
#define DAQmxErrorRepeatedAIPhysicalChan                                       (-200620)
#define DAQmxErrorMultScanOpsInOneChassis                                      (-200619)
#define DAQmxErrorInvalidAIChanOrder                                           (-200618)
#define DAQmxErrorReversePowerProtectionActivated                              (-200617)
#define DAQmxErrorInvalidAsynOpHandle                                          (-200616)
#define DAQmxErrorFailedToEnableHighSpeedOutput                                (-200615)
#define DAQmxErrorCannotReadPastEndOfRecord                                    (-200614)
#define DAQmxErrorAcqStoppedToPreventInputBufferOverwriteOneDataXferMech       (-200613)
#define DAQmxErrorZeroBasedChanIndexInvalid                                    (-200612)
#define DAQmxErrorNoChansOfGivenTypeInTask                                     (-200611)
#define DAQmxErrorSampClkSrcInvalidForOutputValidForInput                      (-200610)
#define DAQmxErrorOutputBufSizeTooSmallToStartGen                              (-200609)
#define DAQmxErrorInputBufSizeTooSmallToStartAcq                               (-200608)
#define DAQmxErrorExportTwoSignalsOnSameTerminal                               (-200607)
#define DAQmxErrorChanIndexInvalid                                             (-200606)
#define DAQmxErrorRangeSyntaxNumberTooBig                                      (-200605)
#define DAQmxErrorNULLPtr                                                      (-200604)
#define DAQmxErrorScaledMinEqualMax                                            (-200603)
#define DAQmxErrorPreScaledMinEqualMax                                         (-200602)
#define DAQmxErrorPropertyNotSupportedForScaleType                             (-200601)
#define DAQmxErrorChannelNameGenerationNumberTooBig                            (-200600)
#define DAQmxErrorRepeatedNumberInScaledValues                                 (-200599)
#define DAQmxErrorRepeatedNumberInPreScaledValues                              (-200598)
#define DAQmxErrorLinesAlreadyReservedForOutput                                (-200597)
#define DAQmxErrorSwitchOperationChansSpanMultipleDevsInList                   (-200596)
#define DAQmxErrorInvalidIDInListAtBeginningOfSwitchOperation                  (-200595)
#define DAQmxErrorMStudioInvalidPolyDirection                                  (-200594)
#define DAQmxErrorMStudioPropertyGetWhileTaskNotVerified                       (-200593)
#define DAQmxErrorRangeWithTooManyObjects                                      (-200592)
#define DAQmxErrorCppDotNetAPINegativeBufferSize                               (-200591)
#define DAQmxErrorCppCantRemoveInvalidEventHandler                             (-200590)
#define DAQmxErrorCppCantRemoveEventHandlerTwice                               (-200589)
#define DAQmxErrorCppCantRemoveOtherObjectsEventHandler                        (-200588)
#define DAQmxErrorDigLinesReservedOrUnavailable                                (-200587)
#define DAQmxErrorDSFFailedToResetStream                                       (-200586)
#define DAQmxErrorDSFReadyForOutputNotAsserted                                 (-200585)
#define DAQmxErrorSampToWritePerChanNotMultipleOfIncr                          (-200584)
#define DAQmxErrorAOPropertiesCauseVoltageBelowMin                             (-200583)
#define DAQmxErrorAOPropertiesCauseVoltageOverMax                              (-200582)
#define DAQmxErrorPropertyNotSupportedWhenRefClkSrcNone                        (-200581)
#define DAQmxErrorAIMaxTooSmall                                                (-200580)
#define DAQmxErrorAIMaxTooLarge                                                (-200579)
#define DAQmxErrorAIMinTooSmall                                                (-200578)
#define DAQmxErrorAIMinTooLarge                                                (-200577)
#define DAQmxErrorBuiltInCJCSrcNotSupported                                    (-200576)
#define DAQmxErrorTooManyPostTrigSampsPerChan                                  (-200575)
#define DAQmxErrorTrigLineNotFoundSingleDevRoute                               (-200574)
#define DAQmxErrorDifferentInternalAIInputSources                              (-200573)
#define DAQmxErrorDifferentAIInputSrcInOneChanGroup                            (-200572)
#define DAQmxErrorInternalAIInputSrcInMultipleChanGroups                       (-200571)
#define DAQmxErrorCAPIChanIndexInvalid                                         (-200570)
#define DAQmxErrorCollectionDoesNotMatchChanType                               (-200569)
#define DAQmxErrorOutputCantStartChangedRegenerationMode                       (-200568)
#define DAQmxErrorOutputCantStartChangedBufferSize                             (-200567)
#define DAQmxErrorChanSizeTooBigForU32PortWrite                                (-200566)
#define DAQmxErrorChanSizeTooBigForU8PortWrite                                 (-200565)
#define DAQmxErrorChanSizeTooBigForU32PortRead                                 (-200564)
#define DAQmxErrorChanSizeTooBigForU8PortRead                                  (-200563)
#define DAQmxErrorInvalidDigDataWrite                                          (-200562)
#define DAQmxErrorInvalidAODataWrite                                           (-200561)
#define DAQmxErrorWaitUntilDoneDoesNotIndicateDone                             (-200560)
#define DAQmxErrorMultiChanTypesInTask                                         (-200559)
#define DAQmxErrorMultiDevsInTask                                              (-200558)
#define DAQmxErrorCannotSetPropertyWhenTaskRunning                             (-200557)
#define DAQmxErrorCannotGetPropertyWhenTaskNotCommittedOrRunning               (-200556)
#define DAQmxErrorLeadingUnderscoreInString                                    (-200555)
#define DAQmxErrorTrailingSpaceInString                                        (-200554)
#define DAQmxErrorLeadingSpaceInString                                         (-200553)
#define DAQmxErrorInvalidCharInString                                          (-200552)
#define DAQmxErrorDLLBecameUnlocked                                            (-200551)
#define DAQmxErrorDLLLock                                                      (-200550)
#define DAQmxErrorSelfCalConstsInvalid                                         (-200549)
#define DAQmxErrorInvalidTrigCouplingExceptForExtTrigChan                      (-200548)
#define DAQmxErrorWriteFailsBufferSizeAutoConfigured                           (-200547)
#define DAQmxErrorExtCalAdjustExtRefVoltageFailed                              (-200546)
#define DAQmxErrorSelfCalFailedExtNoiseOrRefVoltageOutOfCal                    (-200545)
#define DAQmxErrorExtCalTemperatureNotDAQmx                                    (-200544)
#define DAQmxErrorExtCalDateTimeNotDAQmx                                       (-200543)
#define DAQmxErrorSelfCalTemperatureNotDAQmx                                   (-200542)
#define DAQmxErrorSelfCalDateTimeNotDAQmx                                      (-200541)
#define DAQmxErrorDACRefValNotSet                                              (-200540)
#define DAQmxErrorAnalogMultiSampWriteNotSupported                             (-200539)
#define DAQmxErrorInvalidActionInControlTask                                   (-200538)
#define DAQmxErrorPolyCoeffsInconsistent                                       (-200537)
#define DAQmxErrorSensorValTooLow                                              (-200536)
#define DAQmxErrorSensorValTooHigh                                             (-200535)
#define DAQmxErrorWaveformNameTooLong                                          (-200534)
#define DAQmxErrorIdentifierTooLongInScript                                    (-200533)
#define DAQmxErrorUnexpectedIDFollowingSwitchChanName                          (-200532)
#define DAQmxErrorRelayNameNotSpecifiedInList                                  (-200531)
#define DAQmxErrorUnexpectedIDFollowingRelayNameInList                         (-200530)
#define DAQmxErrorUnexpectedIDFollowingSwitchOpInList                          (-200529)
#define DAQmxErrorInvalidLineGrouping                                          (-200528)
#define DAQmxErrorCtrMinMax                                                    (-200527)
#define DAQmxErrorWriteChanTypeMismatch                                        (-200526)
#define DAQmxErrorReadChanTypeMismatch                                         (-200525)
#define DAQmxErrorWriteNumChansMismatch                                        (-200524)
#define DAQmxErrorOneChanReadForMultiChanTask                                  (-200523)
#define DAQmxErrorCannotSelfCalDuringExtCal                                    (-200522)
#define DAQmxErrorMeasCalAdjustOscillatorPhaseDAC                              (-200521)
#define DAQmxErrorInvalidCalConstCalADCAdjustment                              (-200520)
#define DAQmxErrorInvalidCalConstOscillatorFreqDACValue                        (-200519)
#define DAQmxErrorInvalidCalConstOscillatorPhaseDACValue                       (-200518)
#define DAQmxErrorInvalidCalConstOffsetDACValue                                (-200517)
#define DAQmxErrorInvalidCalConstGainDACValue                                  (-200516)
#define DAQmxErrorInvalidNumCalADCReadsToAverage                               (-200515)
#define DAQmxErrorInvalidCfgCalAdjustDirectPathOutputImpedance                 (-200514)
#define DAQmxErrorInvalidCfgCalAdjustMainPathOutputImpedance                   (-200513)
#define DAQmxErrorInvalidCfgCalAdjustMainPathPostAmpGainAndOffset              (-200512)
#define DAQmxErrorInvalidCfgCalAdjustMainPathPreAmpGain                        (-200511)
#define DAQmxErrorInvalidCfgCalAdjustMainPreAmpOffset                          (-200510)
#define DAQmxErrorMeasCalAdjustCalADC                                          (-200509)
#define DAQmxErrorMeasCalAdjustOscillatorFrequency                             (-200508)
#define DAQmxErrorMeasCalAdjustDirectPathOutputImpedance                       (-200507)
#define DAQmxErrorMeasCalAdjustMainPathOutputImpedance                         (-200506)
#define DAQmxErrorMeasCalAdjustDirectPathGain                                  (-200505)
#define DAQmxErrorMeasCalAdjustMainPathPostAmpGainAndOffset                    (-200504)
#define DAQmxErrorMeasCalAdjustMainPathPreAmpGain                              (-200503)
#define DAQmxErrorMeasCalAdjustMainPathPreAmpOffset                            (-200502)
#define DAQmxErrorInvalidDateTimeInEEPROM                                      (-200501)
#define DAQmxErrorUnableToLocateErrorResources                                 (-200500)
#define DAQmxErrorDotNetAPINotUnsigned32BitNumber                              (-200499)
#define DAQmxErrorInvalidRangeOfObjectsSyntaxInString                          (-200498)
#define DAQmxErrorAttemptToEnableLineNotPreviouslyDisabled                     (-200497)
#define DAQmxErrorInvalidCharInPattern                                         (-200496)
#define DAQmxErrorIntermediateBufferFull                                       (-200495)
#define DAQmxErrorLoadTaskFailsBecauseNoTimingOnDev                            (-200494)
#define DAQmxErrorCAPIReservedParamNotNULLNorEmpty                             (-200493)
#define DAQmxErrorCAPIReservedParamNotNULL                                     (-200492)
#define DAQmxErrorCAPIReservedParamNotZero                                     (-200491)
#define DAQmxErrorSampleValueOutOfRange                                        (-200490)
#define DAQmxErrorChanAlreadyInTask                                            (-200489)
#define DAQmxErrorVirtualChanDoesNotExist                                      (-200488)
#define DAQmxErrorChanNotInTask                                                (-200486)
#define DAQmxErrorTaskNotInDataNeighborhood                                    (-200485)
#define DAQmxErrorCantSaveTaskWithoutReplace                                   (-200484)
#define DAQmxErrorCantSaveChanWithoutReplace                                   (-200483)
#define DAQmxErrorDevNotInTask                                                 (-200482)
#define DAQmxErrorDevAlreadyInTask                                             (-200481)
#define DAQmxErrorCanNotPerformOpWhileTaskRunning                              (-200479)
#define DAQmxErrorCanNotPerformOpWhenNoChansInTask                             (-200478)
#define DAQmxErrorCanNotPerformOpWhenNoDevInTask                               (-200477)
#define DAQmxErrorCannotPerformOpWhenTaskNotRunning                            (-200475)
#define DAQmxErrorOperationTimedOut                                            (-200474)
#define DAQmxErrorCannotReadWhenAutoStartFalseAndTaskNotRunningOrCommitted     (-200473)
#define DAQmxErrorCannotWriteWhenAutoStartFalseAndTaskNotRunningOrCommitted    (-200472)
#define DAQmxErrorTaskVersionNew                                               (-200470)
#define DAQmxErrorChanVersionNew                                               (-200469)
#define DAQmxErrorEmptyString                                                  (-200467)
#define DAQmxErrorChannelSizeTooBigForPortReadType                             (-200466)
#define DAQmxErrorChannelSizeTooBigForPortWriteType                            (-200465)
#define DAQmxErrorExpectedNumberOfChannelsVerificationFailed                   (-200464)
#define DAQmxErrorNumLinesMismatchInReadOrWrite                                (-200463)
#define DAQmxErrorOutputBufferEmpty                                            (-200462)
#define DAQmxErrorInvalidChanName                                              (-200461)
#define DAQmxErrorReadNoInputChansInTask                                       (-200460)
#define DAQmxErrorWriteNoOutputChansInTask                                     (-200459)
#define DAQmxErrorPropertyNotSupportedNotInputTask                             (-200457)
#define DAQmxErrorPropertyNotSupportedNotOutputTask                            (-200456)
#define DAQmxErrorGetPropertyNotInputBufferedTask                              (-200455)
#define DAQmxErrorGetPropertyNotOutputBufferedTask                             (-200454)
#define DAQmxErrorInvalidTimeoutVal                                            (-200453)
#define DAQmxErrorAttributeNotSupportedInTaskContext                           (-200452)
#define DAQmxErrorAttributeNotQueryableUnlessTaskIsCommitted                   (-200451)
#define DAQmxErrorAttributeNotSettableWhenTaskIsRunning                        (-200450)
#define DAQmxErrorDACRngLowNotMinusRefValNorZero                               (-200449)
#define DAQmxErrorDACRngHighNotEqualRefVal                                     (-200448)
#define DAQmxErrorUnitsNotFromCustomScale                                      (-200447)
#define DAQmxErrorInvalidVoltageReadingDuringExtCal                            (-200446)
#define DAQmxErrorCalFunctionNotSupported                                      (-200445)
#define DAQmxErrorInvalidPhysicalChanForCal                                    (-200444)
#define DAQmxErrorExtCalNotComplete                                            (-200443)
#define DAQmxErrorCantSyncToExtStimulusFreqDuringCal                           (-200442)
#define DAQmxErrorUnableToDetectExtStimulusFreqDuringCal                       (-200441)
#define DAQmxErrorInvalidCloseAction                                           (-200440)
#define DAQmxErrorExtCalFunctionOutsideExtCalSession                           (-200439)
#define DAQmxErrorInvalidCalArea                                               (-200438)
#define DAQmxErrorExtCalConstsInvalid                                          (-200437)
#define DAQmxErrorStartTrigDelayWithExtSampClk                                 (-200436)
#define DAQmxErrorDelayFromSampClkWithExtConv                                  (-200435)
#define DAQmxErrorFewerThan2PreScaledVals                                      (-200434)
#define DAQmxErrorFewerThan2ScaledValues                                       (-200433)
#define DAQmxErrorPhysChanOutputType                                           (-200432)
#define DAQmxErrorPhysChanMeasType                                             (-200431)
#define DAQmxErrorInvalidPhysChanType                                          (-200430)
#define DAQmxErrorLabVIEWEmptyTaskOrChans                                      (-200429)
#define DAQmxErrorLabVIEWInvalidTaskOrChans                                    (-200428)
#define DAQmxErrorInvalidRefClkRate                                            (-200427)
#define DAQmxErrorInvalidExtTrigImpedance                                      (-200426)
#define DAQmxErrorHystTrigLevelAIMax                                           (-200425)
#define DAQmxErrorLineNumIncompatibleWithVideoSignalFormat                     (-200424)
#define DAQmxErrorTrigWindowAIMinAIMaxCombo                                    (-200423)
#define DAQmxErrorTrigAIMinAIMax                                               (-200422)
#define DAQmxErrorHystTrigLevelAIMin                                           (-200421)
#define DAQmxErrorInvalidSampRateConsiderRIS                                   (-200420)
#define DAQmxErrorInvalidReadPosDuringRIS                                      (-200419)
#define DAQmxErrorImmedTrigDuringRISMode                                       (-200418)
#define DAQmxErrorTDCNotEnabledDuringRISMode                                   (-200417)
#define DAQmxErrorMultiRecWithRIS                                              (-200416)
#define DAQmxErrorInvalidRefClkSrc                                             (-200415)
#define DAQmxErrorInvalidSampClkSrc                                            (-200414)
#define DAQmxErrorInsufficientOnBoardMemForNumRecsAndSamps                     (-200413)
#define DAQmxErrorInvalidAIAttenuation                                         (-200412)
#define DAQmxErrorACCouplingNotAllowedWith50OhmImpedance                       (-200411)
#define DAQmxErrorInvalidRecordNum                                             (-200410)
#define DAQmxErrorZeroSlopeLinearScale                                         (-200409)
#define DAQmxErrorZeroReversePolyScaleCoeffs                                   (-200408)
#define DAQmxErrorZeroForwardPolyScaleCoeffs                                   (-200407)
#define DAQmxErrorNoReversePolyScaleCoeffs                                     (-200406)
#define DAQmxErrorNoForwardPolyScaleCoeffs                                     (-200405)
#define DAQmxErrorNoPolyScaleCoeffs                                            (-200404)
#define DAQmxErrorReversePolyOrderLessThanNumPtsToCompute                      (-200403)
#define DAQmxErrorReversePolyOrderNotPositive                                  (-200402)
#define DAQmxErrorNumPtsToComputeNotPositive                                   (-200401)
#define DAQmxErrorWaveformLengthNotMultipleOfIncr                              (-200400)
#define DAQmxErrorCAPINoExtendedErrorInfoAvailable                             (-200399)
#define DAQmxErrorCVIFunctionNotFoundInDAQmxDLL                                (-200398)
#define DAQmxErrorCVIFailedToLoadDAQmxDLL                                      (-200397)
#define DAQmxErrorNoCommonTrigLineForImmedRoute                                (-200396)
#define DAQmxErrorNoCommonTrigLineForTaskRoute                                 (-200395)
#define DAQmxErrorF64PrptyValNotUnsignedInt                                    (-200394)
#define DAQmxErrorRegisterNotWritable                                          (-200393)
#define DAQmxErrorInvalidOutputVoltageAtSampClkRate                            (-200392)
#define DAQmxErrorStrobePhaseShiftDCMBecameUnlocked                            (-200391)
#define DAQmxErrorDrivePhaseShiftDCMBecameUnlocked                             (-200390)
#define DAQmxErrorClkOutPhaseShiftDCMBecameUnlocked                            (-200389)
#define DAQmxErrorOutputBoardClkDCMBecameUnlocked                              (-200388)
#define DAQmxErrorInputBoardClkDCMBecameUnlocked                               (-200387)
#define DAQmxErrorInternalClkDCMBecameUnlocked                                 (-200386)
#define DAQmxErrorDCMLock                                                      (-200385)
#define DAQmxErrorDataLineReservedForDynamicOutput                             (-200384)
#define DAQmxErrorInvalidRefClkSrcGivenSampClkSrc                              (-200383)
#define DAQmxErrorNoPatternMatcherAvailable                                    (-200382)
#define DAQmxErrorInvalidDelaySampRateBelowPhaseShiftDCMThresh                 (-200381)
#define DAQmxErrorStrainGageCalibration                                        (-200380)
#define DAQmxErrorInvalidExtClockFreqAndDivCombo                               (-200379)
#define DAQmxErrorCustomScaleDoesNotExist                                      (-200378)
#define DAQmxErrorOnlyFrontEndChanOpsDuringScan                                (-200377)
#define DAQmxErrorInvalidOptionForDigitalPortChannel                           (-200376)
#define DAQmxErrorUnsupportedSignalTypeExportSignal                            (-200375)
#define DAQmxErrorInvalidSignalTypeExportSignal                                (-200374)
#define DAQmxErrorUnsupportedTrigTypeSendsSWTrig                               (-200373)
#define DAQmxErrorInvalidTrigTypeSendsSWTrig                                   (-200372)
#define DAQmxErrorRepeatedPhysicalChan                                         (-200371)
#define DAQmxErrorResourcesInUseForRouteInTask                                 (-200370)
#define DAQmxErrorResourcesInUseForRoute                                       (-200369)
#define DAQmxErrorRouteNotSupportedByHW                                        (-200368)
#define DAQmxErrorResourcesInUseForExportSignalPolarity                        (-200367)
#define DAQmxErrorResourcesInUseForInversionInTask                             (-200366)
#define DAQmxErrorResourcesInUseForInversion                                   (-200365)
#define DAQmxErrorExportSignalPolarityNotSupportedByHW                         (-200364)
#define DAQmxErrorInversionNotSupportedByHW                                    (-200363)
#define DAQmxErrorOverloadedChansExistNotRead                                  (-200362)
#define DAQmxErrorInputFIFOOverflow2                                           (-200361)
#define DAQmxErrorCJCChanNotSpecd                                              (-200360)
#define DAQmxErrorCtrExportSignalNotPossible                                   (-200359)
#define DAQmxErrorRefTrigWhenContinuous                                        (-200358)
#define DAQmxErrorIncompatibleSensorOutputAndDeviceInputRanges                 (-200357)
#define DAQmxErrorCustomScaleNameUsed                                          (-200356)
#define DAQmxErrorPropertyValNotSupportedByHW                                  (-200355)
#define DAQmxErrorPropertyValNotValidTermName                                  (-200354)
#define DAQmxErrorResourcesInUseForProperty                                    (-200353)
#define DAQmxErrorCJCChanAlreadyUsed                                           (-200352)
#define DAQmxErrorForwardPolynomialCoefNotSpecd                                (-200351)
#define DAQmxErrorTableScaleNumPreScaledAndScaledValsNotEqual                  (-200350)
#define DAQmxErrorTableScalePreScaledValsNotSpecd                              (-200349)
#define DAQmxErrorTableScaleScaledValsNotSpecd                                 (-200348)
#define DAQmxErrorIntermediateBufferSizeNotMultipleOfIncr                      (-200347)
#define DAQmxErrorEventPulseWidthOutOfRange                                    (-200346)
#define DAQmxErrorEventDelayOutOfRange                                         (-200345)
#define DAQmxErrorSampPerChanNotMultipleOfIncr                                 (-200344)
#define DAQmxErrorCannotCalculateNumSampsTaskNotStarted                        (-200343)
#define DAQmxErrorScriptNotInMem                                               (-200342)
#define DAQmxErrorOnboardMemTooSmall                                           (-200341)
#define DAQmxErrorReadAllAvailableDataWithoutBuffer                            (-200340)
#define DAQmxErrorPulseActiveAtStart                                           (-200339)
#define DAQmxErrorCalTempNotSupported                                          (-200338)
#define DAQmxErrorDelayFromSampClkTooLong                                      (-200337)
#define DAQmxErrorDelayFromSampClkTooShort                                     (-200336)
#define DAQmxErrorAIConvRateTooHigh                                            (-200335)
#define DAQmxErrorDelayFromStartTrigTooLong                                    (-200334)
#define DAQmxErrorDelayFromStartTrigTooShort                                   (-200333)
#define DAQmxErrorSampRateTooHigh                                              (-200332)
#define DAQmxErrorSampRateTooLow                                               (-200331)
#define DAQmxErrorPFI0UsedForAnalogAndDigitalSrc                               (-200330)
#define DAQmxErrorPrimingCfgFIFO                                               (-200329)
#define DAQmxErrorCannotOpenTopologyCfgFile                                    (-200328)
#define DAQmxErrorInvalidDTInsideWfmDataType                                   (-200327)
#define DAQmxErrorRouteSrcAndDestSame                                          (-200326)
#define DAQmxErrorReversePolynomialCoefNotSpecd                                (-200325)
#define DAQmxErrorDevAbsentOrUnavailable                                       (-200324)
#define DAQmxErrorNoAdvTrigForMultiDevScan                                     (-200323)
#define DAQmxErrorInterruptsInsufficientDataXferMech                           (-200322)
#define DAQmxErrorInvalidAttentuationBasedOnMinMax                             (-200321)
#define DAQmxErrorCabledModuleCannotRouteSSH                                   (-200320)
#define DAQmxErrorCabledModuleCannotRouteConvClk                               (-200319)
#define DAQmxErrorInvalidExcitValForScaling                                    (-200318)
#define DAQmxErrorNoDevMemForScript                                            (-200317)
#define DAQmxErrorScriptDataUnderflow                                          (-200316)
#define DAQmxErrorNoDevMemForWaveform                                          (-200315)
#define DAQmxErrorStreamDCMBecameUnlocked                                      (-200314)
#define DAQmxErrorStreamDCMLock                                                (-200313)
#define DAQmxErrorWaveformNotInMem                                             (-200312)
#define DAQmxErrorWaveformWriteOutOfBounds                                     (-200311)
#define DAQmxErrorWaveformPreviouslyAllocated                                  (-200310)
#define DAQmxErrorSampClkTbMasterTbDivNotAppropriateForSampTbSrc               (-200309)
#define DAQmxErrorSampTbRateSampTbSrcMismatch                                  (-200308)
#define DAQmxErrorMasterTbRateMasterTbSrcMismatch                              (-200307)
#define DAQmxErrorSampsPerChanTooBig                                           (-200306)
#define DAQmxErrorFinitePulseTrainNotPossible                                  (-200305)
#define DAQmxErrorExtMasterTimebaseRateNotSpecified                            (-200304)
#define DAQmxErrorExtSampClkSrcNotSpecified                                    (-200303)
#define DAQmxErrorInputSignalSlowerThanMeasTime                                (-200302)
#define DAQmxErrorCannotUpdatePulseGenProperty                                 (-200301)
#define DAQmxErrorInvalidTimingType                                            (-200300)
#define DAQmxErrorPropertyUnavailWhenUsingOnboardMemory                        (-200297)
#define DAQmxErrorCannotWriteAfterStartWithOnboardMemory                       (-200295)
#define DAQmxErrorNotEnoughSampsWrittenForInitialXferRqstCondition             (-200294)
#define DAQmxErrorNoMoreSpace                                                  (-200293)
#define DAQmxErrorSamplesCanNotYetBeWritten                                    (-200292)
#define DAQmxErrorGenStoppedToPreventIntermediateBufferRegenOfOldSamples       (-200291)
#define DAQmxErrorGenStoppedToPreventRegenOfOldSamples                         (-200290)
#define DAQmxErrorSamplesNoLongerWriteable                                     (-200289)
#define DAQmxErrorSamplesWillNeverBeGenerated                                  (-200288)
#define DAQmxErrorNegativeWriteSampleNumber                                    (-200287)
#define DAQmxErrorNoAcqStarted                                                 (-200286)
#define DAQmxErrorSamplesNotYetAvailable                                       (-200284)
#define DAQmxErrorAcqStoppedToPreventIntermediateBufferOverflow                (-200283)
#define DAQmxErrorNoRefTrigConfigured                                          (-200282)
#define DAQmxErrorCannotReadRelativeToRefTrigUntilDone                         (-200281)
#define DAQmxErrorSamplesNoLongerAvailable                                     (-200279)
#define DAQmxErrorSamplesWillNeverBeAvailable                                  (-200278)
#define DAQmxErrorNegativeReadSampleNumber                                     (-200277)
#define DAQmxErrorExternalSampClkAndRefClkThruSameTerm                         (-200276)
#define DAQmxErrorExtSampClkRateTooLowForClkIn                                 (-200275)
#define DAQmxErrorExtSampClkRateTooHighForBackplane                            (-200274)
#define DAQmxErrorSampClkRateAndDivCombo                                       (-200273)
#define DAQmxErrorSampClkRateTooLowForDivDown                                  (-200272)
#define DAQmxErrorProductOfAOMinAndGainTooSmall                                (-200271)
#define DAQmxErrorInterpolationRateNotPossible                                 (-200270)
#define DAQmxErrorOffsetTooLarge                                               (-200269)
#define DAQmxErrorOffsetTooSmall                                               (-200268)
#define DAQmxErrorProductOfAOMaxAndGainTooLarge                                (-200267)
#define DAQmxErrorMinAndMaxNotSymmetric                                        (-200266)
#define DAQmxErrorInvalidAnalogTrigSrc                                         (-200265)
#define DAQmxErrorTooManyChansForAnalogRefTrig                                 (-200264)
#define DAQmxErrorTooManyChansForAnalogPauseTrig                               (-200263)
#define DAQmxErrorTrigWhenOnDemandSampTiming                                   (-200262)
#define DAQmxErrorInconsistentAnalogTrigSettings                               (-200261)
#define DAQmxErrorMemMapDataXferModeSampTimingCombo                            (-200260)
#define DAQmxErrorInvalidJumperedAttr                                          (-200259)
#define DAQmxErrorInvalidGainBasedOnMinMax                                     (-200258)
#define DAQmxErrorInconsistentExcit                                            (-200257)
#define DAQmxErrorTopologyNotSupportedByCfgTermBlock                           (-200256)
#define DAQmxErrorBuiltInTempSensorNotSupported                                (-200255)
#define DAQmxErrorInvalidTerm                                                  (-200254)
#define DAQmxErrorCannotTristateTerm                                           (-200253)
#define DAQmxErrorCannotTristateBusyTerm                                       (-200252)
#define DAQmxErrorNoDMAChansAvailable                                          (-200251)
#define DAQmxErrorInvalidWaveformLengthWithinLoopInScript                      (-200250)
#define DAQmxErrorInvalidSubsetLengthWithinLoopInScript                        (-200249)
#define DAQmxErrorMarkerPosInvalidForLoopInScript                              (-200248)
#define DAQmxErrorIntegerExpectedInScript                                      (-200247)
#define DAQmxErrorPLLBecameUnlocked                                            (-200246)
#define DAQmxErrorPLLLock                                                      (-200245)
#define DAQmxErrorDDCClkOutDCMBecameUnlocked                                   (-200244)
#define DAQmxErrorDDCClkOutDCMLock                                             (-200243)
#define DAQmxErrorClkDoublerDCMBecameUnlocked                                  (-200242)
#define DAQmxErrorClkDoublerDCMLock                                            (-200241)
#define DAQmxErrorSampClkDCMBecameUnlocked                                     (-200240)
#define DAQmxErrorSampClkDCMLock                                               (-200239)
#define DAQmxErrorSampClkTimebaseDCMBecameUnlocked                             (-200238)
#define DAQmxErrorSampClkTimebaseDCMLock                                       (-200237)
#define DAQmxErrorAttrCannotBeReset                                            (-200236)
#define DAQmxErrorExplanationNotFound                                          (-200235)
#define DAQmxErrorWriteBufferTooSmall                                          (-200234)
#define DAQmxErrorSpecifiedAttrNotValid                                        (-200233)
#define DAQmxErrorAttrCannotBeRead                                             (-200232)
#define DAQmxErrorAttrCannotBeSet                                              (-200231)
#define DAQmxErrorNULLPtrForC_Api                                              (-200230)
#define DAQmxErrorReadBufferTooSmall                                           (-200229)
#define DAQmxErrorBufferTooSmallForString                                      (-200228)
#define DAQmxErrorNoAvailTrigLinesOnDevice                                     (-200227)
#define DAQmxErrorTrigBusLineNotAvail                                          (-200226)
#define DAQmxErrorCouldNotReserveRequestedTrigLine                             (-200225)
#define DAQmxErrorTrigLineNotFound                                             (-200224)
#define DAQmxErrorSCXI1126ThreshHystCombination                                (-200223)
#define DAQmxErrorAcqStoppedToPreventInputBufferOverwrite                      (-200222)
#define DAQmxErrorTimeoutExceeded                                              (-200221)
#define DAQmxErrorInvalidDeviceID                                              (-200220)
#define DAQmxErrorInvalidAOChanOrder                                           (-200219)
#define DAQmxErrorSampleTimingTypeAndDataXferMode                              (-200218)
#define DAQmxErrorBufferWithOnDemandSampTiming                                 (-200217)
#define DAQmxErrorBufferAndDataXferMode                                        (-200216)
#define DAQmxErrorMemMapAndBuffer                                              (-200215)
#define DAQmxErrorNoAnalogTrigHW                                               (-200214)
#define DAQmxErrorTooManyPretrigPlusMinPostTrigSamps                           (-200213)
#define DAQmxErrorInconsistentUnitsSpecified                                   (-200212)
#define DAQmxErrorMultipleRelaysForSingleRelayOp                               (-200211)
#define DAQmxErrorMultipleDevIDsPerChassisSpecifiedInList                      (-200210)
#define DAQmxErrorDuplicateDevIDInList                                         (-200209)
#define DAQmxErrorInvalidRangeStatementCharInList                              (-200208)
#define DAQmxErrorInvalidDeviceIDInList                                        (-200207)
#define DAQmxErrorTriggerPolarityConflict                                      (-200206)
#define DAQmxErrorCannotScanWithCurrentTopology                                (-200205)
#define DAQmxErrorUnexpectedIdentifierInFullySpecifiedPathInList               (-200204)
#define DAQmxErrorSwitchCannotDriveMultipleTrigLines                           (-200203)
#define DAQmxErrorInvalidRelayName                                             (-200202)
#define DAQmxErrorSwitchScanlistTooBig                                         (-200201)
#define DAQmxErrorSwitchChanInUse                                              (-200200)
#define DAQmxErrorSwitchNotResetBeforeScan                                     (-200199)
#define DAQmxErrorInvalidTopology                                              (-200198)
#define DAQmxErrorAttrNotSupported                                             (-200197)
#define DAQmxErrorUnexpectedEndOfActionsInList                                 (-200196)
#define DAQmxErrorPowerBudgetExceeded                                          (-200195)
#define DAQmxErrorHWUnexpectedlyPoweredOffAndOn                                (-200194)
#define DAQmxErrorSwitchOperationNotSupported                                  (-200193)
#define DAQmxErrorOnlyContinuousScanSupported                                  (-200192)
#define DAQmxErrorSwitchDifferentTopologyWhenScanning                          (-200191)
#define DAQmxErrorDisconnectPathNotSameAsExistingPath                          (-200190)
#define DAQmxErrorConnectionNotPermittedOnChanReservedForRouting               (-200189)
#define DAQmxErrorCannotConnectSrcChans                                        (-200188)
#define DAQmxErrorCannotConnectChannelToItself                                 (-200187)
#define DAQmxErrorChannelNotReservedForRouting                                 (-200186)
#define DAQmxErrorCannotConnectChansDirectly                                   (-200185)
#define DAQmxErrorChansAlreadyConnected                                        (-200184)
#define DAQmxErrorChanDuplicatedInPath                                         (-200183)
#define DAQmxErrorNoPathToDisconnect                                           (-200182)
#define DAQmxErrorInvalidSwitchChan                                            (-200181)
#define DAQmxErrorNoPathAvailableBetween2SwitchChans                           (-200180)
#define DAQmxErrorExplicitConnectionExists                                     (-200179)
#define DAQmxErrorSwitchDifferentSettlingTimeWhenScanning                      (-200178)
#define DAQmxErrorOperationOnlyPermittedWhileScanning                          (-200177)
#define DAQmxErrorOperationNotPermittedWhileScanning                           (-200176)
#define DAQmxErrorHardwareNotResponding                                        (-200175)
#define DAQmxErrorInvalidSampAndMasterTimebaseRateCombo                        (-200173)
#define DAQmxErrorNonZeroBufferSizeInProgIOXfer                                (-200172)
#define DAQmxErrorVirtualChanNameUsed                                          (-200171)
#define DAQmxErrorPhysicalChanDoesNotExist                                     (-200170)
#define DAQmxErrorMemMapOnlyForProgIOXfer                                      (-200169)
#define DAQmxErrorTooManyChans                                                 (-200168)
#define DAQmxErrorCannotHaveCJTempWithOtherChans                               (-200167)
#define DAQmxErrorOutputBufferUnderwrite                                       (-200166)
#define DAQmxErrorSensorInvalidCompletionResistance                            (-200163)
#define DAQmxErrorVoltageExcitIncompatibleWith2WireCfg                         (-200162)
#define DAQmxErrorIntExcitSrcNotAvailable                                      (-200161)
#define DAQmxErrorCannotCreateChannelAfterTaskVerified                         (-200160)
#define DAQmxErrorLinesReservedForSCXIControl                                  (-200159)
#define DAQmxErrorCouldNotReserveLinesForSCXIControl                           (-200158)
#define DAQmxErrorCalibrationFailed                                            (-200157)
#define DAQmxErrorReferenceFrequencyInvalid                                    (-200156)
#define DAQmxErrorReferenceResistanceInvalid                                   (-200155)
#define DAQmxErrorReferenceCurrentInvalid                                      (-200154)
#define DAQmxErrorReferenceVoltageInvalid                                      (-200153)
#define DAQmxErrorEEPROMDataInvalid                                            (-200152)
#define DAQmxErrorCabledModuleNotCapableOfRoutingAI                            (-200151)
#define DAQmxErrorChannelNotAvailableInParallelMode                            (-200150)
#define DAQmxErrorExternalTimebaseRateNotKnownForDelay                         (-200149)
#define DAQmxErrorFREQOUTCannotProduceDesiredFrequency                         (-200148)
#define DAQmxErrorMultipleCounterInputTask                                     (-200147)
#define DAQmxErrorCounterStartPauseTriggerConflict                             (-200146)
#define DAQmxErrorCounterInputPauseTriggerAndSampleClockInvalid                (-200145)
#define DAQmxErrorCounterOutputPauseTriggerInvalid                             (-200144)
#define DAQmxErrorCounterTimebaseRateNotSpecified                              (-200143)
#define DAQmxErrorCounterTimebaseRateNotFound                                  (-200142)
#define DAQmxErrorCounterOverflow                                              (-200141)
#define DAQmxErrorCounterNoTimebaseEdgesBetweenGates                           (-200140)
#define DAQmxErrorCounterMaxMinRangeFreq                                       (-200139)
#define DAQmxErrorCounterMaxMinRangeTime                                       (-200138)
#define DAQmxErrorSuitableTimebaseNotFoundTimeCombo                            (-200137)
#define DAQmxErrorSuitableTimebaseNotFoundFrequencyCombo                       (-200136)
#define DAQmxErrorInternalTimebaseSourceDivisorCombo                           (-200135)
#define DAQmxErrorInternalTimebaseSourceRateCombo                              (-200134)
#define DAQmxErrorInternalTimebaseRateDivisorSourceCombo                       (-200133)
#define DAQmxErrorExternalTimebaseRateNotknownForRate                          (-200132)
#define DAQmxErrorAnalogTrigChanNotFirstInScanList                             (-200131)
#define DAQmxErrorNoDivisorForExternalSignal                                   (-200130)
#define DAQmxErrorAttributeInconsistentAcrossRepeatedPhysicalChannels          (-200128)
#define DAQmxErrorCannotHandshakeWithPort0                                     (-200127)
#define DAQmxErrorControlLineConflictOnPortC                                   (-200126)
#define DAQmxErrorLines4To7ConfiguredForOutput                                 (-200125)
#define DAQmxErrorLines4To7ConfiguredForInput                                  (-200124)
#define DAQmxErrorLines0To3ConfiguredForOutput                                 (-200123)
#define DAQmxErrorLines0To3ConfiguredForInput                                  (-200122)
#define DAQmxErrorPortConfiguredForOutput                                      (-200121)
#define DAQmxErrorPortConfiguredForInput                                       (-200120)
#define DAQmxErrorPortConfiguredForStaticDigitalOps                            (-200119)
#define DAQmxErrorPortReservedForHandshaking                                   (-200118)
#define DAQmxErrorPortDoesNotSupportHandshakingDataIO                          (-200117)
#define DAQmxErrorCannotTristate8255OutputLines                                (-200116)
#define DAQmxErrorTemperatureOutOfRangeForCalibration                          (-200113)
#define DAQmxErrorCalibrationHandleInvalid                                     (-200112)
#define DAQmxErrorPasswordRequired                                             (-200111)
#define DAQmxErrorIncorrectPassword                                            (-200110)
#define DAQmxErrorPasswordTooLong                                              (-200109)
#define DAQmxErrorCalibrationSessionAlreadyOpen                                (-200108)
#define DAQmxErrorSCXIModuleIncorrect                                          (-200107)
#define DAQmxErrorAttributeInconsistentAcrossChannelsOnDevice                  (-200106)
#define DAQmxErrorSCXI1122ResistanceChanNotSupportedForCfg                     (-200105)
#define DAQmxErrorBracketPairingMismatchInList                                 (-200104)
#define DAQmxErrorInconsistentNumSamplesToWrite                                (-200103)
#define DAQmxErrorIncorrectDigitalPattern                                      (-200102)
#define DAQmxErrorIncorrectNumChannelsToWrite                                  (-200101)
#define DAQmxErrorIncorrectReadFunction                                        (-200100)
#define DAQmxErrorPhysicalChannelNotSpecified                                  (-200099)
#define DAQmxErrorMoreThanOneTerminal                                          (-200098)
#define DAQmxErrorMoreThanOneActiveChannelSpecified                            (-200097)
#define DAQmxErrorInvalidNumberSamplesToRead                                   (-200096)
#define DAQmxErrorAnalogWaveformExpected                                       (-200095)
#define DAQmxErrorDigitalWaveformExpected                                      (-200094)
#define DAQmxErrorActiveChannelNotSpecified                                    (-200093)
#define DAQmxErrorFunctionNotSupportedForDeviceTasks                           (-200092)
#define DAQmxErrorFunctionNotInLibrary                                         (-200091)
#define DAQmxErrorLibraryNotPresent                                            (-200090)
#define DAQmxErrorDuplicateTask                                                (-200089)
#define DAQmxErrorInvalidTask                                                  (-200088)
#define DAQmxErrorInvalidChannel                                               (-200087)
#define DAQmxErrorInvalidSyntaxForPhysicalChannelRange                         (-200086)
#define DAQmxErrorMinNotLessThanMax                                            (-200082)
#define DAQmxErrorSampleRateNumChansConvertPeriodCombo                         (-200081)
#define DAQmxErrorAODuringCounter1DMAConflict                                  (-200079)
#define DAQmxErrorAIDuringCounter0DMAConflict                                  (-200078)
#define DAQmxErrorInvalidAttributeValue                                        (-200077)
#define DAQmxErrorSuppliedCurrentDataOutsideSpecifiedRange                     (-200076)
#define DAQmxErrorSuppliedVoltageDataOutsideSpecifiedRange                     (-200075)
#define DAQmxErrorCannotStoreCalConst                                          (-200074)
#define DAQmxErrorSCXIModuleNotFound                                           (-200073)
#define DAQmxErrorDuplicatePhysicalChansNotSupported                           (-200072)
#define DAQmxErrorTooManyPhysicalChansInList                                   (-200071)
#define DAQmxErrorInvalidAdvanceEventTriggerType                               (-200070)
#define DAQmxErrorDeviceIsNotAValidSwitch                                      (-200069)
#define DAQmxErrorDeviceDoesNotSupportScanning                                 (-200068)
#define DAQmxErrorScanListCannotBeTimed                                        (-200067)
#define DAQmxErrorConnectOperatorInvalidAtPointInList                          (-200066)
#define DAQmxErrorUnexpectedSwitchActionInList                                 (-200065)
#define DAQmxErrorUnexpectedSeparatorInList                                    (-200064)
#define DAQmxErrorExpectedTerminatorInList                                     (-200063)
#define DAQmxErrorExpectedConnectOperatorInList                                (-200062)
#define DAQmxErrorExpectedSeparatorInList                                      (-200061)
#define DAQmxErrorFullySpecifiedPathInListContainsRange                        (-200060)
#define DAQmxErrorConnectionSeparatorAtEndOfList                               (-200059)
#define DAQmxErrorIdentifierInListTooLong                                      (-200058)
#define DAQmxErrorDuplicateDeviceIDInListWhenSettling                          (-200057)
#define DAQmxErrorChannelNameNotSpecifiedInList                                (-200056)
#define DAQmxErrorDeviceIDNotSpecifiedInList                                   (-200055)
#define DAQmxErrorSemicolonDoesNotFollowRangeInList                            (-200054)
#define DAQmxErrorSwitchActionInListSpansMultipleDevices                       (-200053)
#define DAQmxErrorRangeWithoutAConnectActionInList                             (-200052)
#define DAQmxErrorInvalidIdentifierFollowingSeparatorInList                    (-200051)
#define DAQmxErrorInvalidChannelNameInList                                     (-200050)
#define DAQmxErrorInvalidNumberInRepeatStatementInList                         (-200049)
#define DAQmxErrorInvalidTriggerLineInList                                     (-200048)
#define DAQmxErrorInvalidIdentifierInListFollowingDeviceID                     (-200047)
#define DAQmxErrorInvalidIdentifierInListAtEndOfSwitchAction                   (-200046)
#define DAQmxErrorDeviceRemoved                                                (-200045)
#define DAQmxErrorRoutingPathNotAvailable                                      (-200044)
#define DAQmxErrorRoutingHardwareBusy                                          (-200043)
#define DAQmxErrorRequestedSignalInversionForRoutingNotPossible                (-200042)
#define DAQmxErrorInvalidRoutingDestinationTerminalName                        (-200041)
#define DAQmxErrorInvalidRoutingSourceTerminalName                             (-200040)
#define DAQmxErrorRoutingNotSupportedForDevice                                 (-200039)
#define DAQmxErrorWaitIsLastInstructionOfLoopInScript                          (-200038)
#define DAQmxErrorClearIsLastInstructionOfLoopInScript                         (-200037)
#define DAQmxErrorInvalidLoopIterationsInScript                                (-200036)
#define DAQmxErrorRepeatLoopNestingTooDeepInScript                             (-200035)
#define DAQmxErrorMarkerPositionOutsideSubsetInScript                          (-200034)
#define DAQmxErrorSubsetStartOffsetNotAlignedInScript                          (-200033)
#define DAQmxErrorInvalidSubsetLengthInScript                                  (-200032)
#define DAQmxErrorMarkerPositionNotAlignedInScript                             (-200031)
#define DAQmxErrorSubsetOutsideWaveformInScript                                (-200030)
#define DAQmxErrorMarkerOutsideWaveformInScript                                (-200029)
#define DAQmxErrorWaveformInScriptNotInMem                                     (-200028)
#define DAQmxErrorKeywordExpectedInScript                                      (-200027)
#define DAQmxErrorBufferNameExpectedInScript                                   (-200026)
#define DAQmxErrorProcedureNameExpectedInScript                                (-200025)
#define DAQmxErrorScriptHasInvalidIdentifier                                   (-200024)
#define DAQmxErrorScriptHasInvalidCharacter                                    (-200023)
#define DAQmxErrorResourceAlreadyReserved                                      (-200022)
#define DAQmxErrorSelfTestFailed                                               (-200020)
#define DAQmxErrorADCOverrun                                                   (-200019)
#define DAQmxErrorDACUnderflow                                                 (-200018)
#define DAQmxErrorInputFIFOUnderflow                                           (-200017)
#define DAQmxErrorOutputFIFOUnderflow                                          (-200016)
#define DAQmxErrorSCXISerialCommunication                                      (-200015)
#define DAQmxErrorDigitalTerminalSpecifiedMoreThanOnce                         (-200014)
#define DAQmxErrorDigitalOutputNotSupported                                    (-200012)
#define DAQmxErrorInconsistentChannelDirections                                (-200011)
#define DAQmxErrorInputFIFOOverflow                                            (-200010)
#define DAQmxErrorTimeStampOverwritten                                         (-200009)
#define DAQmxErrorStopTriggerHasNotOccurred                                    (-200008)
#define DAQmxErrorRecordNotAvailable                                           (-200007)
#define DAQmxErrorRecordOverwritten                                            (-200006)
#define DAQmxErrorDataNotAvailable                                             (-200005)
#define DAQmxErrorDataOverwrittenInDeviceMemory                                (-200004)
#define DAQmxErrorDuplicatedChannel                                            (-200003)
#define DAQmxWarningTimestampCounterRolledOver                                  (200003)
#define DAQmxWarningInputTerminationOverloaded                                  (200004)
#define DAQmxWarningADCOverloaded                                               (200005)
#define DAQmxWarningPLLUnlocked                                                 (200007)
#define DAQmxWarningCounter0DMADuringAIConflict                                 (200008)
#define DAQmxWarningCounter1DMADuringAOConflict                                 (200009)
#define DAQmxWarningStoppedBeforeDone                                           (200010)
#define DAQmxWarningRateViolatesSettlingTime                                    (200011)
#define DAQmxWarningRateViolatesMaxADCRate                                      (200012)
#define DAQmxWarningUserDefInfoStringTooLong                                    (200013)
#define DAQmxWarningTooManyInterruptsPerSecond                                  (200014)
#define DAQmxWarningPotentialGlitchDuringWrite                                  (200015)
#define DAQmxWarningDevNotSelfCalibratedWithDAQmx                               (200016)
#define DAQmxWarningAISampRateTooLow                                            (200017)
#define DAQmxWarningAIConvRateTooLow                                            (200018)
#define DAQmxWarningReadOffsetCoercion                                          (200019)
#define DAQmxWarningPretrigCoercion                                             (200020)
#define DAQmxWarningSampValCoercedToMax                                         (200021)
#define DAQmxWarningSampValCoercedToMin                                         (200022)
#define DAQmxWarningPropertyVersionNew                                          (200024)
#define DAQmxWarningUserDefinedInfoTooLong                                      (200025)
#define DAQmxWarningCAPIStringTruncatedToFitBuffer                              (200026)
#define DAQmxWarningSampClkRateTooLow                                           (200027)
#define DAQmxWarningPossiblyInvalidCTRSampsInFiniteDMAAcq                       (200028)
#define DAQmxWarningRISAcqCompletedSomeBinsNotFilled                            (200029)
#define DAQmxWarningPXIDevTempExceedsMaxOpTemp                                  (200030)
#define DAQmxWarningOutputGainTooLowForRFFreq                                   (200031)
#define DAQmxWarningOutputGainTooHighForRFFreq                                  (200032)
#define DAQmxWarningMultipleWritesBetweenSampClks                               (200033)
#define DAQmxWarningDeviceMayShutDownDueToHighTemp                              (200034)
#define DAQmxWarningReadNotCompleteBeforeSampClk                                (209800)
#define DAQmxWarningWriteNotCompleteBeforeSampClk                               (209801)


#ifdef __cplusplus
   }
#endif

#endif 
