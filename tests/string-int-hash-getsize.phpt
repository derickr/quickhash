--TEST--
Test for getSize().
--INI--
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

$hash->delete( "1j7e" );
var_dump( $hash->getSize() );

$hash->delete( "1j7f" );
var_dump( $hash->getSize() );

$hash->add( "nine", 3 );
var_dump( $hash->getSize() );

$hash->add( "nine", 4 );
var_dump( $hash->getSize() );

// Now we check with dupe checking enabled.
$file = dirname( __FILE__ ) . "/simple.hash.string";
$hash = QuickHashStringIntHash::loadFromFile( $file, 0, QuickHashStringIntHash::CHECK_FOR_DUPES );
var_dump( $hash->getSize() );
$hash->add( "15d0", 5 );
$hash->add( "15d1", 9 );
$hash->add( "15d1", 9 );
var_dump( $hash->getSize() );
?>
--EXPECT--
int(0)
int(2)
int(32)
int(31)
int(31)
int(32)
int(33)
int(32)
int(33)
