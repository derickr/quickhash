--TEST--
More extensive test for deleting elements.
--INI--
--FILE--
<?php
$set = new QuickHashIntSet( 64 );

$count = 0;
for ( $i = 0; $i < 128; $i++ )
{
	$count += $set->add( $i );
}
echo $count, "\n";

$count = 0;
for ( $i = 0; $i < 128; $i++ )
{
	$count += $set->exists( $i );
}
echo $count, "\n";

$count = 0;
for ( $i = 0; $i < 128; $i += 2 )
{
	$count += $set->delete( $i );
}
echo $count, "\n";

$count = 0;
for ( $i = 0; $i < 128; $i++ )
{
	$count += $set->exists( $i );
}
echo $count, "\n";
?>
--EXPECT--
128
128
64
64
