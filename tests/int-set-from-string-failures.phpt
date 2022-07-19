--TEST--
Test for failure conditions for loadFromString.
--INI--
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
catch( Error $e )
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
catch( Error $e )
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
catch( Error $e )
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
catch( Error $e )
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
catch( Error $e )
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
catch( Error $e )
{
	echo $e->getMessage(), "\n";
}
?>
--EXPECTF--

Wrong params: 
QuickHashIntSet::loadFromString() expects at least 1 %s, 0 given
QuickHashIntSet::loadFromString() expects at most 3 %s, 4 given
QuickHashIntSet::loadFromString()%s, string given
QuickHashIntSet::loadFromString()%s, string given
QuickHashIntSet::loadFromString()%sstring, %s given

Wrong size: 
QuickHashIntSet::loadFromString(): String is in the wrong format
