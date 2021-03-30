[home](index) [next level >>](level2)

# Level 1

Blinky Inc. wants us to make a device that blinks a green LED. The CTO told me to let the new guy do it - so it's your job now to do that. Have Fun.

## Specification

- Blink the green LED
- It should be on for about half a second and off for the same time


## Help


<details markdown=1><summary>Cable connections</summary>
  
- MCU1_P0 -> Green LED
 
</details>

<p></p>

<details markdown=1><summary>Hints</summary>
  
- Use the MOV and the SLP instructions
- One unit of sleep is about 100ms
 
</details>

<p></p>

<details markdown=1><summary>Solution</summary>

MCU1

```
mov 100 p0
slp 5
mov 0 p0
slp 5
```


</details>