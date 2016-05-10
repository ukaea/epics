######################################################################
#    This module provides the ability to start and stop an EPICS
#    IOC, and methods methods to interact with its ioc shell by
#    sending commands through its stdin and reading the response
#    it sends to stdout.
#
#    The ability to send commands may not work in some cases if
#    the IOC is built with GNU Readline as that can mangle the
#    terminator (command prompt string). Later versions of Base
#    support the IOCSH_HISTEDIT_DISABLE environment variable to
#    resolve this issue.
#
#    Original Author: Shantha Condamoor, SLAC
#    Creation Date: 1-Sep-2011
#    Current Author: Andrew Johnson
######################################################################

use strict;
use warnings;

package IOC;

require 5.003;

use Symbol 'gensym';
use IPC::Open3;

######################################################################
# Constructor
######################################################################

sub new {
    my $proto = shift;
    my $class = ref $proto || $proto;

    my $self  = {
        pid => undef,
        stdin => gensym,
        stdout => gensym,
        stderr => gensym,
        debug => 0,
        terminator => '__END__'
    };

    bless $self, $class;
}

######################################################################
# Get the process-id of the IOC
######################################################################

sub pid {
    my $self = shift;

    return $self->{pid};
}

######################################################################
# Get/set the debug flag
######################################################################

sub debug {
    my $self = shift;

    $self->{debug} = shift if scalar @_;
    return $self->{debug};
}

######################################################################
# Launch an IOC binary
#   $exe : binary executable
#   @args : arguments
######################################################################

sub start {
    my ($self, $exe, @args) = @_;

    # Turn off readline or its equivalents
    local $ENV{IOCSH_HISTEDIT_DISABLE} = "TRUE";

    # The iocsh prompt marks the end of the previous command
    local $ENV{IOCSH_PS1} = $self->{terminator} . "\n";

    # Run the IOC as a child process
    $self->{pid} = open3($self->{stdin}, $self->{stdout}, $self->{stderr},
        $exe, @args)
        or die "can't start $exe: $!";

    printf "#%d running %s\n", $self->{pid}, $exe if $self->{debug};
}

######################################################################
# Check if IOC was started
######################################################################
sub started {
    my $self = shift;

    return defined($self->pid);
}

######################################################################
# Send a command to the IOC's stdin
######################################################################
sub send {
    my ($self, $cmd) = @_;
    my $stdin = $self->{stdin};

    printf "#%d << %s", $self->{pid}, $cmd if $self->{debug};

    print $stdin $cmd;
}

######################################################################
# Get one line from the IOC's stdout
######################################################################
sub getline {
    my $self = shift;

    my $line = readline $self->{stdout};
    if (defined $line) {
        chomp $line;
        printf "#%d >> %s\n", $self->{pid}, $line if $self->{debug};
    }
    elsif (eof($self->{stdout})) {
        printf "#%d <EOF>\n", $self->{pid} if $self->{debug};
    }
    else {
        printf "#%d Error: %s\n", $self->{pid}, $! if $self->{debug};
    }
    return $line;
}

######################################################################
# Read lines from the IOC's stdout until EOF or terminator is seen
#   $term : string or regular expression to match last line
######################################################################
sub getlines {
    my ($self, $term) = @_;

    my @response = ();

    while (my $line = $self->getline) {
        push @response, $line;
        last if defined $term && $line =~ $term;
    }
    return @response;
}

######################################################################
# Send IOC a command and return its response
#   $cmd : iocsh command string to send
#   @args : optional command parameters
######################################################################
sub cmd {
    my ($self, $cmd, @args) = @_;

    my @response;
    my $term = $self->{terminator};

    if (defined $cmd) {
        if (@args) {
            my @qargs = map {
                m/^ (-? \d+) | (" .* ") $/x ? $_ : "\"$_\""
            } @args;
            $cmd .= ' ' . join(' ', @qargs);
        }
        $self->send("$cmd\n");

        my $echo = $self->getline;
        return () unless defined $echo;    # undef => reached EOF
        if ($echo ne $cmd) {
            return @response if $echo =~ $term;
            push @response, $echo;
        }
    }

    push @response, $self->getlines($term);
    pop @response;  # Remove terminator line
    return @response;
}

######################################################################
# Send signal SIGTERM to kill the ioc shell
######################################################################
sub kill {
    my $self = shift;

    return ()
        unless $self->started;

    close $self->{stdin};
    $self->{stdin} = gensym;

    my @response = $self->getlines; # No terminator
    close $self->{stdout};
    $self->{stdout} = gensym;

    close $self->{stderr};
    $self->{stderr} = gensym;

    kill 'TERM', $self->{pid};
    waitpid $self->{pid}, 0;
    $self->{pid} = undef;

    return @response;
}

######################################################################
# Destructor
######################################################################
sub DESTROY {
    my $self = shift;

    $self->kill;
}

######################################################################
# Shortcuts for a few common iocsh commands
######################################################################

sub dbLoadRecords {
    my ($self, $file, $macros) = @_;

    $macros = '' unless defined $macros;
    $self->cmd('dbLoadRecords', $file, $macros);
}

sub iocInit {
    shift->cmd('iocInit');
}

sub dbl {
    my $self = shift;

    return $self->cmd('dbl')
}

sub dbgf {
    my ($self, $pv) = @_;

    my @res = $self->cmd('dbgf', $pv);
    $res[0] =~ m/^DBR_[A-Z]+:\s*"?+(.*?)"?\s*$/;
    return $1;
}

sub dbpf {
    my ($self, $pv, $val) = @_;

    my @res = $self->cmd('dbpf', $pv, "\"$val\"");
    $res[0] =~ m/^DBR_[A-Z]+:\s*"?+(.*?)"?\s*$/;
    return $1;
}

######################################################################

1;

__END__
