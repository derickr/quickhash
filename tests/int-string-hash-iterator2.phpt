--TEST--
Test for iterating over a hash while adding/deleting entries.
--INI--
--FILE--
<?php
$found = 0; $value = 0;
$file = dirname( __FILE__ ) . "/simple.string.hash";

echo "Normal:\n";
$hash = QuickHashIntStringHash::loadFromFile( $file );
foreach( $hash as $key => $value )
{
	printf( "%4d-%s\n", $key, $value );
}
echo "\n\n";

echo "Add:\n";
foreach( $hash as $key => $value )
{
	printf( "%4d-%s\n", $key, $value );
	if ( $key == 17 )
	{
		var_dump( $hash->add( 41, 1681 ) );
	}
}
echo "\n\n";

echo "Delete:\n";
foreach( $hash as $key => $value )
{
	printf( "%4d-%s\n", $key, $value );
	if ( $key == 5 )
	{
		var_dump( $hash->delete( 23 ) );
	}
}
echo "\n\n";

echo "Set:\n";
foreach( $hash as $key => $value )
{
	printf( "%4d-%s\n", $key, $value );
	if ( $key == 23 )
	{
		var_dump( $hash->set( 1, 9999 ) );
	}
}
echo "\n\n";
?>
--EXPECTF--
Normal:
  17-two hundred eighty nine.
  11-hundred twenty.
  13-one hundred ninety six.
  29-eight hundred fourty one.
   5-twenty five.
  27-seven hundred twenty nine.
  23-five hundred twenty nine.
   1-one.
  19-three hunderd sixty one.
   7-fourty nine.
   2-four.
  31-nine hundred sixty one.
   3-nine.


Add:
  17-two hundred eighty nine.
bool(true)
  11-hundred twenty.
  13-one hundred ninety six.
  29-eight hundred fourty one.
   5-twenty five.
  27-seven hundred twenty nine.
  23-five hundred twenty nine.
   1-one.
  19-three hunderd sixty one.
  41-1681
   7-fourty nine.
   2-four.
  31-nine hundred sixty one.
   3-nine.


Delete:
  17-two hundred eighty nine.
  11-hundred twenty.
  13-one hundred ninety six.
  29-eight hundred fourty one.
   5-twenty five.
bool(false)
  27-seven hundred twenty nine.
  23-five hundred twenty nine.
   1-one.
  19-three hunderd sixty one.
  41-1681
   7-fourty nine.
   2-four.
  31-nine hundred sixty one.
   3-nine.


Set:
  17-two hundred eighty nine.
  11-hundred twenty.
  13-one hundred ninety six.
  29-eight hundred fourty one.
   5-twenty five.
  27-seven hundred twenty nine.
  23-five hundred twenty nine.
int(1)
   1-9999
  19-three hunderd sixty one.
  41-1681
   7-fourty nine.
   2-four.
  31-nine hundred sixty one.
   3-nine.
