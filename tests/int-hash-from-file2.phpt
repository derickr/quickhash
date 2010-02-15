--TEST--
Slightly larger test for reading hashes from files.
--INI--
xdebug.default_enable=0
--FILE--
<?php
$found = 0; $value = 0;
$file = dirname( __FILE__ ) . "/primes.hash";
$hash = QuickHashIntHash::loadFromFile( $file );
for ( $i = 3; $i < 1299828; $i += 2 )
{
	$found += $hash->exists( $i );
	if ( $found ) {
		$value += $hash->getValue( $i );
	}
}
printf( "Found: %d (%d)\n", $found, $value );
echo memory_get_usage(), "\n";
echo memory_get_peak_usage(), "\n";
unset( $hash );
echo memory_get_usage(), "\n";

$found = 0; $value = 0;
$hash = QuickHashIntHash::loadFromFile( $file, QuickHashIntHash::DO_NOT_USE_ZEND_ALLOC );
for ( $i = 3; $i < 1299828; $i += 2 )
{
	$found += $hash->exists( $i );
	if ( $found ) {
		$value += $hash->getValue( $i );
	}
}
printf( "Found: %d (%d)\n", $found, $value );
echo memory_get_usage(), "\n";
echo memory_get_peak_usage(), "\n";
unset( $hash );
echo memory_get_usage(), "\n";
?>
--EXPECTF--
Found: 50003 (31135874555)
%d
%d
%d
Found: 50003 (31135874555)
%d
%d
%d
