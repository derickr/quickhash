--TEST--
Test for string length failure conditions for loadFromString.
--INI--
xdebug.default_enable=0
--FILE--
<?php
for ( $i = 0; $i < 16; $i++ )
{
	try
	{
		$contents = "QH\x11\x00\x08\x00\x00\x00" . str_repeat( chr(1), $i );
		$hash = QuickHashIntHash::loadFromString( $contents );
		echo "+";
	}
	catch( Exception $e )
	{
		echo "-";
	}
}
?>
--EXPECT--
+-------+-------
