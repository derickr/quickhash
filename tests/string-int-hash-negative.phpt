--TEST--
Test that checks for negative value issues.
--INI--
xdebug.default_enable=0
--FILE--
<?php
$hash = new QuickHashStringIntHash( 1024 );

var_dump( $hash->add( "one", 1 ) );
var_dump( $hash->add( "minus two", 2 ) );
var_dump( $hash->add( "three", -3 ) );
var_dump( $hash->add( "minus four", -4 ) );

var_dump( $hash->get( "one" ) );
var_dump( $hash->get( "minus two" ) );
var_dump( $hash->get( "three" ) );
var_dump( $hash->get( "minus four" ) );
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
int(1)
int(2)
int(-3)
int(-4)
