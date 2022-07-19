--TEST--
Test for failure conditions for loadFromFile.
--INI--
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
catch( Error $e )
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
catch( Error $e )
{
	echo $e->getMessage(), "\n";
}
catch( Error $e )
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
catch( Error $e )
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
catch( Error $e )
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
catch( Error $e )
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
catch( Error $e )
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
catch( Error $e )
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
catch( Error $e )
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
catch( Error $e )
{
	echo $e->getMessage(), "\n";
}
?>
--EXPECTF--

Wrong params: 
QuickHashIntSet::loadFromFile() expects at least 1 %s, 0 given
QuickHashIntSet::loadFromFile() expects at most 3 %s, 4 given
QuickHashIntSet::loadFromFile()%s, string given
QuickHashIntSet::loadFromFile()%s, string given
QuickHashIntSet::loadFromFile()%sstring, %s given

Empty file: 
%s

Directory: 
QuickHashIntSet::loadFromFile(): File is not a normal file

Non existing: 
QuickHashIntSet::loadFromFile(%stests/does-not-exist.set): %s to open stream: No such file or directory

Wrong size: 
QuickHashIntSet::loadFromFile(): File is in the wrong format

URL: 
QuickHashIntSet::loadFromFile(): Could not obtain file information
