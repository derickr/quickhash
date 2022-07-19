--TEST--
Test for failures for updating a value.
--INI--
--FILE--
<?php
$hash = new QuickHashIntHash( 1024 );

$hash->add( 3141592654, 2718281828 );

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
	$hash->update( 3141592654, 'space chimps' );
}
catch ( Error $e )
{
	echo $e->getMessage(), "\n";
}
?>
--EXPECTF--
%SQuickHashIntHash::update() expects exactly 2 %s, 1 given%S
%AQuickHashIntHash::update() expects exactly 2 %s, 3 given%S
%AQuickHashIntHash::update()%s, string given%S
%AQuickHashIntHash::update()%s, string given%S
