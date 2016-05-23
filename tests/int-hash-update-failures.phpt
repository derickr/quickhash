--TEST--
Test for failures for updating a value.
--INI--
xdebug.default_enable=0
--FILE--
<?php
$hash = new QuickHashIntHash( 1024 );

$hash->add( 3141592654, 2718281828 );

$hash->update( 3141592654 );
$hash->update( 3141592654, 1, 2 );

$hash->update( 'fooabar', 2 );
$hash->update( 3141592654, 'space chimps' );
?>
--EXPECTF--
Warning: QuickHashIntHash::update() expects exactly 2 parameters, 1 given in %sint-hash-update-failures.php on line 6

Warning: QuickHashIntHash::update() expects exactly 2 parameters, 3 given in %sint-hash-update-failures.php on line 7

Warning: QuickHashIntHash::update() expects parameter 1 to be %s, string given in %sint-hash-update-failures.php on line 9

Warning: QuickHashIntHash::update() expects parameter 2 to be %s, string given in %sint-hash-update-failures.php on line 10
