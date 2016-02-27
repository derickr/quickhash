--TEST--
Test for failures for updating a value.
--INI--
xdebug.default_enable=0
--FILE--
<?php
$hash = new QuickHashIntStringHash( 1024 );

$hash->add( 3141592654, "a lot" );

$hash->update( 3141592654 );
$hash->update( 3141592654, 1, 2 );

$hash->update( 'fooabar', 2 );
$hash->update( 3141592654, new stdclass );
?>
--EXPECTF--
Warning: QuickHashIntStringHash::update() expects exactly 2 parameters, 1 given in %sint-string-hash-update-failures.php on line 6

Warning: QuickHashIntStringHash::update() expects exactly 2 parameters, 3 given in %sint-string-hash-update-failures.php on line 7

Warning: QuickHashIntStringHash::update() expects parameter 1 to be integer, string given in %sint-string-hash-update-failures.php on line 9

Warning: QuickHashIntStringHash::update() expects parameter 2 to be string, object given in %sint-string-hash-update-failures.php on line 10
