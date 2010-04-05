--TEST--
Basic test for adding elements.
--INI--
xdebug.default_enable=0
--FILE--
<?php
echo "without dupe checking\n";
$hash = new QuickHashStringIntHash( 1024 );
var_dump( $hash );
var_dump( $hash->exists( "four" ) );
var_dump( $hash->get( "four" ) );
var_dump( $hash->add( "four", 22 ) );
var_dump( $hash->exists( "four" ) );
var_dump( $hash->get( "four" ) );
var_dump( $hash->add( "four", 12 ) );

echo "\nwith dupe checking\n";
$hash = new QuickHashStringIntHash( 1024, QuickHashStringIntHash::CHECK_FOR_DUPES );
var_dump( $hash );
var_dump( $hash->exists( "four" ) );
var_dump( $hash->get( "four" ) );
var_dump( $hash->add( "four", 78 ) );
var_dump( $hash->exists( "four" ) );
var_dump( $hash->get( "four" ) );
var_dump( $hash->add( "four", 9 ) );
?>
--EXPECT--
without dupe checking
object(QuickHashStringIntHash)#1 (0) {
}
bool(false)
bool(false)
bool(true)
bool(true)
int(22)
bool(true)

with dupe checking
object(QuickHashStringIntHash)#2 (0) {
}
bool(false)
bool(false)
bool(true)
bool(true)
int(78)
bool(false)
