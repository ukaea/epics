#!/usr/bin/perl

# Once configured, this script mimics the most basic responses
# of a device communicating on a serial port.  It requires "perl".
# It can be modified for increasingly realistic responses.
#
# Before using this script, see the README to make the modifications
# associated with the device you want to simulate.  To use, make
# sure the intended account (likely your own) has write permission
# on the serial port.  Then either
# > perl simSerialDevice
# or
# > simSerialDevice
# Then on the other end of the serial line run your client software
# (e.g. IOC application, minicom, HyperTerm, PuTTY).  Make sure the
# serial port configuration matches up.
#
# Note: this script will print to the serial port AND to the terminal
# screen.  The latter is useful as a diagnostic as well as a monitor
# (think of it as the simulated display panel of the target device).

@args = @ARGV;

# Set $verbose > 0 for increasingly detailed diagnostic output.

$verbose = 0;

# If the device echoes the incoming command in the outgoing response,
# enable this behaviour by setting $echoOn to "1", otherwise set it to "0".

$echoOn = 0;

# The default communication bus:

$comType = "serial";

# The default named pipe:

$TTY = "commpipe";

# Uncomment Time:HiRes only if you need higher time resolution (and if you are
# running on an x86 arch.
#use Time::HiRes qw( alarm sleep usleep ualarm time gettimeofday tv_interval );

for( $i = 0; $i < scalar(@args); $i++)
{
	if( $args[$i] eq "-c")
	{
		$i++;
		if( $args[$i] eq "serial") { $comType = $args[$i];}
		elsif( $args[$i] eq "pipe") { $comType = $args[$i];}
		else
		{
			print "Communication bus type $args[$i] is not supported.\n";
			Usage();
			exit;
		}
	}
	elsif( $args[$i] eq "-d")
	{
		$i++;
		$TTY = $args[$i];
	}
	elsif( $args[$i] eq "-D")
	{
		$i++;
		$verbose = $args[$i];
	}
	else { Usage(); exit;}
}

$ttytimeout = 0.1;

$RC = chr( 0x02);	# start character
$EC = chr( 0x03);	# end character
$ACK = chr( 0x06);	# acknowledgement character
$ILT = "";		# input line terminator (usually, \n or \r or both)
$LT = "";		# line terminator (usually \n or \r or both)

# Set up the serial port
if( $comType eq "serial")
{
	SetupSerial();
}
elsif( $comType eq "pipe")
{
	SetupPipe();
}

if( $verbose > 1) {print "DEBUG: opening I/O\n";}
if( $verbose > 2) {print "DEBUG: opening input\n";}
# Open input from comm port
open( ITTY, "<${TTY}") or die( "Could not open input for simulated device.\n");
if( $verbose > 2) {print "DEBUG: opening output\n";}
# Open output to comm port
open( OTTY, ">${TTY}") or die( "Could not open output for simulated device.\n");
if( $verbose > 1) {print "DEBUG: opened I/O\n";}

# Prepare 'select()'
$rin = $win = $ein = '';
vec($rin,fileno(ITTY),1) = 1;
#print "DEBUG: rin=$rin\n";

sleep(1);
if( ( $comType eq "serial") && ( $verbose > 0)) {print "DEBUG: stty done\n";}

init_data();
# Push "no errors" out
$resp = sprintf( "", $RC, $addr, ord( 0xff), $EC);

$timeStart = time();
while( 1)
{
	$nfound = select($rout=$rin, undef, undef, $ttytimeout);
	if( $nfound)
	{
		$timeNow = time();
		&GetSerial();
		if( length( $str))
		{
			parse_input();
		}
	}
}

#clean up

if( $comType eq "serial")
{
	system "stty", "-F ${TTY}", 'icanon', 'eol', '^@';
}

close( ITTY);
close( OTTY);

#
# End of main routine
#

