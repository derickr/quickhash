--TEST--
Timing tests for exists();
--INI--
memory_limit=256M
--FILE--
<?php
//generate 200000 elements
$array = range( 0, 199999 );
$existingEntries = array_rand( array_flip( $array ), 180000 );
$testForEntries = array_rand( array_flip( $array ), 1000 );
$foundCount = 0;

echo "Creating hash: ", microtime( true ), "\n";
$hash = new QuickHashIntHash( 100000 );
echo "Adding elements: ", microtime( true ), "\n";
foreach( $existingEntries as $key )
{
	$hash->add( $key, $key * 2 );
}

echo "Doing 1000 tests: ", microtime( true ), "\n";
foreach( $testForEntries as $key )
{
	$foundCount += $hash->exists( $key );
}
echo "Done, $foundCount found: ", microtime( true ), "\n";
?>
--EXPECTF--
Creating hash: %f
Adding elements: %f
Doing 1000 tests: %f
Done, %d found: %f
