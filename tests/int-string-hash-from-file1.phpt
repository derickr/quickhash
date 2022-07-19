--TEST--
Basic test for reading hashes from files.
--INI--
--FILE--
<?php
$file = dirname( __FILE__ ) . "/simple.string.hash";
$hash = QuickHashIntStringHash::loadFromFile( $file, 0, QuickHashIntStringHash::DO_NOT_USE_ZEND_ALLOC );
foreach( range( 0, 0x1f ) as $key )
{
	printf( "Key %3d (%2x) is ", $key, $key );
	$value = $hash->get( $key );
	print $value === false ? 'unset' : $value;
	print "\n";
}
?>
--EXPECT--
Key   0 ( 0) is unset
Key   1 ( 1) is one.
Key   2 ( 2) is four.
Key   3 ( 3) is nine.
Key   4 ( 4) is unset
Key   5 ( 5) is twenty five.
Key   6 ( 6) is unset
Key   7 ( 7) is fourty nine.
Key   8 ( 8) is unset
Key   9 ( 9) is unset
Key  10 ( a) is unset
Key  11 ( b) is hundred twenty.
Key  12 ( c) is unset
Key  13 ( d) is one hundred ninety six.
Key  14 ( e) is unset
Key  15 ( f) is unset
Key  16 (10) is unset
Key  17 (11) is two hundred eighty nine.
Key  18 (12) is unset
Key  19 (13) is three hunderd sixty one.
Key  20 (14) is unset
Key  21 (15) is unset
Key  22 (16) is unset
Key  23 (17) is five hundred twenty nine.
Key  24 (18) is unset
Key  25 (19) is unset
Key  26 (1a) is unset
Key  27 (1b) is seven hundred twenty nine.
Key  28 (1c) is unset
Key  29 (1d) is eight hundred fourty one.
Key  30 (1e) is unset
Key  31 (1f) is nine hundred sixty one.
