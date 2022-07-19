--TEST--
Basic tests for creating an StringIntHash
--INI--
--FILE--
<?php
var_dump( new QuickHashStringIntHash( 1024 ) );
var_dump( new QuickHashStringIntHash( 1024, 1 ) );
var_dump( new QuickHashStringIntHash( 1024, QuickHashStringIntHash::CHECK_FOR_DUPES ) );

try
{
	new QuickHashStringIntHash();
}
catch ( Exception $e )
{
	echo $e->getMessage(), "\n";
}
catch( Error $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	new QuickHashStringIntHash( 1024, QuickHashStringIntHash::CHECK_FOR_DUPES, 4 );
}
catch ( Exception $e )
{
	echo $e->getMessage(), "\n";
}
catch( Error $e )
{
	echo $e->getMessage(), "\n";
}
?>
--EXPECTF--
object(QuickHashStringIntHash)#1 (0) {
}
object(QuickHashStringIntHash)#1 (0) {
}
object(QuickHashStringIntHash)#1 (0) {
}
QuickHashStringIntHash::__construct() expects at least 1 %s, 0 given
QuickHashStringIntHash::__construct() expects at most 2 %s, 3 given
