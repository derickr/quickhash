--TEST--
More extensive test for adding elements with negative keys.
--INI--
xdebug.default_enable=0
--FILE--
<?php
//generate 500 elements
$array = range( -250, 249 );
$existingEntries = array_rand( array_flip( $array ), 200 );
$nonExistingEntries = array_diff( array_flip( $array ), $existingEntries );

echo "without dupe checking\n";
$addedCount = 0;
$foundCount = 0;
$hash = new QuickHashIntHash( 1024 );
foreach( $existingEntries as $key ) {
	$addedCount += $hash->add( $key, $key * 2 );
}
foreach( $array as $key ) {
	$foundCount += $hash->exists( $key );
}
echo "Added: $addedCount\n";
echo "Found: $foundCount\n";

echo "\nwith dupe checking\n";
$addedCount = 0;
$foundCount = 0;
$hash = new QuickHashIntHash( 1024, QuickHashIntHash::CHECK_FOR_DUPES );
foreach( $existingEntries as $key ) {
	$addedCount += $hash->add( $key, $key * 2 );
}
foreach( $array as $key ) {
	$foundCount += $hash->exists( $key );
}
echo "Added: $addedCount\n";
echo "Found: $foundCount\n";

foreach( $array as $key ) {
	$addedCount += $hash->add( $key, $key * 2 );
}
foreach( $array as $key ) {
	$foundCount += $hash->exists( $key );
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