sub GetSerial
{
	if( $verbose > 9) {print "DEBUG: ";}
	$str = "";
	while()
	{
		$chr = '';
		sysread(ITTY, $chr, 1, 0);

		# Add any junk character filtering here (e.g. software
		# flow controls such as the next line)

		#print $chr, " (", ord( $chr), ")\n";
		# Set the condition that is NOT the input completion condition
		# here (the completion condition is often a line terminator,
		# but some stream devices don't use a line terminator and
		# use a fixed format or identifiable pattern that signals
		# when input is complete)
		if( ! ( $str =~ /^${RC}.\d{3}.*${EC}./))
		{
			# Trap end of input in case of bad input
			if( length($chr) == 0) { last;}
			if( $verbose > 9) { print $chr; }
			$str .= $chr;
		}
		else
		{
			if( $verbose > 9) { print "\n"; }
			$str .= $chr;
			print "Incoming: ", $str, "\n";
			last;
		}
	}
}

sub SetupSerial()
{
	# Set serial port parameters
	$baud = 9600;
	$stopBit = 1;
	$dataBit = 8;
	$parity = "none";
	print "Using serial port ", $TTY, " with ", $baud, " baud, ", $dataBit, " bits, ", $stopBit, " stop bit(s), ", $parity, " parity\n";
	#
	# Initialize tty
	system "stty", "-F", "${TTY}", $baud;
	# We turn off Xon/Xoff to leave out the ^s/^q characters that would
	# otherwise increase overhead (as much as factor of 2)
	system "stty", "-F", "${TTY}", '-icanon', "-ixon";
	system "stty", "-F", "${TTY}", 'eol', '0';
	system "stty", "-F", "${TTY}", "cs${dataBit}";
	if( $parity eq "odd")
	{
		system "stty", "-F", "${TTY}", "parenb", "parodd";
	}
	elsif( $parity eq "even")
	{
		system "stty", "-F", "${TTY}", "parenb", "-parodd";
	}
	else
	{
		system "stty", "-F", "${TTY}", "-parenb";
	}
	if( $stopBit == 2)
	{
		system "stty", "-F", "${TTY}", "cstopb";
	}
	else
	{
		system "stty", "-F", "${TTY}", "-cstopb";
	}
}

sub SetupPipe()
{
	print "Using named pipe ", $TTY, "\n";
	if( ! -e $TTY)
	{
		system( "mkfifo -m a=rw $TTY");
	}
}

sub Usage()
{
	print "Usage: simSerialDevice.pl  [-c serial] -d <devname> [-D <level>]\n";
	print "   or\n";
	print "       simSerialDevice.pl  [-c pipe -d <devname>] [-D <level>]\n";
	print "   or\n";
	print "       simSerialDevice.pl  -h|--help\n";
	print "   where:\n";
	print "          -c : communication bus (default is serial)\n";
	print "          -d : device name (e.g. /dev/ttyS0) for default (serial) mode,\n";
	print "               or name of pipe for '-c pipe'.\n";
	print "  -h, --help : this usage message.\n";
	print "          -D : set debug level.\n";
}

# Each device parameter/variable is stored as an element of $data{},
# referenced with device command codes or keys as an indeces
# (i.e. perl "hashes").

