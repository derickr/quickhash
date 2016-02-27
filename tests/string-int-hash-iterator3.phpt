--TEST--
Test for iterating over a hash while adding/deleting entries through ArrayAccess.
--INI--
xdebug.default_enable=0
--FILE--
<?php
$found = 0; $value = 0;
$file = dirname( __FILE__ ) . "/simple.hash.string";

echo "Normal:\n";
$hash = QuickHashStringIntHash::loadFromFile( $file );
foreach( $hash as $key => $value )
{
	printf( "%s-%s\n", $key, $value );
}
echo "\n\n";

echo "Add:\n";
foreach( $hash as $key => $value )
{
	printf( "%s-%s\n", $key, $value );
	if ( $key == '12uf' )
	{
		var_dump( $hash['fourty one'] = 1681 );
	}
}
echo "\n\n";

echo "Delete:\n";
foreach( $hash as $key => $value )
{
	printf( "%s-%s\n", $key, $value );
	if ( $key == '23ca' )
	{
		unset( $hash['1o8m'] );
	}
}
echo "\n\n";

echo "Set:\n";
foreach( $hash as $key => $value )
{
	printf( "%s-%s\n", $key, $value );
	if ( $key == '24ll' )
	{
		var_dump( $hash['20to'] = 9999 );
	}
}
echo "\n\n";
?>
--EXPECTF--
Normal:
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


Add:
12uf-50343
int(1681)
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
fourty one-1681
17vn-56867
1qr8-81332
1ae9-60129
1kgp-73177
16mc-55236
194y-58498
1j7e-71546
1gos-68284
1zkd-92749


Delete:
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
fourty one-1681
17vn-56867
1qr8-81332
1ae9-60129
1kgp-73177
16mc-55236
194y-58498
1j7e-71546
1gos-68284
1zkd-92749


Set:
12uf-50343
1lq0-74808
24ll-99273
int(9999)
1cwv-63391
1vsg-87856
1ffh-66653
1hy3-69915
20to-9999
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
fourty one-1681
17vn-56867
1qr8-81332
1ae9-60129
1kgp-73177
16mc-55236
194y-58498
1j7e-71546
1gos-68284
1zkd-92749
