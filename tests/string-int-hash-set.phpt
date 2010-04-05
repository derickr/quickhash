--TEST--
Test for setting (add or update) a value.
--INI--
xdebug.default_enable=0
--FILE--
<?php
$hash = new QuickHashStringIntHash( 1024 );

echo "Add\n";
var_dump( $hash->get( "fourty six thousand six hundred ninety one" ) );
var_dump( $hash->add( "fourty six thousand six hundred ninety one", 16091 ) );
var_dump( $hash->get( "fourty six thousand six hundred ninety one" ) );

echo "Set->Add\n";
var_dump( $hash->get( "fourty six thousand six hundred ninety two" ) );
var_dump( $hash->set( "fourty six thousand six hundred ninety two", 16091 ) );
var_dump( $hash->get( "fourty six thousand six hundred ninety two" ) );

echo "Set->Update\n";
var_dump( $hash->set( "fourty six thousand six hundred ninety two", 29906 ) );
var_dump( $hash->get( "fourty six thousand six hundred ninety two" ) );
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
