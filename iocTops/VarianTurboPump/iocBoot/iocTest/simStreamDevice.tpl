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
# > perl simStreamDevice
# or
# > simStreamDevice
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

$RC = chr( 0x02);	# start character
$EC = chr( 0x03);	# end character
$ACK = chr( 0x06);	# acknowledgement character
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
	print "Usage: simStreamDevice.pl  [-c pipe] [-d <devname>] [-D <level>]\n";
	print "   or\n";
	print "       simStreamDevice.pl  [-c serial -d <devname>] [-D <level>]\n";
	print "   or\n";
	print "       simStreamDevice.pl  [-c ip -d <devname>] [-D <level>]\n";
	print "   or\n";
	print "       simStreamDevice.pl  -h|--help\n";
	print "   where:\n";
	print "          -c : communication bus (default is \"pipe\")\n";
	print "          -d : name of pipe (default: $TTY) for default or '-c pipe'\n";
	print "               or device name (e.g. /dev/ttyS0) for '-c serial',\n";
	print "               or IP address/hostname:port (e.g. localhost:9001) for '-c ip'.\n";
	print "  -h, --help : this usage message.\n";
	print "          -D : set debug level.\n";
}

