# Perl module that starts the exampleDatabase,
# then runs a test program while the IOC is active.

use strict;
use warnings;

use Cwd qw(getcwd abs_path);
use IOC;

$ENV{HARNESS_ACTIVE} = 1 if scalar @ARGV && shift eq '-tap';

my $arch = $ENV{EPICS_HOST_ARCH};

my $iocTop = abs_path('../../database');
my $iocExe = "$iocTop/bin/$arch/exampleDatabase";
my $iocDir = "$iocTop/iocBoot/exampleDatabase";
my $iocArg = 'st.cmd';

sub runTest {
    my $test = shift;

    # Create IOC object
    my $ioc = IOC->new();
    # $ioc->debug(1);

    # Save cwd, go to $iocDir
    my $runDir = getcwd();
    chdir $iocDir;

    # Run it
    $ioc->start($iocExe, $iocArg);

    # Wait for command prompt after startup script finishes
    $ioc->cmd;

    # Go back where we were
    chdir $runDir;

    # Run the test program
    my $ret = system "./$test";

    # Stop the IOC
    $ioc->kill;

    # Finish
    exit $ret;
}

1;
