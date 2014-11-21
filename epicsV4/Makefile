default: base helloWorld

base : pvCommon pvData pvAccess pvaSrv pvDatabase nt

all : default archSrv pyaPy

pvCommon : FORCE
	make -C pvCommonCPP

pvData : FORCE
	make -C pvDataCPP 

pvAccess : FORCE
	make -C pvAccessCPP

pvaSrv : FORCE
	make -C pvaSrv

pvDatabase : FORCE
	make -C pvDatabaseCPP

nt : FORCE
	make -C normativeTypesCPP

helloWorld : FORCE
	make -C exampleCPP/HelloWorld

archSrv : FORCE
	make -C exampleCPP/ChannelArchiverService

pyaPy : FORCE
	make -C pvaPy

clean : 
	make -C pvCommonCPP clean
	make -C pvDataCPP clean
	make -C pvAccessCPP clean
	make -C pvaSrv clean
	make -C pvDatabaseCPP clean
	make -C normativeTypesCPP clean
	make -C exampleCPP/HelloWorld clean
	make -C exampleCPP/ChannelArchiverService clean
	make -C pvaPy clean

distclean : clean config_clean 

configure: FORCE
	./configure.sh

config_multi: FORCE
	./configure.sh all

config_clean:
	./configure.sh clean

FORCE:


