[<< back](index) [next level >>](level3)

# Level 2

Our sales guy did not pay attention when talking to Blinky Inc. - they want the LED to blink _only_ if a button is pressed. Please change the code!

## Specification

- Blink the green LED
- It should be on for about half a second and off for the same time
- It should blink only while button 1 is pressed

## Help


<details markdown=1><summary>Cable connections</summary>
  
- MCU1_P0 -> Green LED
- MCU1_P1 -> Button 1
 
</details>

<p></p>

<details markdown=1><summary>Hints</summary>
  
- Use the TEQ instruction
 
</details>

<p></p>

<details markdown=1><summary>Solution</summary>

MCU1

```
teq p1 100
+ mov 100 p0
+ slp 5
+ mov 0 p0
slp 5
```

</details>