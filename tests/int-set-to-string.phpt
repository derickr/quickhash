--TEST--
Basic test for returning sets as strings.
--INI--
--FILE--
<?php
$file = dirname( __FILE__ ) . "/simple.set";
$hash = QuickHashIntSet::loadFromFile( $file );
$string = $hash->saveToString();

for ( $i = 0; $i < strlen( $string ); $i++ )
{
	printf( '%02X ', ord( $string[$i] ) );
	if ( $i % 16 == 15 ) {
		echo "\n";
	}
}
echo "\n\n";

$hash = QuickHashIntSet::loadFromString( $string );

foreach( range( 0, 0x1f ) as $key )
{
	printf( "Key %3d (%2x) is %s\n", $key, $key, $hash->exists( $key ) ? 'set' : 'unset' );
}
?>
--EXPECT--
11 00 00 00 0B 00 00 00 0D 00 00 00 1D 00 00 00 
05 00 00 00 1B 00 00 00 17 00 00 00 01 00 00 00 
13 00 00 00 07 00 00 00 02 00 00 00 1F 00 00 00 
03 00 00 00 

Key   0 ( 0) is unset
Key   1 ( 1) is set
Key   2 ( 2) is set
Key   3 ( 3) is set
Key   4 ( 4) is unset
Key   5 ( 5) is set
Key   6 ( 6) is unset
Key   7 ( 7) is set
Key   8 ( 8) is unset
Key   9 ( 9) is unset
Key  10 ( a) is unset
Key  11 ( b) is set
Key  12 ( c) is unset
Key  13 ( d) is set
Key  14 ( e) is unset
Key  15 ( f) is unset
Key  16 (10) is unset
Key  17 (11) is set
Key  18 (12) is unset
Key  19 (13) is set
Key  20 (14) is unset
Key  21 (15) is unset
Key  22 (16) is unset
Key  23 (17) is set
Key  24 (18) is unset
Key  25 (19) is unset
Key  26 (1a) is unset
Key  27 (1b) is set
Key  28 (1c) is unset
Key  29 (1d) is set
Key  30 (1e) is unset
Key  31 (1f) is set
