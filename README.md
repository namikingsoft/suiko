Suiko
==================
This is prototype of Hydroponics system for aeration and liquid fertilizer adjustment via Bluetooth.

Circuit Diagram
------------------
https://www.tinkercad.com/things/fpSljIVI2Si

![Suiko](https://user-images.githubusercontent.com/3187220/93266167-a620e800-f7e4-11ea-939b-ee56c9987325.png)

Serial Commands via USB or Bluetooth
------------------
- Software Serial: D10->RxD, D11->TxD
- Newline code: NONE
- BPS: 9600

| Command | Description |
| --- | --- |
| C | Cycle pump: ON |
| c | Cycle pump: OFF |
| I | Input pump: ON |
| i | Input pump: OFF |
| m | Measure EC and Temperature |
| o | Observe Mode: OFF |
| &gt;n.nn | Observe Mode: If EC &gt; n.nn (e.g. 1.00) Then Input pump ON |
| &lt;n.nn | Observe Mode: If EC &lt; n.nn (e.g. 0.80) Then Input pump ON |
