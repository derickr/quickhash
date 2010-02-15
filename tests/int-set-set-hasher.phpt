--TEST--
Basic test for using different hashers.
--INI--
xdebug.default_enable=0
--FILE--
<?php
$file = dirname( __FILE__ ) . "/simple.set";
$hash = QuickHashIntSet::loadFromFile( $file );
for ( $i = 0; $i < 0x1f; $i++ )
{
	echo $hash->exists( $i ) ? '+' : ' ';
}
echo "\n";

$hash = QuickHashIntSet::loadFromFile( $file, QuickHashIntSet::HASHER_NO_HASH );
for ( $i = 0; $i < 0x1f; $i++ )
{
	echo $hash->exists( $i ) ? '+' : ' ';
}
echo "\n";

$hash = QuickHashIntSet::loadFromFile( $file, QuickHashIntSet::HASHER_JENKINS1 );
for ( $i = 0; $i < 0x1f; $i++ )
{
	echo $hash->exists( $i ) ? '+' : ' ';
}
echo "\n";

$hash = QuickHashIntSet::loadFromFile( $file, QuickHashIntSet::HASHER_JENKINS2);
for ( $i = 0; $i < 0x1f; $i++ )
{
	echo $hash->exists( $i ) ? '+' : ' ';
}
echo "\n";

?>
--EXPECT--
 +++ + +   + +   + +   +   + + 
 +++ + +   + +   + +   +   + + 
 +++ + +   + +   + +   +   + + 
 +++ + +   + +   + +   +   + +
