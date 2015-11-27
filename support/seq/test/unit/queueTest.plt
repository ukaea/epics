use strict;
use Cwd;

my $host_arch = $ENV{EPICS_HOST_ARCH};

my $path = $ENV{PATH};

my $top = Cwd::abs_path($ENV{TOP});

my $pathsep = ':';
my $exe = '';
if ("$host_arch" =~ /win32/ || "$host_arch" =~ /windows/) {
  $pathsep = ';';
  $exe = '.exe';
}

$ENV{HARNESS_ACTIVE} = 1;
$ENV{PATH} = "$top/bin/$host_arch$pathsep$path";

exec "./queueTest$exe" or die 'exec failed';
