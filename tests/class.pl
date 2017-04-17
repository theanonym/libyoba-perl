package Yoba;

use strict;
use 5.010;
use Test::More;

sub new {
   my $package = shift;
   is($package, __PACKAGE__, "Perl::newObject(classname)");

   if(@_ == 1) {
      my($param) = @_;
      is($_[0], "param", "Perl::newObject(classname, Scalar param)");
   } elsif(@_ > 1) {
      my(@params) = @_;
      my $name = "Perl::newObject(classname, Array params)";
      eq_array(\@params, [1, 2, 3]) ? pass $name : fail $name;
   }

   return bless {}, __PACKAGE__;
}

sub void_void {
   my($self) = @_;
   isa_ok($self, __PACKAGE__);
   pass("Perl::callMethod<void>");
   return;
}

sub void_scalar {
   my($self, $param) = @_;
   isa_ok($self, __PACKAGE__);
   is($param, "param", "Perl::callMethod<void, Scalar>");
   return;
}

sub void_array {
   my($self, @params) = @_;
   isa_ok($self, __PACKAGE__);

   my $name = "Perl::callMethod<void, Array>";
   eq_array(\@params, [1, 2, 3]) ? pass $name : fail $name;
   return;
}

sub scalar_void {
   my($self) = @_;
   isa_ok($self, __PACKAGE__);
   return "result";
}

sub scalar_scalar {
   my($self, $param) = @_;
   isa_ok($self, __PACKAGE__);
   is($param, "param", "Perl::callMethod<Scalar, Scalar>");
   return "result";
}

sub scalar_array {
   my($self, @params) = @_;
   isa_ok($self, __PACKAGE__);

   my $name = "Perl::callMethod<Scalar, Array>";
   eq_array(\@params, [1, 2, 3]) ? pass $name : fail $name;
   return "result";
}

sub array_void {
   my($self) = @_;
   isa_ok($self, __PACKAGE__);
   return (1, 2, 3);
}

sub array_scalar {
   my($self, $param) = @_;
   isa_ok($self, __PACKAGE__);
   is($param, "param", "Perl::callMethod<Scalar, Scalar>");
   return (1, 2, 3);
}

sub array_array {
   my($self, @params) = @_;
   isa_ok($self, __PACKAGE__);

   my $name = "Perl::callMethod<Scalar, Array>";
   eq_array(\@params, [1, 2, 3]) ? pass $name : fail $name;
   return (1, 2, 3);
}

1;
