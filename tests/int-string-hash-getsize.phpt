--TEST--
Test for getSize().
--INI--
--FILE--
<?php
$hash = new QuickHashIntStringHash( 1024 );
var_dump( $hash->getSize() );

$hash->add( 4, "one" );
$hash->add( 4, "two" );
var_dump( $hash->getSize() );

$file = dirname( __FILE__ ) . "/simple.string.hash";
$hash = QuickHashIntStringHash::loadFromFile( $file );
var_dump( $hash->getSize() );

$hash->delete( 5 );
var_dump( $hash->getSize() );

$hash->delete( 9 );
var_dump( $hash->getSize() );

$hash->add( 9, "three" );
var_dump( $hash->getSize() );

$hash->add( 9, "four" );
var_dump( $hash->getSize() );

// Now we check with dupe checking enabled.
$file = dirname( __FILE__ ) . "/simple.string.hash";
$hash = QuickHashIntStringHash::loadFromFile( $file, 0, QuickHashIntStringHash::CHECK_FOR_DUPES );
var_dump( $hash->getSize() );
$hash->add( 5, "five" );
$hash->add( 9, "six" );
$hash->add( 9, "Seven" );
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
