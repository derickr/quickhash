--TEST--
Test for failure conditions for loadFromString.
--INI--
xdebug.default_enable=0
--FILE--
<?php
echo "\nWrong params: \n";
try
{
	$hash = QuickHashIntStringHash::loadFromString();
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	$hash = QuickHashIntStringHash::loadFromString( 1024, 1024, 2, 'stuff' );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	$hash = QuickHashIntStringHash::loadFromString( 1024, 2, 'stuff' );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	$hash = QuickHashIntStringHash::loadFromString( 1024, 'stuff' );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	$hash = QuickHashIntStringHash::loadFromString( new StdClass );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

echo "\nWrong size: \n";
try
{
	$contents = file_get_contents( dirname( __FILE__ ) . "/broken-file.hash" );
	$hash = QuickHashIntStringHash::loadFromString( $contents );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}
?>
--EXPECTF--

Wrong params: 
QuickHashIntStringHash::loadFromString() expects at least 1 parameter, 0 given
QuickHashIntStringHash::loadFromString() expects at most 3 parameters, 4 given
QuickHashIntStringHash::loadFromString() expects parameter 3 to be %s, string given
QuickHashIntStringHash::loadFromString() expects parameter 2 to be %s, string given
QuickHashIntStringHash::loadFromString() expects parameter 1 to be string, object given

Wrong size: 
QuickHashIntStringHash::loadFromString(): String is in the wrong format
