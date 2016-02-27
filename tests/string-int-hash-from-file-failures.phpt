--TEST--
Test for failure conditions for loadFromFile.
--INI--
xdebug.default_enable=0
--FILE--
<?php
echo "\nWrong params: \n";
try
{
	$hash = QuickHashStringIntHash::loadFromFile();
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	$hash = QuickHashStringIntHash::loadFromFile( 1024, 1024, 2, 'stuff' );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	$hash = QuickHashStringIntHash::loadFromFile( 1024, 2, 'stuff' );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	$hash = QuickHashStringIntHash::loadFromFile( 1024, 'stuff' );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	$hash = QuickHashStringIntHash::loadFromFile( new StdClass );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

echo "\nEmpty file: \n";
try
{
	$hash = QuickHashStringIntHash::loadFromFile( '' );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

echo "\nDirectory: \n";
try
{
	$file = dirname( __FILE__ );
	$hash = QuickHashStringIntHash::loadFromFile( $file );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

echo "\nNon existing: \n";
try
{
	$file = dirname( __FILE__ ) . "/does-not-exist.set";
	$hash = QuickHashStringIntHash::loadFromFile( $file );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

echo "\nWrong size: \n";
try
{
	$file = dirname( __FILE__ ) . "/broken-file.set";
	$hash = QuickHashStringIntHash::loadFromFile( $file );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

echo "\nURL: \n";
try
{
	$hash = QuickHashStringIntHash::loadFromFile( "http://derickrethans.nl/" );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}
?>
--EXPECTF--

Wrong params: 
QuickHashStringIntHash::loadFromFile() expects at least 1 parameter, 0 given
QuickHashStringIntHash::loadFromFile() expects at most 3 parameters, 4 given
QuickHashStringIntHash::loadFromFile() expects parameter 3 to be integer, string given
QuickHashStringIntHash::loadFromFile() expects parameter 2 to be integer, string given
QuickHashStringIntHash::loadFromFile() expects parameter 1 to be string, object given

Empty file: 
QuickHashStringIntHash::loadFromFile(): Filename cannot be empty

Directory: 
QuickHashStringIntHash::loadFromFile(): File is not a normal file

Non existing: 
QuickHashStringIntHash::loadFromFile(%stests/does-not-exist.set): failed to open stream: No such file or directory

Wrong size: 
QuickHashStringIntHash::loadFromFile(): File is in the wrong format

URL: 
QuickHashStringIntHash::loadFromFile(): Could not obtain file information
