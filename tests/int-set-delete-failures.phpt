--TEST--
Test for failures for deleting a value.
--INI--
xdebug.default_enable=0
--FILE--
<?php
$set = new QuickHashIntSet( 1024 );

$set->add( 3141592654 );

$set->delete();
$set->delete( 3141592654, 1 );

$set->delete( 'fooabar' );
?>
--EXPECTF--
Warning: QuickHashIntSet::delete() expects exactly 1 parameter, 0 given in %sint-set-delete-failures.php on line 6

Warning: QuickHashIntSet::delete() expects exactly 1 parameter, 2 given in %sint-set-delete-failures.php on line 7

Warning: QuickHashIntSet::delete() expects parameter 1 to be %s, string given in %sint-set-delete-failures.php on line 9
