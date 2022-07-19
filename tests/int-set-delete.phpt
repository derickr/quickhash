--TEST--
Basic test for deleting elements.
--INI--
--FILE--
<?php
$set = new QuickHashIntSet( 1024 );
var_dump( $set );
var_dump( $set->exists( 4 ) );
var_dump( $set->add( 4 ) );
var_dump( $set->delete( 4 ) );
var_dump( $set->exists( 4 ) );
var_dump( $set->delete( 4 ) );
?>
--EXPECT--
object(QuickHashIntSet)#1 (0) {
}
bool(false)
bool(true)
bool(true)
bool(false)
bool(false)
