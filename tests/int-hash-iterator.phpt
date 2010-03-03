--TEST--
Test for iterating over a hash (with three different hashing algorithm).
--INI--
xdebug.default_enable=0
--FILE--
<?php
$found = 0; $value = 0;
$file = dirname( __FILE__ ) . "/simple.hash";
$hash = QuickHashIntHash::loadFromFile( $file, QuickHashIntHash::HASHER_NO_HASH );
foreach( $hash as $key => $value )
{
	printf( '%4d-%4d  ', $key, $value );
}
echo "\n";
$hash = QuickHashIntHash::loadFromFile( $file, QuickHashIntHash::HASHER_JENKINS1 );
foreach( $hash as $key => $value )
{
	printf( '%4d-%4d  ', $key, $value );
}
echo "\n";
$hash = QuickHashIntHash::loadFromFile( $file, QuickHashIntHash::HASHER_JENKINS2 );
foreach( $hash as $key => $value )
{
	printf( '%4d-%4d  ', $key, $value );
}
echo "\n";
?>
--EXPECTF--
  17- 289    23- 529    13- 196     2-   4    31- 961     5-  25    27- 729     1-   1     3-   9    11- 121    29- 841    19- 361     7-  49  
  23- 529    13- 196     5-  25    27- 729     1-   1    11- 121    29- 841     7-  49    17- 289     2-   4    31- 961     3-   9    19- 361  
  13- 196     1-   1     2-   4    19- 361    23- 529     5-  25    27- 729    11- 121    29- 841     7-  49    17- 289    31- 961     3-   9  
