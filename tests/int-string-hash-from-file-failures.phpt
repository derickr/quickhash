--TEST--
Test for failure conditions for loadFromFile.
--INI--
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
catch( Error $e )
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
catch( Error $e )
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
catch( Error $e )
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
catch( Error $e )
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
catch( Error $e )
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
catch( Error $e )
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
catch( Error $e )
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
catch( Error $e )
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
catch( Error $e )
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
catch( Error $e )
{
	echo $e->getMessage(), "\n";
}
?>
--EXPECTF--

Wrong params: 
QuickHashIntStringHash::loadFromFile() expects at least 1 %s, 0 given
QuickHashIntStringHash::loadFromFile() expects at most 3 %s, 4 given
QuickHashIntStringHash::loadFromFile()%s%s, string given
QuickHashIntStringHash::loadFromFile()%s%s, string given
QuickHashIntStringHash::loadFromFile()%sstring, %s given

Empty file: 
%s

Directory: 
QuickHashIntStringHash::loadFromFile(): File is not a normal file

Non existing: 
QuickHashIntStringHash::loadFromFile(%stests/does-not-exist.set): %s to open stream: No such file or directory

Wrong size: 
QuickHashIntStringHash::loadFromFile(): File is in the wrong format

URL: 
QuickHashIntStringHash::loadFromFile(): Could not obtain file information
