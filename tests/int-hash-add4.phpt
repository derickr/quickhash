--TEST--
Test for the default value for add().
--INI--
xdebug.default_enable=0
--FILE--
<?php
$hash = new QuickHashIntHash( 1024 );

$hash->add( 1618033988 );
$hash->add( 3141592654, 2718281828 );

echo $hash->getValue( 1618033988 ), "\n";
echo $hash->getValue( 3141592654 ), "\n";
?>
--EXPECTF--
1
2718281828
