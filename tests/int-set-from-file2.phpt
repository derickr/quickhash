--TEST--
Slightly larger test for reading sets from files.
--INI--
xdebug.default_enable=0
--FILE--
<?php
$found = 0;
$file = dirname( __FILE__ ) . "/primes.set";
$hash = QuickHashIntSet::loadFromFile( $file );
for ( $i = 3; $i < 1299828; $i += 2 )
{
	$found += $hash->exists( $i );
}
printf( "Found: %d\n", $found );
echo memory_get_usage(), "\n";
echo memory_get_peak_usage(), "\n";
unset( $hash );
echo memory_get_usage(), "\n";

$found = 0;
$hash = QuickHashIntSet::loadFromFile( $file, 0, QuickHashIntSet::DO_NOT_USE_ZEND_ALLOC );
for ( $i = 3; $i < 1299828; $i += 2 )
{
	$found += $hash->exists( $i );
}
printf( "Found: %d\n", $found );
echo memory_get_usage(), "\n";
echo memory_get_peak_usage(), "\n";
unset( $hash );
echo memory_get_usage(), "\n";
?>
--EXPECTF--
Found: 100007
%d
%d
%d
Found: 100007
%d
%d
%d
