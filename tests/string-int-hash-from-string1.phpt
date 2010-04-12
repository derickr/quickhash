--TEST--
Basic test for reading hashes from strings.
--INI--
xdebug.default_enable=0
--FILE--
<?php
$file = dirname( __FILE__ ) . "/simple.hash.string";
$contents = file_get_contents( $file );
$hash = QuickHashStringIntHash::loadFromString( $contents, 0, QuickHashStringIntHash::DO_NOT_USE_ZEND_ALLOC );
foreach( range( 0, 0x1f ) as $key )
{
    $i = 48712 + $key * 1631;
    $k = base_convert( $i, 10, 36 );
    echo $k, ' => ', $hash->get( $k ), "\n";
}
?>
--EXPECT--
11l4 => 48712
12uf => 50343
143q => 51974
15d1 => 53605
16mc => 55236
17vn => 56867
194y => 58498
1ae9 => 60129
1bnk => 61760
1cwv => 63391
1e66 => 65022
1ffh => 66653
1gos => 68284
1hy3 => 69915
1j7e => 71546
1kgp => 73177
1lq0 => 74808
1mzb => 76439
1o8m => 78070
1phx => 79701
1qr8 => 81332
1s0j => 82963
1t9u => 84594
1uj5 => 86225
1vsg => 87856
1x1r => 89487
1yb2 => 91118
1zkd => 92749
20to => 94380
222z => 96011
23ca => 97642
24ll => 99273
