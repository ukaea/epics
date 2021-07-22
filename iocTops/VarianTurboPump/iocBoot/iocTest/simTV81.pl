#!/usr/bin/perl

# Once configured, this script mimics the most basic responses
# of a device communicating on a serial port, TCP/IP port, or
# a named pipe.  It requires "perl".  It can be modified for
# increasingly realistic responses.
#
# Before using this script, see the README to make the modifications
# associated with the device you want to simulate.  To use, make
# sure the intended account (likely your own) has write permission
# on the serial port, connection, or pipe.  Then either
# > perl simSerialDevice
# or
# > simSerialDevice
# Then on the other end of the connection run your client software
# (e.g. IOC application, minicom, HyperTerm, PuTTY).  In the case
# of serial ports, make sure the port configurations match up.
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

$comType = "pipe";

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
		elsif( $args[$i] eq "ip") { $comType = $args[$i];}
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

$RC = chr( 0x02);		# start character
$EC = chr( 0x03);		# end character
$ACK = chr( 0x06);		# acknowledgement character
$ILT = "";		# input line terminator (usually, \n or \r or both)
$LT = "";		# line terminator (usually \n or \r or both)

# Set up the serial port
if( $comType eq "pipe")
{
	SetupPipe();
}
elsif( $comType eq "serial")
{
	SetupSerial();
}
elsif( $comType eq "ip")
{
	use Socket;

	SetupIP();
}

if( $verbose > 1) {print "DEBUG: opening I/O\n";}
if( $comType ne "ip")
{
	if( $verbose > 2) {print "DEBUG: opening input\n";}
	# Open input from comm port
	open( ITTY, "<${TTY}") or die( "Could not open input for simulated device.\n");
	if( $verbose > 2) {print "DEBUG: opening output\n";}
	# Open output to comm port
	open( OTTY, ">${TTY}") or die( "Could not open output for simulated device.\n");
}
else
{
	if( $verbose > 2) {print "DEBUG: opening input\n";}
	# Open input from comm port
	open( ITTY, "<&CSH") or die( "Could not open input for simulated device.\n");
	if( $verbose > 2) {print "DEBUG: opening output\n";}
	# Open output to comm port
	open( OTTY, ">&CSH") or die( "Could not open output for simulated device.\n");
	# Turn on autoflush - avoiding IO:Handle to reduce bloat
	$oldfh = select OTTY; $| = 1; select( $oldfh);
}
if( $verbose > 1) {print "DEBUG: opened I/O\n";}

# Prepare 'select()'
$rin = $win = $ein = '';
vec($rin,fileno(ITTY),1) = 1;
print "DEBUG: rin=$rin\n";

sleep(1);
if( ( $comType eq "serial") && ( $verbose > 0)) {print "DEBUG: stty done\n";}

init_data();
# Push "no errors" out
$resp = sprintf( "", $RC, $addr, ord( 0xff), $EC);

