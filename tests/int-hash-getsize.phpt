--TEST--
Test for getSize().
--INI--
--FILE--
<?php
$hash = new QuickHashIntHash( 1024 );
var_dump( $hash->getSize() );

$hash->add( 4 );
$hash->add( 4 );
var_dump( $hash->getSize() );

$file = dirname( __FILE__ ) . "/primes.hash";
$hash = QuickHashIntHash::loadFromFile( $file );
var_dump( $hash->getSize() );

$hash->delete( 5 );
var_dump( $hash->getSize() );

$hash->delete( 9 );
var_dump( $hash->getSize() );

$hash->add( 9 );
var_dump( $hash->getSize() );

$hash->add( 9 );
var_dump( $hash->getSize() );

// Now we check with dupe checking enabled.
$file = dirname( __FILE__ ) . "/primes.hash";
$hash = QuickHashIntHash::loadFromFile( $file, null, QuickHashIntHash::CHECK_FOR_DUPES );
var_dump( $hash->getSize() );
$hash->add( 5 );
$hash->add( 9 );
$hash->add( 9 );
var_dump( $hash->getSize() );
?>
--EXPECT--
int(0)
int(2)
int(50004)
int(50003)
int(50003)
int(50004)
int(50005)
int(50004)
int(50005)
