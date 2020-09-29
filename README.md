Suiko
====================================
This is prototype of Hydroponics system for aeration and liquid fertilizer adjustment via Bluetooth.

![Suiko](https://user-images.githubusercontent.com/3187220/93475749-f6f32680-f933-11ea-9229-4175fcb4df4b.jpeg)


Circuit Diagram
------------------------------------
https://www.tinkercad.com/things/fpSljIVI2Si


Serial Commands via USB or Bluetooth
------------------------------------
- Software Serial: D10->RxD, D11->TxD
- Newline code: NONE
- BPS: 9600

| Command | Description |
| --- | --- |
| C | Cycle pump: ON |
| c | Cycle pump: OFF |
| I | Input pump: ON |
| i | Input pump: OFF |
| A | Air pump: ON |
| a | Air pump: OFF |
| m | Measure EC and pH and Temperature |
| o | Observe Mode: OFF |
| &gt;n.nn | Observe Mode: If EC &gt; n.nn (e.g. 1.00) Then Input pump ON |
| &lt;n.nn | Observe Mode: If EC &lt; n.nn (e.g. 0.80) Then Input pump ON |


References about EC and pH
------------------------------------
- https://hackaday.io/project/7008-fly-wars-a-hackers-solution-to-world-hunger/log/24646-three-dollar-ec-ppm-meter-arduino
- https://www.botshop.co.za/how-to-use-a-ph-probe-and-sensor/
- https://nadegata.info/ec-automation-how-to-make/
- https://jitaku-yasai.com/home-made/ec-meter-selfmade/


Licence
------------------------------------
- GNU General Public License v3
    - From [refs](https://hackaday.io/project/7008-fly-wars-a-hackers-solution-to-world-hunger/log/24646-three-dollar-ec-ppm-meter-arduino)