$timeStart = time();
while( 1)
{
	$nfound = select($rout=$rin, $wout=$win, undef, $ttytimeout);
	if( $nfound)
	{
		$timeNow = time();
		&GetSerial();
		if( length( $str))
		{
			parse_input();
		}
		else
		{
			if( $comType eq "ip")
			{
				close CSH;
				close ITTY;
				close OTTY;
				$caddr = accept( CSH, SSH);
				if( $verbose > 2) {print "DEBUG: re-opening input\n";}
				open( ITTY, "<&CSH") or die( "Could not re-open input for simulated device.\n");
				if( $verbose > 2) {print "DEBUG: re-opening output\n";}
				open( OTTY, ">&CSH") or die( "Could not re-open output for simulated device.\n");
				# Turn on autoflush - avoiding IO:Handle to reduce bloat
				$oldfh = select OTTY; $| = 1; select( $oldfh);
			}
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

if( $comType eq "ip")
{
	close( CSH);
	close( SSH);
}

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
		$chr = getc(ITTY);

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

sub echoCmd()
{
	if( $echoOn == 0) { return "";}

	# Replace or add to "$cmd" to match the output of the target device
	return $cmd . $ILT;
}

sub SetupSerial()
{
	# Set serial port parameters
	$baud = 9600;
	$stopBit = 1;
	$dataBit = 8;
	$parity = "even";
	print "Using serial port at ", $baud, " baud, ", $dataBit, " bits, ", $stopBit, " stop bit(s), ", $parity, " parity\n";
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
	if( ! -e $TTY)
	{
		system( "mkfifo -m a=rw $TTY");
	}
}

sub SetupIP()
{
	if( $TTY =~ /(\d+\.\d+\.\d+\.\d+):(\d+)/)
	{
		# IP address already given
		$ipaddr = $1;
		$port = $2;
	}
	elsif( $TTY =~ /(.*):(\d+)/)
	{
		if( $verbose > 1) { print "DEBUG: hostname=", $1, ", port=", $2, "\n";}
		# Hostname given, get IP address
		$ipaddr = gethostbyname( $1);
		$port = $2;
		if( $verbose > 2) { print "DEBUG: ip addr=", $ipaddr, "\n";}
		if( length( $ipaddr) == 0)
		{
			print "ERROR: cannot parse \"", $TTY, "\" as hostname/ip:port.\n";
			exit;
		}
		else
		{
			if( $port =~ /^\d+$/)
			{
				print "ERROR: parsed hostname but not port number from \"", $TTY, "\"\n";
				exit;
			}
		}
	}
	$proto = getprotobyname('tcp');
	socket( SSH, PF_INET, SOCK_STREAM, $proto) or die "socket: $!";
	setsockopt( SSH, SOL_SOCKET, SO_REUSEADDR, pack("l", 1)) or die "setsocketopt: $!";
	bind( SSH, sockaddr_in( $port, inet_aton( $ipaddr))) or die "bind: $!";
	listen( SSH, SOMAXCONN) or die "listen: $!";
	$caddr = accept( CSH, SSH);
}

sub Usage()
{
	print "Usage: simSerialDevice2.pl  [-c pipe] [-d <devname>] [-D <level>]\n";
	print "   or\n";
	print "       simSerialDevice2.pl  [-c serial -d <devname>] [-D <level>]\n";
	print "   or\n";
	print "       simSerialDevice2.pl  [-c ip -d <devname>] [-D <level>]\n";
	print "   where:\n";
	print "          -c : communication bus (default is \"pipe\")\n";
	print "          -d : name of pipe (default: $TTY) for default or '-c pipe'\n";
	print "               or device name (e.g. /dev/ttyS0) for '-c serial',\n";
	print "               or IP address/hostname:port (e.g. localhost:9001) for '-c ip'.\n";
	print "          -D : set debug level.\n";
}

sub init_data()
{
	# Parameters for time-dependent variables
	$rampup = 500;
	$rampdn = 500;
	$lifeLaunch = 1000;

	# Change the names of the value storage variables to suit your
	# needs and set their (starting) values
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
	$cycleTime = 0;
	$cycleNum = 14;
	$life = $lifeStart;
	$crcEPROM = "QE80000";
	$crcPar = "PA80000";
	$crcParStruct = "0000";
	$serialAddr = 0;
	$serialType = 1;
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

sub parse_input()
{
	# Update any time-dependent parameters
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
	$temperature = sprintf( "%.1f", 30 + rand( 0.1));
	$airTemp = sprintf( "%.1f", 35 + rand( 0.1));
	$heatSinkTemp = sprintf( "%.1f", 32 + rand( 0.1));
	if( ( $status == 2) || ( $status == 4) || ( $status == 5))
	{
		$cycleTime = int( ( $timeNow - $timeStart) / 60.0);
		$life = int($lifeStart + $cycleTime / 60.0);
	}

	# Add more "if" blocks to parse custom text or for custom responses
	# replace patterns to match expected input of target device

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

		# Replace "cmd#" with a command string and "data#" with
		# the name of the value storage variable for that
		# command 
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
		}
		elsif( $cmd eq "008") { $ctrlMode = $dat; }
		elsif( $cmd eq "100") { $softStart = $dat; }
		elsif( $cmd eq "101") { $SP1Type = $dat; }
		elsif( $cmd eq "102") { $SP1 = $dat; }
		elsif( $cmd eq "103") { $SP1Delay = $dat; }
		elsif( $cmd eq "104") { $SP1Signal = $dat; }
		elsif( $cmd eq "105") { $SP1Hyst = $dat; }
		elsif( $cmd eq "106") { $WaterCool = $dat; }
		elsif( $cmd eq "107") { $ActiveStop = $dat; }
		elsif( $cmd eq "108") { $baudRate = $dat; }
		elsif( $cmd eq "109") { $resetCounters = $dat; }
		elsif( $cmd eq "110") { $interlockType = $dat; }
		elsif( $cmd eq "111") { $analogOutType = $dat; }
		elsif( $cmd eq "117") { $lowSpeedFreq = $dat; }
		elsif( $cmd eq "120") { $rotFreq = $dat; }
		elsif( $cmd eq "122") { $ventValve = $dat; }
		elsif( $cmd eq "125") { $ventValveMode = $dat; }
		elsif( $cmd eq "126") { $ventValveDelay = $dat; }
		elsif( $cmd eq "147") { $ventOpenTime = $dat; }
		elsif( $cmd eq "157") { $loadType = $dat; }
		elsif( $cmd eq "161") { $gaugePCorr = $dat; }
		elsif( $cmd eq "162") { $SP1Press = $dat; }
		elsif( $cmd eq "163") { $pressUnits = $dat; }
		elsif( $cmd eq "167") { $stopSpeedRead = $dat; }
		elsif( $cmd eq "171") { $SP2Type = $dat; }
		elsif( $cmd eq "172") { $SP2 = $dat; }
		elsif( $cmd eq "173") { $SP2Delay = $dat; }
		elsif( $cmd eq "174") { $SP2Signal = $dat; }
		elsif( $cmd eq "175") { $SP2Hyst = $dat; }
		elsif( $cmd eq "176") { $SP2Press = $dat; }
		elsif( $cmd eq "503") { $serialAddr = $dat; }
		elsif( $cmd eq "504") { $serialType = $dat; }
		else { return;}

		# Replace the args of print after "OTTY" to match the output
		# of the target device
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

		# Replace "cmd#?" with a command string and "data#" with
		# the name of the value storage variable for that
		# command 
		if( $cmd eq "000") { $dat = $runState;}
		elsif( $cmd eq "001") { $dat = $lowSpeed;}
		elsif( $cmd eq "008") { $dat = $ctrlMode;}
		elsif( $cmd eq "100") { $dat = $softStart;}
		elsif( $cmd eq "101") { $dat = $SP1Type;}
		elsif( $cmd eq "102") { $dat = $SP1;}
		elsif( $cmd eq "103") { $dat = $SP1Delay;}
		elsif( $cmd eq "104") { $dat = $SP1Signal;}
		elsif( $cmd eq "105") { $dat = $SP1Hyst;}
		elsif( $cmd eq "106") { $dat = $WaterCool;}
		elsif( $cmd eq "107") { $dat = $ActiveStop;}
		elsif( $cmd eq "108") { $dat = $baudRate;}
		elsif( $cmd eq "110") { $dat = $interlockType;}
		elsif( $cmd eq "111") { $dat = $analogOutType;}
		elsif( $cmd eq "117") { $dat = $lowSpeedFreq;}
		elsif( $cmd eq "120") { $dat = $rotFreq;}
		elsif( $cmd eq "122") { $dat = $ventValve;}
		elsif( $cmd eq "125") { $dat = $ventValveMode;}
		elsif( $cmd eq "126") { $dat = $ventValveDelay;}
		elsif( $cmd eq "147") { $dat = $ventOpenTime;}
		elsif( $cmd eq "155") { $dat = $powerLimit;}
		elsif( $cmd eq "157") { $dat = $loadType;}
		elsif( $cmd eq "161") { $dat = $gaugePCorr;}
		elsif( $cmd eq "162") { $dat = sprintf( "%6.1E ", $SP1Press);}
		elsif( $cmd eq "163") { $dat = $pressUnits;}
		elsif( $cmd eq "167") { $dat = $stopSpeedRead;}
		elsif( $cmd eq "171") { $dat = $SP2Type;}
		elsif( $cmd eq "172") { $dat = $SP2;}
		elsif( $cmd eq "173") { $dat = $SP2Delay;}
		elsif( $cmd eq "174") { $dat = $SP2Signal;}
		elsif( $cmd eq "175") { $dat = $SP2Hyst;}
		elsif( $cmd eq "176") { $dat = sprintf( "%6.1E ", $SP2Press);}
		elsif( $cmd eq "200") { $dat = $current;}
		elsif( $cmd eq "201") { $dat = $voltage;}
		elsif( $cmd eq "202") { $dat = $power;}
		elsif( $cmd eq "203") { $dat = $drivingFreq;}
		elsif( $cmd eq "204") { $dat = $temperature;}
		elsif( $cmd eq "205") { $dat = $status;}
		elsif( $cmd eq "206") { $dat = $errorCode;}
		elsif( $cmd eq "211") { $dat = $heatSinkTemp;}
		elsif( $cmd eq "216") { $dat = $airTemp;}
		elsif( $cmd eq "224") { $dat = sprintf( "%6.1E ", $pressure);}
		elsif( $cmd eq "226") { $dat = $freq;}
		elsif( $cmd eq "300") { $dat = $cycleTime;}
		elsif( $cmd eq "301") { $dat = $cycleNum;}
		elsif( $cmd eq "302") { $dat = $life;}
		elsif( $cmd eq "400") { $dat = sprintf( "%-10s", $crcEPROM);}
		elsif( $cmd eq "402") { $dat = sprintf( "%-10s", $crcPar);}
		elsif( $cmd eq "404") { $dat = sprintf( "%-10s", $crcParStruct);}
		elsif( $cmd eq "503") { $dat = $serialAddr;}
		elsif( $cmd eq "504") { $dat = $serialType;}
		else { return;}

		# Replace the args of print after "OTTY" to match the output
		# of the target device
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
#		# Replace "cmd#" with a command string and "data#" with
#		# the name of the value storage variable for that
#		# command 
#		if( $cmd eq "cmd1") { $dat = $data1; }
#		elsif( $cmd eq "cmd2") { $dat = $data2; }
#		else { return;}
#
#		# Replace the args of print after "OTTY" to match the output
#		# of the target device
#		print OTTY echoCmd(), "OK", $LT;
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
