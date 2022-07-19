--TEST--
Test that checks for negative value issues.
--INI--
--FILE--
<?php
$hash = new QuickHashIntHash( 1024 );

var_dump( $hash->add( 1, 1 ) );
var_dump( $hash->add( 2, -2 ) );
var_dump( $hash->add( -3, 3 ) );
var_dump( $hash->add( -4, -4 ) );

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
int(1)
int(-2)
int(3)
int(-4)
