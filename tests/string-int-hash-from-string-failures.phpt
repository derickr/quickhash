--TEST--
Test for failure conditions for loadFromString.
--INI--
--FILE--
<?php
echo "\nWrong params: \n";
try
{
	$hash = QuickHashStringIntHash::loadFromString();
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	$hash = QuickHashStringIntHash::loadFromString( 1024, 1024, 2, 'stuff' );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	$hash = QuickHashStringIntHash::loadFromString( 1024, 2, 'stuff' );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	$hash = QuickHashStringIntHash::loadFromString( 1024, 'stuff' );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	$hash = QuickHashStringIntHash::loadFromString( new StdClass );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

echo "\nWrong size: \n";
try
{
	$contents = file_get_contents( dirname( __FILE__ ) . "/broken-file.hash" );
	$hash = QuickHashStringIntHash::loadFromString( $contents );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}
?>
--EXPECTF--

Wrong params: 
QuickHashStringIntHash::loadFromString() expects at least 1 parameter, 0 given
QuickHashStringIntHash::loadFromString() expects at most 3 parameters, 4 given
QuickHashStringIntHash::loadFromString() expects parameter 3 to be %s, string given
QuickHashStringIntHash::loadFromString() expects parameter 2 to be %s, string given
QuickHashStringIntHash::loadFromString() expects parameter 1 to be string, object given

Wrong size: 
QuickHashStringIntHash::loadFromString(): String is in the wrong format
