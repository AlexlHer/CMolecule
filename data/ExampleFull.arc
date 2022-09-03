<?xml version="1.0"?>
<case codename="CMolecule" xml:lang="en" codeversion="1.0">
  <arcane>
    <title>Sample</title>
    <timeloop>CMAMALoop</timeloop>
  </arcane>

  <meshes>
    <mesh>
      <generator name="Cartesian3D" >
        <nb-part-x>3</nb-part-x> 
        <nb-part-y>2</nb-part-y>
        <nb-part-z>2</nb-part-z>

        <origin>0.0 0.0 0.0</origin>

        <x>
          <n>20</n>
          <length>2.0</length>
        </x>

        <y>
          <n>20</n>
          <length>2.0</length>
        </y>

        <z>
          <n>20</n>
          <length>2.0</length>
        </z>

      </generator>
    </mesh>
  </meshes>
  <c-m>
    <diametre>1000</diametre>
    <espece>
      <name>E</name>
      <size>7</size>
      <speed>0.5</speed>
      <init>50</init>
    </espece>
    <reaction>
      <first-left>E</first-left>
      <sec-left>S</sec-left>
      <first-right>ES</first-right>
      <sec-right>E</sec-right>
      <proba>0.3</proba>
    </reaction>
  </c-m>
</case>
