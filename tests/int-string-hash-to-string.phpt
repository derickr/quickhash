--TEST--
Basic test for returning hashes as strings.
--INI--
xdebug.default_enable=0
--FILE--
<?php
$file = dirname( __FILE__ ) . "/simple.string.hash";
$hash = QuickHashIntStringHash::loadFromFile( $file );
$string = $hash->saveToString();

for ( $i = 0; $i < strlen( $string ); $i++ )
{
	printf( '%02X ', ord( $string[$i] ) );
	if ( $i % 16 == 15 ) {
		echo "\n";
	}
}
echo "\n\n";

$hash = QuickHashIntStringHash::loadFromString( $string );

foreach( range( 0, 0x1f ) as $key )
{
	printf( "Key %3d (%2x) is ", $key, $key );
	$value = $hash->get( $key );
	print $value === false ? 'unset' : $value;
	print "\n";
}
?>
--EXPECT--
FE FF FF FF EC 00 00 00 6F 6E 65 2E 00 66 6F 75 
72 2E 00 6E 69 6E 65 2E 00 74 77 65 6E 74 79 20 
66 69 76 65 2E 00 66 6F 75 72 74 79 20 6E 69 6E 
65 2E 00 68 75 6E 64 72 65 64 20 74 77 65 6E 74 
79 2E 00 6F 6E 65 20 68 75 6E 64 72 65 64 20 6E 
69 6E 65 74 79 20 73 69 78 2E 00 74 77 6F 20 68 
75 6E 64 72 65 64 20 65 69 67 68 74 79 20 6E 69 
6E 65 2E 00 74 68 72 65 65 20 68 75 6E 64 65 72 
64 20 73 69 78 74 79 20 6F 6E 65 2E 00 66 69 76 
65 20 68 75 6E 64 72 65 64 20 74 77 65 6E 74 79 
20 6E 69 6E 65 2E 00 73 65 76 65 6E 20 68 75 6E 
64 72 65 64 20 74 77 65 6E 74 79 20 6E 69 6E 65 
2E 00 65 69 67 68 74 20 68 75 6E 64 72 65 64 20 
66 6F 75 72 74 79 20 6F 6E 65 2E 00 6E 69 6E 65 
20 68 75 6E 64 72 65 64 20 73 69 78 74 79 20 6F 
6E 65 2E 00 11 00 00 00 53 00 00 00 0B 00 00 00 
2B 00 00 00 0D 00 00 00 3B 00 00 00 1D 00 00 00 
BA 00 00 00 05 00 00 00 11 00 00 00 1B 00 00 00 
9F 00 00 00 17 00 00 00 85 00 00 00 01 00 00 00 
00 00 00 00 13 00 00 00 6C 00 00 00 07 00 00 00 
1E 00 00 00 02 00 00 00 05 00 00 00 1F 00 00 00 
D4 00 00 00 03 00 00 00 0B 00 00 00 

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
