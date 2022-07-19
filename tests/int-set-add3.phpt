--TEST--
More extensive test for adding elements with negative keys.
--INI--
--FILE--
<?php
//generate 500 elements
$array = range( -250, 249 );
$existingEntries = array_rand( array_flip( $array ), 200 );
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
Added: 200
Found: 200

with dupe checking
Added: 200
Found: 200
Added: 500
Found: 700
