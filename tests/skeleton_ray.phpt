--TEST--
skeleton_vector basic test
--FILE--
<?php
$position = new \skeleton\Vector3(1,2,3);
$direction = new \skeleton\Vector3(4,5,6);

$ray = new \skeleton\Ray($position, $direction);

echo 'Z: ', $ray->position->z, PHP_EOL;

$position->z = 9;

echo 'Z: ', $ray->position->z, PHP_EOL;
?>
--EXPECT--
Z: 3
Z: 9