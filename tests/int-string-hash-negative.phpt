--TEST--
Test that checks for negative value issues.
--INI--
xdebug.default_enable=0
--FILE--
<?php
$hash = new QuickHashIntStringHash( 1024 );

var_dump( $hash->add( 1, "one" ) );
var_dump( $hash->add( 2, "minus two" ) );
var_dump( $hash->add( -3, "three" ) );
var_dump( $hash->add( -4, "minus four" ) );

var_dump( $hash->get( 1 ) );
var_dump( $hash->get( 2 ) );
var_dump( $hash->get( -3 ) );
var_dump( $hash->get( -4 ) );
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
string(3) "one"
string(9) "minus two"
string(5) "three"
string(10) "minus four"
