--TEST--
Basic tests for creating an IntHash
--INI--
xdebug.default_enable=0
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

try
{
	new QuickHashIntHash( 1024, QuickHashIntHash::CHECK_FOR_DUPES, 4 );
}
catch ( Exception $e )
{
	echo $e->getMessage(), "\n";
}
?>
--EXPECT--
object(QuickHashIntHash)#1 (0) {
}
object(QuickHashIntHash)#1 (0) {
}
object(QuickHashIntHash)#1 (0) {
}
QuickHashIntSet::__construct() expects at least 1 parameter, 0 given
QuickHashIntSet::__construct() expects at most 2 parameters, 3 given
