--TEST--
Basic test for deleting elements.
--INI--
--FILE--
<?php
$hash = new QuickHashIntStringHash( 1024 );
var_dump( $hash );
var_dump( $hash->exists( 4 ) );
var_dump( $hash->add( 4, "five" ) );
var_dump( $hash->get( 4 ) );
var_dump( $hash->delete( 4 ) );
var_dump( $hash->exists( 4 ) );
var_dump( $hash->get( 4 ) );
var_dump( $hash->delete( 4 ) );
?>
--EXPECT--
object(QuickHashIntStringHash)#1 (0) {
}
bool(false)
bool(true)
string(4) "five"
bool(true)
bool(false)
bool(false)
bool(false)
