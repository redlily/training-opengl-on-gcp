<?php

$x = (int)$_GET['x'] % 90;
$y = (int)$_GET['y'] % 180;
$z = (int)$_GET['z'] % 180;

exec("./main {$x} {$y} {$z}");

echo "<html><head><title>server rendering</title></head><body><img src='image/rendering-{$x}-{$y}-{$z}.png'></body></html>";
