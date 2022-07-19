--TEST--
Test for failures for updating a value.
--INI--
--FILE--
<?php
$hash = new QuickHashStringIntHash( 1024 );

$hash->add( "pi", 2139487234 );

try
{
	$hash->update( "pi" );
}
catch ( Error $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	$hash->update( "pi", 1, 2 );
}
catch ( Error $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	$hash->update( "pi", new stdclass );
}
catch ( Error $e )
{
	echo $e->getMessage(), "\n";
}
?>
--EXPECTF--
%SQuickHashStringIntHash::update() expects exactly 2 %s, 1 given%S
%AQuickHashStringIntHash::update() expects exactly 2 %s, 3 given%S
%AQuickHashStringIntHash::update()%s, %s given%S
