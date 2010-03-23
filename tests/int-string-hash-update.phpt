--TEST--
Test for updating a value.
--INI--
xdebug.default_enable=0
--FILE--
<?php
$hash = new QuickHashIntStringHash( 1024 );

$hash->add( 161803398, "--" );
$hash->add( 314159265, "a lot" );

echo $hash->get( 161803398 ), "\n";
echo $hash->get( 314159265 ), "\n";

var_dump( $hash->update( 314159265, "a lot plus one" ) );
var_dump( $hash->update( 314159999, "a lot plus one" ) );

echo $hash->get( 161803398 ), "\n";
echo $hash->get( 314159265 ), "\n";
?>
--EXPECTF--
--
a lot
bool(true)
bool(false)
--
a lot plus one
