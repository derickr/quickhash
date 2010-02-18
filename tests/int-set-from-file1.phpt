--TEST--
Basic test for reading sets from files.
--INI--
xdebug.default_enable=0
--FILE--
<?php
$file = dirname( __FILE__ ) . "/simple.set";
$hash = QuickHashIntSet::loadFromFile( $file, 0, QuickHashIntSet::DO_NOT_USE_ZEND_ALLOC );
foreach( range( 0, 0x1f ) as $key )
{
	printf( "Key %3d (%2x) is %s\n", $key, $key, $hash->exists( $key ) ? 'set' : 'unset' );
}
?>
--EXPECT--
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
