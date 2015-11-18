my $in_prod_list = 0;
my $after_program_section_header = 0;
print ".. productionlist::\n";
while(<>) {
  if (m/^Program$/) {
    $after_program_section_header = 1;
  }
  elsif ($after_program_section_header) {
    if (m/^.. productionlist::$/) {
      $in_prod_list = 1;
    }
    elsif ($in_prod_list) {
      if (m/^\s*$/) {
        $in_prod_list = 0;
      }
      else {
        print;
      }
    }
  }
}
