--TEST--
ArrayAccess test.
--INI--
--FILE--
<?php
$hash = new QuickHashStringIntHash( 1024 );
var_dump( $hash );
var_dump( isset( $hash['four'] ) );
$hash['four'] = 5;
var_dump( $hash['four'] );
unset( $hash['four'] );
var_dump( isset( $hash['four'] ) );
var_dump( $hash['four'] );
unset( $hash['four'] );

var_dump( $hash['eight'] );
var_dump( $hash[8] );

var_dump( $hash['nine'] );
$hash->add( '9', 999 );
var_dump( $hash['9'] );

var_dump( $hash['minus one'] );
$hash['minus two'] = -42;
var_dump( $hash['minus two'] );
?>
--EXPECTF--
object(QuickHashStringIntHash)#1 (0) {
}
bool(false)
int(5)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
int(999)
bool(false)
int(-42)

