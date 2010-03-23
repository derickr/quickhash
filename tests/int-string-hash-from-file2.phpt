--TEST--
Slightly larger test for reading hashes from files.
--INI--
xdebug.default_enable=0
--FILE--
<?php
$found = 0;
$file = dirname( __FILE__ ) . "/tags.string.hash";
$hash = QuickHashIntStringHash::loadFromFile( $file );
for ( $i = 1; $i <= 200000; $i++ )
{
	$found += $hash->exists( $i );
}
printf( "Found: %d\n", $found );
echo memory_get_usage(), "\n";
echo memory_get_peak_usage(), "\n";
unset( $hash );
echo memory_get_usage(), "\n";

$found = 0;
$hash = QuickHashIntStringHash::loadFromFile( $file, 0, QuickHashIntStringHash::DO_NOT_USE_ZEND_ALLOC );
for ( $i = 1; $i <= 200000; $i++ )
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
Found: 200000
%d
%d
%d
Found: 200000
%d
%d
%d