# Set starting values
# Note: You must define $data{...} for every device command, even
#       for commands that don't normally have associated data
#       (in which case assign a dummy value).
#       This is to ensure that bad input can be trapped.
sub init_data()
{
	# Parameters for time-dependent variables
	$rampup = 500;
	$rampdn = 500;
	$lifeStart = 1000;

	# device parameters, not used for anything other than initialization
	# and mnemonic cross reference
	$runState = 0;
	$lowSpeed = 0;
	$ctrlMode = 0;
	$softStart = 0;
	$SP1Type = 3;
	$SP1 = 867;
	$SP1Delay = 0;
	$SP1Signal = 0;
	$SP1Hyst = 2;
	$WaterCool = 0;
	$ActiveStop = 0;
	$baudRate = 4;
	$interlockType = 0;
	$analogOutType = 0;
	$lowSpeedFreq = 900;
	$rotFreq = 1100;
	$ventValve = 0;
	$ventValveMode = 0;
	$ventValveDelay = 15;
	$ventOpenTime = 0;
	$powerLimit = 100;
	$loadType = 0;
	$gaugePCorr = 10;
	$SP1Press = 0.1;
	$pressUnits = 0;
	$stopSpeedRead = 0;
	$SP2Type = 3;
	$SP2 = 867;
	$SP2Delay = 0;
	$SP2Signal = 0;
	$SP2Hyst = 2;
	$SP2Press = 0;
	$gaugeSP2Type = 3;
	$gaugeSP2 = 867;
	$gaugeSP2Delay = 0;
	$gaugeSP2Signal = 0;
	$gaugeSP2Hyst = 2;
	$fanConfig = 2;
	$fan = 0;
	$startOutMode = 1;
	$gasType = 0;
	$otherGasCorr = 1.0;
	$current = 0;
	$voltage = 0;
	$power = 0;
	$drivingFreq = 0;
	$temperature = 30;
	$status = 0;
	$errorCode = 0;
	$heatSinkTemp = 33;
	$airTemp = 35;
	$pressure = 0;
	$freq = 0;
	$gaugeStatus = 1;
	$gaugePower = 1;
	$cycleTime = 0;
	$cycleNum = 14;
	$life = $lifeStart;
	$crcEPROM = "QE80000";
	$crcPar = "PA80000";
	$crcParStruct = "0000";
	$serialAddr = 0;
	$serialType = 1;

	$data{"000"} = $runState;
	$data{"001"} = $lowSpeed;
	$data{"008"} = $ctrlMode;
	$data{"100"} = $softStart;
	$data{"101"} = $SP1Type;
	$data{"102"} = $SP1;
	$data{"103"} = $SP1Delay;
	$data{"104"} = $SP1Signal;
	$data{"105"} = $SP1Hyst;
	$data{"106"} = $WaterCool;
	$data{"107"} = $ActiveStop;
	$data{"108"} = $baudRate;
	$data{"110"} = $interlockType;
	$data{"111"} = $analogOutType;
	$data{"117"} = $lowSpeedFreq;
	$data{"120"} = $rotFreq;
	$data{"122"} = $ventValve;
	$data{"125"} = $ventValveMode;
	$data{"126"} = $ventValveDelay;
	$data{"136"} = $gaugeSP2Type;
	$data{"137"} = $gaugeSP2;
	$data{"138"} = $gaugeSP2Delay;
	$data{"139"} = $gaugeSP2Signal;
	$data{"140"} = $gaugeSP2Hyst;
	$data{"143"} = $fanConfig;
	$data{"144"} = $fan;
	$data{"147"} = $ventOpenTime;
	$data{"155"} = $powerLimit;
	$data{"157"} = $loadType;
	$data{"161"} = $gaugePCorr;
	$data{"162"} = sprintf( "%6.1E ", $SP1Press);
	$data{"163"} = $pressUnits;
	$data{"167"} = $stopSpeedRead;
	$data{"171"} = $SP2Type;
	$data{"172"} = $SP2;
	$data{"173"} = $SP2Delay;
	$data{"174"} = $SP2Signal;
	$data{"175"} = $SP2Hyst;
	$data{"176"} = sprintf( "%6.1E ", $SP2Press);
	$data{"177"} = $startOutMode;
	$data{"181"} = $gasType;
	$data{"182"} = $otherGasCorr;
	$data{"200"} = $current;
	$data{"201"} = $voltage;
	$data{"202"} = $power;
	$data{"203"} = $drivingFreq;
	$data{"204"} = $temperature;
	$data{"205"} = $status;
	$data{"206"} = $errorCode;
	$data{"211"} = $heatSinkTemp;
	$data{"216"} = $airTemp;
	$data{"224"} = sprintf( "%6.1E ", $pressure);
	$data{"226"} = $freq;
	$data{"257"} = $gaugeStatus;
	$data{"267"} = $gaugePower;
	$data{"300"} = $cycleTime;
	$data{"301"} = $cycleNum;
	$data{"302"} = $life;
	$data{"400"} = sprintf( "%-10s", $crcEPROM);
	$data{"402"} = sprintf( "%-10s", $crcPar);
	$data{"404"} = sprintf( "%-10s", $crcParStruct);
	$data{"503"} = $serialAddr;
	$data{"504"} = $serialType;
}

