--TEST--
Basic tests for creating an IntHash
--INI--
xdebug.default_enable=0
--FILE--
<?php
var_dump( new QuickHashIntSet( 1024 ) );
var_dump( new QuickHashIntSet( 1024, 1 ) );
var_dump( new QuickHashIntSet( 1024, QuickHashIntSet::CHECK_FOR_DUPES ) );

try
{
	new QuickHashIntSet();
}
catch ( Exception $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	new QuickHashIntSet( 1024, QuickHashIntSet::CHECK_FOR_DUPES, 4 );
}
catch ( Exception $e )
{
	echo $e->getMessage(), "\n";
}
?>
--EXPECT--
object(QuickHashIntSet)#1 (0) {
}
object(QuickHashIntSet)#1 (0) {
}
object(QuickHashIntSet)#1 (0) {
}
QuickHashIntSet::__construct() expects at least 1 parameter, 0 given
QuickHashIntSet::__construct() expects at most 2 parameters, 3 given
