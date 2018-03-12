#! /usr/bin/perl

use warnings;
use strict;
use File::Copy;

my $usage = <<EOF;
Usage: $0 MOD_NAME TEST_NAME(s)
Generate test templates for a module.
Example:
	$0 mod mod_func1 mod_func2
	This example will generate test templates test_cns_mod_func1 and
	test_cns_mod_func2 for module cns_mod in file ../test/test_cns_mod.c
EOF
if (@ARGV < 1) {
  print $usage;
  exit;
}

my $suite_name = $ARGV[0];
my @test_names = @ARGV[1..$#ARGV];

my $tests_str = "";
my $test_add_str = "";
foreach my $test_name (@test_names) {
  $tests_str = $tests_str.<<EOF;

START_TEST(test_cns_${test_name})
{
}
END_TEST
EOF

  $test_add_str = $test_add_str.<<EOF;
	tcase_add_test(tc_$suite_name, test_cns_${test_name});
EOF
}
chomp $tests_str;
chomp $test_add_str;

my $suite_tpl = <<EOF;
#include "test_cnnsim.h"
#include "../src/cns_util.h"
#include "../src/cns_${suite_name}.h"

static void setup(void)
{
}

static void teardown(void)
{
}
$tests_str
/* end of tests */

Suite *make_${suite_name}_suite(void)
{
	Suite *s;
	TCase *tc_${suite_name};

	s = suite_create("${suite_name}");
	tc_${suite_name} = tcase_create("${suite_name}");
	tcase_add_checked_fixture(tc_${suite_name}, setup, teardown);

$test_add_str
	/* end of adding tests */

	suite_add_tcase(s, tc_${suite_name});

	return s;
}
EOF

my $test_file = "../test/test_cns_${suite_name}.c";
if (-e $test_file) {
  copy($test_file, "$test_file.bak")
    or die "Cannot backup file $test_file: $!";
  open TEST_BAK, '<', "$test_file.bak"
    or die "Cannot open $test_file.bak: $!";
  open TEST, '>', $test_file
    or die "Cannot open $test_file: $!";
  while (<TEST_BAK>) {
    s|/\* end of tests \*/|$tests_str\n/* end of tests */|;
    s|\t/\* end of adding tests \*/|$test_add_str\n\t/* end of adding tests */|;
    print TEST;
  }
  close TEST;
  close TEST_BAK;
  exit 0;
}
open TEST, '>', $test_file
  or die "Cannot open $test_file: $!";
print TEST $suite_tpl;
close TEST;

my $declare = "Suite *make_${suite_name}_suite(void);";
my $header_file = "../test/test_cnnsim.h";
copy($header_file, "$header_file.bak")
  or die "Cannot backup file $header_file: $!";
open HEADER_BAK, '<', "$header_file.bak"
  or die "Cannot open $header_file.bak: $!";
open HEADER, '>', $header_file
  or die "Cannot open $header_file: $!";
while (<HEADER_BAK>) {
  s|/\* end of declarations \*/|$declare\n\t/* end of declarations */|;
  print HEADER;
}
close HEADER;
close HEADER_BAK;

my $adding_suite = "srunner_add_suite(sr, make_${suite_name}_suite());";
my $main_file = "../test/test_cnnsim.c";
copy($main_file, "$main_file.bak")
  or die "Cannot backup file $main_file: $!";
open MAIN_BAK, '<', "$main_file.bak"
  or die "Cannot open $main_file.bak: $!";
open MAIN, '>', $main_file
  or die "Cannot open $main_file: $!";
while (<MAIN_BAK>) {
  s|/\* end of adding suites \*/|$adding_suite\n\t/* end of adding suites */|;
  print MAIN;
}
close MAIN;
close MAIN_BAK;
