--TEST--
Test for failure conditions for saveToFile.
--INI--
xdebug.default_enable=0
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
QuickHashIntSet::saveToString() expects exactly 0 parameters, 2 given
QuickHashIntSet::saveToString() expects exactly 0 parameters, 1 given
