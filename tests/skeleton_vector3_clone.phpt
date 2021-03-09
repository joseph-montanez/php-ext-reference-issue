--TEST--
skeleton_vector3 basic test
--FILE--
<?php
$vector = new \skeleton\Vector3(1,2,3);
$vector2 = clone $vector;

$vector2->z = 5;

var_dump($vector2);

?>
--EXPECT--
object(skeleton\Vector3)#1 (3) {
  ["x"]=>
  float(1)
  ["y"]=>
  float(2)
  ["z"]=>
  float(5)
}