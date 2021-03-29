[<< back](index)

# Level 5

The customer TrafficControl Inc. wants you to create a new control mechanism for their pedestrian traffic lights.

## Specification

- On start the traffic light needs to be red.
- When a pedestrian presses a button a display shall count down from 10 to 0
- When the timer reaches zero the traffic light goes from red to red-yellow to green
- After some seconds the traffic light goes back to yellow and then to red 

## Hints


<details><summary>Cable connections</summary>
<p markdown=1>
  
- MCU1-P0 -> Button1
- MCU1-P1 -> Red LED
- MCU1-X0 -> Display
- MCU1-X1 -> MCU2-X0
- MCU1-P0 -> Green LED
- MCU1-P1 -> Yellow LED

</p>  
</details>


<details><summary>Hints</summary>
<p markdown=1>
  
- MCU2 should control the green and yellow LEDs when MCU1 sends an XBus signal
- MCU1 displays the countdown in a loop and then sends the signal to MCU2
- After sending the signal MCU1 waits for an XBus signal from MCU2 to switch back to red

</p>  
</details>


<details><summary>Solution</summary>
<p markdown=1>

MCU1

```
CODE
```

MCU2

```
CODE
```

</p> 
</details>
