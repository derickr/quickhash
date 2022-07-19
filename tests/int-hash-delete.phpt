--TEST--
Basic test for deleting elements.
--INI--
--FILE--
<?php
$hash = new QuickHashIntHash( 1024 );
var_dump( $hash );
var_dump( $hash->exists( 4 ) );
var_dump( $hash->add( 4, 5 ) );
var_dump( $hash->get( 4 ) );
var_dump( $hash->delete( 4 ) );
var_dump( $hash->exists( 4 ) );
var_dump( $hash->get( 4 ) );
var_dump( $hash->delete( 4 ) );
?>
--EXPECT--
object(QuickHashIntHash)#1 (0) {
}
bool(false)
bool(true)
int(5)
bool(true)
bool(false)
bool(false)
bool(false)
