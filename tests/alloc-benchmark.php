<?php
$found = 0; $value = 0;
$file = dirname( __FILE__ ) . "/primes.hash";

$first = microtime( true );
$hash = QuickHashIntHash::loadFromFile( $file );
$second = microtime( true );
unset( $hash );

$third = microtime( true );
$hash = QuickHashIntHash::loadFromFile( $file, 0, QuickHashIntHash::DO_NOT_USE_ZEND_ALLOC );
$fourth = microtime( true );

echo $second - $first, "\n";
echo $fourth - $third, "\n";
?>
