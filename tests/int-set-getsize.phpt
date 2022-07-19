--TEST--
Test for getSize().
--INI--
--FILE--
<?php
$set = new QuickHashIntSet( 1024 );
var_dump( $set->getSize() );

$set->add( 4 );
$set->add( 4 );
var_dump( $set->getSize() );

$file = dirname( __FILE__ ) . "/primes.set";
$set = QuickHashIntSet::loadFromFile( $file );
var_dump( $set->getSize() );

$set->delete( 5 );
var_dump( $set->getSize() );

$set->delete( 9 );
var_dump( $set->getSize() );

$set->add( 9 );
var_dump( $set->getSize() );

$set->add( 9 );
var_dump( $set->getSize() );

// Now we check with dupe checking enabled.
$file = dirname( __FILE__ ) . "/primes.set";
$set = QuickHashIntSet::loadFromFile( $file, null, QuickHashIntSet::CHECK_FOR_DUPES );
var_dump( $set->getSize() );
$set->add( 5 );
$set->add( 9 );
$set->add( 9 );
var_dump( $set->getSize() );
?>
--EXPECT--
int(0)
int(2)
int(100008)
int(100007)
int(100007)
int(100008)
int(100009)
int(100008)
int(100009)
