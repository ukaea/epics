default: base

base : pvCommon pvData nt pvAccess pvaSrv pvDatabase pvaClient

all : default archSrv pvaPy

pvCommon : FORCE
	make -C pvCommonCPP

pvData : FORCE
	make -C pvDataCPP 

nt : FORCE
	make -C normativeTypesCPP

pvAccess : FORCE
	make -C pvAccessCPP

pvaSrv : FORCE
	make -C pvaSrv

pvDatabase : FORCE
	make -C pvDatabaseCPP

pvaClient : FORCE
	make -C pvaClientCPP

archSrv : FORCE
	make -C exampleCPP/ChannelArchiverService

pvaPy : FORCE
	make -C pvaPy

clean : 
	make -C pvCommonCPP clean
	make -C pvDataCPP clean
	make -C pvAccessCPP clean
	make -C pvaSrv clean
	make -C pvDatabaseCPP clean
	make -C normativeTypesCPP clean
	make -C exampleCPP/ChannelArchiverService clean
	make -C pvaPy clean

uninstall : 
	make -C pvCommonCPP uninstall
	make -C pvDataCPP uninstall
	make -C pvAccessCPP uninstall
	make -C pvaSrv uninstall
	make -C pvDatabaseCPP uninstall
	make -C normativeTypesCPP uninstall
	make -C exampleCPP/ChannelArchiverService uninstall
	make -C pvaPy uninstall

distclean : clean config_clean 

configure: FORCE
	./configure.sh

config_multi: FORCE
	./configure.sh all

config_clean:
	./configure.sh clean

FORCE:
