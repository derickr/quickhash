--TEST--
Basic test for checking values that are linked to a hash keys.
--INI--
xdebug.default_enable=0
--FILE--
<?php
$hash = new QuickHashIntStringHash( 8 );
$last1 = 0;
$last2 = 1;
foreach( range( 0, 0x1f ) as $key )
{
	$new = $last1 + $last2;
	$hash->add( $key, $new );
	$last1 = $last2;
	$last2 = $new;
}
foreach( range( 0, 0x1f ) as $key )
{
	printf( "Key %3d (%2x) is %d\n", $key, $key, $hash->get( $key ) );
}
?>
--EXPECT--
Key   0 ( 0) is 1
Key   1 ( 1) is 2
Key   2 ( 2) is 3
Key   3 ( 3) is 5
Key   4 ( 4) is 8
Key   5 ( 5) is 13
Key   6 ( 6) is 21
Key   7 ( 7) is 34
Key   8 ( 8) is 55
Key   9 ( 9) is 89
Key  10 ( a) is 144
Key  11 ( b) is 233
Key  12 ( c) is 377
Key  13 ( d) is 610
Key  14 ( e) is 987
Key  15 ( f) is 1597
Key  16 (10) is 2584
Key  17 (11) is 4181
Key  18 (12) is 6765
Key  19 (13) is 10946
Key  20 (14) is 17711
Key  21 (15) is 28657
Key  22 (16) is 46368
Key  23 (17) is 75025
Key  24 (18) is 121393
Key  25 (19) is 196418
Key  26 (1a) is 317811
Key  27 (1b) is 514229
Key  28 (1c) is 832040
Key  29 (1d) is 1346269
Key  30 (1e) is 2178309
Key  31 (1f) is 3524578
