--TEST--
Test for iterating over a hash (with three different hashing algorithm).
--INI--
xdebug.default_enable=0
--FILE--
<?php
$found = 0; $value = 0;
$file = dirname( __FILE__ ) . "/simple.string.hash";
$hash = QuickHashIntStringHash::loadFromFile( $file, QuickHashIntStringHash::HASHER_NO_HASH );
foreach( $hash as $key => $value )
{
	printf( "%4d-%s\n", $key, $value );
}
echo "\n";
$hash = QuickHashIntStringHash::loadFromFile( $file, QuickHashIntStringHash::HASHER_JENKINS1 );
foreach( $hash as $key => $value )
{
	printf( "%4d-%s\n", $key, $value );
}
echo "\n";
$hash = QuickHashIntStringHash::loadFromFile( $file, QuickHashIntStringHash::HASHER_JENKINS2 );
foreach( $hash as $key => $value )
{
	printf( "%4d-%s\n", $key, $value );
}
echo "\n";
?>
--EXPECTF--
  17-two hundred eighty nine.
  23-five hundred twenty nine.
  13-one hundred ninety six.
   2-four.
  31-nine hundred sixty one.
   5-twenty five.
  27-seven hundred twenty nine.
   1-one.
   3-nine.
  11-hundred twenty.
  29-eight hundred fourty one.
  19-three hunderd sixty one.
   7-fourty nine.

  23-five hundred twenty nine.
  13-one hundred ninety six.
   5-twenty five.
  27-seven hundred twenty nine.
   1-one.
  11-hundred twenty.
  29-eight hundred fourty one.
   7-fourty nine.
  17-two hundred eighty nine.
   2-four.
  31-nine hundred sixty one.
   3-nine.
  19-three hunderd sixty one.

  13-one hundred ninety six.
   1-one.
   2-four.
  19-three hunderd sixty one.
  23-five hundred twenty nine.
   5-twenty five.
  27-seven hundred twenty nine.
  11-hundred twenty.
  29-eight hundred fourty one.
   7-fourty nine.
  17-two hundred eighty nine.
  31-nine hundred sixty one.
   3-nine.
