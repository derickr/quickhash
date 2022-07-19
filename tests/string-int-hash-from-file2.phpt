--TEST--
Slightly larger test for reading hashes from files.
--INI--
--FILE--
<?php
$found = 0;
$file = dirname( __FILE__ ) . "/tags.hash.string";
$hash = QuickHashStringIntHash::loadFromFile( $file );
foreach ( $hash as $key => $dummy )
{
	$found += $hash->exists( $key );
}
printf( "Found: %d\n", $found );
echo memory_get_usage(), "\n";
echo memory_get_peak_usage(), "\n";
unset( $hash );
echo memory_get_usage(), "\n";

$found = 0;
$hash = QuickHashStringIntHash::loadFromFile( $file, 0, QuickHashStringIntHash::DO_NOT_USE_ZEND_ALLOC );
foreach ( $hash as $key => $dummy )
{
	$found += $hash->exists( $key );
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
