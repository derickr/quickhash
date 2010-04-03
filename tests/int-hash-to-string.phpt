--TEST--
Basic test for returning hashes as strings.
--INI--
xdebug.default_enable=0
--FILE--
<?php
$file = dirname( __FILE__ ) . "/simple.hash";
$hash = QuickHashIntHash::loadFromFile( $file );
$string = $hash->saveToString();

for ( $i = 0; $i < strlen( $string ); $i++ )
{
	printf( '%02X ', ord( $string[$i] ) );
	if ( $i % 16 == 15 ) {
		echo "\n";
	}
}
echo "\n\n";

$hash = QuickHashIntHash::loadFromString( $string );

foreach( range( 0, 0x1f ) as $key )
{
	printf( "Key %3d (%2x) is ", $key, $key );
	$value = $hash->get( $key );
	print $value === false ? 'unset' : $value;
	print "\n";
}
?>
--EXPECT--
51 48 11 00 0D 00 00 00 11 00 00 00 21 01 00 00 
0B 00 00 00 79 00 00 00 0D 00 00 00 C4 00 00 00 
1D 00 00 00 49 03 00 00 05 00 00 00 19 00 00 00 
1B 00 00 00 D9 02 00 00 17 00 00 00 11 02 00 00 
01 00 00 00 01 00 00 00 13 00 00 00 69 01 00 00 
07 00 00 00 31 00 00 00 02 00 00 00 04 00 00 00 
1F 00 00 00 C1 03 00 00 03 00 00 00 09 00 00 00 


Key   0 ( 0) is unset
Key   1 ( 1) is 1
Key   2 ( 2) is 4
Key   3 ( 3) is 9
Key   4 ( 4) is unset
Key   5 ( 5) is 25
Key   6 ( 6) is unset
Key   7 ( 7) is 49
Key   8 ( 8) is unset
Key   9 ( 9) is unset
Key  10 ( a) is unset
Key  11 ( b) is 121
Key  12 ( c) is unset
Key  13 ( d) is 196
Key  14 ( e) is unset
Key  15 ( f) is unset
Key  16 (10) is unset
Key  17 (11) is 289
Key  18 (12) is unset
Key  19 (13) is 361
Key  20 (14) is unset
Key  21 (15) is unset
Key  22 (16) is unset
Key  23 (17) is 529
Key  24 (18) is unset
Key  25 (19) is unset
Key  26 (1a) is unset
Key  27 (1b) is 729
Key  28 (1c) is unset
Key  29 (1d) is 841
Key  30 (1e) is unset
Key  31 (1f) is 961
