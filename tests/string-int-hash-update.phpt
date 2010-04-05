--TEST--
Test for updating a value.
--INI--
xdebug.default_enable=0
--FILE--
<?php
$hash = new QuickHashStringIntHash( 1024 );

$hash->add( 'six', 314159265 );
$hash->add( "a lot", 314159265 );

echo $hash->get( 'six' ), "\n";
echo $hash->get( 'a lot' ), "\n";

var_dump( $hash->update( 'a lot', 314159266 ) ); 
var_dump( $hash->update( "a lot plus one", 314159999 ) );

echo $hash->get( 'six' ), "\n";
echo $hash->get( 'a lot' ), "\n";
?>
--EXPECTF--
314159265
314159265
bool(true)
bool(false)
314159265
314159266
