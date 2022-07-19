--TEST--
Test for string length failure conditions for loadFromString.
--INI--
--FILE--
<?php
for ( $i = 0; $i < 16; $i++ )
{
	try
	{
		$contents = str_repeat( chr(1), $i );
		$hash = QuickHashIntSet::loadFromString( $contents );
		echo "+";
	}
	catch( Exception $e )
	{
		echo "-";
	}
}
?>
--EXPECT--
+---+---+---+---
