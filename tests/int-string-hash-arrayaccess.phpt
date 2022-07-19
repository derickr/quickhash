--TEST--
Basic test for deleting elements.
--INI--
--FILE--
<?php
$hash = new QuickHashIntStringHash( 1024 );
var_dump( $hash );
var_dump( isset( $hash[4] ) );
$hash[4] = "five";
var_dump( $hash[4] );
unset( $hash[4] );
var_dump( isset( $hash[4] ) );
var_dump( $hash[4] );
unset( $hash[4] );

var_dump( $hash[8] );
try
{
	var_dump( $hash['foobar'] );
}
catch ( Error $e )
{
	echo $e->getMessage(), "\n";
	var_dump( false );
}

var_dump( $hash[-1] );
$hash[-2] = "minus fourty two";
var_dump( $hash[-2] );
?>
--EXPECTF--
object(QuickHashIntStringHash)#1 (0) {
}
bool(false)
string(4) "five"
bool(false)
bool(false)
bool(false)
%AQuickHashIntStringHash::offsetGet()%s, string given%S
bool(false)
bool(false)
string(16) "minus fourty two"

