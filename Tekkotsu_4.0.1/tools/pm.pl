#!/afs/cs/project/skinnerbots/sw/perl/bin/perl -I/afs/cs/project/skinnerbots/sw/perl/lib/5.8.0/i686-linux-thread-multi -I/afs/cs/project/skinnerbots/sw/perl/lib/5.8.0
# Alok Ladsariya 

use threads;
use threads::shared;
use Config;
use Thread::Queue;

$nonice=0;        # don't be nice: use with CAUTION!
$verbose=0;
$max_processes=1; # awwww

my $jobs = new Thread::Queue;
my $log = new Thread::Queue;
my $stats = new Thread::Queue;
my $progress : shared = 0;
my $running : shared = 1;

sub usage {
  die "usage: ./pm.pl machines_file commands_file\n";
}

sub machine_th {
  my ($jobs, $log, $stats, $cmd, $verbose, $mPtr) = @_;

  $SIG{INT}=\&ignorer;
  $SIG{TERM}=\&ignorer;
  my %mInfo = %{$mPtr};
  $name=$mInfo{"name"};
  $basedir=$mInfo{"basedir"};
  $priority=$nonice?0:$mInfo{"priority"};

  while (1) {
    last if ($jobs->pending<1);
    my $job=$jobs->dequeue;

    $cmdcpy=$job;
    $cmdcpy=~s/_BD_/$basedir/g;
    $cmdcpy=~s/\$/\\\$/g;

    $ccolorcmd="ssh -x $name \"cd $basedir ; sh -c '/bin/nice -n+$priority $cmdcpy'\" 2>&1";

    $ret=system($ccolorcmd); 
    if ($ret) {
      $jobs->enqueue($job);
      print "(ERR) $ccolorcmd\n";
      sleep (50); 
      next;
    }

    $log->enqueue($cmdcpy);
    $stats->enqueue($cmdcpy);
    $progress++;
  }
}

sub status {
  my ($jobsNum, $log, $stats)=@_;
  $reported=-1;
  open (LOG, ">>logfile");
  open (STATS, ">statfile");
  while ($progress<$jobsNum) {
    if ($progress>$reported) {
      while ($log->pending>0) {
        print LOG $log->dequeue;
      }
      select((select(LOG), $| = 1)[0]);
      while ($stats->pending>0) {
        print STATS $stats->dequeue."\n";
      }
      select((select(STATS), $| = 1)[0]);
      print "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\bstatus: $progress/$jobsNum"; $|=1;
      $reported=$progress;
    }
    sleep(1);
  }
  print "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\bstatus: $progress/$jobsNum\n";
  while ($log->pending>0) {
    print LOG $log->dequeue;
  }
  while ($stats->pending>0) {
    print STATS $stats->dequeue;
  }
  close (LOG);
  close (STATS);
}

sub reaper {
  my $signame = shift;
  print "SIG$signame received\n" if $verbose;
  $running = 0;
  cleanup();
  exit(1);
}

sub ignorer {
  # just go to sleep and timeout the cleanup process
  sleep(25);
  exit(1);
}

sub cleanup {
  print "\ncleaning up: "; $|=1;
  sleep (1);
  foreach $mPtr (@machinesList) { 
    %mInfo=%{$mPtr};
    $name=$mInfo{"name"};
    $basedir=$mInfo{"basedir"};
    # kill processes
    system("ssh -x $name \"killall g++\" 2>&1 | cat > /dev/null");
    print "*";
  }
  print "\n";
}

$SIG{INT}=\&reaper;
$SIG{TERM}=\&reaper;

$cmd="";
$listFilename=$ARGV[1];
$machinesFilename=$ARGV[0];
$max_processes=100 unless ($max_processes);
$jobsNum=0;
$id=0;

usage() if (@ARGV < 2);

unless (-r $listFilename) { die "can't read: $listFilename\n"; } 
unless (-r $machinesFilename) { die "can't read: $machinesFilename\n"; } 
unless ($Config{useithreads}) { die "require perl 5.8.0+ with ithreads\n"; }

  for ($i=0; $i<@ARGV; $i++) {
    $cmd=$cmd." ".$ARGV[$i];
  }

# initialize jobs
  open (LIST,$listFilename);
  while (<LIST>) {
    chomp;
    next if (/^#/ || /^$/);
    $jobs->enqueue($_);
    $jobsNum++;
  }
  close (LIST);
  open (LOG, ">logfile");
  print LOG "# $cmd\n"; 
  close (LOG);

# initialize machines
$machinesNum=0;
@machinesList={};
open (MACHINES,$machinesFilename);
while (<MACHINES>) {
  chomp;
  next if (/^#/ || /^$/);
  @attrs=split;
  if (@attrs==4) {
    my %mach=("name"=>$attrs[0],
              "basedir"=>$attrs[1],
              "priority"=>$attrs[2],
              "forceprocesses"=>$attrs[3]
              );
    $machinesList[$machinesNum]=\%mach;
    $machinesNum++;
  }
}
close (MACHINES);
print "$jobsNum jobs, $machinesNum machines\n";
print "checking machines: ";
#stat
$threadsNum=0;
foreach $mPtr (@machinesList) { 
  %mInfo=%{$mPtr};
  $num_processors=0;
  $free_pct=0;
  $name=$mInfo{"name"};
#  open (ST, "ssh -x $name \"top -b -n 1 | grep CPU\" |");
#  while (<ST>) {
#    if (/\s+([\d\.]+?)%\s+idle/) {
#      $free_pct+=$1;
#      $num_processors++;
#    }
#  }
#  close (ST);
#  $processes=int($free_pct/100+0.5);
#  $processes=($processes>$max_processes)?$max_processes:$processes;
#  $min_processes=$mInfo{"forceprocesses"};
#  $processes=($processes<$min_processes)?$min_processes:$processes;
#  $verbose?print "\n  $name: $num_processors processors, $free_pct % free, $processes processes":print "*";
$processes=1;
  for ($i=0; $i<$processes; $i++) {
    $m_th = threads->new(\&machine_th, $jobs, $log, $stats, $cmd, $verbose, $mPtr);
    $threadsNum++;
  }
}
print "\n";
print "$threadsNum workers created\n";
status($jobsNum,$log,$stats);
sleep(1); # give the threads a chance to exit
#cleanup(); disable safety cleanup
