--TEST--
Test for setting (add or update) a value.
--INI--
xdebug.default_enable=0
--FILE--
<?php
$hash = new QuickHashIntStringHash( 1024 );

echo "Add\n";
var_dump( $hash->get( 46691 ) );
var_dump( $hash->add( 46691, "sixteen thousand ninety one" ) );
var_dump( $hash->get( 46691 ) );

echo "Set->Add\n";
var_dump( $hash->get( 46692 ) );
var_dump( $hash->set( 46692, "sixteen thousand ninety one" ) );
var_dump( $hash->get( 46692 ) );

echo "Set->Update\n";
var_dump( $hash->set( 46692, "twenty nine thousand nine hundred six" ) );
var_dump( $hash->get( 46692 ) );
?>
--EXPECT--
Add
bool(false)
bool(true)
string(27) "sixteen thousand ninety one"
Set->Add
bool(false)
int(2)
string(27) "sixteen thousand ninety one"
Set->Update
int(1)
string(37) "twenty nine thousand nine hundred six"
