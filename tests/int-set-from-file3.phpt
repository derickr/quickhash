--TEST--
Test with a very small set.
--INI--
--FILE--
<?php
$found = 0;
$file = dirname( __FILE__ ) . "/small.set";
$hash = QuickHashIntSet::loadFromFile( $file );
for ( $i = 0; $i < 10; $i++ )
{
	$found += $hash->exists( $i );
}
printf( "Found: %d\n", $found );
?>
--EXPECT--
Found: 2
