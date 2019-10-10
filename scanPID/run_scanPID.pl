#!/apps/perl/bin/perl

$stubFile = $ARGV[0];
$tFlag = $ARGV[1];
$home = $ENV{"HOME"};
$binpath = "$home/bin/Linux";

open (FILES2PROCESS, "$stubFile") || die "Can't open $stubFile\n";

while (defined($file = <FILES2PROCESS>)) {
  chomp($file);
  if($file =~ /.*run(\d+)/){
    $run = $1;

    if($tFlag==0 || $tFlag==1){
      $num = "all";
      $cmd = "$binpath/scanPID $file";
      print "EXECUTING : $cmd\n";
      system("date");
      system("$cmd");
      rename_files();
      print "Done $run\n";
    }
    if($tFlag==0 || $tFlag==2){
      for($j=2;$j<6;$j++){
	if($j<4){
	  $trig = 2**$j;
	}
	if($j>=4 && $j<8){
	  $trig = 2**($j-4)*10;
	}
	$num = "tr$j";
	$cmd = "$binpath/scanPID -t0x$trig $file";
	print "EXECUTING : $cmd\n";
	system("date");
	system("$cmd");
	rename_files();
	print "Done $run\n";
      }
    }
  }
}

close (FILES2PROCESS);

sub rename_files(){

  if (-e "scanPID.root") {
    $cp_tree = "cp -f -v scanPID.root scanPID$run\_$num.root";
    system("$cp_tree");
    $rm_tree = "/bin/rm scanPID.root";
    system("$rm_tree");
  }
}
