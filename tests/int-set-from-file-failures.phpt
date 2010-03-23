--TEST--
Test for failure conditions for loadFromFile.
--INI--
xdebug.default_enable=0
--FILE--
<?php
echo "\nWrong params: \n";
try
{
	$hash = QuickHashIntSet::loadFromFile();
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	$hash = QuickHashIntSet::loadFromFile( 1024, 1024, 2, 'stuff' );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	$hash = QuickHashIntSet::loadFromFile( 1024, 2, 'stuff' );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	$hash = QuickHashIntSet::loadFromFile( 1024, 'stuff' );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	$hash = QuickHashIntSet::loadFromFile( new StdClass );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

echo "\nEmpty file: \n";
try
{
	$hash = QuickHashIntSet::loadFromFile( '' );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

echo "\nDirectory: \n";
try
{
	$file = dirname( __FILE__ );
	$hash = QuickHashIntSet::loadFromFile( $file );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

echo "\nNon existing: \n";
try
{
	$file = dirname( __FILE__ ) . "/does-not-exist.set";
	$hash = QuickHashIntSet::loadFromFile( $file );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

echo "\nWrong size: \n";
try
{
	$file = dirname( __FILE__ ) . "/broken-file.set";
	$hash = QuickHashIntSet::loadFromFile( $file );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}

echo "\nURL: \n";
try
{
	$hash = QuickHashIntSet::loadFromFile( "http://derickrethans.nl/" );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}
?>
--EXPECT--

Wrong params: 
QuickHashIntSet::loadFromFile() expects at least 1 parameter, 0 given
QuickHashIntSet::loadFromFile() expects at most 3 parameters, 4 given
QuickHashIntSet::loadFromFile() expects parameter 3 to be long, string given
QuickHashIntSet::loadFromFile() expects parameter 2 to be long, string given
QuickHashIntSet::loadFromFile() expects parameter 1 to be string, object given

Empty file: 
QuickHashIntSet::loadFromFile(): Filename cannot be empty

Directory: 
QuickHashIntSet::loadFromFile(): File is not a normal file

Non existing: 
QuickHashIntSet::loadFromFile(/home/derick/dev/quickhash/tests/does-not-exist.set): failed to open stream: No such file or directory

Wrong size: 
QuickHashIntSet::loadFromFile(): File is in the wrong format

URL: 
QuickHashIntSet::loadFromFile(): Could not obtain file information
