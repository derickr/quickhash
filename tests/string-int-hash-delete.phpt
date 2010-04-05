--TEST--
Basic test for deleting elements.
--INI--
xdebug.default_enable=0
--FILE--
<?php
$hash = new QuickHashStringIntHash( 1024 );
var_dump( $hash );
var_dump( $hash->exists( 'four' ) );
var_dump( $hash->add( 'four', 5 ) );
var_dump( $hash->get( 'four' ) );
var_dump( $hash->delete( 'four' ) );
var_dump( $hash->exists( 'four' ) );
var_dump( $hash->get( 'four' ) );
var_dump( $hash->delete( 'four' ) );
?>
--EXPECT--
object(QuickHashStringIntHash)#1 (0) {
}
bool(false)
bool(true)
int(5)
bool(true)
bool(false)
bool(false)
bool(false)
