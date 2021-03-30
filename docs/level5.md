[^ index](index) [<< prev level](level4) [next level >>](level6)

# Level 5

Clickybright Co. liked your device a lot. Now they want a second product that switches on the light with one button and off with another. So get going!

## Specification

- The red LED should turn on when button 1 is pressed
- It should turn off when button 2 is pressed

## Help


<details markdown=1><summary>Cable connections</summary>
  
- MCU1_P0 -> Button_1
- MCU1_P1 -> Button_2
- MCU1_X0 -> MCU2_X0
- MCU2_P0 -> Red LED
 
</details>

<p></p>

<details markdown=1><summary>Hints</summary>
  
- Use both MC5000 controllers
 
</details>

<p></p>

<details markdown=1><summary>Solution</summary>

MCU1

```
teq p0 100
- mov 0 dat
+ teq dat 0 
+ mov 1 dat
+ mov 100 x0
teq p1 100
- mov 0 dat
+ teq dat 0 
+ mov 1 dat
+ mov 0 x0
```

MCU2

```
mov x0 p0
```

</details>