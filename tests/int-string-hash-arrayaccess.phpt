--TEST--
Basic test for deleting elements.
--INI--
xdebug.default_enable=0
--FILE--
<?php
$hash = new QuickHashIntStringHash( 1024 );
var_dump( $hash );
var_dump( isset( $hash[4] ) );
$hash[4] = "five";
var_dump( $hash[4] );
unset( $hash[4] );
var_dump( isset( $hash[4] ) );
var_dump( $hash[4] );
unset( $hash[4] );

var_dump( $hash[8] );
var_dump( $hash['foobar'] );

var_dump( $hash[-1] );
$hash[-2] = "minus fourty two";
var_dump( $hash[-2] );
?>
--EXPECTF--
object(QuickHashIntStringHash)#1 (0) {
}
bool(false)
string(4) "five"
bool(false)
bool(false)
bool(false)

Warning: QuickHashIntStringHash::offsetGet() expects parameter 1 to be integer, string given in %sint-string-hash-arrayaccess.php on line 13
bool(false)
bool(false)
string(16) "minus fourty two"

