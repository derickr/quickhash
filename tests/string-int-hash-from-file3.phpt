--TEST--
Test with a very small hash.
--INI--
--FILE--
<?php
$found = 0;
$values = '';
$file = dirname( __FILE__ ) . "/small.hash.string";
$hash = QuickHashStringIntHash::loadFromFile( $file );
foreach( array( 'ONE', 'NINE' ) as $i )
{
	if ( $found += $hash->exists( $i ) )
	{
		$values .= $i . ': ' . $hash->get( $i ) . '; ';
	}
}
printf( "Found: %d (%s)\n", $found, $values );
?>
--EXPECT--
Found: 2 (ONE: 1; NINE: 3; )