sub add_checksum()
{
	$checksum = 0;
	for( $i = 1; $i < length( $resp); $i++)
	{
		$checksum ^= ord( substr( $resp, $i, 1));
	}
	$resp = sprintf( "%s%02X", $resp, $checksum);
}

# Parse/decode commands, output response as necessary
# Note: $data{...} must previously exist (by virtue of init_data()).
#
# NOTE: TVRP, TV550 currently not supported
#
sub parse_input()
{
	# Update any time-dependent parameters here.
	if( $status == 2)
	{
		$current = 250;
		if( $freq < $drivingFreq * 60) {
			$freq += $rampup;
			$voltage = 120;
		}
		else {
			$status = 5;
			$voltage = 20;
		}
	}
	elsif( $status == 4)
	{
		$current = 0;
		$voltage = 0;
		if( $drivingFreq * 60 < $freq) {
			$freq -= $rampdn;
			if( $freq < 0) { $freq = 0;}
		}
		else {
			if( $drivingFreq > 0) { $status = 5; $voltage = 20; $current = 250; }
			else {
				$status = 0;
				$lifeStart += int( $cycleTime / 60);
			}
		}
	}
	$power = $current * $voltage / 1000;
	$temperature = sprintf( "%.1f", 30 + rand( 0.11));
	$airTemp = sprintf( "%.1f", 35 + rand( 0.11));
	$heatSinkTemp = sprintf( "%.1f", 32 + rand( 0.11));
	if( ( $status == 2) || ( $status == 4) || ( $status == 5))
	{
		$cycleTime = int( ( $timeNow - $timeStart) / 60.0);
		$life = int($lifeStart + $cycleTime / 60.0);
	}
	$data{"200"} = $current;
	$data{"201"} = $voltage;
	$data{"202"} = $power;
	$data{"204"} = $temperature;
	$data{"205"} = $status;
	$data{"211"} = $heatSinkTemp;
	$data{"216"} = $airTemp;
	$data{"226"} = $freq;
	$data{"300"} = $cycleTime;
	$data{"302"} = $life;

	# Replace patterns to match expected input of target device.
	# Add more "if" blocks to parse custom text or for custom responses.

	# The syntax uses:
	# <STX> = start character, 1 char
	# <ADDR> = address, 1 char
	# <WIN> = command, 3 char
	# <COM> = command type, 1 char
	# <DATA> = data, up to 10 char
	# <ETX> = end character, 1 char
	# <CRC> = checksum, 2 char
	# <ACK> = acknowledgement, 1 char

	# Commands that SET values in target device
	# command: <STX><ADDR><WIN><COM><DATA><ETX><CRC>
	# resonse: <STX><ADDR><ACK><ETX><CRC>
	if( $str =~ /^${RC}(.)(\d{3})1(.*)${EC}../)
	{
		$addr = $1;
		$cmd = $2;
		$dat = $3;

		# Make sure only valid commands are handled.
		if( $data{$cmd} ne undef) { $data{$cmd} = $dat; }
		else { return;}

		# If the data for $cmd requires further processing, add
		# the processing here.  For example:
		if( $cmd eq "000") {
			$runState = $dat;
			if( $dat == 1)
			{
				if( $freq == 0) {
					$cycleNum++;
					$timeStart = $timeNow;
				}
				$status = 2;
				if( $lowSpeed == 0) { $drivingFreq = $rotFreq; }
				else { $drivingFreq = $lowSpeedFreq; }
			}
			else
			{
				$status = 4;
				$drivingFreq = 0;
			}
			$data{"000"} = $runState;
			$data{"203"} = $drivingFreq;
			$data{"205"} = $status;
			$data{"301"} = $cycleNum;
		}
		elsif( $cmd eq "001") {
			$lowSpeed = $dat;
			if( $runState)
			{
				if( $lowSpeed == 0) {
					$drivingFreq = $rotFreq;
					$status = 2;
				}
				else {
					$drivingFreq = $lowSpeedFreq;
					$status = 4;
				}
			}
			$data{"001"} = $lowSpeed;
			$data{"203"} = $drivingFreq;
			$data{"205"} = $status;
		}
		elsif( $cmd eq "109") {
			$cycleTime = 0;
			$cycleNum = 0;
			$life = 0;
			$data{"300"} = $cycleTime;
			$data{"301"} = $cycleNum;
			$data{"302"} = $life;
		}

		$resp = sprintf( "%1s%1s%1s%1s", $RC, $addr, $ACK, $EC);
#		$checksum = 0;
#		for( $i = 1; $i < length( $resp); $i++)
#		{
#			$checksum ^= ord( substr( $resp, $i, 1));
#		}
#		$resp = sprintf( "%1s%1s%1s%1s%02X", $RC, $addr, $ACK, $EC, $checksum);
		add_checksum();
		print OTTY $resp;
		print "Response: ", $cmd, " -> ", $resp, "\n";
	}
	# Commands that GET values from target device
	# command: <STX><ADDR><WIN><COM><ETX><CRC>
	# resonse: <STX><ADDR><WIN><DATA><ETX><CRC>
	elsif( $str =~ /^${RC}(.)(\d{3})0${EC}../)
	{
		$addr = $1;
		$cmd = $2;
		$dat = "";

		# Make sure only valid commands are handled.
		if( $data{$cmd} ne undef) { $dat = $data{$cmd};}
		else { return;}

		# If the data for $cmd requires preprocessing, add
		# the processing here.
		if( $cmd eq "162") { $dat = sprintf( "%6.1E ", $SP1Press);}
		elsif( $cmd eq "176") { $dat = sprintf( "%6.1E ", $SP2Press);}
		elsif( $cmd eq "224") { $dat = sprintf( "%6.1E ", $pressure);}
		elsif( $cmd eq "400") { $dat = sprintf( "%-10s", $crcEPROM);}
		elsif( $cmd eq "402") { $dat = sprintf( "%-10s", $crcPar);}
		elsif( $cmd eq "404") { $dat = sprintf( "%-10s", $crcParStruct);}

		$resp = sprintf( "%1s%1s%03d%1s%1s", $RC, $addr, $cmd, $dat, $EC);
#		$checksum = 0;
#		for( $i = 1; $i < length( $resp); $i++)
#		{
#			$checksum ^= ord( substr( $resp, $i, 1));
#			#print substr( $resp, $i, 1), " (", ord( substr( $resp, $i, 1)), ") -> ", $checksum, "\n";
#		}
#		$resp = sprintf( "%1s%1s%03d%1s%1s%02X", $RC, $addr, $cmd, $dat, $EC, $checksum);
		add_checksum();
		print OTTY $resp;
		print "Response: ", $cmd, ": ", $dat, " -> ", $resp, "\n";
	}
#	# Commands that neither take input nor return output
#	elsif( $str =~ /^${RC}\w(\w+)${EC}\w\w$/)
#	{
#		$cmd = $1;
#
#		# Make sure only valid commands are handled.
#		if( $data{$cmd} eq undef) { return;}
#
#		# Replace "cmd#" with a command string and "data#" with
#		# the name of the value storage variable for that
#		# command 
#		if( $cmd eq "devcmd1") { $dat = $data{$cmd}; }
#		elsif( $cmd eq "devcmd2") { $dat = $data{$cmd}; }
#		else { return;}
#
#		# Replace the args of print after "OTTY" to match the output
#		# of the target device
#		print OTTY $cmd, $LT, "OK", $LT;
#		print "Response: ", $cmd, "\n";
#	}
	elsif( $str =~ /^clearError/)
	{
		$resp = sprintf( "%1s%1s%1s%1s", $RC, $addr, ord( 0xff), $EC);
		add_checksum();
		print OTTY $resp;
		print "Response: ", $cmd, " -> ", $resp, "\n";
	}
	else
	{
		print "ERROR: Cannot parse input: ", $str, "\n";
	}
}
