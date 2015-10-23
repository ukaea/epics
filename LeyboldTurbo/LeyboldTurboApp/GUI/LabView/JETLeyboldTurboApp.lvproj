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
		<Item Name="JETLeyboldTurboApp.vi" Type="VI" URL="../JETLeyboldTurboApp.vi"/>
		<Item Name="Measusurements.ctl" Type="VI" URL="../Measusurements.ctl"/>
		<Item Name="PumpRecord.ctl" Type="VI" URL="../PumpRecord.ctl"/>
		<Item Name="PumpStates.ctl" Type="VI" URL="../PumpStates.ctl"/>
		<Item Name="RunStates.ctl" Type="VI" URL="../RunStates.ctl"/>
		<Item Name="SetPVValue.vi" Type="VI" URL="../SetPVValue.vi"/>
		<Item Name="SetRunningButton.vi" Type="VI" URL="../SetRunningButton.vi"/>
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
		</Item>
		<Item Name="Build Specifications" Type="Build"/>
	</Item>
</Project>
