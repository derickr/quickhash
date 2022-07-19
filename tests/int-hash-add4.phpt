--TEST--
Test for the default value for add().
--INI--
--FILE--
<?php
$hash = new QuickHashIntHash( 1024 );

$hash->add( 161803398 );
$hash->add( 314159265, 271828182 );

echo $hash->get( 161803398 ), "\n";
echo $hash->get( 314159265 ), "\n";
?>
--EXPECTF--
1
271828182
