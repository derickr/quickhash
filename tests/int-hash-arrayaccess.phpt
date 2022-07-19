--TEST--
Basic test for deleting elements.
--INI--
--FILE--
<?php
$hash = new QuickHashIntHash( 1024 );
var_dump( $hash );
var_dump( isset( $hash[4] ) );
$hash[4] = 5;
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
$hash[-2] = -42;
var_dump( $hash[-2] );
?>
--EXPECTF--
object(QuickHashIntHash)#1 (0) {
}
bool(false)
int(5)
bool(false)
bool(false)
bool(false)
%AQuickHashIntHash::offsetGet()%s, string given%S
bool(false)
bool(false)
int(-42)

