<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="14008000">
	<Item Name="My Computer" Type="My Computer">
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="DisableControls.vi" Type="VI" URL="../DisableControls.vi"/>
		<Item Name="Fault.ctl" Type="VI" URL="../Fault.ctl"/>
		<Item Name="FindPaneAndControl.vi" Type="VI" URL="../FindPaneAndControl.vi"/>
		<Item Name="FindRunningButton.vi" Type="VI" URL="../FindRunningButton.vi"/>
		<Item Name="GeneratePVNames.vi" Type="VI" URL="../GeneratePVNames.vi"/>
		<Item Name="GetFault.vi" Type="VI" URL="../GetFault.vi"/>
		<Item Name="GetMeasurements.vi" Type="VI" URL="../GetMeasurements.vi"/>
		<Item Name="GetPumpStatus.vi" Type="VI" URL="../GetPumpStatus.vi"/>
		<Item Name="GetPumpWarnings.vi" Type="VI" URL="../GetPumpWarnings.vi"/>
		<Item Name="Leybold.ico" Type="Document" URL="../Leybold.ico"/>
		<Item Name="LeyboldTurboGUI.vi" Type="VI" URL="../LeyboldTurboGUI.vi"/>
		<Item Name="Measusurements.ctl" Type="VI" URL="../Measusurements.ctl"/>
		<Item Name="PumpRecord.ctl" Type="VI" URL="../PumpRecord.ctl"/>
		<Item Name="PumpStates.ctl" Type="VI" URL="../PumpStates.ctl"/>
		<Item Name="RunStates.ctl" Type="VI" URL="../RunStates.ctl"/>
		<Item Name="SetPVValue.vi" Type="VI" URL="../SetPVValue.vi"/>
		<Item Name="SetRunningButton.vi" Type="VI" URL="../SetRunningButton.vi"/>
		<Item Name="SetTabPages.vi" Type="VI" URL="../SetTabPages.vi"/>
		<Item Name="TabControl.ctl" Type="VI" URL="../TabControl.ctl"/>
		<Item Name="Warnings.ctl" Type="VI" URL="../Warnings.ctl"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="user.lib" Type="Folder">
				<Item Name="caLab.dll" Type="Document" URL="/&lt;userlib&gt;/caLab/lib/caLab.dll"/>
				<Item Name="CaLabGet.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/CaLabGet.vi"/>
				<Item Name="CaLabGet_Main.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/CaLabGet_Main.vi"/>
				<Item Name="CaLabPut.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/CaLabPut.vi"/>
				<Item Name="CaLabPut_Main.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/CaLabPut_Main.vi"/>
				<Item Name="Get_PV-1D.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/Get_PV-1D.vi"/>
				<Item Name="Get_PV.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/Get_PV.vi"/>
				<Item Name="Put_Boolean-1D_PV-1D.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/Put_Boolean-1D_PV-1D.vi"/>
				<Item Name="Put_Boolean-1D_PV.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/Put_Boolean-1D_PV.vi"/>
				<Item Name="Put_Boolean-2D_PV-1D.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/Put_Boolean-2D_PV-1D.vi"/>
				<Item Name="Put_Boolean_PV.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/Put_Boolean_PV.vi"/>
				<Item Name="Put_DBL-1D_PV-1D.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/Put_DBL-1D_PV-1D.vi"/>
				<Item Name="Put_DBL-1D_PV.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/Put_DBL-1D_PV.vi"/>
				<Item Name="Put_DBL-2D_PV-1D.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/Put_DBL-2D_PV-1D.vi"/>
				<Item Name="Put_DBL_PV.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/Put_DBL_PV.vi"/>
				<Item Name="Put_I8-1D_PV-1D.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/Put_I8-1D_PV-1D.vi"/>
				<Item Name="Put_I8-1D_PV.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/Put_I8-1D_PV.vi"/>
				<Item Name="Put_I8-2D_PV-1D.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/Put_I8-2D_PV-1D.vi"/>
				<Item Name="Put_I8_PV.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/Put_I8_PV.vi"/>
				<Item Name="Put_I16-1D_PV-1D.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/Put_I16-1D_PV-1D.vi"/>
				<Item Name="Put_I16-1D_PV.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/Put_I16-1D_PV.vi"/>
				<Item Name="Put_I16-2D_PV-1D.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/Put_I16-2D_PV-1D.vi"/>
				<Item Name="Put_I16_PV.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/Put_I16_PV.vi"/>
				<Item Name="Put_I32-1D_PV-1D.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/Put_I32-1D_PV-1D.vi"/>
				<Item Name="Put_I32-1D_PV.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/Put_I32-1D_PV.vi"/>
				<Item Name="Put_I32-2D_PV-1D.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/Put_I32-2D_PV-1D.vi"/>
				<Item Name="Put_I32_PV.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/Put_I32_PV.vi"/>
				<Item Name="Put_I64-1D_PV-1D.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/Put_I64-1D_PV-1D.vi"/>
				<Item Name="Put_I64-1D_PV.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/Put_I64-1D_PV.vi"/>
				<Item Name="Put_I64-2D_PV-1D.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/Put_I64-2D_PV-1D.vi"/>
				<Item Name="Put_I64_PV.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/Put_I64_PV.vi"/>
				<Item Name="Put_SGL-1D_PV-1D.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/Put_SGL-1D_PV-1D.vi"/>
				<Item Name="Put_SGL-1D_PV.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/Put_SGL-1D_PV.vi"/>
				<Item Name="Put_SGL-2D_PV-1D.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/Put_SGL-2D_PV-1D.vi"/>
				<Item Name="Put_SGL_PV.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/Put_SGL_PV.vi"/>
				<Item Name="Put_String-1D_PV-1D.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/Put_String-1D_PV-1D.vi"/>
				<Item Name="Put_String-1D_PV.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/Put_String-1D_PV.vi"/>
				<Item Name="Put_String-2D_PV-1D.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/Put_String-2D_PV-1D.vi"/>
				<Item Name="Put_String_PV.vi" Type="VI" URL="/&lt;userlib&gt;/caLab/Private/Put_String_PV.vi"/>
				<Item Name="PV Info.ctl" Type="VI" URL="/&lt;userlib&gt;/caLab/PV Info.ctl"/>
			</Item>
			<Item Name="vi.lib" Type="Folder">
				<Item Name="BuildHelpPath.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/BuildHelpPath.vi"/>
				<Item Name="Check Special Tags.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Check Special Tags.vi"/>
				<Item Name="Clear Errors.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Clear Errors.vi"/>
				<Item Name="Convert property node font to graphics font.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Convert property node font to graphics font.vi"/>
				<Item Name="Details Display Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Details Display Dialog.vi"/>
				<Item Name="DialogType.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/DialogType.ctl"/>
				<Item Name="DialogTypeEnum.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/DialogTypeEnum.ctl"/>
				<Item Name="Error Code Database.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Error Code Database.vi"/>
				<Item Name="ErrWarn.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/ErrWarn.ctl"/>
				<Item Name="eventvkey.ctl" Type="VI" URL="/&lt;vilib&gt;/event_ctls.llb/eventvkey.ctl"/>
				<Item Name="Find Tag.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Find Tag.vi"/>
				<Item Name="Format Message String.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Format Message String.vi"/>
				<Item Name="General Error Handler Core CORE.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/General Error Handler Core CORE.vi"/>
				<Item Name="General Error Handler.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/General Error Handler.vi"/>
				<Item Name="Get String Text Bounds.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Get String Text Bounds.vi"/>
				<Item Name="Get Text Rect.vi" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Get Text Rect.vi"/>
				<Item Name="GetHelpDir.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/GetHelpDir.vi"/>
				<Item Name="GetRTHostConnectedProp.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/GetRTHostConnectedProp.vi"/>
				<Item Name="Longest Line Length in Pixels.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Longest Line Length in Pixels.vi"/>
				<Item Name="LVBoundsTypeDef.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/miscctls.llb/LVBoundsTypeDef.ctl"/>
				<Item Name="LVRectTypeDef.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/miscctls.llb/LVRectTypeDef.ctl"/>
				<Item Name="Not Found Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Not Found Dialog.vi"/>
				<Item Name="Search and Replace Pattern.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Search and Replace Pattern.vi"/>
				<Item Name="Set Bold Text.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Set Bold Text.vi"/>
				<Item Name="Set String Value.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Set String Value.vi"/>
				<Item Name="Simple Error Handler.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Simple Error Handler.vi"/>
				<Item Name="Space Constant.vi" Type="VI" URL="/&lt;vilib&gt;/dlg_ctls.llb/Space Constant.vi"/>
				<Item Name="TagReturnType.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/TagReturnType.ctl"/>
				<Item Name="Three Button Dialog CORE.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Three Button Dialog CORE.vi"/>
				<Item Name="Three Button Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Three Button Dialog.vi"/>
				<Item Name="Trim Whitespace.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Trim Whitespace.vi"/>
				<Item Name="whitespace.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/whitespace.ctl"/>
			</Item>
			<Item Name="Controls.ctl" Type="VI" URL="../Controls.ctl"/>
			<Item Name="mscorlib" Type="VI" URL="mscorlib">
				<Property Name="NI.PreserveRelativePath" Type="Bool">true</Property>
			</Item>
		</Item>
		<Item Name="Build Specifications" Type="Build">
			<Item Name="LeyboldTurboGUI" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{46998D0B-258E-420B-BC89-BFEE2497B41F}</Property>
				<Property Name="App_INI_GUID" Type="Str">{7DEDDF8E-D6CE-4B14-A5AE-6F17E9249B8D}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{0476A861-4894-488E-8567-B5AD4A6C6B21}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">LeyboldTurboGUI</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">/D/EPICS/LeyboldTurbo/Release_LIB</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{01B58664-AF5D-41A5-B400-ECA9354810ED}</Property>
				<Property Name="Bld_supportedLanguage[0]" Type="Str">English</Property>
				<Property Name="Bld_supportedLanguageCount" Type="Int">1</Property>
				<Property Name="Bld_version.build" Type="Int">16</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">LeyboldTurboGUI.exe</Property>
				<Property Name="Destination[0].path" Type="Path">/D/EPICS/LeyboldTurbo/Release_LIB/NI_AB_PROJECTNAME.exe</Property>
				<Property Name="Destination[0].path.type" Type="Str">&lt;none&gt;</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">/D/EPICS/LeyboldTurbo/Release_LIB/data</Property>
				<Property Name="Destination[1].path.type" Type="Str">&lt;none&gt;</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_iconItemID" Type="Ref">/My Computer/Leybold.ico</Property>
				<Property Name="Source[0].itemID" Type="Str">{6C36D32B-CF05-4097-A831-FA368E243E5C}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/LeyboldTurboGUI.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="Source[10].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[10].itemID" Type="Ref">/My Computer/GetPumpWarnings.vi</Property>
				<Property Name="Source[10].sourceInclusion" Type="Str">Include</Property>
				<Property Name="Source[10].type" Type="Str">VI</Property>
				<Property Name="Source[11].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[11].itemID" Type="Ref">/My Computer/Measusurements.ctl</Property>
				<Property Name="Source[11].sourceInclusion" Type="Str">Include</Property>
				<Property Name="Source[11].type" Type="Str">VI</Property>
				<Property Name="Source[12].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[12].itemID" Type="Ref">/My Computer/PumpRecord.ctl</Property>
				<Property Name="Source[12].sourceInclusion" Type="Str">Include</Property>
				<Property Name="Source[12].type" Type="Str">VI</Property>
				<Property Name="Source[13].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[13].itemID" Type="Ref">/My Computer/PumpStates.ctl</Property>
				<Property Name="Source[13].sourceInclusion" Type="Str">Include</Property>
				<Property Name="Source[13].type" Type="Str">VI</Property>
				<Property Name="Source[14].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[14].itemID" Type="Ref">/My Computer/RunStates.ctl</Property>
				<Property Name="Source[14].sourceInclusion" Type="Str">Include</Property>
				<Property Name="Source[14].type" Type="Str">VI</Property>
				<Property Name="Source[15].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[15].itemID" Type="Ref">/My Computer/SetPVValue.vi</Property>
				<Property Name="Source[15].sourceInclusion" Type="Str">Include</Property>
				<Property Name="Source[15].type" Type="Str">VI</Property>
				<Property Name="Source[16].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[16].itemID" Type="Ref">/My Computer/SetRunningButton.vi</Property>
				<Property Name="Source[16].sourceInclusion" Type="Str">Include</Property>
				<Property Name="Source[16].type" Type="Str">VI</Property>
				<Property Name="Source[17].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[17].itemID" Type="Ref">/My Computer/Warnings.ctl</Property>
				<Property Name="Source[17].sourceInclusion" Type="Str">Include</Property>
				<Property Name="Source[17].type" Type="Str">VI</Property>
				<Property Name="Source[2].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[2].itemID" Type="Ref">/My Computer/DisableControls.vi</Property>
				<Property Name="Source[2].sourceInclusion" Type="Str">Include</Property>
				<Property Name="Source[2].type" Type="Str">VI</Property>
				<Property Name="Source[3].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[3].itemID" Type="Ref">/My Computer/Fault.ctl</Property>
				<Property Name="Source[3].sourceInclusion" Type="Str">Include</Property>
				<Property Name="Source[3].type" Type="Str">VI</Property>
				<Property Name="Source[4].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[4].itemID" Type="Ref">/My Computer/FindPaneAndControl.vi</Property>
				<Property Name="Source[4].sourceInclusion" Type="Str">Include</Property>
				<Property Name="Source[4].type" Type="Str">VI</Property>
				<Property Name="Source[5].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[5].itemID" Type="Ref">/My Computer/FindRunningButton.vi</Property>
				<Property Name="Source[5].sourceInclusion" Type="Str">Include</Property>
				<Property Name="Source[5].type" Type="Str">VI</Property>
				<Property Name="Source[6].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[6].itemID" Type="Ref">/My Computer/GeneratePVNames.vi</Property>
				<Property Name="Source[6].sourceInclusion" Type="Str">Include</Property>
				<Property Name="Source[6].type" Type="Str">VI</Property>
				<Property Name="Source[7].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[7].itemID" Type="Ref">/My Computer/GetFault.vi</Property>
				<Property Name="Source[7].sourceInclusion" Type="Str">Include</Property>
				<Property Name="Source[7].type" Type="Str">VI</Property>
				<Property Name="Source[8].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[8].itemID" Type="Ref">/My Computer/GetMeasurements.vi</Property>
				<Property Name="Source[8].sourceInclusion" Type="Str">Include</Property>
				<Property Name="Source[8].type" Type="Str">VI</Property>
				<Property Name="Source[9].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[9].itemID" Type="Ref">/My Computer/GetPumpStatus.vi</Property>
				<Property Name="Source[9].sourceInclusion" Type="Str">Include</Property>
				<Property Name="Source[9].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">18</Property>
				<Property Name="TgtF_companyName" Type="Str">the United Kingdom Atomic Energy Authority</Property>
				<Property Name="TgtF_fileDescription" Type="Str">LeyboldTurboGUI</Property>
				<Property Name="TgtF_internalName" Type="Str">LeyboldTurboGUI</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2015 the United Kingdom Atomic Energy Authority</Property>
				<Property Name="TgtF_productName" Type="Str">LeyboldTurboGUI</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{29615AB3-48B7-4CD8-94F4-44FDACA35B82}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">LeyboldTurboGUI.exe</Property>
			</Item>
		</Item>
	</Item>
</Project>
