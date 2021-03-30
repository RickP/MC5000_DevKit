[home](index) [<< prev level](level5) To be continued...

# Level 6

The customer TrafficControl Inc. wants you to create a new control mechanism for their pedestrian traffic lights.

## Specification

- On start the traffic light needs to be red.
- When a pedestrian presses a button a display shall count down from 10 to 0
- When the timer reaches zero the traffic light goes from red to red-yellow to green
- After some seconds the traffic light goes back to yellow and then to red 

## Help


<details markdown=1><summary>Cable connections</summary>
  
- MCU1-P0 -> Button1
- MCU1-P1 -> Red LED
- MCU1-X0 -> Display
- MCU1-X1 -> MCU2-X0
- MCU1-P0 -> Green LED
- MCU1-P1 -> Yellow LED
 
</details>

<p></p>

<details markdown=1><summary>Hints</summary>
  
- MCU2 should control the green and yellow LEDs when MCU1 sends an XBus signal
- MCU1 displays the countdown in a loop and then sends the signal to MCU2
- After sending the signal MCU1 waits for an XBus signal from MCU2 to switch back to red
 
</details>

<p></p>

<details markdown=1><summary>Solution</summary>

MCU1

```
start:
mov 100 p1
teq p0 100
- jmp start
mov 10 acc
loop:
sub 1
mov acc x0
slp 4
tgt acc 0
+ jmp loop
mov 1 x1
slp 4
mov 0 p1
mov x1 dat
```

MCU2

```
teq x0 1
+ mov 100 p1
+ slp 5
+ mov 0 p1
+ mov 100 p0
+ slp 30
+ mov 0 p0
+ mov 100 p1
+ slp 5
+ mov 0 p1
+ mov 0 x0
```

</details>
