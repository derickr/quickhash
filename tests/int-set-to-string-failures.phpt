--TEST--
Test for failure conditions for saveToString.
--INI--
--FILE--
<?php
$hash = new QuickHashIntSet( 1024 );

echo "\nWrong params: \n";
try
{
	$hash->saveToString( 1024, 'stuff' );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}
catch( Error $e )
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
catch( Error $e )
{
	echo $e->getMessage(), "\n";
}
?>
--EXPECTF--

Wrong params: 
QuickHashIntSet::saveToString() expects exactly 0 %s, 2 given
QuickHashIntSet::saveToString() expects exactly 0 %s, 1 given
