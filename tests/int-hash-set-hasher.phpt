--TEST--
Basic test for using different hashers.
--INI--
xdebug.default_enable=0
--FILE--
<?php
$file = dirname( __FILE__ ) . "/simple.hash";
$hash = QuickHashIntHash::loadFromFile( $file );
for ( $i = 0; $i < 0x1f; $i++ )
{
	printf( "%3s ", NULL !== ( $value = $hash->getValue( $i ) ) ? $value : ' xx ' );
}
echo "\n";

$hash = QuickHashIntHash::loadFromFile( $file, QuickHashIntHash::HASHER_NO_HASH );
for ( $i = 0; $i < 0x1f; $i++ )
{
	printf( "%3s ", NULL !== ( $value = $hash->getValue( $i ) ) ? $value : ' xx ' );
}
echo "\n";

$hash = QuickHashIntHash::loadFromFile( $file, QuickHashIntHash::HASHER_JENKINS1 );
for ( $i = 0; $i < 0x1f; $i++ )
{
	printf( "%3s ", NULL !== ( $value = $hash->getValue( $i ) ) ? $value : ' xx ' );
}
echo "\n";

$hash = QuickHashIntHash::loadFromFile( $file, QuickHashIntHash::HASHER_JENKINS2);
for ( $i = 0; $i < 0x1f; $i++ )
{
	printf( "%3s ", NULL !== ( $value = $hash->getValue( $i ) ) ? $value : ' xx ' );
}
echo "\n";

?>
--EXPECT--
 xx    1   4   9  xx   25  xx   49  xx   xx   xx  121  xx  196  xx   xx   xx  289  xx  361  xx   xx   xx  529  xx   xx   xx  729  xx  841  xx  
 xx    1   4   9  xx   25  xx   49  xx   xx   xx  121  xx  196  xx   xx   xx  289  xx  361  xx   xx   xx  529  xx   xx   xx  729  xx  841  xx  
 xx    1   4   9  xx   25  xx   49  xx   xx   xx  121  xx  196  xx   xx   xx  289  xx  361  xx   xx   xx  529  xx   xx   xx  729  xx  841  xx  
 xx    1   4   9  xx   25  xx   49  xx   xx   xx  121  xx  196  xx   xx   xx  289  xx  361  xx   xx   xx  529  xx   xx   xx  729  xx  841  xx  
