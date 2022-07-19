--TEST--
Test for failures for deleting a value.
--INI--
--FILE--
<?php
$set = new QuickHashIntSet( 1024 );

$set->add( 3141592654 );

try
{
	$set->delete();
}
catch ( Error $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	$set->delete( 3141592654, 1 );
}
catch ( Error $e )
{
	echo $e->getMessage(), "\n";
}

try
{
	$set->delete( 'fooabar' );
}
catch ( Error $e )
{
	echo $e->getMessage(), "\n";
}
?>
--EXPECTF--
%SQuickHashIntSet::delete() expects exactly 1 %s, 0 given%S
%AQuickHashIntSet::delete() expects exactly 1 %s, 2 given%S
%AQuickHashIntSet::delete()%s, string given%S
