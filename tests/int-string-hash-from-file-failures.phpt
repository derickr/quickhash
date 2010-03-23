--TEST--
Test for failure conditions for loadFromFile.
--INI--
xdebug.default_enable=0
--FILE--
<?php
echo "\nWrong params: \n";
try
{
	$hash = QuickHashIntStringHash::loadFromFile();
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	$hash = QuickHashIntStringHash::loadFromFile( 1024, 1024, 2, 'stuff' );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	$hash = QuickHashIntStringHash::loadFromFile( 1024, 2, 'stuff' );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	$hash = QuickHashIntStringHash::loadFromFile( 1024, 'stuff' );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	$hash = QuickHashIntStringHash::loadFromFile( new StdClass );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

echo "\nEmpty file: \n";
try
{
	$hash = QuickHashIntStringHash::loadFromFile( '' );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

echo "\nDirectory: \n";
try
{
	$file = dirname( __FILE__ );
	$hash = QuickHashIntStringHash::loadFromFile( $file );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

echo "\nNon existing: \n";
try
{
	$file = dirname( __FILE__ ) . "/does-not-exist.set";
	$hash = QuickHashIntStringHash::loadFromFile( $file );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

echo "\nWrong size: \n";
try
{
	$file = dirname( __FILE__ ) . "/broken-file.set";
	$hash = QuickHashIntStringHash::loadFromFile( $file );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

echo "\nURL: \n";
try
{
	$hash = QuickHashIntStringHash::loadFromFile( "http://derickrethans.nl/" );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}
?>
--EXPECT--

Wrong params: 
QuickHashIntStringHash::loadFromFile() expects at least 1 parameter, 0 given
QuickHashIntStringHash::loadFromFile() expects at most 3 parameters, 4 given
QuickHashIntStringHash::loadFromFile() expects parameter 3 to be long, string given
QuickHashIntStringHash::loadFromFile() expects parameter 2 to be long, string given
QuickHashIntStringHash::loadFromFile() expects parameter 1 to be string, object given

Empty file: 
QuickHashIntStringHash::loadFromFile(): Filename cannot be empty

Directory: 
QuickHashIntStringHash::loadFromFile(): File is not a normal file

Non existing: 
QuickHashIntStringHash::loadFromFile(/home/derick/dev/quickhash/tests/does-not-exist.set): failed to open stream: No such file or directory

Wrong size: 
QuickHashIntStringHash::loadFromFile(): File is in the wrong format

URL: 
QuickHashIntStringHash::loadFromFile(): Could not obtain file information
