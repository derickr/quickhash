--TEST--
Basic test for deleting elements.
--INI--
--FILE--
<?php
$hash = new QuickHashIntHash( 1024 );
var_dump( $hash );
var_dump( isset( $hash[4] ) );
$hash[4] = 5;
var_dump( $hash[4] );
unset( $hash[4] );
var_dump( isset( $hash[4] ) );
var_dump( $hash[4] );
unset( $hash[4] );

var_dump( $hash[8] );
var_dump( $hash['foobar'] );

var_dump( $hash[-1] );
$hash[-2] = -42;
var_dump( $hash[-2] );
?>
--EXPECTF--
object(QuickHashIntHash)#1 (0) {
}
bool(false)
int(5)
bool(false)
bool(false)
bool(false)

Warning: QuickHashIntHash::offsetGet() expects parameter 1 to be %s, string given in %sint-hash-arrayaccess.php on line 13
bool(false)
bool(false)
int(-42)

