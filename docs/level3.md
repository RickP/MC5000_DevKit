[home](index) [<< prev level](level2) [next level >>](level4)

# Level 3

Crimedingdong Inc. approached us with an idea for a new home alarm system. They found out that most thiefs use a flashlight when breaking into houses. So they want us to create an alarm system that beeps when it detects light.

## Specification

- Use the "Photontech i5" light sensor to detect the level of light
- When the light level is above 50 sound an alarm with the "Audiomaster Extreme" module
- Stop the alarm when it's getting dark again
- Shine a flashlight (e.g. on your phone) to the light sensor to test your code

## Help


<details markdown=1><summary>Cable connections</summary>
  
- MCU1-P0 -> Light Sensor
- MCU1-X1 -> Buzzer
 
</details>

<p></p>

<details markdown=1><summary>Hints</summary>
  
- Do the development in a darker room
- The "Audiomaster Extreme" beeps with a high tone if you send the number '30' over XBus
- Use the TGT instruction
 
</details>

<p></p>

<details markdown=1><summary>Solution</summary>

MCU1

```
tgt p0 50
+ mov 30 x0
slp 2
```

</details>
