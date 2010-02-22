--TEST--
Test for failure conditions for saveToString.
--INI--
xdebug.default_enable=0
--FILE--
<?php
$hash = new QuickHashIntHash( 1024 );

echo "\nWrong params: \n";
try
{
	$hash->saveToString( 1024, 'stuff' );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	$hash->saveToString( new StdClass );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}
?>
--EXPECT--

Wrong params: 
QuickHashIntHash::saveToString() expects exactly 0 parameters, 2 given
QuickHashIntHash::saveToString() expects exactly 0 parameters, 1 given
