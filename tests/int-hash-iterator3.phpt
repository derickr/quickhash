--TEST--
Test for iterating over a hash while adding/deleting entries through ArrayAccess.
--INI--
xdebug.default_enable=0
--FILE--
<?php
$found = 0; $value = 0;
$file = dirname( __FILE__ ) . "/simple.hash";

echo "Normal:\n";
$hash = QuickHashIntHash::loadFromFile( $file );
foreach( $hash as $key => $value )
{
	printf( '%4d-%4d  ', $key, $value );
}
echo "\n\n";

echo "Add:\n";
foreach( $hash as $key => $value )
{
	printf( '%4d-%4d  ', $key, $value );
	if ( $key == 17 )
	{
		var_dump( $hash[41] = 1681 );
	}
}
echo "\n\n";

echo "Delete:\n";
foreach( $hash as $key => $value )
{
	printf( '%4d-%4d  ', $key, $value );
	if ( $key == 5 )
	{
		unset( $hash[23] );
	}
}
echo "\n\n";

echo "Set:\n";
foreach( $hash as $key => $value )
{
	printf( '%4d-%4d  ', $key, $value );
	if ( $key == 23 )
	{
		var_dump( $hash[1] = 9999 );
	}
}
echo "\n\n";
?>
--XFAIL--
For now, there is a memleak that I can not find which only occurs when using
the ArrayAccess way of removing an element from the list (with unset()).
--EXPECTF--
Normal:
  17- 289    11- 121    13- 196    29- 841     5-  25    27- 729    23- 529     1-   1    19- 361     7-  49     2-   4    31- 961     3-   9  

Add:
  17- 289  int(1681)
  11- 121    13- 196    29- 841     5-  25    27- 729    23- 529     1-   1    19- 361    41-1681     7-  49     2-   4    31- 961     3-   9  

Delete:
  17- 289    11- 121    13- 196    29- 841     5-  25    27- 729    23- 529     1-   1    19- 361    41-1681     7-  49     2-   4    31- 961     3-   9  

Set:
  17- 289    11- 121    13- 196    29- 841     5-  25    27- 729    23- 529  int(9999)
   1-9999    19- 361    41-1681     7-  49     2-   4    31- 961     3-   9  
