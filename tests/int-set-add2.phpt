--TEST--
More extensive test for adding elements.
--INI--
--FILE--
<?php
//generate 50000 elements
$array = range( 0, 49999 );
$existingEntries = array_rand( array_flip( $array ), 20000 );
$nonExistingEntries = array_diff( array_flip( $array ), $existingEntries );

echo "without dupe checking\n";
$addedCount = 0;
$foundCount = 0;
$set = new QuickHashIntSet( 1024 );
foreach( $existingEntries as $key ) {
	$addedCount += $set->add( $key );
}
foreach( $array as $key ) {
	$foundCount += $set->exists( $key );
}
echo "Added: $addedCount\n";
echo "Found: $foundCount\n";

echo "\nwith dupe checking\n";
$addedCount = 0;
$foundCount = 0;
$set = new QuickHashIntSet( 1024, QuickHashIntSet::CHECK_FOR_DUPES );
foreach( $existingEntries as $key ) {
	$addedCount += $set->add( $key );
}
foreach( $array as $key ) {
	$foundCount += $set->exists( $key );
}
echo "Added: $addedCount\n";
echo "Found: $foundCount\n";

foreach( $array as $key ) {
	$addedCount += $set->add( $key );
}
foreach( $array as $key ) {
	$foundCount += $set->exists( $key );
}
echo "Added: $addedCount\n";
echo "Found: $foundCount\n";
?>
--EXPECT--
without dupe checking
Added: 20000
Found: 20000

with dupe checking
Added: 20000
Found: 20000
Added: 50000
Found: 70000
