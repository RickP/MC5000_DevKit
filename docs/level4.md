[home](index) [<< prev level](level3) [next level >>](level5)

# Level 4

Our client Clickybright Co. is making light switches. They found out, that simple tactile buttons are way cheaper than the toggle buttons they are using right now. So they want you to make software that turns the light on with one press and off with anoter one.

## Specification

- Turn on the red LED when pressing button 1
- The LED should turn off with another press of the same button
- It should not matter how long you press the putton

## Help


<details markdown=1><summary>Cable connections</summary>
  
- MCU1_P0 -> Button1
- MCU1_P1 -> Red LED
 
</details>

<p></p>

<details markdown=1><summary>Hints</summary>
  
- Save the information about the last state of the button to a register
- Use the NOT instruction
 
</details>

<p></p>

<details markdown=1><summary>Solution</summary>

MCU1

```
teq p0 100 # check button
- mov 0 dat # not pressed dat=0
+ teq dat 0 # if unpressed before
+ mov 1 dat # mark as pressed
+ not # toggle acc
mov acc p1 # write acc to LED
```


</details>