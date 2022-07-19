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
$keyValueCount = 0;
$valueCount = 0;
$hash = new QuickHashIntHash( 1024 );
foreach( $existingEntries as $key ) {
	$addedCount += $hash->add( $key, $key * 2 );
}
foreach( $array as $key ) {
	$foundCount += $hash->exists( $key );
}
foreach( $array as $key ) {
	if ( false !== ( $value = $hash->get( $key ) ) ) {
		$keyValueCount += $key;
		$valueCount += $value;
	}
}
echo "Added: $addedCount\n";
echo "Found: $foundCount\n";
echo "Value: ", $valueCount, " / ", $keyValueCount, " : ", $valueCount / $keyValueCount, "\n";

echo "\nwith dupe checking\n";
$addedCount = 0;
$foundCount = 0;
$keyValueCount = 0;
$valueCount = 0;
$hash = new QuickHashIntHash( 1024, QuickHashIntHash::CHECK_FOR_DUPES );
foreach( $existingEntries as $key ) {
	$addedCount += $hash->add( $key, $key * 3 );
}
foreach( $array as $key ) {
	$foundCount += $hash->exists( $key );
}
echo "Added: $addedCount\n";
echo "Found: $foundCount\n";

foreach( $array as $key ) {
	$addedCount += $hash->add( $key, $key * 3 );
}
foreach( $array as $key ) {
	$foundCount += $hash->exists( $key );
}
foreach( $array as $key ) {
	if ( false !== ( $value = $hash->get( $key ) ) ) {
		$keyValueCount += $key;
		$valueCount += $value;
	}
}
echo "Added: $addedCount\n";
echo "Found: $foundCount\n";
echo "Value: ", $valueCount, " / ", $keyValueCount, " : ", $valueCount / $keyValueCount, "\n";
?>
--EXPECTF--
without dupe checking
Added: 20000
Found: 20000
Value: %d / %d : 2

with dupe checking
Added: 20000
Found: 20000
Added: 50000
Found: 70000
Value: %d / %d : 3
