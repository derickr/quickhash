<?php
$f = file( $argv[1] );
foreach( $f as $line )
{
	$line = trim( $line );
	$line = preg_replace( '@\s{2,}@', ' ', $line );
	foreach( explode( ' ', $line ) as $nr )
	{
		echo pack( 'l', $nr );
	}
}
