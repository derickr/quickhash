--TEST--
Test for failure conditions for saveToString.
--INI--
--FILE--
<?php
$hash = new QuickHashIntStringHash( 1024 );

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
QuickHashIntStringHash::saveToString() expects exactly 0 parameters, 2 given
QuickHashIntStringHash::saveToString() expects exactly 0 parameters, 1 given
