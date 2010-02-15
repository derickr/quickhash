--TEST--
Test with a very small hash.
--INI--
xdebug.default_enable=0
--FILE--
<?php
$found = 0;
$values = 0;
$file = dirname( __FILE__ ) . "/small.hash";
$hash = QuickHashIntHash::loadFromFile( $file );
for ( $i = 0; $i < 10; $i++ )
{
	if ( $found += $hash->exists( $i ) )
	{
		$values += $hash->getValue( $i );
	}
}
printf( "Found: %d (%d)\n", $found, $values );
?>
--EXPECT--
Found: 2 (10)
