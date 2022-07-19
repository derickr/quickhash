--TEST--
Test for failures for updating a value.
--INI--
--FILE--
<?php
$hash = new QuickHashIntStringHash( 1024 );

$hash->add( 3141592654, "a lot" );

try
{
	$hash->update( 3141592654 );
}
catch ( Error $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	$hash->update( 3141592654, 1, 2 );
}
catch ( Error $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	$hash->update( 'fooabar', 2 );
}
catch ( Error $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	$hash->update( 3141592654, new stdclass );
}
catch ( Error $e )
{
	echo $e->getMessage(), "\n";
}
?>
--EXPECTF--
%SQuickHashIntStringHash::update() expects exactly 2 %s, 1 given%S
%AQuickHashIntStringHash::update() expects exactly 2 %s, 3 given%S
%AQuickHashIntStringHash::update()%s, string given%S
%AQuickHashIntStringHash::update()%s string, %s given%S
