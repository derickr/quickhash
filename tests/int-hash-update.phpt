--TEST--
Test for updating a value.
--INI--
xdebug.default_enable=0
--FILE--
<?php
$hash = new QuickHashIntHash( 1024 );

$hash->add( 1618033988 );
$hash->add( 3141592654, 2718281828 );

echo $hash->get( 1618033988 ), "\n";
echo $hash->get( 3141592654 ), "\n";

var_dump( $hash->update( 3141592654, 2718281829 ) );
var_dump( $hash->update( 314159265, 2718281829 ) );

echo $hash->get( 1618033988 ), "\n";
echo $hash->get( 3141592654 ), "\n";
?>
--EXPECTF--
1
2718281828
bool(true)
bool(false)
1
2718281829
