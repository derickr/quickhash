--TEST--
Test for setting (add or update) a value.
--INI--
--FILE--
<?php
$hash = new QuickHashIntHash( 1024 );

echo "Add\n";
var_dump( $hash->get( 46691 ) );
var_dump( $hash->add( 46691, 16091 ) );
var_dump( $hash->get( 46691 ) );

echo "Set->Add\n";
var_dump( $hash->get( 46692 ) );
var_dump( $hash->set( 46692, 16091 ) );
var_dump( $hash->get( 46692 ) );

echo "Set->Update\n";
var_dump( $hash->set( 46692, 29906 ) );
var_dump( $hash->get( 46692 ) );
?>
--EXPECT--
Add
bool(false)
bool(true)
int(16091)
Set->Add
bool(false)
int(2)
int(16091)
Set->Update
int(1)
int(29906)
