--TEST--
Test for failure conditions for saveToFile.
--INI--
xdebug.default_enable=0
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

try
{
	$hash->saveToFile( 1024, 'stuff' );
}
catch( Exception $e )
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

echo "\nEmpty file: \n";
try
{
	$hash->saveToFile( '' );
}
catch( Exception $e )
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

echo "\nURL: \n";
try
{
	$hash->saveToFile( "http://derickrethans.nl/" );
}
catch( Exception $e )
{
	echo $e->getMessage(), "\n";
}
?>
--EXPECT--

Wrong params: 
QuickHashIntHash::saveToFile() expects exactly 1 parameter, 0 given
QuickHashIntHash::saveToFile() expects exactly 1 parameter, 2 given
QuickHashIntHash::saveToFile() expects parameter 1 to be string, object given

Empty file: 
QuickHashIntHash::saveToFile(): Filename cannot be empty

Directory: 
QuickHashIntHash::saveToFile(/home/derick/dev/quickhash/tests): failed to open stream: Is a directory

URL: 
QuickHashIntHash::saveToFile(http://derickrethans.nl/): failed to open stream: HTTP wrapper does not support writeable connections
