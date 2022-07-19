--TEST--
Test for updating a value.
--INI--
--FILE--
<?php
$hash = new QuickHashIntHash( 1024 );

$hash->add( 161803398 );
$hash->add( 314159265, 271828182 );

echo $hash->get( 161803398 ), "\n";
echo $hash->get( 314159265 ), "\n";

var_dump( $hash->update( 314159265, 271828183 ) );
var_dump( $hash->update( 314159999, 271828183 ) );

echo $hash->get( 161803398 ), "\n";
echo $hash->get( 314159265 ), "\n";
?>
--EXPECTF--
1
271828182
bool(true)
bool(false)
1
271828183
