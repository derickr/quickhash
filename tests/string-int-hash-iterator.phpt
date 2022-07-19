--TEST--
Test for iterating over a hash (with three different hashing algorithm).
--INI--
--FILE--
<?php
$found = 0; $value = 0;
$file = dirname( __FILE__ ) . "/simple.hash.string";
$hash = QuickHashStringIntHash::loadFromFile( $file );
foreach( $hash as $key => $value )
{
	printf( "%s-%4d\n", $key, $value );
}
echo "\n";
?>
--EXPECT--
12uf-50343
1lq0-74808
24ll-99273
1cwv-63391
1vsg-87856
1ffh-66653
1hy3-69915
20to-94380
143q-51974
23ca-97642
1bnk-61760
1x1r-89487
222z-96011
1t9u-84594
1mzb-76439
15d1-53605
1s0j-82963
1yb2-91118
1phx-79701
11l4-48712
1e66-65022
1o8m-78070
1uj5-86225
17vn-56867
1qr8-81332
1ae9-60129
1kgp-73177
16mc-55236
194y-58498
1j7e-71546
1gos-68284
1zkd-92749
