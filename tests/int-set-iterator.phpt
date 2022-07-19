--TEST--
Test for iterating over a set (with three different hashing algorithm).
--INI--
--FILE--
<?php
$found = 0; $value = 0;
$file = dirname( __FILE__ ) . "/simple.set";
$set = QuickHashIntSet::loadFromFile( $file, QuickHashIntSet::HASHER_NO_HASH );
foreach( $set as $key => $value )
{
	printf( '%4d-%4d  ', $key, $value );
}
echo "\n";
$set = QuickHashIntSet::loadFromFile( $file, QuickHashIntSet::HASHER_JENKINS1 );
foreach( $set as $key => $value )
{
	printf( '%4d-%4d  ', $key, $value );
}
echo "\n";
$set = QuickHashIntSet::loadFromFile( $file, QuickHashIntSet::HASHER_JENKINS2 );
foreach( $set as $key => $value )
{
	printf( '%4d-%4d  ', $key, $value );
}
echo "\n";
?>
--EXPECTF--
  17-  17    23-  23    13-  13     2-   2    31-  31     5-   5    27-  27     1-   1     3-   3    11-  11    29-  29    19-  19     7-   7  
  23-  23    13-  13     5-   5    27-  27     1-   1    11-  11    29-  29     7-   7    17-  17     2-   2    31-  31     3-   3    19-  19  
  13-  13     1-   1     2-   2    19-  19    23-  23     5-   5    27-  27    11-  11    29-  29     7-   7    17-  17    31-  31     3-   3
