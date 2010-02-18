<?php
$found = 0; $value = 0;
$file = dirname( __FILE__ ) . "/primes1.set";

for ( $i = 4; $i < 25; $i++ )
{
	$size = pow( 2, $i );
	echo $size, ', ';

	$first = microtime( true );
	for ( $j = 0; $j < 50; $j++ )
	{
		$hash = QuickHashIntSet::loadFromFile( $file, $size, QuickHashIntSet::DO_NOT_USE_ZEND_ALLOC );
	}
	$second = microtime( true );
	echo $second - $first, ", ";

	$first = microtime( true );
	$found = 0;
	for ( $j = 0; $j < 10000; $j++ )
	{
		$key = rand( 2, 1000/*1299827*/ /* 15,485,863 */);
		$found += $hash->exists( $key );
	}
	$second = microtime( true );
	echo $second - $first, ", ", $found, "\n";

	unset( $hash );
}
?>
