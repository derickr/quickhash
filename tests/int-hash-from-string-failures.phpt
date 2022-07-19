--TEST--
Test for failure conditions for loadFromString.
--INI--
--FILE--
<?php
echo "\nWrong params: \n";
try
{
	$hash = QuickHashIntHash::loadFromString();
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
	$hash = QuickHashIntHash::loadFromString( 1024, 1024, 2, 'stuff' );
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
	$hash = QuickHashIntHash::loadFromString( 1024, 2, 'stuff' );
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
	$hash = QuickHashIntHash::loadFromString( 1024, 'stuff' );
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
	$hash = QuickHashIntHash::loadFromString( new StdClass );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}
catch( Error $e )
{
	echo $e->getMessage(), "\n";
}

echo "\nWrong size: \n";
try
{
	$contents = file_get_contents( dirname( __FILE__ ) . "/broken-file.hash" );
	$hash = QuickHashIntHash::loadFromString( $contents );
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
QuickHashIntHash::loadFromString() expects at least 1 %s, 0 given
QuickHashIntHash::loadFromString() expects at most 3 %s, 4 given
QuickHashIntHash::loadFromString()%s, string given
QuickHashIntHash::loadFromString()%s, string given
QuickHashIntHash::loadFromString()%s, %s given

Wrong size: 
QuickHashIntHash::loadFromString(): String is in the wrong format
