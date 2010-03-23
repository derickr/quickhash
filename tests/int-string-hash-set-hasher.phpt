--TEST--
Basic test for using different hashers.
--INI--
xdebug.default_enable=0
--FILE--
<?php
$file = dirname( __FILE__ ) . "/simple.string.hash";
$hash = QuickHashIntStringHash::loadFromFile( $file );
for ( $i = 0; $i < 0x1f; $i++ )
{
	printf( "%3s ", false !== ( $value = $hash->get( $i ) ) ? $value : ' xx ' );
}
echo "\n";

$hash = QuickHashIntStringHash::loadFromFile( $file, QuickHashIntStringHash::HASHER_NO_HASH );
for ( $i = 0; $i < 0x1f; $i++ )
{
	printf( "%3s ", false !== ( $value = $hash->get( $i ) ) ? $value : ' xx ' );
}
echo "\n";

$hash = QuickHashIntStringHash::loadFromFile( $file, QuickHashIntStringHash::HASHER_JENKINS1 );
for ( $i = 0; $i < 0x1f; $i++ )
{
	printf( "%3s ", false !== ( $value = $hash->get( $i ) ) ? $value : ' xx ' );
}
echo "\n";

$hash = QuickHashIntStringHash::loadFromFile( $file, QuickHashIntStringHash::HASHER_JENKINS2);
for ( $i = 0; $i < 0x1f; $i++ )
{
	printf( "%3s ", false !== ( $value = $hash->get( $i ) ) ? $value : ' xx ' );
}
echo "\n";

?>
--EXPECT--
 xx  one. four. nine.  xx  twenty five.  xx  fourty nine.  xx   xx   xx  hundred twenty.  xx  one hundred ninety six.  xx   xx   xx  two hundred eighty nine.  xx  three hunderd sixty one.  xx   xx   xx  five hundred twenty nine.  xx   xx   xx  seven hundred twenty nine.  xx  eight hundred fourty one.  xx  
 xx  one. four. nine.  xx  twenty five.  xx  fourty nine.  xx   xx   xx  hundred twenty.  xx  one hundred ninety six.  xx   xx   xx  two hundred eighty nine.  xx  three hunderd sixty one.  xx   xx   xx  five hundred twenty nine.  xx   xx   xx  seven hundred twenty nine.  xx  eight hundred fourty one.  xx  
 xx  one. four. nine.  xx  twenty five.  xx  fourty nine.  xx   xx   xx  hundred twenty.  xx  one hundred ninety six.  xx   xx   xx  two hundred eighty nine.  xx  three hunderd sixty one.  xx   xx   xx  five hundred twenty nine.  xx   xx   xx  seven hundred twenty nine.  xx  eight hundred fourty one.  xx  
 xx  one. four. nine.  xx  twenty five.  xx  fourty nine.  xx   xx   xx  hundred twenty.  xx  one hundred ninety six.  xx   xx   xx  two hundred eighty nine.  xx  three hunderd sixty one.  xx   xx   xx  five hundred twenty nine.  xx   xx   xx  seven hundred twenty nine.  xx  eight hundred fourty one.  xx
