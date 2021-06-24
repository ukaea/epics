## CLF Installation
Clone this repo to support folder `SUPPORT=/usr/local/epics/support` with
macro `PARKER6K=       $(SUPPORT)/parker6k/current`


## parker6k

EPICS support for the Parker 6K controller.  *FORK* _https://github.com/mp49/parker6k/_

### Introduction

The parker6k driver enables control of a Parker 6K controller
via the EPICS motor record. 

This is the Asyn 'model3' version which
has a number of advantages over the model 1 driver:

<ul>
<li>Detect motion outside of a normal move command</li>
<li>Reflect error conditions in the motor record alarm fields</li>
<li>Control and read controller-wide information</li>
<li>Additonal axis specific capabilities outside of the motor record</li>
<li>Easy debugging (asyn record, low level commands via waveform PVs, custom logging).</li>
<li>Adjustable polling rates</li>
<li>Ability to set both motor and encoder position</li>
</ul>

To build the module you may have to edit the ```configure``` directory for your local site.

### Notes

The driver has been tested on stepper drives (with and without encoders) using 4 and 8-axis compumotor controllers. 
The driver has not been tested with servo drives.

The move function automatically sets up the S-curve parameters at
the start of the move (similar to model 1 driver).
It uses half the acceleration rate for the jerk parameters (AA, ADA).

The home function uses the home velocity before executing the home (HOM).
It is expected that the controller home parameters have already been 
configured (eg. HOMZ). NOTE: for encoder based systems the controller
does not reset the encoder position on a successful home. Currently the 
user must do this after a home.

In order to set the position on an axis:

1. set SET field to 1
2. set DVAL to desired position 
3. set OFF back to what it was
4. set SET back to 0

By setting the position to zero, the user can perform a manual home
on a limit switch for example (which the controller does not support).

It may be necessary to set ERES again before setting the position (or toggle UEIP).

This driver also has built in support for reading an external encoder over Modbus (using the EPICS Modbus support).

### IOC Startup File

There is an example IOC in parker6k/example that
demonstrates how to use the driver. There are 
several IOC startup file commands required:

```
  # Connect TCP socket to controller:
  drvAsynIPPortConfigure("6K","192.168.200.177:4001",0,0,0)

  # Create 'controller' object
  # Arguments:
  # Port name
  # Low level comms port name
  # Low level comms port addr
  # Number of axes (1 based, including un-used axes)
  # Moving polling rate
  # Idle polling rate
  p6kCreateController("P6K","6K",0,2,500,1000)

  # Optionally upload a controller configuration
  # Arguments:
  # Controller port name
  # Full path for file
  p6kUpload("P6K", "/home/controls/motion/bl1a/mcc1/config")
```

The above file must only contain a list of commands with 
a newline separating each command. 

For example:

```
  ECHO0
  COMEXC1
  1DRES25000
  1ERES4000
  1ENCCNT1
  1ESTALL1
  1ESK1
  1LH0
  2DRES25000
  2ERES4000
  2LH0
  2DRFEN1
  LIMLVL001000000000
```

It is not necessary to upload a controller config file,
but it is advantageous to do so in order to easily recover
after a power cycle. Otherwise there must be a manual
step to configure the controller before running the IOC.


After instantiating the controller object and uploading a config 
the axis objects must be created:

```
  # Create 'axis' objects
  # Arguments:
  # Controller port name
  # Axis number (must be <=number of axes passed into controller object)
  p6kCreateAxis("P6K",1)
  p6kCreateAxis("P6K",2)
  etc.
```

### IOC src/Makefile

It is only necessary to include this dbd file (along with the usual motor and asyn support):

```
  parker6kSupport.dbd
```

and this library:

```
  parker6kSupport
```

### Additonal database records

The driver can be used with a standard 'asynMotor' motor record.
However, it is useful to combine this with several other records
to provide functions like:

* disabling the motor record
* automatic amplifier power control
* access to driver parameters that are not mapped to motor record fields.

To provide these records there are database template files in parker6k:

p6k_controller.template (for controller specific control/parameters):

* Read controller error messages and comms errors
* Read state of initial controller config
* Enable simple logging (via stdout) of commands sent to controller
* Deferred moves control
* Low level command/response capability
* An asyn record for debugging and enabling tracing.

p6k_axis.template (for axis specific control/parameters):

* Disable motor record if we are in comms error. Usually this is
because we have lost network connection or the controller has been
power cycled. By default the driver will not automatically 
reconnect to the controller (Asyn IP port auto reconnect has been
turned off). This is because it can be dangerous for a user to move 
a mis-configured controller. At the very least it can mess up auto
save settings by making changes that are autosaved and not able 
to be reflected on the controller.
* Set a delay time for the driver to indicate 'done moving'. This 
can be useful to take into account setting time between each move.
NOTE: this is different from the motor record DLY if the motor
record is doing additional moves like backlash or retries.
* Read axis specific error messages.
* Enable automatic drive enable at the start of each move (with an optional
delay time between enabling the amplifier and the start of the move).
* Enable automatic drive disable at the end of the move (with an optional
delay time between the end of the move and drive disable). This is implemented
in database logic due to the possibility of the motor record doing
mutliple moves (so we use DMOV transitions to implement this).

The example IOC substitutions file references basic_asyn_motor_sns.db. This is a version of the standard motor support module basic_asyn_motor.db, with some additional useful records added. There is an example of this file in the exampleAdd/Db directory, along with error_calc.db and home_visibility.db templates files (which are referenced by basic_asyn_motor_sns.db). It will be up to the site developer wether to use these additional records. 

### Building and Testing

You may need to modify the files in the top level configure directory
for your local site, especially the RELEASE file.

The module was developed using:

* EPICS BASE 3.14.12.2
* ASYN 4-22
* MOTOR >6-8 (use SVN version until a version >6-8 has been released:
`svn co https://subversion.xor.aps.anl.gov/synApps/motor/trunk`)

In addition, the example IOC was built to include support
for autosave and devIocStats.

### Contributions

Originally developed at SNS in 2014 by Matt Pearson.


