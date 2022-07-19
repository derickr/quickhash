--TEST--
Basic tests for creating an IntHash
--INI--
--FILE--
<?php
var_dump( new QuickHashIntHash( 1024 ) );
var_dump( new QuickHashIntHash( 1024, 1 ) );
var_dump( new QuickHashIntHash( 1024, QuickHashIntHash::CHECK_FOR_DUPES ) );

try
{
	new QuickHashIntHash();
}
catch ( Exception $e )
{
	echo $e->getMessage(), "\n";
}
catch ( Error $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	new QuickHashIntHash( 1024, QuickHashIntHash::CHECK_FOR_DUPES, 4 );
}
catch ( Exception $e )
{
	echo $e->getMessage(), "\n";
}
catch ( Error $e )
{
	echo $e->getMessage(), "\n";
}
?>
--EXPECTF--
object(QuickHashIntHash)#1 (0) {
}
object(QuickHashIntHash)#1 (0) {
}
object(QuickHashIntHash)#1 (0) {
}
QuickHashIntHash::__construct() expects at least 1 %s, 0 given
QuickHashIntHash::__construct() expects at most 2 %s, 3 given
