--TEST--
Test with a very small hash.
--INI--
xdebug.default_enable=0
--FILE--
<?php
$found = 0;
$values = '';
$file = dirname( __FILE__ ) . "/small.string.hash";
$hash = QuickHashIntStringHash::loadFromFile( $file );
for ( $i = 0; $i < 10; $i++ )
{
	if ( $found += $hash->exists( $i ) )
	{
		$values .= $hash->get( $i );
	}
}
printf( "Found: %d (%s)\n", $found, $values );
?>
--EXPECT--
Found: 2 (ONENINE)
