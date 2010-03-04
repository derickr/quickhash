--TEST--
Test for iterating over a set while adding/deleting entries.
--INI--
xdebug.default_enable=0
--FILE--
<?php
$found = 0; $value = 0;
$file = dirname( __FILE__ ) . "/simple.set";

echo "Normal:\n";
$set = QuickHashIntSet::loadFromFile( $file );
foreach( $set as $key => $value )
{
	printf( '%4d-%4d  ', $key, $value );
}
echo "\n\n";

echo "Add:\n";
foreach( $set as $key => $value )
{
	printf( '%4d-%4d  ', $key, $value );
	if ( $key == 17 )
	{
		var_dump( $set->add( 41 ) );
	}
}
echo "\n\n";

echo "Delete:\n";
foreach( $set as $key => $value )
{
	printf( '%4d-%4d  ', $key, $value );
	if ( $key == 5 )
	{
		var_dump( $set->delete( 23 ) );
	}
}
echo "\n\n";
?>
--EXPECTF--
Normal:
  17-  17    11-  11    13-  13    29-  29     5-   5    27-  27    23-  23     1-   1    19-  19     7-   7     2-   2    31-  31     3-   3  

Add:
  17-  17  bool(true)
  11-  11    13-  13    29-  29     5-   5    27-  27    23-  23     1-   1    19-  19    41-  41     7-   7     2-   2    31-  31     3-   3  

Delete:
  17-  17    11-  11    13-  13    29-  29     5-   5  bool(false)
  27-  27    23-  23     1-   1    19-  19    41-  41     7-   7     2-   2    31-  31     3-   3
