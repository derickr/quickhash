--TEST--
ArrayAccess test.
--INI--
xdebug.default_enable=0
--FILE--
<?php
$set = new QuickHashIntSet( 1024 );
var_dump( $set );
var_dump( isset( $set[4] ) );
var_dump( $set[4] );
$set[4] = 5;
var_dump( $set[4] );
unset( $set[4] );
var_dump( isset( $set[4] ) );
var_dump( $set[4] );
unset( $set[4] );
echo "\n";

var_dump( $set[8] );
var_dump( $set[8] );
echo "\n";

var_dump( $set[9] );
$set->add( '9' );
var_dump( isset( $set['9'] ) );
var_dump( $set['9'] );
echo "\n";

var_dump( $set[-1] );
$set[-2] = -42;
var_dump( isset( $set[-2] ) );
var_dump( $set[-2] );
?>
--EXPECTF--
object(QuickHashIntSet)#1 (0) {
}
bool(false)
bool(false)
bool(true)
bool(false)
bool(false)

bool(false)
bool(false)

bool(false)
bool(true)
bool(true)

bool(false)
bool(true)
bool(true)

