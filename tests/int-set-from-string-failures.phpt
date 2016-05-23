--TEST--
Test for failure conditions for loadFromString.
--INI--
xdebug.default_enable=0
--FILE--
<?php
echo "\nWrong params: \n";
try
{
	$hash = QuickHashIntSet::loadFromString();
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	$hash = QuickHashIntSet::loadFromString( 1024, 1024, 2, 'stuff' );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	$hash = QuickHashIntSet::loadFromString( 1024, 2, 'stuff' );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	$hash = QuickHashIntSet::loadFromString( 1024, 'stuff' );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	$hash = QuickHashIntSet::loadFromString( new StdClass );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

echo "\nWrong size: \n";
try
{
	$contents = file_get_contents( dirname( __FILE__ ) . "/broken-file.set" );
	$hash = QuickHashIntSet::loadFromString( $contents );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}
?>
--EXPECTF--

Wrong params: 
QuickHashIntSet::loadFromString() expects at least 1 parameter, 0 given
QuickHashIntSet::loadFromString() expects at most 3 parameters, 4 given
QuickHashIntSet::loadFromString() expects parameter 3 to be %s, string given
QuickHashIntSet::loadFromString() expects parameter 2 to be %s, string given
QuickHashIntSet::loadFromString() expects parameter 1 to be string, object given

Wrong size: 
QuickHashIntSet::loadFromString(): String is in the wrong format
