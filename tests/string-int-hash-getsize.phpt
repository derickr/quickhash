--TEST--
Test for getSize().
--INI--
xdebug.default_enable=0
--FILE--
<?php
$hash = new QuickHashStringIntHash( 1024 );
var_dump( $hash->getSize() );

$hash->add( "one", 4 );
$hash->add( "two", 4 );
var_dump( $hash->getSize() );

$file = dirname( __FILE__ ) . "/simple.hash.string";
$hash = QuickHashStringIntHash::loadFromFile( $file );
var_dump( $hash->getSize() );

$hash->delete( "five" );
var_dump( $hash->getSize() );

$hash->delete( "nine" );
var_dump( $hash->getSize() );

$hash->add( "nine", 3 );
var_dump( $hash->getSize() );

$hash->add( "nine", 4 );
var_dump( $hash->getSize() );

// Now we check with dupe checking enabled.
$file = dirname( __FILE__ ) . "/simple.hash.string";
$hash = QuickHashStringIntHash::loadFromFile( $file, null, QuickHashStringIntHash::CHECK_FOR_DUPES );
var_dump( $hash->getSize() );
$hash->add( "five", 5 );
$hash->add( "six",  9 );
$hash->add( "Seven", 9 );
var_dump( $hash->getSize() );
?>
--EXPECT--
int(0)
int(2)
int(13)
int(12)
int(12)
int(13)
int(14)
int(13)
int(14)
