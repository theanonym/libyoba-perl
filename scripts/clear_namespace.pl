#!/usr/bin/perl
use 5.010;
use strict;
use Fatal qw/open/;
use Config;

my @input = qw/EXTERN.h perl.h embed.h perlapi.h sv.h av.h hv.h/;

open my $out, '>', "include/yobaperl/clear_namespace.hpp";
print $out <<END;
// Include this file after perl.hpp to clear global namespace

END

for my $file (@input) {
   open my $in, '<', "$Config{archlibexp}/CORE/$file";
   for(<$in>) {
      if(/^#\s*?define\s+(.*?)[\s\(]/) {
         #~ print $out "#ifdef $1\n#undef $1\n#endif\n";
         print $out "#undef $1\n";
      }
   }
}
