--TEST--
Basic test for adding elements.
--INI--
--FILE--
<?php
echo "without dupe checking\n";
$hash = new QuickHashIntStringHash( 1024 );
var_dump( $hash );
var_dump( $hash->exists( 4 ) );
var_dump( $hash->get( 4 ) );
var_dump( $hash->add( 4, "twenty two" ) );
var_dump( $hash->exists( 4 ) );
var_dump( $hash->get( 4 ) );
var_dump( $hash->add( 4, "twelve" ) );

echo "\nwith dupe checking\n";
$hash = new QuickHashIntStringHash( 1024, QuickHashIntStringHash::CHECK_FOR_DUPES );
var_dump( $hash );
var_dump( $hash->exists( 4 ) );
var_dump( $hash->get( 4 ) );
var_dump( $hash->add( 4, "seventy eight" ) );
var_dump( $hash->exists( 4 ) );
var_dump( $hash->get( 4 ) );
var_dump( $hash->add( 4, "nine" ) );
?>
--EXPECT--
without dupe checking
object(QuickHashIntStringHash)#1 (0) {
}
bool(false)
bool(false)
bool(true)
bool(true)
string(10) "twenty two"
bool(true)

with dupe checking
object(QuickHashIntStringHash)#2 (0) {
}
bool(false)
bool(false)
bool(true)
bool(true)
string(13) "seventy eight"
bool(false)
