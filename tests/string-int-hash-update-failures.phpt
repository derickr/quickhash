--TEST--
Test for failures for updating a value.
--INI--
xdebug.default_enable=0
--FILE--
<?php
$hash = new QuickHashStringIntHash( 1024 );

$hash->add( "pi", 2139487234 );

$hash->update( "pi" );
$hash->update( "pi", 1, 2 );

$hash->update( "pi", new stdclass );
?>
--EXPECTF--
Warning: QuickHashStringIntHash::update() expects exactly 2 parameters, 1 given in %sstring-int-hash-update-failures.php on line 6

Warning: QuickHashStringIntHash::update() expects exactly 2 parameters, 3 given in %sstring-int-hash-update-failures.php on line 7

Warning: QuickHashStringIntHash::update() expects parameter 2 to be integer, object given in %sstring-int-hash-update-failures.php on line 9
