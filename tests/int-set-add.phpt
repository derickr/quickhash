--TEST--
Basic test for adding elements.
--INI--
xdebug.default_enable=0
--FILE--
<?php
echo "without dupe checking\n";
$set = new QuickHashIntSet( 1024 );
var_dump( $set );
var_dump( $set->exists( 4 ) );
var_dump( $set->add( 4 ) );
var_dump( $set->exists( 4 ) );
var_dump( $set->add( 4 ) );

echo "\nwith dupe checking\n";
$set = new QuickHashIntSet( 1024, QuickHashIntSet::CHECK_FOR_DUPES );
var_dump( $set );
var_dump( $set->exists( 4 ) );
var_dump( $set->add( 4 ) );
var_dump( $set->exists( 4 ) );
var_dump( $set->add( 4 ) );
?>
--EXPECT--
without dupe checking
object(QuickHashIntSet)#1 (0) {
}
bool(false)
bool(true)
bool(true)
bool(true)

with dupe checking
object(QuickHashIntSet)#2 (0) {
}
bool(false)
bool(true)
bool(true)
bool(false)
