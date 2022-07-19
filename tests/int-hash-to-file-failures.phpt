--TEST--
Test for failure conditions for saveToFile.
--INI--
--FILE--
<?php
$hash = new QuickHashIntHash( 1024 );

echo "\nWrong params: \n";
try
{
	$hash->saveToFile();
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
	$hash->saveToFile( 1024, 'stuff' );
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
	$hash->saveToFile( new StdClass );
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
	$hash->saveToFile( '' );
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
	$hash->saveToFile( $file );
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
	$hash->saveToFile( "http://derickrethans.nl/" );
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
QuickHashIntHash::saveToFile() expects exactly 1 %s, 0 given
QuickHashIntHash::saveToFile() expects exactly 1 %s, 2 given
QuickHashIntHash::saveToFile()%sstring, %s given

Empty file: 
%s

Directory: 
QuickHashIntHash::saveToFile(%stests): %s to open stream: Is a directory

URL: 
QuickHashIntHash::saveToFile(http://derickrethans.nl/): %s to open stream: HTTP wrapper does not support writeable connection%s
